//Call centre program (Call class header) - Christopher Haynes - 10748982
#ifndef CALL_H	//Used to clarify that this header file should only be used
#define CALL_H	//once when the linker is compling the program

class Call		//Defenition of the class Call
{
private:		//Private variables and functions are not accessable from outside the class
	int active;		//Marks whether a call exists, 0 - no call, 1 - active call
	int callNum;		//The global call number for this call
	int queueID;		//The current queue position for this call
	DWORD startTime;	//The tick count from when the call first entered the queue
	float holdTime;		//The total time in seconds the call has been in the queue
public:			//Public variables and functions are accessable from outside the class
	void Init(int, int);	//Initialises the call with the required values
	void Clear();			//Marks a call inactive and resets data values
	int GetActive() { return active; };			//Getter for "active"
	int GetCallNum() { return callNum; };		//Getter for "callNum"
	int GetID() { return queueID; };			//Getter for "queueID"
	float GetHoldTime() { return holdTime; }	//Getter for "holdTime"
	void AdvancePosition();						//Advance the calls queue position by 1
	void UpdateCallTime();						//Update call time based on current tick count
};

#endif