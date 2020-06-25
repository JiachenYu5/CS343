#include "groupoff.h"
#include "MPRNG.h"
#include <vector>
using namespace std;

extern MPRNG mprng;

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int maxTripCost, unsigned int groupoffDelay ) :
    prt( prt ), numStudents( numStudents ), maxTripCost( maxTripCost ), groupoffDelay( groupoffDelay ) {} // Groupoff::Groupoff

Groupoff::~Groupoff() {
    for ( unsigned int i = 0; i < watCards.size(); i++ ) {
        delete watCards[i];
    } // for
} // Groupoff::~Groupoff

WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard futureCard; // create future gift card
    futureGiftCards.push_back(futureCard); // add it to list
    return futureCard;
} // Groupoff::giftCard

void Groupoff::main() {
    prt.print( Printer::Kind::Groupoff, 'S' );
    // for each student that calls giftCard (only once), add a gift card
    for ( unsigned int i = 0; i < numStudents; i++ ) {
        _Accept( giftCard ); // _Accept
    } // for
    for ( ; ; ) {
        _Accept ( ~Groupoff ) {
            break;
        } _Else {
            // if all giftcards are assigned, break loop
            if ( futureGiftCards.size() == 0 ) break; // if
            yield( groupoffDelay ); // delay
            unsigned int pickedStudent = mprng( futureGiftCards.size() - 1 ); // pick a random student to assign
            // create WATCard and add it to list
            WATCard *card = new WATCard();
            watCards.push_back(card);

            card->deposit( maxTripCost );
            futureGiftCards[pickedStudent].delivery( card );
            prt.print( Printer::Kind::Groupoff, 'D', maxTripCost ); // print info
            futureGiftCards.erase( futureGiftCards.begin() + pickedStudent ); // remove the giftcard assigned from the list
        } // _Accept
    } // for
    prt.print( Printer::Kind::Groupoff, 'F' );
} // Groupoff::main