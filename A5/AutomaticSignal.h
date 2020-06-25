#ifndef __AUTOMATICSIGNAL_H__
#define __AUTOMATICSIGNAL_H__ 1

#define AUTOMATIC_SIGNAL uCondition bench
#define WAITUNTIL( pred, before, after ) \
    if ( !pred ) { \
        while ( !bench.empty() ) { \
            bench.signal(); \
        } \
        do { \
            before; \
            bench.wait(); \
        } while ( !pred ); \
        after; \
    } \

#define EXIT() \
    while ( !bench.empty() ) { \
        bench.signal(); \
    } \

#endif