#pragma once
#include "ProcessorEDF.h"
#include"Scheduler.h"

//Responsible for all the schedueling in the EDF (bonus)
#ifndef SchedulingAlgorithm
void ProcessorEDF::SchedulingAlgorithm(int Timestep)
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
		Process* P;
		Process* Z;
		bool processCpuDecremented = false;
		if (GetStatusOfProcessor() == busy)
		{
			Z = GetRunningProcess();
			if (GetRunningProcess() != NULL)
			{
				//cout << "########################" << "process to be terminated by EDF ID: " << Z->GetProcessId() << endl;
				//cout << "########################" << "cpu before: " << Z->GetCpuTime() << endl;

				bool y = scheduler->RUNTOTRM(Z, this);
				if (y == true)
				{

					//cout << "########################" << "successfuly terminated by EDF ID: " << Z->GetProcessId() << endl;
					Z->SetStateOfProcess(Terminated);
					Z->SetTerminationTime(Timestep);


				}
				//cout << "#######################" << "cpu after: " << Z->GetCpuTime() << endl;

			}
		}
		if (GetStatusOfProcessor() == idle)
		{
			if (!readyList.isEmpty())
			{
				P = GetfirstReady();
				if (P->GethaveMovedAlready() == true)
				{
					Process* ProcessThatisNotFirst;
					LinkedQueue<Process*> temp;
					if (RUN == NULL && P != nullptr)
					{

						while (P != nullptr && P->GethaveMovedAlready() == true)
						{
							temp.enqueue(P);
							P = GetfirstReady();
						}
						while (!temp.isEmpty())
						{
							Process* tempP = new Process;
							temp.dequeue(tempP);
							AddToRDYQueue(tempP);
						}
					}
				}

				if (P == nullptr) // that means there was only one process that is already moved
				{
					P = GetfirstReady();
				}

				if (P->GethaveMovedAlready() == false)
				{
					ToRUN_State(P);
					//cout << "########################" << "put that process from edf to RUN" << P->GetProcessId() << endl;
					//for output file
					if (P->getIsThatFirstTime() == true)
					{
						P->setTimeStepWhenProcessIsSentToReadyForFirstTime(Timestep);
						P->setIsThatFirstTime(false);
					}
					processCpuDecremented = false;

				}
				else if (P->GethaveMovedAlready() == true)
				{
					AddToRDYQueue(P);
				}
			}
		}
		else if (GetStatusOfProcessor() == busy)
		{

			bool flag = false;
			Z = GetRunningProcess();

			//HANDLING EDT if Processor come with earlier deadline to be added to run and process in run to be added to ready again
			if (!readyList.isEmpty())
			{

				//Process* P = this->GetfirstReady();
				P = GetfirstReady();
				if (P->GethaveMovedAlready() == true)
				{
					Process* ProcessThatisNotFirst;
					LinkedQueue<Process*> temp;
					if (P != nullptr)
					{

						while (P != nullptr && P->GethaveMovedAlready() == true)
						{
							temp.enqueue(P);
							P = GetfirstReady();
						}
						while (!temp.isEmpty())
						{
							Process* tempP = new Process;
							temp.dequeue(tempP);
							AddToRDYQueue(tempP);
						}
					}
				}
				if (P == NULL)
				{
					P = GetfirstReady();
				}
				
				if (P->GethaveMovedAlready() == false)
				{
					if (P->getProcessDeadline() < Z->getProcessDeadline() && P->getProcessDeadline() != 0)
					{
						//cout << "########################" << "process p in ready Deadline: " << P->getProcessDeadline() << " process z Deadline: " << Z->getProcessDeadline() <<endl;
						//cout<<"so its to be replaced in run"<<endl;
						//cout << "process z " << Z->GetProcessId() << endl;
						//cout << "process p " << P->GetProcessId() << endl;
						//remove process in RUN to Ready
						AddToRDYQueue(Z);
						RUN = NULL;
						flag = true;
						processCpuDecremented = false;
						//ADD process with earlier deadline in RUN
						P->SetStateOfProcess(Running);
						stateOfProcessor = busy;
						//scheduler->incrementnoOfProcessInRUN();

						//for output file
						if (P->getIsThatFirstTime() == true)
						{
							P->setTimeStepWhenProcessIsSentToReadyForFirstTime(Timestep);
							P->setIsThatFirstTime(false);
						}

						SetRunningProcess(P);
						/*P->decrementcputime();
						P->incrementWorkingtimeRUN();*/
						P->SethaveMovedAlready(true);
					}
					else
					{
						AddToRDYQueue(P);
					}
				}
				else
				{
					AddToRDYQueue(P);
				}

			}

			if (GetStatusOfProcessor() == busy)
			{
				Z = GetRunningProcess();

				//RUN to terminated call here 
				if (GetRunningProcess() != NULL)
				{
					//cout << "########################" << "process to be terminated by EDF ID: " << Z->GetProcessId() << endl;
					//cout << "########################" << "cpu before: " << Z->GetCpuTime() << endl;

					bool y = scheduler->RUNTOTRM(Z, this);
					if (y == true)
					{

						//cout << "########################" << "successfuly terminated by EDF ID: " << Z->GetProcessId() << endl;
						Z->SetStateOfProcess(Terminated);
						Z->SetTerminationTime(Timestep);


					}
					if (Z != nullptr)
					{
						processCpuDecremented = true;
						Z->decrementcputime();
						Z->incrementWorkingtimeRUN();
					}

					//cout << "########################" << "cpu after: " << Z->GetCpuTime() << endl;
				}

			}



			processCpuDecremented = false;
		}
	}
}
#endif

