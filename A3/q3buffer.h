#ifndef __Q3BUFFER_H__
#define __Q3BUFFER_H__ 1

#ifdef BUSY                            // busy waiting implementation
template<typename T> class BoundedBuffer {
    unsigned int size;
    uOwnerLock mlk;
    uCondLock prod_clk;
    uCondLock cons_clk;
    std::vector<T> values;
  public:
    void insert( T elem );
    T remove();
    BoundedBuffer( const unsigned int size = 10 );
};

template <typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int size ) : size(size) {} // BoundedBuffer<T>::BoundedBuffer

template <typename T>
void BoundedBuffer<T>::insert( T elem ) {
    mlk.acquire();
    try {
        // while buffer value list is full, let producers wait for consumers to consume
        while ( (unsigned int) values.size() == size ) {
            prod_clk.wait( mlk );
        } // while
        values.push_back( elem );           // add element to value list
        cons_clk.signal();                  // signal consumer to consume
    } _Finally {
        mlk.release();
    } // try
} // BoundedBuffer<T>::insert

template <typename T>
T BoundedBuffer<T>::remove() {
    T returnVal;
    mlk.acquire();
    try {
        // while buffer value list if empty, let consumers wait for producers to produce
        while ( values.size() == 0 ) {
            cons_clk.wait( mlk );
        } // while
        returnVal = values[0];              // get the recently added value
        values.erase( values.begin() );      // remove the added value
        prod_clk.signal();                  // signal producer to produce
    } _Finally {
        mlk.release();
    } // try
    return returnVal;
} // BoundedBuffer<T>::remove
#endif

#ifdef NOBUSY                          // no busy waiting implementation
template<typename T> class BoundedBuffer {
    unsigned int size;
    uOwnerLock mlk;
    uCondLock prod_clk;
    uCondLock cons_clk;
    std::vector<T> values;
    bool isTaken;                     // add flag variable to indicate signalling
    uCondLock barge_clk;             // lock used for barging avoidance
  public:
    void insert( T elem );
    T remove();
    BoundedBuffer( const unsigned int size = 10 );
};

template <typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int size ) : size(size), isTaken(false) {} // BoundedBuffer<T>::BoundedBuffer

template <typename T>
void BoundedBuffer<T>::insert( T elem ) {
    mlk.acquire();
    try {
        if ( isTaken ) barge_clk.wait( mlk ); // if // avoid barging
        if ( (unsigned int) values.size() == size ) {
            if ( !barge_clk.empty() ) {         // if not empty, signal other blocked buffer tasks
                barge_clk.signal();
            } else {
                isTaken = false;            // condition reset
            } // if
            prod_clk.wait( mlk );           // buffer value list is full, let producers wait for consumers to consume
        } // if
        values.push_back( elem );           // add element to value list
        isTaken = true;
        if ( !cons_clk.empty() ) {          // if lock for consumer not empty, signal other blocked consumers tasks
            cons_clk.signal();
        } else if ( !barge_clk.empty() ) {  // if lock for barging not empty, signal other blocked buffer tasks
            barge_clk.signal();
        } else {
            isTaken = false;                // condition reset
        } // if
    } _Finally {
        mlk.release();
    } // try
} // BoundedBuffer<T>::insert

template <typename T>
T BoundedBuffer<T>::remove() {
    T returnVal;
    mlk.acquire();
    try {
        if ( isTaken ) barge_clk.wait( mlk ); // avoid barging
        if ( values.size() == 0 ) {
            if ( !barge_clk.empty() ) {         // if not empty, signal other blocked buffer tasks
                barge_clk.signal();
            } else {
                isTaken = false;            // condition reset
            }
            cons_clk.wait( mlk );           // buffer value list is empty, let consumers wait for producers to produce
        }
        returnVal = values[0];              // get the recently added value
        values.erase( values.begin() );     // remove the added value
        isTaken = true;
        if ( !prod_clk.empty() ) {          // if lock for consumer not empty, signal other blocked consumers tasks
            prod_clk.signal();
        } else if ( !barge_clk.empty() ) {  // if lock for barging not empty, signal other blocked buffer tasks
            barge_clk.signal();
        } else {
            isTaken = false;                // condition reset
        }
    } _Finally {
        mlk.release();
    }
    return returnVal;
} // BoundedBuffer<T>::remove
#endif

#endif