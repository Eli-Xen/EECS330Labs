#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <iostream>
#include <algorithm>

#include "MyVector.h"

template <typename DataType>
class MyQueue : private MyVector<DataType>
{  
  private:
    size_t dataStart, dataEnd;

    void resize(size_t newSize) // changes the size of the array to newSize
    {   this->MyVector<DataType>::resize(newSize);     }

    
    void reserve(size_t newCapacity) // requests for newCapacity amount of space
    {   this->MyVector<DataType>::reserve(newCapacity); }

  public:
    explicit MyQueue(size_t initSize = 0): // default constructor
        MyVector<DataType>(initSize), //initalize using myvector implmentation 
        dataStart{0}, //then initalize dataStart and dataEnd before the contructor can start 
        dataEnd{0}
    {}

    MyQueue(const MyQueue & rhs): // copy constructor
        MyVector<DataType>(rhs), 
        dataStart{rhs.dataStart}, 
        dataEnd{rhs.dataEnd}
    {}

    MyQueue(MyQueue && rhs): // move constructor
        MyVector<DataType>(std::move(rhs)), 
        dataStart{rhs.dataStart}, 
        dataEnd{rhs.dataEnd}
    {   rhs.dataStart=rhs.dataEnd=0; } //make both equal to 0 after moving thier data 

    ~MyQueue() //when destructor is called it destructs the derived queue class and then the base class it inherits from; this destructor implicty calls destructor of myvector, and since we reuse elements from vector we only have to delete new elements 
    {   dataStart=dataEnd=0; } //reset the start and end since that is new data not from myvector 
    
    MyQueue & operator= (const MyQueue & rhs) // copy assignment
    {   if (this!=&rhs)
        {   MyVector<DataType>::operator=(rhs); 
            dataStart=rhs.dataStart; 
            dataEnd=rhs.dataEnd; 
        }
        return *this; 
    }

    MyQueue & operator= (MyQueue && rhs) // move assignment
    {   if (this!=&rhs)
        {   MyVector<DataType>::operator=(std::move(rhs)); 
            dataStart=rhs.dataStart; 
            dataEnd=rhs.dataEnd; 
            rhs.dataStart=rhs.dataEnd=0; 
        }
        return *this; 
    }

    void enqueue(const DataType & x) // insert x into the queue
    {   this->push_back(x); 
        dataEnd++; 
    }

    void enqueue(DataType && x) // insert x into the queue
    {   this->push_back(std::move(x)); 
        dataEnd++; 
    }

    void dequeue(void) // remove the first element from the queue
    {   if (!this->empty()) //if its not empty 
        {   dataEnd--; //incriment dataEnd 
            //shit all elements over //*shift 
            for (size_t i=0; i<this->theSize-1; i++) //loop through the vector and move all elements up one space 
                this->data[i]=this->data[i+1]; 
            this->theSize--; //decrement the size so it reflects the change 
        }
    }

    const DataType & front(void) const // access the first element of the queue
    {   return (this->operator[](dataStart)); }//could be this->data[](dataStart) but this should be function call 


    bool empty(void) const // check if the queue is empty; return TRUE is empty; FALSE otherwise
    {   return (dataStart==dataEnd); }//see if start of data and size are the same, if dataStart has gotten to the last index aka the size, and ie start=9 and size=9 then we cant access any elements before the start and the queue is empty  

    size_t size() const // access the size of the queue
    {   return this->theSize;    }

    
    size_t capacity(void) const // access the capacity of the queue
    {   return this->MyVector<DataType>::capacity();     }

};


#endif // __MYQUEUE_H__