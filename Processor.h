#ifndef Processor_H
#define Processor_H
#pragma once
using namespace std;
#include "LinkedQueue.h"
#include "Process.h"
#include "LinkedList.h"
#include "PriorityQueue.h"
//#include"UI.h"
//#include"Scheduler.h"

class Scheduler;

enum statusOfProcessor
{
	idle = 0,
	busy = 1
};

enum TypeOfProcessor
{
	FCFS = 1,
	SJF = 0,
	RR = 2,
	EDF = 3
};

class Processor
{
	//LinkedQueue<Process>  blkList;
	//Scheduler* S;

protected:

	Scheduler* scheduler;
	statusOfProcessor stateOfProcessor;
	int currentExpectedFinishTime;							 // its not a must but will help us later in scheduler class functions
	double processorLoad;
	int totalProcessorBusyTime;
	int totalProcessorIdleTime;
	int numberOfProcessInReady;
	Process* RUN;

	//calculation utilities
	double Processor_Load = 0;
	double Processor_utilization = 0;
	bool isoverheat = false;
	int overheattime = 0;

public:
	Processor(Scheduler* Sptr); 		//Constructor and Destructor
	virtual ~Processor();
	virtual void SchedulingAlgorithm(int Timestep) = 0;			//Scheduling
	
	//return processor type
	virtual TypeOfProcessor GetTypeOfProcessor() = 0;
	
	//check whether the process needs IO 
	bool I_ORequirey(Process P);
	

	//seters 
	void SetStatusOfProcessor(statusOfProcessor s);				//Setting and Getting StatusOfProcessor
	void SetRunningProcess(Process* p);
	void SetProcessorLoad(int load);


	statusOfProcessor GetStatusOfProcessor();
	//getting firt process in the ready list
	virtual Process* GetfirstReady() = 0;						//readyList utilities 
	virtual Process* PeekfirstReady() = 0;
	//adding to ready list accourding to the Processor algorithm
	virtual void AddToRDYQueue(Process* P) = 0;
	virtual void ResethaveMovedAlreadyProcesses() = 0;
	virtual int GetnumberOfProcessInReady() = 0;
	virtual bool isReadyEmpty() = 0;
	virtual void PrintRDY() = 0;								//Printing

	void PrintProcessor();
	void IncrementExpectedFinishTime(int time);
	void DecrementExpectedFinishTime(int time);
	
	//calculations
	double GetProcessorLoad(int time);
	double GetProcessorUtilization();
	


	//setting the process to RUN sate
	void ToRUN_State(Process* P);
	//Setting and Getting RunningProcess
	Process* GetRunningProcess();
	int GetcurrentExpectedFinishTime();

	//Bonus utilities 
	void Setisoverheat(bool x);
	bool Getisoverheat();
	void Setoverheattime(int overheat);
	int  Getoverheattime();
	void decremntOverHeatTime();

};

#endif
