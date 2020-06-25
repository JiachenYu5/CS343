#ifndef __PRINTER_H__
#define __PRINTER_H__ 1

#include "global.h"

_Monitor Printer {
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Timer, Train, Conductor, TrainStop, Student, WATCardOfficeCourier };
  private:
  	unsigned int numStudents;
	unsigned int numTrains;
	unsigned int numStops;
	unsigned int numCouriers;
	unsigned int numColumns;			// number of columns to print
	struct Items { 
        char state;
        bool updated; 
		unsigned int id;
		int oid;
        int value1;
		int value2;
		char additionalInfo;
    } * columns;
	void flushInfo();
	unsigned int getColumnIndex( Kind kind, unsigned int id );
  public:
	Printer( unsigned int numStudents, unsigned int numTrains, unsigned int numStops, unsigned int numCouriers );
	~Printer();
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state, unsigned int oid, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state, char c );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, char c );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2, char c );
}; // Printer

#endif
