#pragma once
#include "Processor.h"
#include "Process.h"
class Scheduler;
//#include"Scheduler.h"

struct SIGKILL
{
	int PID;
	int time;
};

//ProcessorFCFS::ProcessorFCFS(Scheduler* Sptr) :Processor(Sptr) {};

class ProcessorFCFS :public Processor
{

	LinkedList<Process*> readyList;        // so that we can kill or remove process from the list
	static LinkedQueue<SIGKILL> SignalKill;
	static double ForkProb;
	static int SignalListCount;

public:

	ProcessorFCFS(Scheduler* Sptr) : Processor(Sptr) { scheduler = Sptr; };	//Constructor and Destructor
	~ProcessorFCFS();
	void SchedulingAlgorithm(int Timestep);								//Scheduling
	//get type of the processor
	TypeOfProcessor GetTypeOfProcessor();
	//getting first Process ready in the ready list
	Process* GetfirstReady();								//readyList utilities 
	Process* PeekfirstReady();

	//adding to ready list
	void AddToRDYQueue(Process* P);

	void ResethaveMovedAlreadyProcesses();
	//geter
	int GetnumberOfProcessInReady();
	bool isReadyEmpty();
	//print ready list
	void PrintRDY();
	//load fun for Sigkill & forking probability
	void LoadProcessorFCFS(int numOfProcessorFCFS, int& sumOfProcessors, Processor* listOfProcessorss[200], ifstream& inputFile);
	void setForkProb(double ForkP);							//Setting and Getting ForkProb
	double getForkProb();
	//adding to siganl kill list
	void AddSignalKill(SIGKILL Sg);
	SIGKILL GetSignalKill();

	void DeleteRandomProcess(int index); //phase one
	
	//killing Process
	bool Kill(Process* P);
	bool killFCFS(Process* P, int timestep, int x);
	//seter get for signal list
	int GestSignailListCount();
	void SetSignallistcount(int count);
};