TypeOfProcessor ProcessorEDF::GetTypeOfProcessor()
{
	return EDF;
}

Process* ProcessorEDF::GetfirstReady()
{
	Process* t;
	t = readyList.remove();
	if (t != NULL) {
		currentExpectedFinishTime = currentExpectedFinishTime - t->GetCpuTime();
		numberOfProcessInReady--;
	}
	return t;

}

Process* ProcessorEDF::PeekfirstReady()
{
	Process* t;
	readyList.peek(t);
	return t;
}

void ProcessorEDF::AddToRDYQueue(Process* P)
{
	currentExpectedFinishTime = currentExpectedFinishTime + P->GetCpuTime();
	int x = 0;
	if (P->getProcessDeadline() != 0)
	{
		x = P->getProcessDeadline();
	}
	else
	{
		x = INT_MAX - P->GetArrivalTime();
	}
	readyList.insert(P, x);
	numberOfProcessInReady++;
	P->SetStateOfProcess(Ready);

}

void ProcessorEDF::ResethaveMovedAlreadyProcesses()
{
	for (size_t i = 0; i < numberOfProcessInReady; i++)
	{
		Process* p = new Process;
		p = readyList.remove();
		p->SethaveMovedAlready(false);
		readyList.insert(p, p->getProcessDeadline());
	}
}

int ProcessorEDF::GetnumberOfProcessInReady()
{
	return numberOfProcessInReady;
}

bool ProcessorEDF::isReadyEmpty()
{
	return readyList.isEmpty();
}

void ProcessorEDF::PrintRDY()
{
	cout << numberOfProcessInReady << " RDY: ";
	readyList.Print();
}

void ProcessorEDF::LoadProcessorEDF(int numOfProcessorEDF, int& sumOfProcessors, Processor* listOfProcessors[200], ifstream& inputFile) {
	for (size_t i = 0; i < numOfProcessorEDF; i++)
	{
		Processor* pEDF = new ProcessorEDF(scheduler);
		listOfProcessors[sumOfProcessors++] = pEDF;
		SetStatusOfProcessor(idle);
	}
}

ProcessorEDF::~ProcessorEDF()
{
	readyList.~PriorityQueue();
}