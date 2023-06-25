#pragma once
using namespace std;
#include<iostream>
#include"Scheduler.h"

Scheduler::Scheduler()
{
	listOfProcessors = new Processor*;
	pUI = new UI(this);
	//P = new Processor(this);
	pProcess = NULL;
	rTF = 0;
	maxW = 0;
	sTL = 0;
	LongestQueueIndex = -1;
	ShortestQueueIndex = -1;
	TimeStep = 1;
	ProcessesNo = 0;
	TerminationListCount = 0;
	BlockListCount = 0;
	numberOfProcessInRun = 0;
	totalNumberOfProcesses = 0;
	noOfProcessorFCFS = 0;
	noOfProcessorSJF = 0;
	noOfProcessorRR = 0;
	totalNumberOfProcessors = 0;

}

void Scheduler::WorkStealing()
{
	GetLongest_ShortestReady();					//determine which is the shortest & longest queue
	int stealLimit = GetStealLimit();			//Calculate steal limit
	while (stealLimit > 40)						//while steallimit more than 0.4 do the stealing process
	{

		Process* p;
		p = listOfProcessors[LongestQueueIndex]->GetfirstReady();
		//cout << "###########  will transfer this process" << p->GetProcessId() << endl;
		listOfProcessors[ShortestQueueIndex]->AddToRDYQueue(p);
		numOfProcessStoled++;
		//recalculates the steallimit
		stealLimit = GetStealLimit(); //calculate steal limit but this time with the same longest and shortest queue 
	}
}

void Scheduler::IO_requestHandling(Processor* pProc)
{
	if (pProc->GetRunningProcess() != NULL)
	{
		Process* process1 = pProc->GetRunningProcess();
		//check if process in run has io list or not
		if (!process1->IsIoListEmpty())
		{
			//check if process in run has io_D time has come after the duration needed to be executed in run
			//should only peek
			int ReqTime = process1->GetIO_R();
			int workingtimeonProcess = process1->GetworkingtimesRUNforIO();
			if (workingtimeonProcess == ReqTime)
			{
				//cout<<"##### request to block list ######"<<endl;
				//cout << "process1->GetworkingtimesRUN()" << process1->GetworkingtimesRUN() << endl;
				//cout <<"process1->GetIO_R( )"<< process1->GetIO_R() <<endl;
				process1->GetIO_R_ByDeque();

				process1->SetCurrentDuration();
				AddToBlockList(process1);
				pProc->SetRunningProcess(NULL);
				pProc->SetStatusOfProcessor(idle);
				process1->SethaveMovedAlready(true);
				process1->SetworkingtimesRUNforIO(0);
			}
		}
	}
}

void Scheduler::GetLongest_ShortestReady()
{
	int max = -9999999;
	int min = 9999999;
	for (int i = 0; i < totalNumberOfProcessors; i++)
	{
		if (listOfProcessors[i]->Getisoverheat() == false)
		{
			int timeTakenToFinish = listOfProcessors[i]->GetcurrentExpectedFinishTime();
			if (timeTakenToFinish > max)
			{
				max = timeTakenToFinish;
				LongestQueueIndex = i;
			}
			if (timeTakenToFinish < min)
			{
				min = timeTakenToFinish;
				ShortestQueueIndex = i;
			}
		}
	}
}

double Scheduler::GetStealLimit()
{
	int LongestQueueFirst = listOfProcessors[LongestQueueIndex]->GetcurrentExpectedFinishTime();
	int ShortestQueueFirst = listOfProcessors[ShortestQueueIndex]->GetcurrentExpectedFinishTime();
	if (LongestQueueFirst > 0)
	{
		int value = (LongestQueueFirst - ShortestQueueFirst) * 100 / LongestQueueFirst;
		return value;
	}
}

