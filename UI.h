#ifndef UI_H
#define UI_H

#pragma once
#include<iostream>
using namespace std;

//#include "Processor.h"
class Scheduler;
class Processor;
enum UI_Mode						//Three modes
{
	MODE_INTERACTIVE = 0,			//Interactive mode
	MODE_STEP_BY_STEP = 1,			//Step by step mode
	MODE_SILENT = 2					//Silent mode
};
class UI
{
	bool itsSilentCloseProgram = false;
	Scheduler* Sched;
public:
	UI(Scheduler* Sptr) ;			//Constructor and Destructor
	~UI();
	//choosing the mode of the program
	UI_Mode which_MODE();
	//interactive mode
	void Interactive_Mode();
	//step by step mode
	void Step_By_step_Mode();
	//silent mode
	void Silent_Mode();
	//detailed fun to print the detailed info
	void detailed();

	bool getitsSilentCloseProgram();

private:
	Scheduler* S;
};


#endif