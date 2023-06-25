#pragma once
#include"ProcessorRR.h"
#include"Scheduler.h"

//Responsible for all the schedueling in the RR processor
#ifndef SchedulingAlgorithm
void ProcessorRR::SchedulingAlgorithm(int Timestep)
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

		scheduler->IO_requestHandling(this);
		int tempTimeSlice = ProcessorRR::timeSlice;
		Process* P;
		if (GetStatusOfProcessor() == idle)
		{
			if (!readyList.isEmpty())
			{
				P = GetfirstReady();
				int waitingTime = Timestep - (P->GetArrivalTime()) - (P->GetworkingtimesRUN());
				P->SetWaitingTime(waitingTime);
				if (P->GethaveMovedAlready() == false)
				{
					//for output file
					if (P->getIsThatFirstTime() == true)
					{
						P->setTimeStepWhenProcessIsSentToReadyForFirstTime(Timestep);
						P->setIsThatFirstTime(false);
					}
					ToRUN_State(P);
					P->incrementWorkingtimeRUNRR();
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
			Process* Z = GetRunningProcess();
			int waitingTime = Timestep - (Z->GetArrivalTime()) - (Z->GetworkingtimesRUN());
			Z->SetWaitingTime(waitingTime);
			scheduler->ProcessMigrationRR(Z, this);
			if (Z->GetStateOfProcess() == Running)
			{
				bool y = scheduler->RUNTOTRM(GetRunningProcess(), this);
				if (y == true)
				{
					Z->SetTerminationTime(Timestep);
				}
				else
				{
					Z->decrementcputime();
					Z->incrementWorkingtimeRUN();
					Z->incrementWorkingtimeRUNRR();
					if (Z->GetworkingtimesRUNRR() > tempTimeSlice)
					{
						Z->SetStateOfProcess(Ready);
						SetRunningProcess(NULL);
						stateOfProcessor = idle;
						scheduler->decrementnoOfprocessRUN();
						AddToRDYQueue(Z);
					}
					if (Z->GetworkingtimesRUNRR() > tempTimeSlice)
					{
						Z->setworkingtimesRUNRR(0);
					}
				}
			}

		}
	}
}
#endif

TypeOfProcessor ProcessorRR::GetTypeOfProcessor()
{
	return RR;
}

Process* ProcessorRR::GetfirstReady()
{
	Process* t = NULL;
	bool x = readyList.dequeue(t);
	if (t != NULL)
	{
		currentExpectedFinishTime = currentExpectedFinishTime - t->GetCpuTime();
	}
	if (x == false)
	{

		t = NULL;
	}
	else if (x == true)
	{
		numberOfProcessInReady--;
	}
	return t;
}

Process* ProcessorRR::PeekfirstReady()
{
	Process* t;
	readyList.peek(t);
	return t;
}

void ProcessorRR::AddToRDYQueue(Process* P)
{
	currentExpectedFinishTime = currentExpectedFinishTime + P->GetCpuTime();
	readyList.enqueue(P);
	numberOfProcessInReady++;
	P->SetStateOfProcess(Ready);
}

void ProcessorRR::ResethaveMovedAlreadyProcesses()
{
	for (size_t i = 0; i < numberOfProcessInReady; i++)
	{
		Process* p = new Process;
		readyList.dequeue(p);
		p->SethaveMovedAlready(false);
		readyList.enqueue(p);
	}
}

int ProcessorRR::GetnumberOfProcessInReady()
{
	return numberOfProcessInReady;
}

bool ProcessorRR::isReadyEmpty()
{
	return readyList.isEmpty();
}

void ProcessorRR::PrintRDY()
{
	cout << numberOfProcessInReady << " RDY:";
	LinkedQueue<Process*> readyListtemp = readyList;
	Process* p = new Process();
	for (size_t i = 0; i < numberOfProcessInReady; i++)
	{
		readyListtemp.dequeue(p);
		cout << " " << p->GetProcessId();
		if (i + 1 != numberOfProcessInReady)
		{
			cout << ",";
		}
	}
}

void ProcessorRR::LoadProcessorRR(int timeSliceRR, int numOfProcessorRR, int& sumOfProcessors, Processor* listOfProcessorss[200], ifstream& inputFile)
{
	timeSlice = timeSliceRR;
	for (int i = 0; i < numOfProcessorRR; i++)
	{
		Processor* pRR = new ProcessorRR(scheduler);
		listOfProcessorss[sumOfProcessors++] = pRR;
		SetStatusOfProcessor(idle);
	}
}

int ProcessorRR::timeSlice = 0;

void ProcessorRR::SetTimeSlice(int timeSlice)
{
	if (timeSlice >= 0)
	{
		ProcessorRR::timeSlice = timeSlice;
	}
	else
	{
		cout << "Error ! time slice can not be negative" << endl;
	}
}

int ProcessorRR::getTimeSlice()
{
	return timeSlice;
}

ProcessorRR::~ProcessorRR()
{
	readyList.~LinkedQueue();
}