void Scheduler::FileLoad(string filename)
{
	ifstream inputFile;
	inputFile.open(filename + ".txt", ios::in);
	if (inputFile.fail())
	{
		cout << "Error opening file" << endl;
		exit(1);
	}
	else
	{
		//inputting 1st line
		int numOfProcessorFCFS;
		int numOfProcessorSJF;
		int numOfProcessorRR;
		int numOfProcessorEDF;

		inputFile >> numOfProcessorFCFS >> numOfProcessorSJF >> numOfProcessorRR >> numOfProcessorEDF;

		int sumOfProcessors = 0;
		totalNumberOfProcessors = numOfProcessorFCFS + numOfProcessorSJF + numOfProcessorRR + numOfProcessorEDF;
		noOfProcessorFCFS = numOfProcessorFCFS;
		noOfProcessorRR = numOfProcessorRR;
		noOfProcessorSJF = numOfProcessorSJF;
		noOfProcessorEDF = numOfProcessorEDF;
		//inputting 2nd line
		int timeSliceRR;
		inputFile >> timeSliceRR;

		ProcessorRR p(this);
		//ProcessorFCFS pFCFS(this);
		ProcessorFCFS* pFCFS = new ProcessorFCFS(this);
		ProcessorSJF pSJF(this);
		ProcessorEDF pEDF(this);

		listOfProcessors = new Processor * [numOfProcessorFCFS + numOfProcessorSJF + numOfProcessorRR + numOfProcessorEDF];

		//p.SetTimeSlice(timeSliceRR);
		pFCFS->LoadProcessorFCFS(numOfProcessorFCFS, sumOfProcessors, listOfProcessors, inputFile);
		pSJF.LoadProcessorSJF(numOfProcessorSJF, sumOfProcessors, listOfProcessors, inputFile);
		p.LoadProcessorRR(timeSliceRR, numOfProcessorRR, sumOfProcessors, listOfProcessors, inputFile);
		pEDF.LoadProcessorEDF(numOfProcessorEDF, sumOfProcessors, listOfProcessors, inputFile);

		//inputting 3rd line
		int forkProbability;
		inputFile >> rTF >> maxW >> sTL >> forkProbability >> overheatconst;
		pFCFS->setForkProb(forkProbability);

		//inputting 4th line
		inputFile >> totalNumberOfProcesses;

		//inputting process lines
		for (size_t i = 0; i < totalNumberOfProcesses; i++)
		{
			pProcess->LoadProcess(totalNumberOfProcesses, newList, inputFile);
		}


		//inputting SIG KILL section
		int PIDtemp, timetemp;
		//since we dont know the number of kill signals it should input it until there is nothing to be inputted
		int signallistcount = 0;
		while (inputFile >> timetemp >> PIDtemp)
		{
			SIGKILL Stemp;
			Stemp.PID = PIDtemp;
			Stemp.time = timetemp;
			pFCFS->AddSignalKill(Stemp);
			signallistcount++;
		}
		pFCFS->SetSignallistcount(signallistcount);


	}

}

void Scheduler::AddToTrmList(Process* P)
{
	P->SetStateOfProcess(Terminated);
	trmList.insert(P, P->GetTerminationTime());
	TerminationListCount++;
}

void Scheduler::AddToBlockList(Process* P)
{
	blkList.enqueue(P);
	P->SetStateOfProcess(Blocked);
	BlockListCount++;
	numberOfProcessInRun--;
}

bool Scheduler::RunAvailability()
{
	if (P->GetStatusOfProcessor() == busy)
		return false;
	else
		return true;
}

int Scheduler::GetTimeStep()
{
	return TimeStep;
}

int Scheduler::GetProcessesNo()
{
	return ProcessesNo;
}

int Scheduler::GetTLCount()
{
	return TerminationListCount;
}

int Scheduler::GetnoOfProcessorFCFS()
{
	return noOfProcessorFCFS;
}

int Scheduler::GetnoOfProcessorRR()
{
	return noOfProcessorRR;
}

int Scheduler::GetnoOfProcessorSJF()
{
	return noOfProcessorSJF;
}

int Scheduler::GetTotalNoOfProcessor()
{
	return totalNumberOfProcessors;
}

void Scheduler::PrintRUN()
{
	cout << numberOfProcessInRun << " RUN: ";
	for (size_t i = 0; i < totalNumberOfProcessors; i++)
	{
		if (listOfProcessors[i]->GetRunningProcess() != NULL)
		{
			cout << listOfProcessors[i]->GetRunningProcess()->GetProcessId() << "(P" << i + 1 << ")";
			if (i + 1 != totalNumberOfProcessors)
			{
				cout << ", ";
			}
		}
	}
	cout << endl;
}

