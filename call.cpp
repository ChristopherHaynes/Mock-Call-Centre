//Call centre program (Call class) - Christopher Haynes - 10748982
#include <iostream>
#include <windows.h>
#include "call.h"
using namespace std;

//-----------------------------------------------------------------------------
// Name: Init()
// Desc: Mark a call as active and initalise its data values
//-----------------------------------------------------------------------------
void Call::Init(int queueID, int globalCallCount)
{
	active = 1;						//Set the call as active
	callNum = globalCallCount + 1;	//Designate this calls unique ID as one more than the current global call count
	startTime = GetTickCount();		//Record the tick count at the time the call is initalised
	holdTime = (float)(GetTickCount() - startTime) / 1000;	//Calculate the initial hold time and convert to seconds
	this->queueID = queueID;		//Set the queue position to the provided perameter value
}

//-----------------------------------------------------------------------------
// Name: Clear()
// Desc: Set the call as inactive and reset all the data values
//-----------------------------------------------------------------------------
void Call::Clear()
{
	active = 0;		//Mark the call as inactive
	callNum = 0;	//Set all other variables to 0
	startTime = 0;
	holdTime = 0;
	queueID = 0;
}

//-----------------------------------------------------------------------------
// Name: AdvancePosition()
// Desc: Decreases a calls queue position by 1
//-----------------------------------------------------------------------------
void Call::AdvancePosition()
{
	if (active == 1) {	//If the call is active

		queueID--;		//Decrement the current queue value
	}
	else {					//If the call is not active
		cout << "ERROR";	//An error must have occured, print to screen
	}
}

//-----------------------------------------------------------------------------
// Name: UpdateCallTime()
// Desc: Take the current tick count and calculate the call hold time
//-----------------------------------------------------------------------------
void Call::UpdateCallTime()
{
	if (active == 1) { //If the call is active

		holdTime = (float)(GetTickCount() - startTime) / 1000; //Calculate the hold time and convert to seconds
	}
}