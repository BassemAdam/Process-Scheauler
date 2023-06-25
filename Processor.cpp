#pragma once
#include "Processor.h"
#include"Scheduler.h"

//Percentage of time the processor is executing the process
#ifndef GetProcessorUtilization
double Processor::GetProcessorUtilization()
{
	if ((totalProcessorBusyTime + totalProcessorIdleTime) > 0)
	{
		return (totalProcessorBusyTime * 100) / (totalProcessorBusyTime + totalProcessorIdleTime);
	}
	else
	{
		return 0;
	}
}
#endif

int Processor::GetcurrentExpectedFinishTime()
{
	return currentExpectedFinishTime;
}

void Processor::IncrementExpectedFinishTime(int time)
{
	currentExpectedFinishTime = currentExpectedFinishTime + time;
}

void Processor::DecrementExpectedFinishTime(int time)
{
	currentExpectedFinishTime = currentExpectedFinishTime - time;
}

double Processor::GetProcessorLoad(int time)
{
	processorLoad = (double)(totalProcessorBusyTime * 100) / time;
	return processorLoad;
}

void Processor::SetProcessorLoad(int load)
{
	processorLoad = load;
}

bool Processor::I_ORequirey(Process P)
{
	if (P.GetNumberOfTimes_IO_Requested() == 0)
		return false;
	else
		return true;
}

void Processor::SetStatusOfProcessor(statusOfProcessor s)
{
	stateOfProcessor = s;
}

statusOfProcessor Processor::GetStatusOfProcessor()
{
	return stateOfProcessor;
}

void Processor::PrintProcessor()
{
	for (int i = 0; i < scheduler->GetTotalNoOfProcessor(); i++)
	{
		cout << "Processor " << i + 1 << " ";
	}
}

void Processor::SetRunningProcess(Process* p)
{
	RUN = p;
}

Process* Processor::GetRunningProcess()
{
	if (RUN != NULL) { return RUN; }
	else { return NULL; }

}

//------------------------- Functions used for bonus ----------------------------------//
void Processor::Setisoverheat(bool x)
{
	isoverheat = x;
}

bool Processor::Getisoverheat()
{
	return isoverheat;
}

void Processor::Setoverheattime(int overheat)
{
	overheattime = overheat;
}

int  Processor::Getoverheattime()
{
	return overheattime;
}

void Processor::decremntOverHeatTime()
{
	overheattime--;
}
//------------------------------------------------------------------------------------//

Processor::Processor(Scheduler* Sptr)
{
	scheduler = Sptr;
	currentExpectedFinishTime = 0;
	processorLoad = 0;
	totalProcessorBusyTime = 0;
	totalProcessorIdleTime = 0;
	numberOfProcessInReady = 0;
	stateOfProcessor = idle;
}
void Processor::ToRUN_State(Process* P)
{
	P->SetStateOfProcess(Running);
	stateOfProcessor = busy;
	scheduler->incrementnoOfProcessInRUN();
	SetRunningProcess(P);
	P->decrementcputime();
	P->incrementWorkingtimeRUN();
	P->SethaveMovedAlready(true);
}
Processor::~Processor()
{
	delete RUN;
}