void Scheduler::PrintRDY()
{
	for (int i = 0; i < totalNumberOfProcessors; i++)
	{
		Processor* Ptemp = NULL;

		//  FCFS = 1,
		//	SJF = 0,
		//	RR = 2
		string TypeOfProcessor;
		if (listOfProcessors[i]->GetTypeOfProcessor() == FCFS)
		{

			TypeOfProcessor = " FCFS ";
			Ptemp = new ProcessorFCFS(this);
			Ptemp = listOfProcessors[i];
		}
		else if (listOfProcessors[i]->GetTypeOfProcessor() == SJF)
		{
			TypeOfProcessor = " SJF  ";
			Ptemp = new ProcessorSJF(this);
			Ptemp = listOfProcessors[i];
		}
		else if (listOfProcessors[i]->GetTypeOfProcessor() == RR)
		{
			TypeOfProcessor = " RR   ";
			Ptemp = new ProcessorRR(this);
			Ptemp = listOfProcessors[i];
		}
		else if (listOfProcessors[i]->GetTypeOfProcessor() == EDF)
		{
			TypeOfProcessor = " EDF  ";
			Ptemp = new ProcessorEDF(this);
			Ptemp = listOfProcessors[i];
		}
		if (i + 1 <= 9)
		{
			cout << "Processor " << i + 1 << "   [" << TypeOfProcessor << "]: ";
			if(listOfProcessors[i]->Getisoverheat()==true)
			{
				cout << "overheated: ";
			}
		}
		else
		{
			cout << "Processor " << i + 1 << "  [" << TypeOfProcessor << "]: ";
			if (listOfProcessors[i]->Getisoverheat() == true)
			{
				cout << "overheated: ";
			}
		}
		
		Ptemp->PrintRDY();
		cout << endl;
	}
}

void Scheduler::PrintBLK()
{
	cout << BlockListCount << " BLK: ";
	blkList.Print();
	cout << " " << endl;
}

void Scheduler::PrintTRM()
{
	cout << TerminationListCount << " TRM: ";
	trmList.Print();
	cout << " " << endl;
}

void Scheduler::SimulationFun()
{
	UI_Mode choice;
	//if the time step =1 will ask the user to choose the mode he wish to proceed
	if (TimeStep == 1)
	{
		choice = pUI->which_MODE();
	}
	//loading the input file 
	FileLoad("KillFrok");


	//at each timestep	
	string filename;
	int count = 0;
	int counterForRun = 0;
	bool allowstealing = false;
	while ((totalNumberOfProcesses) != (TerminationListCount))
	{
		//inserting ner process from new list to ready list 
		NEWtoRDY();

		//everytime step checks with a very low probability if there is a process to be overheated 
		int r = (rand() % 100) + 1;
		if (r < 10)
		{
			ProcessorOverheating();
		}

		//calling scheduling algo  
		for (size_t i = 0; i < totalNumberOfProcessors; i++)
		{

			listOfProcessors[i]->SchedulingAlgorithm(TimeStep);
		}

		//every stl period function workstealing get called
		if (allowstealing)
		{
			WorkStealing();
			allowstealing = false;
		}
		if (TimeStep % sTL == 0)
		{
			allowstealing = true;
		}
		
		//every time step checks if there is a process in the block list needs to move to the ready list
		BLKTORDY();



		//reset all process haveMovedAlready to false
		//reset all process in blk list
		for (size_t i = 0; i < BlockListCount; i++)
		{
			Process* tempP = new Process();
			blkList.dequeue(tempP);
			tempP->SethaveMovedAlready(false);
			blkList.enqueue(tempP);
		}

		//reset all process in Run
		for (size_t i = 0; i < totalNumberOfProcessors; i++)
		{
			Process* tempP = new Process();
			tempP = listOfProcessors[i]->GetRunningProcess();
			if (tempP)
			{
				tempP->SethaveMovedAlready(false);
			}
		}

		//reset all process in ready
		for (size_t i = 0; i < totalNumberOfProcessors; i++)
		{
			listOfProcessors[i]->ResethaveMovedAlreadyProcesses();
		}

		//choose the display method depending on the mode chosen above 
		if (choice == MODE_INTERACTIVE)
		{
			pUI->Interactive_Mode();
		}
		else if (choice == MODE_STEP_BY_STEP)
		{
			pUI->Step_By_step_Mode();
		}

		TimeStep++;
	}
	if (choice == MODE_SILENT)
	{
		pUI->Silent_Mode();

	}
	//generating the output file after the simulation is done 
	Output("Outputfile6.txt");
	cout << "The Program ended Successully" << endl;
}


