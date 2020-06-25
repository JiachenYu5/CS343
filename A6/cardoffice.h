#ifndef __CARDOFFICE_H__
#define __CARDOFFICE_H__ 1

#include "global.h"
#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include <list>

_Task WATCardOffice {
	struct Job {                            // marshalled arguments and return future
    	unsigned int sid;					// student id
    	unsigned int amount;				// amount to transfer
    	WATCard * watcard;					// WATCard to transfer
    	WATCard::FWATCard result;           // return future
    	Job( unsigned int sid, unsigned int amount, WATCard * watcard ) : sid( sid ), amount( amount ), watcard( watcard ) {}
    };
	_Task Courier {
		unsigned int id;					// courier id
		WATCardOffice & cardOffice;			
		Printer & prt;
		Bank & bank;
		void main();
	  public:
		Courier( unsigned int id, WATCardOffice & cardOffice, Printer & prt, Bank & bank );
	};
	Printer & prt;
	Bank & bank;
	unsigned int numCouriers;
	Courier ** courierPool;				// courier pool
	std::list<Job *> jobList;
	void main();
  public:
	_Event Lost {};										// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	~WATCardOffice();
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ); // called by student
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ); // called by student
	Job * requestWork();								// called by courier to request/return work
}; // WATCardOffice

#endif