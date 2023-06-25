#pragma once
#include "UI.h"
#include"Scheduler.h"
#include"Processor.h"
#include <chrono>
#include <thread>


UI::UI(Scheduler* Sptr)
{
	S = Sptr;
}


//Responsible for runnin in either Interactive, Step by step or Silent mode depending on users input
#ifndef which_MODE
UI_Mode UI::which_MODE()
{
	UI_Mode Mode = MODE_INTERACTIVE;
	int x = 0;
	cout << "Which Mode would you like to continue with?" << endl;
	cout << "For interactive mode enter 0, For step by step mode enter 1, for silent mode enter 2 " << endl;
	cin >> x;
	cout << endl;
	if (x == 0)
	{
		Mode = MODE_INTERACTIVE;
	}
	else if (x == 1)
	{
		Mode = MODE_STEP_BY_STEP;
	}
	else if (x == 2)
	{
		Mode = MODE_SILENT;
	}
	else
	{
		cout << "invalide entery, Please enter 0 or 1 or 2 " << endl;
		cin >> x;
	}
	cin.get();
	return Mode;
}
#endif

//Responsible for printing the details for each time step
#ifndef detailed
void UI::detailed()
{
	int timeStep = 0;

	timeStep = S->GetTimeStep();

	cout << "Current Timestep:" << timeStep << endl;

	cout << "----------     RDY Processes  ----------------------" << endl;
	S->PrintRDY();

	cout << "----------     BLK Processes  ----------------------" << endl;
	S->PrintBLK();

	cout << "----------     RUN Processes  ----------------------" << endl;
	S->PrintRUN();

	cout << "----------     TRM Processes  ----------------------" << endl;
	S->PrintTRM();
}
#endif

void UI::Interactive_Mode()
{
	detailed();

	//get enter key from the user
	cout << "Press the ENTER key" << endl;
	if (cin.get() == '\n')
		return;
}

void UI::Step_By_step_Mode()
{
	detailed();

	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;

	sleep_for(10ns);
	sleep_until(system_clock::now() + 1s);
}

void UI::Silent_Mode()
{
	itsSilentCloseProgram = true;
	cout << "Silent Mode.................";
	cout << "   Simulation starts..." << endl;
	cout << "Simulations ends,";
	cout << "output file created";
}


bool UI::getitsSilentCloseProgram()
{
	return itsSilentCloseProgram;
}

UI::~UI()
{
}

