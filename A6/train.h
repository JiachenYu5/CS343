#ifndef __TRAIN_H__
#define __TRAIN_H__ 1

#include "global.h"
#include "printer.h"
#include "watcard.h"
#include <vector>

_Task Train {
  public:
	enum Direction { Clockwise, CounterClockwise };
	_Event Ejected {};						// Exception raised at student without ticket

  private:
  	Printer & prt;
	NameServer & nameServer;
	unsigned int id;
	unsigned int maxNumStudents;
	unsigned int numStops;
	unsigned int curNumStudents;	// current student number in the train
	Direction direction;
	std::vector<unsigned int> studentIdList;	// student id list 
	std::vector<unsigned int> cardList; 	// list for proof of payment
	std::vector<unsigned int> destStopList; // destination stop list
	std::vector<unsigned int> ejectList;	// students need to eject
	TrainStop ** trainStopList; // train stop list
	TrainStop * curStop;
	void main();
  public:
	Train( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int maxNumStudents, unsigned int numStops );
	~Train();
	_Nomutex unsigned int getId() const;
	TrainStop * embark( unsigned int studentId, unsigned int destStop, WATCard& card ); // Student waits until train reaches destination stop.
	void scanPassengers(); // called by conductor
};

#endif