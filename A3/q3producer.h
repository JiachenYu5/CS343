#ifndef __Q3PRODUCER_H__
#define __Q3PRODUCER_H__ 1

extern MPRNG mprng;

_Task Producer {
    BoundedBuffer<int> *buffer;
    int Produce;
    int Delay;
    void main();
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay );
};

Producer::Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay ) 
    : buffer(&buffer), Produce(Produce), Delay(Delay) {} // Producer::Producer

void Producer::main() {
    for ( int i = 1; i <= Produce; i++ ) {
        yield( mprng( Delay - 1 ) );
        buffer->insert(i);
    } // for
} // Producer::main

#endif