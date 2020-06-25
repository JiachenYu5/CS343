#ifndef __Q2POLYMULT_H__
#define __Q2POLYMULT_H__ 1

typedef struct {
    int *values;            // actual coefficient array
    int degree;             // number of coefficients
} poly_t;

void polymultiply( const poly_t & a, const poly_t & b, poly_t & c, const size_t delta );

#ifdef TASK
_Task Multiply { 
        poly_t a;
        poly_t b;
        poly_t c;
        size_t startIndex;
        size_t endIndex;
        size_t delta;
        void main();
    public:
        Multiply( const poly_t & a, const poly_t & b, poly_t & c, const size_t startIndex, const size_t endIndex, const size_t delta );
};
#endif

#endif