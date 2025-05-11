#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include <iostream>
#include <algorithm>

#include "MyVector.h"

//g++ --std=c++11 MainTest.cpp -o Lab4 
// for i in {0..9}; do
//     ./Lab4 Inputs/input_$i.txt > output_$i.txt
//     python3 GradingScript.py output_$i.txt Outputs/output_$i.txt
// done

//the basic idea for MyStack and MyQueue is that they reuse MyVector to make a stack/queue, they call all functions from the vector and dont create any new variables (except dataStart and dataEnd in queue) or dynamic memeory 

template <typename DataType> 
class MyStack : private MyVector<DataType> //MyStack directly inherits from MyVector 
{   public:

    explicit MyStack(size_t initSize = 0): // default constructor
        MyVector<DataType>(initSize) {} //call constructor of MyVector so that it creates the size, capacity and data for us 
    
    MyStack(const MyStack & rhs): // copy constructor
        MyVector<DataType>(rhs) {} //calls copy contructor and sends the const rhs 

    MyStack(MyStack && rhs): // move constructor
        MyVector<DataType>(std::move(rhs)) {} //calls move contructor of MyVector but instead of sending data it moves() rhs pointer 

    ~MyStack() //when destructor is called it destructs the derived queue class and then the base class it inherits from 
    {} //this destructor implicty calls destructor of myvector, and since we reuse elements (nothing new) from vector we only have to delete new elements 

    
    MyStack & operator= (const MyStack & rhs) // copy assignment 
    {   if (this!=&rhs) //if our stack doesnt have the same address as rhs 
            this->MyVector<DataType>::operator=(rhs); //implcitly makes call to vector's = overload operator 
        return *this; 
    }

    
    MyStack & operator= (MyStack && rhs) // move assignment
    {   if (this!=&rhs) //if our stack doesnt have the same address as rhs 
            this->MyVector<DataType>::operator=(std::move(rhs)); //using MyVector<DataType>::operator= is needed when reassigning this using = becuase otherwise it calls infinate loop on its of = method 
        return *this; 
    }

    void push(const DataType & x) // insert x to the stack; parameters &x sends copy of value x 
    {   this->MyVector<DataType>::push_back(x);     }

    void push(DataType && x) // insert x to the stack;parameters && x sends direct referance  to temporary varaible x 
    {   this->MyVector<DataType>::push_back(std::move(x));     }

    
    void pop(void) // remove the last element from the stack
    {   this->MyVector<DataType>::pop_back();     }

    
    const DataType & top(void) const // access the last element of the stack
    {   return this->MyVector<DataType>::back();     }

    
    bool empty(void) const // check if the stack is empty; return TRUE is empty; FALSE otherwise
    {   return this->MyVector<DataType>::empty();     }

    
    size_t size() const // access the size of the stack
    {   return this->MyVector<DataType>::size();     }
    
    size_t capacity(void) const // access the capacity of the stack
    {   return this->MyVector<DataType>::capacity();     }

};


#endif // __MYSTACK_H__