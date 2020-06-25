#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__ 1

#include "global.h"
#include "watcard.h"
#include "printer.h"
#include <vector>

_Task Groupoff {
	Printer & prt;
	unsigned int numStudents;
	unsigned int maxTripCost;
	unsigned int groupoffDelay;
	std::vector<WATCard::FWATCard> futureGiftCards;  // array of future gift cards for all students
	std::vector<WATCard *> watCards; // array of real WATCards pointers
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int maxTripCost, unsigned int groupoffDelay );
	~Groupoff();
	WATCard::FWATCard giftCard();
}; // Groupoff

#endif