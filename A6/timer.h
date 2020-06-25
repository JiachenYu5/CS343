#ifndef __TIMER_H__
#define __TIMER_H__ 1

#include "global.h"
#include "printer.h"
#include "nameserver.h"
#include "trainstop.h"

_Task Timer {
    Printer & prt;
    NameServer & nameServer;
    unsigned int timerDelay;
    TrainStop ** trainStopList;
    unsigned int numStops;
    void main();
  public:
    Timer( Printer & prt, NameServer & nameServer, unsigned int timerDelay );
    ~Timer();
};

#endif