void Scheduler::incrementNumberOfProcessor()
{
	numOfProcessKilled++;
}

void Scheduler::ProcessForking(Process* P)
{
	if (P->GetCpuTime() > 1)
	{
			if (P->GetStateOfProcess()==Running)
			{
				int s = P->Getforkedtimes();
				if (s < 2)
				{
					Process* ChildP = new Process();
					numOfProcessForked++;
					int ChildAT = TimeStep;
					int ChildPID = ++totalNumberOfProcesses;
					int ChildCT = P->GetParentRemainingCPUTime();

					ChildP->SetarrivalTime(ChildAT);
					ChildP->SetprocessID(ChildPID);
					ChildP->SetcpuTime(ChildCT);
					ChildP->SetCputTimeForOutput(P->GetParentRemainingCPUTime());

					int x = 0;
					if (P->GetrightChild() == NULL)
					{
						P->SetrightChild(ChildP);
						x = GetShortestRDYProccessorFCFS();
						listOfProcessors[x]->AddToRDYQueue(ChildP);
					}
					else if (P->GetleftChild() == NULL)
					{
						P->SetleftChild(ChildP);
						x = GetShortestRDYProccessorFCFS();
						listOfProcessors[x]->AddToRDYQueue(ChildP);
					}
					P->incrementforkedtimes();
				}
			}
	}
	

}


int Scheduler::GetShortestRDYProccessorFCFS()
{
	int min = 9999999;
	int ShortestQueueIndexFCFS = 0;
	for (int i = 0; i < noOfProcessorFCFS; i++)
	{
		if (listOfProcessors[i]->Getisoverheat() == false)
		{
			int timeTakenToFinish = listOfProcessors[i]->GetcurrentExpectedFinishTime();
			if (timeTakenToFinish < min)
			{
				min = timeTakenToFinish;
				ShortestQueueIndexFCFS = i;
			}
		}
	}
	return ShortestQueueIndexFCFS;
}

void Scheduler::ORPH(Process* P)
{
	if (P->GetStateOfProcess() == Terminated)
	{
		if (P->GetleftChild() != NULL)
		{
			if (P->GetleftChild()->GetStateOfProcess() == Ready)
			{
				for (int i = 0; i < totalNumberOfProcessors; i++)
				{
					for (int j = 0; j < listOfProcessors[i]->GetnumberOfProcessInReady(); j++)
					{
						Process* firstready = listOfProcessors[i]->GetfirstReady();
						if (P->GetleftChild()->GetProcessId() == firstready->GetProcessId())
						{
							if (P->GetleftChild() != nullptr)
							{
								P->GetleftChild()->SetTerminationTime(TimeStep);
							}
							numOfProcessKilled++;
							AddToTrmList(P->GetleftChild());

						}
						else
						{
							listOfProcessors[i]->AddToRDYQueue(firstready);
						}
					}
				}
			}
			else if (P->GetleftChild()->GetStateOfProcess() == Running)
			{
				for (int i = 0; i < totalNumberOfProcessors; i++)
				{
					if (listOfProcessors[i]->GetRunningProcess() == P->GetleftChild())
					{
						if (P->GetleftChild() != nullptr)
						{
							P->GetleftChild()->SetTerminationTime(TimeStep);
						}
						numOfProcessKilled++;
						AddToTrmList(P->GetleftChild());
						listOfProcessors[i]->SetRunningProcess(NULL);
						listOfProcessors[i]->SetStatusOfProcessor(idle);
						numberOfProcessInRun--;
					}
				}
			}

		}
		if (P->GetrightChild() != NULL)
		{
			if (P->GetrightChild()->GetStateOfProcess() == Ready)
			{
				for (int i = 0; i < totalNumberOfProcessors; i++)
				{
					for (int j = 0; j < listOfProcessors[i]->GetnumberOfProcessInReady(); j++)
					{
						Process* firstready = listOfProcessors[i]->GetfirstReady();
						if (P->GetrightChild()->GetProcessId() == firstready->GetProcessId())
						{
							if (P->GetrightChild() != nullptr)
							{
								P->GetrightChild()->SetTerminationTime(TimeStep);
							}
							numOfProcessKilled++;
							AddToTrmList(P->GetrightChild());

						}
						else
						{
							listOfProcessors[i]->AddToRDYQueue(firstready);
						}
					}
				}
			}
			else if (P->GetrightChild()->GetStateOfProcess() == Running)
			{

				for (int i = 0; i < totalNumberOfProcessors; i++)
				{
					if (listOfProcessors[i]->GetRunningProcess() == P->GetrightChild())
					{
						P->GetrightChild()->SetTerminationTime(TimeStep);
						numOfProcessKilled++;

						AddToTrmList(P->GetrightChild());
						listOfProcessors[i]->SetRunningProcess(NULL);
						listOfProcessors[i]->SetStatusOfProcessor(idle);
						numberOfProcessInRun--;
					}
				}
			}
		}

	}
}

