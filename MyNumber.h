#ifndef __MYNUMBER_H__
#define __MYNUMBER_H__
#include <algorithm>
#include <iostream>
using namespace std; 
//cd into lab folder and run these commands to test the lab, if output is Yes then all good
// g++ --std=c++11 MainTest.cpp -o Lab1 
// ./Lab1 Inputs/input_0.txt > output_0.txt
// python3 GradingScript.py output_0.txt Outputs/output_0.txt
template <typename DataType>
class MyNumber
{ public:
    explicit MyNumber(DataType rhs = 0) // default constructor; explicit prevents an implicit making of the default constructor, makes a pointer to where default constructor is stored and creates new memory 
    {   num=new DataType(rhs); }
    
    MyNumber(const MyNumber<DataType> & rhs) // copy constructor takes value from other MyNumber object and copies that value by creating a new object and taking value from it 
    {   num=new DataType(*rhs.num);  } //rhs.num is sytax for copying rhs to num, pointer to the same value that pointer rhs is pointing to 

    
    MyNumber(MyNumber<DataType> && rhs)	// move constructor transfers ownership of value by giving it to num, and then setting other object to null so there isnt two pointers to same thing   
    { //a=10, move the value of 10 to b, but a is still pointing to 10 so we have to make a point to null 
        num=rhs.num;  
        rhs.num=nullptr; 
    }
    
    MyNumber & operator= (const MyNumber<DataType> &rhs) // copy assignment clears our current pointer, then makes it copy value at address of rhs  
    { //instead of creating a new memory instance (like in constructor) we use the ones we already have and move values between them 
        if(this!=rhs) 
        {   delete num; 
            num=new DataType(*rhs.num); }
        return *this; //value of something
    }

    MyNumber & operator= (MyNumber<DataType> && rhs) // move assignment sets our pointer to point at memory that other object owns; swap values between two objects 
    {   if (this!=&rhs) //if this isnt address of rhs 
        {   delete num; 
            num=rhs.num; 
            rhs.num=nullptr; }
        return *this; 
    }

    ~MyNumber(void) // destructor, deletes the pointer and also the value that it points to, freeing memory 
    {   delete num; }
    
    DataType read(void) const
    {   return *num; } //pointer to num; returns the actual value at address pointer points to 

    void write(DataType rhs)
    {   *num=rhs; } //changes value at pointer num's address to rhs

  private:
    DataType *num;
};

#endif // __MYVECTOR_H__