#pragma once
#include "Processor.h"
#include "Process.h"
class Scheduler;
//#include "Scheduler.h"


class ProcessorSJF :public Processor
{
	PriorityQueue<Process*> readyList;								//So that we can kill or remove process from the list
	int noOfRDYList = 0;
public:
	ProcessorSJF(Scheduler* Sptr) :Processor(Sptr) {};				//Constructor and Destructor
	~ProcessorSJF();
	void SchedulingAlgorithm(int Timestep);							//Schedueling
	//get the type of the processor 
	TypeOfProcessor GetTypeOfProcessor();
	//getting the first ready process accourding to the processor algorithm
	Process* GetfirstReady();										//readyList utilities
	Process* PeekfirstReady();
	//add to ready list 
	void AddToRDYQueue(Process* P);
	void ResethaveMovedAlreadyProcesses();
	int GetnumberOfProcessInReady();
	bool isReadyEmpty();
	//printing the ready list
	void PrintRDY();
	//loading information for SJF Processor only 
	void LoadProcessorSJF(int numOfProcessorSJF, int& sumOfProcessors, Processor* listOfProcessorss[200], ifstream& inputFile);

};

