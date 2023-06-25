#pragma once
#include<iostream>
using namespace std;
#include "Process.h"
//ostream & Process::operator<< (ostream& output, const Process P){}

Process::Process()
{
	CpuTimeForOutput = cpuTime;
	ParentRemainingCPUTime = cpuTime;
	rightChild = NULL;
	leftChild = NULL;
	processID = 0;
	arrivalTime = 0;
	responseTime = 0;
	cpuTime = 0;
	workedTimeOnProcess = 0;
	terminationTime = 0;
	total_IO_duration = 0;
	numberOfTimes_IO_Requested = 0;
	stateOfProcess = New;
	haveMovedAlready = false;
}

// Setter function definition
void Process::setProcessDeadline(int deadline)
{
	ProcessDeadline = deadline;
}

bool Process::isItTerminatedbeforeDeadline() const
{
	return m_isItTerminatedbeforeDeadline;
}

void Process::setIsItTerminatedbeforeDeadline(bool value)
{
	m_isItTerminatedbeforeDeadline = value;
}

int Process::getProcessDeadline() const
{
	return ProcessDeadline;
}

Process::Process(int arrivalTime, int processID, int cpuTime, int Process_dealine, int numberOfTimes_IO_Requested)
{
	ProcessDeadline = Process_dealine;
	CpuTimeForOutput = cpuTime;
	ParentRemainingCPUTime = cpuTime;
	turnaroundDuration = terminationTime - arrivalTime;
	waitingTime = turnaroundDuration - cpuTime;
	SetarrivalTime(arrivalTime);
	SetprocessID(processID);
	SetcpuTime(cpuTime);
	SettnumberOfTimes_IO_Requested(numberOfTimes_IO_Requested);
}

int Process::Gettotal_IO_duration()
{
	return total_IO_duration;
}

bool Process::getIsThatFirstTime()
{
	return isThatFirstTime;
}

void Process::setIsThatFirstTime(bool value)
{
	isThatFirstTime = value;
}

int Process::getTimeStepWhenProcessIsSentToReadyForFirstTime()
{
	return timeStepWhenProcessIsSentToReadyForFirstTime;
}

void Process::setTimeStepWhenProcessIsSentToReadyForFirstTime(int value)
{
	timeStepWhenProcessIsSentToReadyForFirstTime = value;
}

void Process::SetprocessID(int processID)
{
	if (processID >= 0)
	{
		this->processID = processID;
	}
	else
	{
		cout << "processID can not be negative" << endl;
	}
}

void Process::SetCputTimeForOutput(int  cpu_time)
{
	CpuTimeForOutput = cpu_time;
}
void Process::SetarrivalTime(int arrivalTime)
{
	if (arrivalTime >= 0)
	{
		this->arrivalTime = arrivalTime;
	}
	else
	{
		cout << "arrival time can not be negative" << endl;
	}
}

void Process::SetcpuTime(int cpuTime)
{
	if (cpuTime >= 0)
	{
		this->cpuTime = cpuTime;
	}
	else
	{
		cout << "cpuTime can not be negative" << endl;
	}
}

void Process::IncrementTotalIO(int IO_d) {
	total_IO_duration = total_IO_duration + IO_d;
}

void Process::SetIO(int IO_r, int	IO_d, int index)
{
	IO io;
	if (IO_r >= 0 && IO_d >= 0)
	{
		io.IO_R = IO_r;
		io.IO_D = IO_d;
		ListOfIO.enqueue(io);

	}
	else
	{
		cout << "IO Request Time can not be negative" << endl;
	}
}

void Process::SettnumberOfTimes_IO_Requested(int numberOfTimes_IO_Requested)
{
	if (numberOfTimes_IO_Requested >= 0)
	{
		this->numberOfTimes_IO_Requested = numberOfTimes_IO_Requested;
	}
	else
	{
		cout << "number Of Times IO Requested can not be negative" << endl;
	}
}

void Process::SetStateOfProcess(statusOfProcess state)
{
	stateOfProcess = state;
}

void Process::SethaveMovedAlready(bool flag)
{
	haveMovedAlready = flag;
}

int Process::GetResponseTime()
{
	responseTime = timeStepWhenProcessIsSentToReadyForFirstTime - arrivalTime;
	if (responseTime <= 0)
	{
		return 0;
	}
	return responseTime;
}

int Process::GetturnaroundDuration()
{
	turnaroundDuration = terminationTime - arrivalTime;
	return turnaroundDuration;
}

int Process::GetCpuTimeForOutput()
{
	return CpuTimeForOutput;
}

int Process::GetProcessId()
{
	return processID;
}

int Process::GetArrivalTime()
{
	return arrivalTime;
}

int Process::GetCpuTime()
{
	return cpuTime;
}


int  Process::GetNumberOfTimes_IO_Requested()
{
	return numberOfTimes_IO_Requested;
}

statusOfProcess Process::GetStateOfProcess()
{
	return stateOfProcess;
}

