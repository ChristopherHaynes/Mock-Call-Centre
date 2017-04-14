//Call centre program - Christopher Haynes - 10748982
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "call.h"	//Include the header file for the "call" class
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000) ? 1 : 0)
using namespace std;

void Menu(void);				//Menu for handling user input
void Push(void);				//"Pushs" data onto the stack (new call incoming)
void Pop(void);					//"Pops" data off the stack (call is answered by operator)
void RemoveCall(int,int);		//Removes call from stack (caller disconnects)
void AdvanceQueue(void);		//Moves all active calls one position up in the queue
void UpdateHoldTimes(void);		//Get the current tick count to update call hold times
void ViewQueue(void);			//Print the current queue data to the screen
void CalculateAverages(void);	//Calculate the average call count and wait time
void gotoxy(int, int);			//declare a function called gotoxy
void clrscr(void);				//declare clear screen function

Call c[20];						//Array of "Call" objects for each of the 20 possible waiting calls
int sp = -1, ep = -1;			//Start pointer and end pointer, -1 represents an empty stack 
int globalCallCount = 0;		//Total number of calls recieved since the program started 
int nextQueuePosition = 1;		//The next available queue position (1 is the front of the queue)

int nCalls = 0, nTimes = 0;				  //Represents "n" value for calculating mean averages
float sumCalls = 0, sumTime = 0;		  //Contains the sums of call counts and hold times
float averageCalls = 0, averageTime = 0;  //Floats for holding the average number of calls and wait time

void main(void)
{
	Menu();	//The main function simply calls the menu
}

//-----------------------------------------------------------------------------
// Name: Menu()
// Desc: Take in user input to simulate call data
//-----------------------------------------------------------------------------
void Menu(void) 
{
	int choice = 0;			//Records the users menu choice
	int removePos = -1;		//Holds the position to be removed from the queue if a caller disconnects
	int pushValue = 0;		//Hold the value which is being pushed onto the queue

	while (!KEY_DOWN(VK_ESCAPE)) {	//Keep looping the menu until the user hits escape

		ViewQueue();	//Print the queue to the screen
		gotoxy(0, 1);	//Move the cursor back to the top to print the menu options

		//Print the menu options and record the user input into "choice"
		cout << "Press 1 - Add a call to the queue \n" << "Press 2 - Put the first call in the queue through to the operator \n"
			 << "Press 3 - Customer disconnected, remove call from queue \n" << "Press 4 - Update call hold time data \n"
			 << "Press 5 - Reset average counters\n";
		cin >> choice;

		if (choice == 1) {

			Push();					//Call the push function to add a call to the end of the queue
			CalculateAverages();	//Calculate the average call amount and hold time
			UpdateHoldTimes();		//Update the time all the calls have been on hold
		}

		else if (choice == 2) {

			Pop();					//Call the pop function to remove the first call in the queue
			CalculateAverages();	//Calculate the average call amount and hold time
			UpdateHoldTimes();		//Update the time all the calls have been on hold
		}

		else if (choice == 3) {

				cout << "Enter Position to remove from queue \n";
				cin >> removePos;	//Let the user choose which call should be disconnected

				if (removePos >= 0 && removePos < 20 && c[removePos].GetActive() == 1) { //If the input is within the valid range and contains an active call

					RemoveCall(removePos, c[removePos].GetID());	//Call the remove call function to remove that call from the queue 
				}
				else {	//If the input was invalud

					removePos = -1;	//Reset the remove position to -1 and print an error message to the screen
					gotoxy(23, 37); cout << "INVALID INUPUT - Press any key to try again"; _getch();
				}

			UpdateHoldTimes();	//Update the time all the calls have been on hold
		}

		else if (choice == 4) {

			UpdateHoldTimes();	//Update the time all the calls have been on hold
		}

		else if (choice == 5) {

			averageCalls = 0; averageTime = 0;	//Reset both average counters to 0
			sumCalls = 0; sumTime = 0;			//Reset both sum counters to 0
			nCalls = 0; nTimes = 0;				//Reset sample counter to 0
		}

		else { gotoxy(23, 37); cout << "INVALID INUPUT - Press any key to try again"; _getch(); clrscr(); }

	}
}

