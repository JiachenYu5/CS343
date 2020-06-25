#ifndef __TRAINSTOP_H__
#define __TRAINSTOP_H__ 1

#include "global.h"
#include "watcard.h"
#include "nameserver.h"
#include "train.h"

_Task TrainStop {
	Printer & prt;
	NameServer & nameServer;
	unsigned int id;
	unsigned int stopCost;
	unsigned int studentWait_1;	// number of waiting students for train in counterclockwise direction
	unsigned int studentWait_2;	// number of waiting students for train in counterclockwise direction
	uCondition * studentCond1;	// condition variables for students waiting for train in clockwise direction
	uCondition * studentCond2;	// condition variables for students waiting for train in counterclockwise direction
	uCondition trainCond; // condition variable for trains
	Train * curTrain;	// get the current train arrived
	Train * curTrain_1;	// the train at current stop going in clockwise direction
	Train * curTrain_2; // the train at current stop going in counterclockwise direction
	void main();
  public:
	_Event Funds {										// Thrown when there are insufficient funds on the card to buy a ticket.
	  public:
		unsigned int amount;
		Funds( unsigned int amt ) : amount( amt ) {}
	};

	TrainStop( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int stopCost );
	~TrainStop();
	_Nomutex unsigned int getId() const;

	// Student uses card to buy a ticket for numStops stops.
	void buy( unsigned int numStops, WATCard & card );
	
	// Student waits for a train traveling in the specified direction.
	Train * wait( unsigned int studentId, Train::Direction direction );
	
	// Student getting off the train at this stop "disembarks" at this stop.				
	void disembark( unsigned int studentId );
	
	// Timer notifies stop that timer ticked.	  
	void tick();

	// Train tells stop in which direction it is traveling, and the maximum number of students it can take.
	// It then blocks until the timer ticks over. Returns the minimum of what it can take, and the number
	// of students waiting.
	unsigned int arrive( unsigned int trainId, Train::Direction direction, unsigned int maxNumStudents );
};

#endif