bool  Process::GethaveMovedAlready()
{
	return haveMovedAlready;
}

int Process::GetTerminationTime()
{
	return terminationTime;
}

//utilities 
void Process::LoadProcess(int numOfProcess, LinkedQueue<Process*>& newList, ifstream& inputFile)
{
	int arrivalTime, processID, cpuTime, numberOfTimes_IO_Requested, Process_dealine;
	int IORequestTime[2];
	inputFile >> arrivalTime >> processID >> cpuTime >> Process_dealine >> numberOfTimes_IO_Requested;
	Process* p = new Process(arrivalTime, processID, cpuTime, Process_dealine, numberOfTimes_IO_Requested);
	p->SetStateOfProcess(New);
	for (size_t i = 0; i < numberOfTimes_IO_Requested; i++)
	{
		char skip; // to eliminate the comma & brackets from inputting
		inputFile >> skip >> IORequestTime[0] >> skip >> IORequestTime[1] >> skip;
		if (i + 1 != numberOfTimes_IO_Requested)
		{
			inputFile >> skip; // since there is middle comma in between brackets except for last bracket
		}

		p->SetIO(IORequestTime[0], IORequestTime[1], i);
	}
	newList.enqueue(p);
}

void Process::IncrementWorkedTimeOnProcess(int time)
{
	workedTimeOnProcess = workedTimeOnProcess + time;
}

void Process::PrintProcess()
{
	cout << terminationTime << " " << processID << " "
		<< arrivalTime << " " << CpuTimeForOutput << " "
		<< total_IO_duration << " " << waitingTime << " "
		<< responseTime << " " << turnaroundDuration << " " << endl;
}

int Process::GetParentRemainingCPUTime()
{
	return cpuTime;
}

void Process::SetParentRemainingCPUTime(int CT)
{
	ParentRemainingCPUTime = CT;
}

void Process::decrementParentRemainingCPUTime()
{
	ParentRemainingCPUTime--;
}

void Process::SetrightChild(Process* RChild)
{
	rightChild = RChild;
}

//---------------------Forking process utilities----------------------------//
void Process::SetleftChild(Process* LChild)
{
	leftChild = LChild;
}

Process* Process::GetrightChild()
{
	return rightChild;
}

Process* Process::GetleftChild()
{
	return leftChild;
}

void Process::Setforkedtimes(int x)
{
	forkedtimes = x;
}

int Process::Getforkedtimes()
{
	return forkedtimes;
}

void Process::incrementforkedtimes()
{
	forkedtimes++;
}
//--------------------------------------------------------------------------//

void Process::decrementcputime()
{
	if (cpuTime > 0)
		cpuTime--;
	else
		cpuTime = 0;
}

bool Process::IsIoListEmpty()
{

	return ListOfIO.isEmpty();
}

IO Process::GetIO()
{
	IO io;
	ListOfIO.dequeue(io);
	return io;
}

int Process::GetIO_R()
{
	IO io;
	ListOfIO.peek(io);
	IO_D = io.IO_D;
	return io.IO_R;
}

int Process::GetIO_R_ByDeque()
{
	IO io;
	ListOfIO.dequeue(io);
	IO_D = io.IO_D;
	return io.IO_R;
}

int Process::GetIO_D()
{
	return IO_D;
}

void Process::incrementWorkingtimeRUN()
{
	workedtimesinRUN++;
	workingtimesRUNforIO++;
}

int Process::GetworkingtimesRUN()
{
	return workedtimesinRUN;
}

int Process::GetworkingtimesRUNforIO()
{
	return workingtimesRUNforIO;
}

void Process::SetworkingtimesRUNforIO(int x)
{
	workingtimesRUNforIO = x;
}

int Process::GetWaitingTime()
{
	return waitingTime;
}

int Process::GetWaitingTimeForOutput()
{

	waitingTimeForOutput = terminationTime - arrivalTime - CpuTimeForOutput;
	if (waitingTimeForOutput > 0)
	{
		return waitingTimeForOutput;
	}
	else
	{
		return 0; //process killed or so
	}

}

void Process::Incrementwaitingtime()
{
	waitingTime++;
}

void Process::SetTerminationTime(int timestep)
{
	terminationTime = timestep;
}

void Process::SetWaitingTime(int waitTime)
{
	waitingTime = waitTime;
}

void Process::incrementWorkingtimeRUNRR()
{
	workedtimesinRUNRR++;
}

int Process::GetworkingtimesRUNRR()
{
	return workedtimesinRUNRR;
}

void Process::setworkingtimesRUNRR(int x)
{
	workedtimesinRUNRR = x;
}

void Process::SetIO_D(int x)
{
	IO_D = x;
}

void Process::IncrementCurrentDuration()
{
	currentDuration++;
}

int Process::GetCurrentDuration()
{
	return currentDuration;
}

void Process::SetCurrentDuration()
{
	currentDuration = 0;
}

Process::~Process()
{
	if (leftChild != NULL)
		delete leftChild;
	if (rightChild != NULL)
		delete rightChild;
}