void Scheduler::decrementnoOfprocessRUN()
{
	numberOfProcessInRun--;
}

void Scheduler::incrementnoOfProcessInRUN()
{
	numberOfProcessInRun++;
}

int Scheduler::GetShortestRDYProccessorSJF()
{
	int min = 9999999;
	int ShortestQueueIndexSJF = 0;
	for (int i = 0; i < totalNumberOfProcessors; i++)
	{
		if (listOfProcessors[i]->GetTypeOfProcessor() == SJF)
		{
			if (listOfProcessors[i]->Getisoverheat() == false)
			{
				int timeTakenToFinish = listOfProcessors[i]->GetcurrentExpectedFinishTime();
				if (timeTakenToFinish < min)
				{
					min = timeTakenToFinish;
					ShortestQueueIndexSJF = i;
				}
			}
		}

	}
	if (listOfProcessors[ShortestQueueIndexSJF]->GetStatusOfProcessor()==SJF) {
		return ShortestQueueIndexSJF;
	}
}

int Scheduler::GetShortestRDYProccessorRR()
{
	int min = 9999999;
	int ShortestQueueIndexRR = 0;
	for (int i = 0; i < totalNumberOfProcessors; i++)
	{
		if (listOfProcessors[i]->GetTypeOfProcessor() == RR)
		{
			if (listOfProcessors[i]->Getisoverheat() == false)
			{
				int timeTakenToFinish = listOfProcessors[i]->GetcurrentExpectedFinishTime();
				if (timeTakenToFinish < min)
				{
					min = timeTakenToFinish;
					ShortestQueueIndexRR = i;
				}
			}
		}

	}
	if (listOfProcessors[ShortestQueueIndexRR]->GetStatusOfProcessor() == SJF) {
		return ShortestQueueIndexRR;
	}
	
}

void Scheduler::NEWtoRDY()
{
	int Temp = totalNumberOfProcesses;
	Process* Check = new Process();
	while (Temp != 0)
	{
		newList.dequeue(Check);
		int id = Check->GetProcessId();
		if (Check->GetArrivalTime() == TimeStep)
		{
			GetLongest_ShortestReady();
			int x = ShortestQueueIndex;
			if (listOfProcessors[x]->Getisoverheat() == false) 
			{

				listOfProcessors[x]->AddToRDYQueue(Check);
				Check->SethaveMovedAlready(true);
			}
		}
		else
		{
			newList.enqueue(Check);
		}
		Temp--;

	}
}

