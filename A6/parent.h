#ifndef __PARENT_H__
#define __PARENT_H__ 1

#include "global.h"
#include "printer.h"
#include "bank.h"

_Task Parent {
	Printer & prt;
	Bank & bank;
	unsigned int numStudents;
	unsigned int parentalDelay;
	unsigned int maxTripCost;
	void main();
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay, unsigned int maxTripCost );
}; // Parent

#endif