#ifndef __BINSERTSORT_H__
#define __BINSERTSORT_H__ 1

template<typename T> _Coroutine Binsertsort {
    T pivot;                        // the pivot value of the binary tree
    T value;                        // communication: value being passed down/up the tree
    bool hasless = false;            // boolean values to indicate that a node has child tree less
    bool hasgreater = false;        // boolean values to indicate that a node has child tree greater
    void main();                    // YOU WRITE THIS ROUTINE
  public:
    _Event Sentinel {};
    void sort( T value );           // value to be sorted
    T retrieve();                   // retrieve sorted value
};

#endif