bool Scheduler::BLKTORDY()
{
	Process* P;
	if (blkList.isEmpty() == false) {
		{
			blkList.peek(P);
			if ((P->GetIO_D()) == P->GetCurrentDuration())
			{
				blkList.dequeue(P);

				P->IncrementTotalIO(P->GetIO_D());
				listOfProcessors[ShortestQueueIndex]->AddToRDYQueue(P);
				P->SethaveMovedAlready(true);
				P->SetStateOfProcess(Ready);
				BlockListCount--;
				return true;
			}
			else
			{
				P->IncrementCurrentDuration();
			}
		}
	}
	return false;
}

bool Scheduler::RUNTOTRM(Process* P, Processor* Pproc)
{
	if (P->GetCpuTime() == 0)
	{
		if (P->getProcessDeadline() != 0 && P->getProcessDeadline() > TimeStep)
		{
			ProcessTerminatedBeforeDeadline++;
		}
		AddToTrmList(P);
		ORPH(Pproc->GetRunningProcess());
		Pproc->SetRunningProcess(NULL);
		Pproc->SetStatusOfProcessor(idle);
		P->SetStateOfProcess(Terminated);
		P->SethaveMovedAlready(true);
		numberOfProcessInRun--;
		return true;
	}
	
	return false;
}

bool Scheduler::Output(string filenamee)
{
	string filename = filenamee;
	ofstream outputFile;
	outputFile.open(filename, ios::out); //if we 've used ofstream(write into files) we dont need to open file  
	if (outputFile.fail())
	{
		cout << "Error opening file" << endl;
		return false;
	}

	//DATA RELATED TO PROCESSES
	int sumWaitingTime = 0;
	int sumResponseTime = 0;
	int sumTerminationTime = 0;
	int sumturnAroundTime = 0;
	outputFile << "TT" << "\t" << "PID" << "\t" << "AT" << "\t" << "CT" << "\t" << "IO_D" << "\t" << "WT" << "\t" << "RT" << "\t" << "TRT" << endl;
	while (!trmList.isEmpty())
	{
		Process* ptemp;
		ptemp = trmList.remove();
		outputFile << ptemp->GetTerminationTime() << "\t" << ptemp->GetProcessId() << "\t" << ptemp->GetArrivalTime() << "\t" << ptemp->GetCpuTimeForOutput() << "\t" << ptemp->Gettotal_IO_duration() << "\t" << ptemp->GetWaitingTimeForOutput() << "\t" << ptemp->GetResponseTime() << "\t" << ptemp->GetturnaroundDuration() << endl;
		sumWaitingTime = sumWaitingTime + ptemp->GetWaitingTimeForOutput();
		sumResponseTime = sumResponseTime + ptemp->GetResponseTime();
		sumturnAroundTime = sumturnAroundTime + ptemp->GetturnaroundDuration();

	}
	int averageWaitingTime = sumWaitingTime / totalNumberOfProcesses;
	int averageResponseTime = sumResponseTime / totalNumberOfProcesses;
	int averageTurnAroundTime = sumturnAroundTime / totalNumberOfProcesses;
	double ProcessTerminatedBeforeDeadline_prec = (double)(ProcessTerminatedBeforeDeadline * 100 / totalNumberOfProcesses);
	outputFile << endl << endl;
	outputFile << "Processes: " << totalNumberOfProcesses << endl;
	outputFile << "Avg WT = " << averageWaitingTime << ",	Avg RT = " << averageResponseTime << ",	Avg TRT = " << averageTurnAroundTime << endl;
	outputFile << "% Of Process Completed Before Deadline = " << ProcessTerminatedBeforeDeadline_prec << endl;

	double migrationsdb = (migrations * 100) / totalNumberOfProcesses;
	double migrationsDueToRTFdb = (migrationsDueToRTF * 100) / migrations;
	double migrationsDueToMaxWdb = (migrationsDueToMaxW * 100) / migrations;
	double numOfProcessStoleddb = (numOfProcessStoled * 100) / totalNumberOfProcesses;
	double numOfProcessKilleddb = (numOfProcessKilled * 100) / totalNumberOfProcesses;
	double numOfProcessForkeddb = (numOfProcessForked * 100) / totalNumberOfProcesses;

	outputFile << "Migration %: " << migrationsdb << "	RTF = " << migrationsDueToRTFdb << ",	MaxW = " << migrationsDueToMaxWdb << endl;
	outputFile << "Work Steal %: " << numOfProcessStoleddb << endl;
	outputFile << "Forked Process: " << numOfProcessForkeddb << "%" << endl;
	outputFile << "Killed Process: " << numOfProcessKilleddb << "%" << endl;

	outputFile << endl << endl;
	//DATA RELATED TO PROCESSORS 
	outputFile << "Processors:" << totalNumberOfProcessors << " [" << noOfProcessorFCFS << " FCFS, " << noOfProcessorSJF << " SJF, " << noOfProcessorRR << " RR, " << noOfProcessorEDF << " EDF ]" << endl;

	int TotalTurnAroundTimeForAllProcess = sumturnAroundTime - sumWaitingTime;
	outputFile << "Processors Load " << endl;
	for (size_t i = 0; i < totalNumberOfProcessors; i++)
	{
		outputFile << "P" << i + 1 << " = " << listOfProcessors[i]->GetProcessorLoad(TotalTurnAroundTimeForAllProcess) << " %, ";
	}
	outputFile << endl << endl;
	double sumOfUtilization = 0;
	outputFile << endl << "Processors Utiliz " << endl;
	for (size_t i = 0; i < totalNumberOfProcessors; i++)
	{

		sumOfUtilization = sumOfUtilization + listOfProcessors[i]->GetProcessorUtilization();
		outputFile << "P" << i + 1 << " = " << listOfProcessors[i]->GetProcessorUtilization() << " %, ";

	}
	double avgUtilization = sumOfUtilization / totalNumberOfProcessors;
	outputFile << endl << "Avg utilization = " << avgUtilization << " %";


	outputFile.close();
	return true;
}

