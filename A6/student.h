#ifndef __STUDENT_H__
#define __STUDENT_H__ 1

#include "global.h"
#include "printer.h"
#include "nameserver.h"
#include "watcard.h"
#include "bank.h"
#include "groupoff.h"
#include "train.h"
#include "trainstop.h"
#include "cardoffice.h"

_Task Student {
    Printer & prt;
    NameServer & nameServer;
    WATCardOffice & cardOffice;
    Groupoff & groupoff;
    unsigned int id;
    unsigned int numStops;
    unsigned int stopCost;
    unsigned int maxStudentDelay;
    unsigned int maxStudentTrips;
    WATCard::FWATCard watCard;  // watcard of the student
    WATCard::FWATCard giftCard; // giftcard of the student
    TrainStop * startStop;  // end stop of the trip
    TrainStop * endStop;  // start stop of the trip
    Train * takenTrain; // the train the student took
    int distance; // the number of stops to take
    char direction; // the direction of the train the student should take
    unsigned int numTrips;  // number of trips the student will take
    bool isRejected;      // whether the student is rejected
    char findDirection(); // help function that find the direction of train the student should take
    void main();
  public:
    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, 
        unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips );
    ~Student();
};

#endif