#pragma once
#include "Processor.h"
class Scheduler;


class ProcessorRR :public Processor
{
	//Scheduler *scheduler;
	static int timeSlice; // static since its just one fixed time slice for all round robin processors
	LinkedQueue<Process*> readyList; // so that we can kill or remove process from the list
	int noOfRDYList = 0;

public:
	ProcessorRR(Scheduler* Sptr) :Processor(Sptr) {};			//Constructor and Destructor
	~ProcessorRR();
	void SchedulingAlgorithm(int Timestep);						//Scheduling
	//geting type of the processor
	TypeOfProcessor GetTypeOfProcessor();
	//getting first ready process in the ready list accourding to processor type
	Process* GetfirstReady();									//readyList utilities
	Process* PeekfirstReady();
	//add to ready list accourding to the process algorithm
	void AddToRDYQueue(Process* P);
	void ResethaveMovedAlreadyProcesses();
	//getter
	int GetnumberOfProcessInReady();
	bool isReadyEmpty();
	//printing ready
	void PrintRDY();
	//loading information for RR processor only
	void LoadProcessorRR(int timeSliceRR, int numOfProcessorRR, int& sumOfProcessors, Processor* listOfProcessorss[200], ifstream& inputFile);
	//setter and getters to time slice
	void SetTimeSlice(int timeSlice);							//Setting and Getting TimeSlice
	int getTimeSlice();


};

