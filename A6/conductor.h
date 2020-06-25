#ifndef __CONDUCTOR_H__
#define __CONDUCTOR_H__ 1

#include "global.h"
#include "printer.h"
#include "train.h"

_Task Conductor {
	Printer & prt;
	unsigned int id;
	Train * train;
	unsigned int conductorDelay;
	void main();
  public:
  	Conductor( Printer & prt, unsigned int id, Train * train, unsigned int delay );
  	~Conductor();
};

#endif