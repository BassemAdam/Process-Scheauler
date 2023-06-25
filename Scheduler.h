#ifndef Scheduler_H
#define Scheduler_H
#pragma once
#include "Process.h"
#include "LinkedQueue.h"
#include "LinkedList.h"
#include "PriorityQueue.h"
#include "Processor.h"
#include "ProcessorFCFS.h"
#include "ProcessorSJF.h"
#include "ProcessorRR.h"
#include "ProcessorEDF.h"
#include"UI.h"
//class UI;
using namespace std;
#include<iostream>
#include <fstream>	
#include <string>

class Scheduler
{
	LinkedQueue<Process*>  newList;					//Process Lists
	PriorityQueue<Process*>  trmList;
	LinkedQueue<Process*>  blkList;
	Processor** listOfProcessors;

	Processor* P;
	Process* pProcess;


	int rTF;
	int maxW;
	int sTL;
	int LongestQueueIndex;
	int ShortestQueueIndex;
	int TimeStep;
	int ProcessesNo;
	int TerminationListCount;					//Number of processes in states
	int BlockListCount;
	int numberOfProcessInRun;
	int totalNumberOfProcesses;
	int noOfProcessorFCFS;						//Number of processors of each type
	int noOfProcessorSJF;
	int noOfProcessorEDF=0;
	int ProcessTerminatedBeforeDeadline = 0;
	int noOfProcessorRR;
	int totalNumberOfProcessors;
	int migrations = 0;
	int migrationsDueToRTF = 0;
	int migrationsDueToMaxW = 0;
	int numOfProcessStoled = 0;
	int numOfProcessKilled = 0;
	int numOfProcessForked = 0;

	UI* pUI;

	int blklistcount = 0;
	int overheatconst=0;

public:
	Scheduler();
	~Scheduler();
	//simulation fun 
	void SimulationFun();

	//chk RDY lists every time step
	void WorkStealing();
	bool Output(string filenamee);
	//From RUN to BLK , called by the processors
	void IO_requestHandling(Processor* pProc);
	//void IO_requestHandling(Processor* pProc); ///Amira
	void GetLongest_ShortestReady(); //geting the shortest and longest ready list for work stealing
	double GetStealLimit();

	void incrementNumberOfProcessor();
	//loading fun
	void FileLoad(string filename);
	
	void AddToTrmList(Process* P);
	
	void AddToBlockList(Process* P);
	
	bool RunAvailability();					//Checks if processor is busy 
	
	//getters
	int GetTimeStep();
	int GetProcessesNo();
	int GetTLCount();
	int GetnoOfProcessorFCFS();
	int GetnoOfProcessorRR();
	int GetnoOfProcessorSJF();
	int GetTotalNoOfProcessor();

	//Printing functions
	void PrintBLK();						
	void PrintTRM();
	void PrintRDY();
	void PrintRUN();
	
	
	//ORPH Process
	void ORPH(Process* P);
	
	//getting the shortest ready amony FCFS processors 
	int GetShortestRDYProccessorFCFS();

	//Forking process
	void ProcessForking(Process* P);
	
	void decrementnoOfprocessRUN();
	void incrementnoOfProcessInRUN();
	
	//getting shortest ready among SJF processors
	int GetShortestRDYProccessorSJF();
	
	//getting shortest ready among RR processors
	int GetShortestRDYProccessorRR();
	
	//moving from new list to ready list
	void NEWtoRDY();
	
	//moving from block list to ready list
	bool BLKTORDY();
	
	// moving from run to termination list if the cpu time is over 
	bool RUNTOTRM(Process* P, Processor* Pproc);

	//Process migration from RR to SJF Processors 
	void ProcessMigrationRR(Process* P, Processor* Pproc);
	
	//process migration from FCFS to RR Processors 
	void ProcessMigrationFCFS(Process* P, Processor* Pproc);
	
	//Processor overheating (bonus)
	void ProcessorOverheating();
	
	
};
#endif