//-----------------------------------------------------------------------------
// Name: Push()
// Desc: If viable, add a call to the back of the queue (call incoming)
//-----------------------------------------------------------------------------
void Push(void)
{

	if ((sp == 0 && ep == 19) || (ep == sp - 1)) {	//Queue is full
		gotoxy(23, 37); cout << "Queue is full, No more calls can be added";
		gotoxy(23, 39); cout << "Press any key to continue"; _getch();
	}
	else {// Queue is not full
		if (ep == -1 && sp == -1) { //Queue is empty
			ep = 0;					//Increase the pointers to point to the first position in the array
			sp = 0;
			c[ep].Init(nextQueuePosition, globalCallCount);	//Initialise a new call		
		}
		else if (ep == 19 && sp != 0) { //End pointer is at end off array, but there is room at the start
			ep = 0;						//Set the end pointer back to the first array index
			c[ep].Init(nextQueuePosition, globalCallCount); //Initialise a new call
		}
		else {		//General case (array not empty and end pointer isn't in last array position)
			ep++;	//Increment the end pointer by 1
			c[ep].Init(nextQueuePosition, globalCallCount); //Initialise a new call	
		}
		sumCalls += nextQueuePosition;	//Add the current number of calls to the sum
		nCalls++;						//Increase the amount of call samples by 1
		nextQueuePosition++;	//Increase the next available queue position by 1
		globalCallCount++;		//Increase the total global call count by 1
		
	}
}

//-----------------------------------------------------------------------------
// Name: Pop()
// Desc: If viable, remove a call from the front of the queue (call answered)
//-----------------------------------------------------------------------------
void Pop(void)
{
	if (sp == -1) //Queue is empty, so no calls are availble to be answered
	{
		gotoxy(2, 2);
		gotoxy(23, 37); cout << "Queue is empty, No calls to be transfered";
		gotoxy(23, 39); cout << "Press any key to continue"; _getch();
	}
	else //Queue is not empty		
	{
		sumTime += c[sp].GetHoldTime();			//Add the hold time of the answered call to the sum
		nTimes++;								//Increase the amount of time samples by 1
		sumCalls += (nextQueuePosition - 1);	//Add the one less than the current number of calls to the sum
		nCalls++;								//Increase the amount of call samples by 1

		if (sp == ep){		//Queue has one item in it
			c[sp].Clear();	//Remove the first item from the queue
			sp = -1;		//As the queue is now empty set both pointers back to -1 
			ep = -1;
		} 
		else if (sp == 19) {//Queue has looped and the start pointer is in the final array position
			c[sp].Clear();	//Remove the first item from the queue
			sp = 0;			//Set the start pointer back to the first index in the array
		} 
		else {				//General Case 
			c[sp].Clear();	//Remove the first item from the queue
			sp++;			//Incrememnt the start pointer by 1 to point at the new first item in the queue
		}

		AdvanceQueue();		//After the first item in the queue has been removed, every other active call
	}						//must have its position in the queue reduced by 1.
}

//-----------------------------------------------------------------------------
// Name: RemoveCall()
// Desc: If viable, remove a call from a user defined position in the queue (call disconnected)
//-----------------------------------------------------------------------------
void RemoveCall(int callPos, int queuePos) 
{
	if (sp == ep) { sp = -1; ep = -1; } //If the disconnected call is the last one in the queue set the pointers out of the range
	else { //If there is more than one call in the queue
		ep--; 
		if (ep == -1) { ep = 19; } //Reduce the end pointer by 1 to point to the new end of the queue
	}
	nextQueuePosition--;			//Reduce the next available queue position by 1

	for (int i = callPos; i < 20; i++) { //Loop from the position being removed to the end of the queue
	
		if (i != 19 && c[i].GetActive() == 1) { //If the end of the array hasn't been reached and there is an active call
		
			c[i] = c[i + 1];		//Move the next call down one position in the array
			c[i].AdvancePosition(); //Once moved, advance that calls position in the queue 
		}														 
		else if (i == 19 && c[i].GetActive() == 1) { //If the end of the array has been reached and there is an active call
		
			c[19] = c[0];				//Move the first position in the array to the last position
			c[19].AdvancePosition();	//Once moved, advance that calls position in the queue

			for (int j = 0; j < callPos; j++) {	//Loop from the start of the queue, up to the position being removed
			
				if (c[j].GetActive() == 1 && c[j].GetID() > queuePos) { //If there is an active call and it's queue position is
																		//greater than the disconnected calls position
					c[j] = c[j + 1];		//Move the next call down one position in the array
					c[j].AdvancePosition();	//Once moved, advance that calls position in the queue 
				}
				else if (c[j].GetActive() == 0) {	//If an inactive call is found, the end of the queue has been reached

					break;	//Break out of the inner loop
				}
			}
		}																			
		else if (c[i].GetActive() == 0) {	//If an inactive call is found, the end of the queue has been reached

			break; //Break out of the outer loop
		}
	}

	if (c[ep + 1].GetActive() == 1 && ep < 19) { c[ep + 1].Clear(); }	//Clear the position above the end pointer (this is useful for creating an
	else if (ep == 19) { c[0].Clear(); }						 		//empty space in the case where the queue is currently circular and full)
}

