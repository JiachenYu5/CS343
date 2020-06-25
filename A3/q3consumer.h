#ifndef __Q3CONSUMER_H__
#define __Q3CONSUMER_H__ 1

extern MPRNG mprng;

_Task Consumer {
    BoundedBuffer<int> *buffer;
    int Delay;
    int Sentinel;
    int *sum;
    void main();
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum );
};

Consumer::Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum ) 
    : buffer(&buffer), Delay(Delay), Sentinel(Sentinel), sum(&sum) {} // Consumer::Consumer

void Consumer::main() {
    int curVal;
    for ( ; ; ) {
        yield( mprng( Delay - 1) );
        curVal = buffer->remove();
        if (curVal == Sentinel) break;
        *sum += curVal;
    } // for
} // Consumer::main

#endif