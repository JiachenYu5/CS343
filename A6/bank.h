#ifndef __BANK_H__
#define __BANK_H__ 1

#include "global.h"

_Monitor Bank {
	unsigned int numStudents;
	unsigned int * accountsBalance;
  public:
	Bank( unsigned int numStudents );
	~Bank();
	void deposit( unsigned int id, unsigned int amount ); // deposit "amount" $ for student "id";
	void withdraw( unsigned int id, unsigned int amount ); // withdrawal "amount" $ from student "id"; block until student has enough funds
}; // Bank

#endif