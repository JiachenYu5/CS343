#ifndef __WATCARD_H__
#define __WATCARD_H__ 1

#include "global.h"
#include <uFuture.h>

class WATCard {
	unsigned int balance;							// current balance of WATCard
	bool hasPOP;									// proof of purchase
	WATCard( const WATCard & ) = delete;				// prevent copying
	WATCard & operator=( const WATCard & ) = delete;
	void markPaid();
  public:
	typedef Future_ISM<WATCard *> FWATCard;				// future watcard pointer
	WATCard();
	~WATCard();
	void deposit( unsigned int amount );
	void withdraw( unsigned int amount );
	unsigned int getBalance();
	bool paidForTicket();
  	void resetPOP();
}; // WATCard

#endif
