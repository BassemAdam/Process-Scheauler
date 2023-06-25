#pragma once
#include "Processor.h"
#include "Process.h"
class Scheduler;
//#include "Scheduler.h"


class ProcessorEDF :public Processor
{
	PriorityQueue<Process*> readyList;						// so that we can kill or remove process from the list

public:
	ProcessorEDF(Scheduler* Sptr) :Processor(Sptr) {};		//Constructor and Destructor
	~ProcessorEDF();
	void SchedulingAlgorithm(int Timestep);					//Schedueling
	TypeOfProcessor GetTypeOfProcessor();
	//getting the first ready in the ready list
	Process* GetfirstReady();								
	Process* PeekfirstReady();                  //dequeue from the ready list
	//adding to the ready list accourding to the processor algorithm
	void AddToRDYQueue(Process* P);
	void ResethaveMovedAlreadyProcesses();
	int GetnumberOfProcessInReady();
	bool isReadyEmpty();
	//printing the ready list
	void PrintRDY();
	//loading information for EDF processor only
	void LoadProcessorEDF(int numOfProcessorEDF, int& sumOfProcessors, Processor* listOfProcessorss[200], ifstream& inputFile);
};


