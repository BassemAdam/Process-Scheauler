#pragma once
#include"ProcessorFCFS.h"
#include"Scheduler.h"

//Responsible for all the schedueling in the FCFC processor
#ifndef SchedulingAlgorithm
void ProcessorFCFS::SchedulingAlgorithm(int Timestep)
{
	if (Getisoverheat() == true)
	{
		decremntOverHeatTime();
		if (Getoverheattime() == 0)
		{
			Setisoverheat(false);
		}
	}
	else {
		if (this->GetStatusOfProcessor() == busy)
		{
			totalProcessorBusyTime++;

		}
		else if (this->GetStatusOfProcessor() == idle)
		{
			totalProcessorIdleTime++;
		}

		Process* P;
		P = GetRunningProcess();
		bool killedorNot = killFCFS(P, Timestep, 1);
		if (killedorNot)
		{
			scheduler->incrementNumberOfProcessor();
		}
		scheduler->IO_requestHandling(this);
		if (GetStatusOfProcessor() == idle)
		{
			if (!readyList.isEmpty())
			{
				P = GetfirstReady();
				int waitingTime = Timestep - (P->GetArrivalTime()) - (P->GetworkingtimesRUN());
				P->SetWaitingTime(waitingTime);
				if (P->GethaveMovedAlready() == false)
				{
					ToRUN_State(P);

					//for output file
					if (P->getIsThatFirstTime() == true)
					{
						P->setTimeStepWhenProcessIsSentToReadyForFirstTime(Timestep);
						P->setIsThatFirstTime(false);
					}

					int r = (rand() % 100) + 1;
					if (P->GetStateOfProcess() == Running)
					{
						if (r <= ForkProb)
						{
							scheduler->ProcessForking(P);
						}
					}
				}
				else if (P->GethaveMovedAlready() == true)
				{
					AddToRDYQueue(P);
					P->Incrementwaitingtime();
				}
			}
		}
		else if (GetStatusOfProcessor() == busy)
		{
			Process* Z;
			Z = GetRunningProcess();
			int waitingTime = Timestep - (Z->GetArrivalTime()) - (Z->GetworkingtimesRUN());
			Z->SetWaitingTime(waitingTime);
			scheduler->ProcessMigrationFCFS(Z, this);

			if (Z->GetStateOfProcess() == Running)
			{
				bool y = scheduler->RUNTOTRM(Z, this);
				if (y == true)
				{
					Z->SetTerminationTime(Timestep);
				}
				else
				{
					Z->decrementcputime();
					Z->incrementWorkingtimeRUN();
					int r = (rand() % 100) + 1;
					if (r <= ForkProb && Z != NULL)
					{
						scheduler->ProcessForking(Z);
					}
					bool killedorNot = killFCFS(Z, Timestep, 0);
					if (killedorNot)
					{
						scheduler->incrementNumberOfProcessor();
					}
				}
			}
		}
	}

}
#endif

TypeOfProcessor ProcessorFCFS::GetTypeOfProcessor()
{
	return FCFS;
}

Process* ProcessorFCFS::GetfirstReady()
{
	Process* t;
	readyList.DeleteLast(t);
	if (t != NULL) {
		currentExpectedFinishTime = currentExpectedFinishTime - t->GetCpuTime();
		numberOfProcessInReady--;
	}
	return t;
}

Process* ProcessorFCFS::PeekfirstReady()
{
	Process* t;
	t = readyList.Getelement(numberOfProcessInReady - 1);
	return t;
}

void ProcessorFCFS::AddToRDYQueue(Process* P)
{
	currentExpectedFinishTime = currentExpectedFinishTime + P->GetCpuTime();
	readyList.InsertBeg(P);
	numberOfProcessInReady++;
	P->SetStateOfProcess(Ready);
}

void ProcessorFCFS::ResethaveMovedAlreadyProcesses()
{
	for (size_t i = 0; i < numberOfProcessInReady; i++)
	{
		if (!readyList.isEmpty())
			readyList.Getelement(i)->SethaveMovedAlready(false);
	}
}

int ProcessorFCFS::GetnumberOfProcessInReady()
{
	return numberOfProcessInReady;
}

bool ProcessorFCFS::isReadyEmpty()
{
	return readyList.isEmpty();
}

void ProcessorFCFS::PrintRDY()
{
	cout << numberOfProcessInReady << " RDY:";
	for (size_t i = 0; i < numberOfProcessInReady; i++)
	{
		Process* p = readyList.Getelement(i);
		if (p != NULL)
		{

			cout << " " << p->GetProcessId();
			if (i + 1 != numberOfProcessInReady)
			{
				cout << ",";
			}
		}
	}
}