//-----------------------------------------------------------------------------
// Name: AdvanceQueue()
// Desc: Decrease each calls queue position by 1, and reduce the next availble queue position by 1.
//-----------------------------------------------------------------------------
void AdvanceQueue(void)
{
	for (int i = 0; i < 20; i++) {	//Loop covering every call 

		c[i].AdvancePosition();		//Call the class function for advancing a calls queue position
	}
	nextQueuePosition--;			//Decrement the value for the next available queue position
}

//-----------------------------------------------------------------------------
// Name: UpdateHoldTImes()
// Desc: Update the time each call has been on hold
//-----------------------------------------------------------------------------
void UpdateHoldTimes(void)
{
	for (int i = 0; i < 20; i++) {	//Loop covering every call 

		c[i].UpdateCallTime();		//Call the class function for updating a calls time
	}
}

//-----------------------------------------------------------------------------
// Name: CalculateAverages()
// Desc: Calculate the average amount of calls and average hold time
//-----------------------------------------------------------------------------
void CalculateAverages(void)
{
	averageCalls = sumCalls / nCalls;
	if (nTimes != 0 && sumTime != 0) averageTime = sumTime / nTimes; //Don't divide by 0
}

//-----------------------------------------------------------------------------
// Name: ViewQueue()
// Desc: Print the current status of the queue and its data to the screen
//-----------------------------------------------------------------------------
void ViewQueue(void)
{
	clrscr();

	for (int i = 20; i >= 0; i--) {	//Loop starting at the last call index and ending at the first call index

		gotoxy(28, 32 - i);

		//For the first line, print the headings of each column of data
		if (i == 20) { gotoxy(23, 12); cout << "Position" << "  " << "QueueID" << "  " << "CallNum" << "  " << "HoldTime(secs)"; }

		else { //For every other line print the calls, array position, queue number, call number and hold time
			cout << i;
			gotoxy(37, 32 - i); cout << c[i].GetID();
			gotoxy(44, 32 - i); cout << c[i].GetCallNum();
			gotoxy(52, 32 - i); cout << c[i].GetHoldTime();
		}
	}
	gotoxy(35, 10); //Print to screen, the current values of the start and end pointer
	cout << "sp=" << sp << " ep=" << ep;
	gotoxy(30, 34);
	cout << "Average Queue Length = " << averageCalls;
	gotoxy(30, 35);
	cout << "Average Hold Time = " << averageTime;
}

//-----------------------------------------------------------------------------
// Name: gotoxy()
// Desc: Go to the provided x,y co-ordinates on the console screen
//-----------------------------------------------------------------------------
void gotoxy(int x, int y)         
{
	static HANDLE hStdout = NULL;
	COORD coord;

	coord.X = x;
	coord.Y = y;
	if (!hStdout) { hStdout = GetStdHandle(STD_OUTPUT_HANDLE); }
	SetConsoleCursorPosition(hStdout, coord);
}

//-----------------------------------------------------------------------------
// Name: clrscr()
// Desc: Clear the console screen
//-----------------------------------------------------------------------------
void clrscr(void)
{
	static HANDLE hStdout = NULL;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0,0 };
	DWORD dummy;

	if (!hStdout)
	{
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hStdout, &csbi);
	}
	FillConsoleOutputCharacter(hStdout, ' ', csbi.dwSize.X * csbi.dwSize.Y, startCoords, &dummy); 
	gotoxy(0, 0);
}

