#pragma once
#include"ProcessorSJF.h"
#include"Scheduler.h"

//Responsible for all the schedueling in the SJF processor
#ifndef SchedulingAlgorithm
void ProcessorSJF::SchedulingAlgorithm(int Timestep)
{
	if (Getisoverheat() == true)
	{
		decremntOverHeatTime();
		if (Getoverheattime() == 0)
		{
			Setisoverheat(false);
		}
	}
	else
	{
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
		if (GetStatusOfProcessor() == idle) {
			if (!readyList.isEmpty())
			{
				P = GetfirstReady();
				if (P->GethaveMovedAlready() == false)
				{
					ToRUN_State(P);
					//for output file
					if (P->getIsThatFirstTime() == true)
					{
						P->setTimeStepWhenProcessIsSentToReadyForFirstTime(Timestep);
						P->setIsThatFirstTime(false);
					}
				}
				else if (P->GethaveMovedAlready() == true)
				{
					AddToRDYQueue(P);
				}
			}
		}
		else if (GetStatusOfProcessor() == busy)
		{
			Process* Z;
			Z = GetRunningProcess();
			if (GetRunningProcess() != NULL)
			{
				Z->decrementcputime();
				Z->incrementWorkingtimeRUN();
				bool y = scheduler->RUNTOTRM(Z, this);
				if (y == true)
				{
					Z->SetTerminationTime(Timestep);
				}
			}
		}
	}
}
#endif

TypeOfProcessor ProcessorSJF::GetTypeOfProcessor()
{
	return SJF;
}

Process* ProcessorSJF::GetfirstReady()
{
	Process* t;
	t = readyList.remove();
	if (t != NULL) {
		currentExpectedFinishTime = currentExpectedFinishTime - t->GetCpuTime();
		numberOfProcessInReady--;
	}
	return t;

}

Process* ProcessorSJF::PeekfirstReady()
{
	Process* t;
	readyList.peek(t);
	return t;
}

void ProcessorSJF::AddToRDYQueue(Process* P)
{
	currentExpectedFinishTime = currentExpectedFinishTime + P->GetCpuTime();
	int x = INT_MAX - P->GetCpuTime();
	readyList.insert(P, x);
	numberOfProcessInReady++;
	P->SetStateOfProcess(Ready);
}

void ProcessorSJF::ResethaveMovedAlreadyProcesses()
{
	for (size_t i = 0; i < numberOfProcessInReady; i++)
	{
		Process* p = new Process;
		p = readyList.remove();
		p->SethaveMovedAlready(false);
		readyList.insert(p, p->GetCpuTime());
	}
}

int ProcessorSJF::GetnumberOfProcessInReady()
{
	return numberOfProcessInReady;
}

bool ProcessorSJF::isReadyEmpty()
{
	return readyList.isEmpty();
}

void ProcessorSJF::PrintRDY()
{
	cout << numberOfProcessInReady << " RDY: ";
	readyList.Print();
}

void ProcessorSJF::LoadProcessorSJF(int numOfProcessorSJF, int& sumOfProcessors, Processor* listOfProcessors[200], ifstream& inputFile)
{
	for (size_t i = 0; i < numOfProcessorSJF; i++)
	{
		Processor* pSJF = new ProcessorSJF(scheduler);
		listOfProcessors[sumOfProcessors++] = pSJF;
		SetStatusOfProcessor(idle);
	}
}

ProcessorSJF::~ProcessorSJF()
{
	readyList.~PriorityQueue();
}