#pragma once
#include<iostream>
#include "LinkedQueue.h"
using namespace std;
#include <fstream>	
#include <string>

enum statusOfProcess
{
	New = 0,
	Ready = 1,
	Running = 2,
	Blocked = 3,
	Terminated = 4
};

struct IO
{
	//first number represents calculated relative start of execution, second number represents how many timeSteps it takes at blk 
	int IO_R;
	int	IO_D;
};

class Process
{
	//int countIoRequest;
	Process* rightChild;
	Process* leftChild;
	int processID;
	int workingtimesRUNforIO=0;
	int arrivalTime;		//Determine when we send it from New to Ready
	int responseTime;		//First time for processor to respond to process, = arrival time - time at which process is sent to ready gets cpu for first time
	int cpuTime;
	int CpuTimeForOutput;	//Needed timesteps to finish its tasks using processor
	int workedTimeOnProcess;
	int terminationTime;	//Time when the process finishes execution
	int turnaroundDuration=0;
	int waitingTime=0;
	int waitingTimeForOutput=0;
	int ProcessDeadline = 0;
	bool m_isItTerminatedbeforeDeadline = 0;
	LinkedQueue<IO>ListOfIO;
	int total_IO_duration;
	int numberOfTimes_IO_Requested;
	statusOfProcess stateOfProcess;
	bool isThatFirstTime = true;
	int timeStepWhenProcessIsSentToReadyForFirstTime=0;
	bool haveMovedAlready;	//to make sure that we move the process only once in the same time step
	int forkedtimes = 0;
	int ParentRemainingCPUTime;
	int workedtimesinRUN = 0;
	int workedtimesinRUNRR = 0;
	int remainingTime = 0;
	int RTF = 0;
	int MaxW = 0;
	int IO_D = 0;
	int currentDuration = 0;

public:
	Process();
	
	
	void IncrementTotalIO(int IO_d);
	Process(int arrivalTime, int processID, int cpuTime, int Process_dealine, int numberOfTimes_IO_Requested);
	bool isItTerminatedbeforeDeadline() const;
	void setIsItTerminatedbeforeDeadline(bool value);

	friend ostream& operator << (ostream& output, const Process& P)
	{
		output << P.processID;
		return output;
	}

	//Setteres
	void setProcessDeadline(int deadline);	// Setter function declaration
	void SetCputTimeForOutput(int processID);
	bool IsIoListEmpty();
	void SetprocessID(int processID);
	void SetarrivalTime(int arrivalTime);
	void SetcpuTime(int cpuTime);
	void SetIO(int IO_R, int	IO_D, int index);
	void SettnumberOfTimes_IO_Requested(int numberOfTimes_IO_Requested);
	void SetStateOfProcess(statusOfProcess state);
	void SethaveMovedAlready(bool flag);
	void setIsThatFirstTime(bool value);
	void setTimeStepWhenProcessIsSentToReadyForFirstTime(int value);
	void SetParentRemainingCPUTime(int CT);
	void SetrightChild(Process* RChild);
	void SetleftChild(Process* LChild);
	void Setforkedtimes(int x);
	void SetTerminationTime(int timestep);
	void SetWaitingTime(int waitTime);
	void SetIO_D(int x);
	void setworkingtimesRUNRR(int x);
	void SetCurrentDuration();
	void SetworkingtimesRUNforIO(int x);


	//Getters
	int GetResponseTime();
	int Gettotal_IO_duration();
	int GetWaitingTimeForOutput();
	bool getIsThatFirstTime();
	int getTimeStepWhenProcessIsSentToReadyForFirstTime();
	int GetturnaroundDuration();
	int GetCpuTimeForOutput();
	int GetProcessId();																			
	int GetArrivalTime();
	int GetCpuTime();
	IO GetIO();
	int getProcessDeadline() const;			// Getter function declaration
	int GetNumberOfTimes_IO_Requested();
	statusOfProcess GetStateOfProcess();
	bool GethaveMovedAlready();
	int GetTerminationTime();
	int GetParentRemainingCPUTime();
	Process* GetrightChild();
	Process* GetleftChild();
	int Getforkedtimes();
	int GetIO_R();
	int GetworkingtimesRUN();
	int GetWaitingTime();
	int GetworkingtimesRUNRR();
	int GetIO_D();
	int GetCurrentDuration();
	int GetIO_R_ByDeque();
	int GetworkingtimesRUNforIO();

	void LoadProcess(int numOfProcess, LinkedQueue<Process*>& newList, ifstream& inputFile);	//utilities
	void IncrementWorkedTimeOnProcess(int time);
	void PrintProcess();

	
	
	void decrementParentRemainingCPUTime();
	void incrementforkedtimes();
	void decrementcputime();
	void incrementWorkingtimeRUN();
	void Incrementwaitingtime();
	void incrementWorkingtimeRUNRR();
	void IncrementCurrentDuration();
	
	

	~Process();
};