void Scheduler::ProcessMigrationRR(Process* P, Processor* Pproc)
{

	if (P->GetStateOfProcess() == Running) {
		if (Pproc->GetTypeOfProcessor() == RR)
		{

			{
				if (P->GetCpuTime() < rTF)
				{
					migrations++;
					migrationsDueToRTF++;
					int x = 0;
					x = GetShortestRDYProccessorSJF();
					listOfProcessors[x]->AddToRDYQueue(P);
					Pproc->SetRunningProcess(NULL);
					Pproc->SetStatusOfProcessor(idle);
					P->SethaveMovedAlready(true);
					numberOfProcessInRun--;
					//cout << "##########Process migrated from RR" << P->GetProcessId() << endl;
				}
			}
		}

	}

}

void Scheduler::ProcessMigrationFCFS(Process* P, Processor* Pproc)
{

	if (Pproc->GetTypeOfProcessor() == FCFS)
	{
		{

			int WT = P->GetWaitingTime();
			if (WT > maxW)
			{
				migrations++;
				migrationsDueToMaxW++;
				int x = 0;
				x = GetShortestRDYProccessorRR();
				listOfProcessors[x]->AddToRDYQueue(P);
				Pproc->SetRunningProcess(NULL);
				Pproc->SetStatusOfProcessor(idle);
				P->SethaveMovedAlready(true);
				numberOfProcessInRun--;
				//cout << "#############Process migrated from FCFS: " << P->GetProcessId() << endl;
			}
		}

	}
}

void Scheduler::ProcessorOverheating()
{
	Process* P;
	int r = (rand() % 100) + 1;
	int rmp = (rand() % totalNumberOfProcessors);
	if (listOfProcessors[rmp]->isReadyEmpty() == false)
	{

		P = listOfProcessors[rmp]->GetfirstReady();
		while (P != NULL)
		{
			listOfProcessors[rmp]->Setisoverheat(true);
			listOfProcessors[rmp]->Setoverheattime(overheatconst);
			GetLongest_ShortestReady();
			listOfProcessors[ShortestQueueIndex]->AddToRDYQueue(P);
			P->SethaveMovedAlready(true);
			P = listOfProcessors[rmp]->GetfirstReady();
		}
		//cout << "#####Processor overheating happened to Processor: " << rmp + 1 << endl;
		return;
	}
}
Scheduler::~Scheduler()
{
	for (int i = 0; i < totalNumberOfProcessors; i++)
		delete listOfProcessors[i];
	newList.~LinkedQueue();
	trmList.remove();
	blkList.~LinkedQueue();

}