double ProcessorFCFS::ForkProb = 0;
int ProcessorFCFS::SignalListCount = 0;
LinkedQueue<SIGKILL> ProcessorFCFS::SignalKill;

void ProcessorFCFS::LoadProcessorFCFS(int numOfProcessorFCFS, int& sumOfProcessors, Processor* listOfProcessors[200], ifstream& inputFile)
{
	for (size_t i = 0; i < numOfProcessorFCFS; i++)
	{
		Processor* pFCFS = new  ProcessorFCFS(scheduler);
		listOfProcessors[sumOfProcessors++] = pFCFS;
		SetStatusOfProcessor(idle);
	}
}

void ProcessorFCFS::setForkProb(double ForkP)
{
	ForkProb = ForkP;
}

double ProcessorFCFS::getForkProb()
{
	return ForkProb;
}

void ProcessorFCFS::AddSignalKill(SIGKILL Sg)
{
	SignalKill.enqueue(Sg);
}

SIGKILL ProcessorFCFS::GetSignalKill()
{
	SIGKILL temp;
	SignalKill.dequeue(temp);
	return temp;
}

void ProcessorFCFS::DeleteRandomProcess(int index)
{
	int reqIndexToKill = -1;
	for (size_t i = 0; i < numberOfProcessInReady; i++)
	{
		if (readyList.Getelement(i)->GetProcessId() == index)
		{
			reqIndexToKill = i;
		}
	}
	if (reqIndexToKill != -1 && readyList.Getelement(reqIndexToKill) != NULL && readyList.Getelement(reqIndexToKill)->GethaveMovedAlready() == false)
	{
		Process* pproc = readyList.Getelement(reqIndexToKill);
		scheduler->AddToTrmList(readyList.Getelement(reqIndexToKill));
		readyList.DeleteNodeAtIndex(reqIndexToKill);
		numberOfProcessInReady--;
	}
}

//Checks whether the Process can be killed or not
bool ProcessorFCFS::Kill(Process* P)
{
	SIGKILL sk;
	bool y = SignalKill.isEmpty();
	if (y == false) {
		if (P->GetStateOfProcess() == Running)
		{
			int siglistCount = SignalListCount;
			while (siglistCount != 0)
			{
				SignalKill.peek(sk);
				if (sk.PID == P->GetProcessId() && sk.time == scheduler->GetTimeStep())
				{
					SignalKill.dequeue(sk);
					scheduler->AddToTrmList(P);
					scheduler->ORPH(P);
					SetRunningProcess(NULL);
					SetStatusOfProcessor(idle);
					SignalListCount--;
					siglistCount--;
					SignalKill.peek(sk);
					return true;
				}
				else
				{
					SignalKill.dequeue(sk);
					SignalKill.enqueue(sk);
					siglistCount--;
				}
			}
		}
		else if (P->GetStateOfProcess() == Ready)
		{
			int siglistCount = SignalListCount;
			while (siglistCount != 0)
			{
				SignalKill.peek(sk);
				if (sk.PID == P->GetProcessId() && sk.time == scheduler->GetTimeStep())
				{
					SignalKill.dequeue(sk);
					scheduler->AddToTrmList(P);
					scheduler->ORPH(P);
					SignalListCount--;
					siglistCount--;
					return true;
				}
				else
				{
					SignalKill.dequeue(sk);
					SignalKill.enqueue(sk);
					siglistCount--;
				}
			}
		}
	}
	return false;
}

//Checks whether to kill from RUN or from RDY in the FCFS
#ifndef killFCFS
bool ProcessorFCFS::killFCFS(Process* P, int timestep, int x)
{
	//		if	x = 0 then kill from run 
	//otherwise x = 1 then kill from ready
	if (x == 0)
	{
		if (P->GetStateOfProcess() == Running)
		{
			bool y = false;
			y = Kill((P));
			if (y == true) {
				P->SetTerminationTime(timestep);
				scheduler->decrementnoOfprocessRUN();
				return true;
			}
			else
			{
				return false;
			}
		}

	}
	else if (x == 1)
	{
		int x = numberOfProcessInReady;
		for (int j = 0; j < x; j++)
		{
			Process* Check = new Process();
			Check = GetfirstReady();
			bool w = Kill(Check);
			if (w == true)
			{
				Check->SetTerminationTime(scheduler->GetTimeStep());
				return true;
			}
			else if (w == false) {
				AddToRDYQueue(Check);
				return false;
			}
		}
	}
	return false;
}
#endif 

void ProcessorFCFS::SetSignallistcount(int count)
{
	SignalListCount = count;
}

int ProcessorFCFS::GestSignailListCount()
{
	return SignalListCount;
}

ProcessorFCFS::~ProcessorFCFS()
{
	readyList.~LinkedList();
	SignalKill.~LinkedQueue();
}

