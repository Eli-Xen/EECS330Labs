#ifndef __MYNUMBER_H__
#define __MYNUMBER_H__

#include <algorithm>
#include <iostream>
using namespace std;
//g++ MainTest.cpp --std=c++11 MainTest.cpp -o Lab1 
template <typename DataType>
class MyNumber
{
  public:
    explicit MyNumber(DataType rhs = 0) // default constructor; explicit prevents an implicit making of the default constructor, makes a pointer to where default constructor is stored and creates new memory 
    { num=new DataType(rhs); }
    
    MyNumber(const MyNumber<DataType> & rhs) // copy constructor with a MyNumber instance; take value from other MyNumber object and copy that value by pointing to the same memeory address where that data is stored 
    {
        // code begins //value of num takes the value of rhs 
        num=new DataType(*rhs.num);  //copying rhs to num, pointer to the same value that pointer rhs is pointing to 
        // code ends 
    }
    
    MyNumber(MyNumber<DataType> && rhs)	// move constructor with a MyNumber instance; set ownership of our pointer to our object and take it away from the other object, other objects pointer now points to null  
    { //a=10, move the value of 10 to b, but a is still pointing to 10 so we have to make a point to null 
        // code begins
        num=rhs.num; 
        rhs.num=nullptr; 
        // code ends
    }
    
    MyNumber & operator= (const MyNumber<DataType> &rhs) // copy assignment with a MyNumber instance; copy value from other MyNumber object after deleting our current num pointer and give it pointer to new value we just copied 
    { //instead of creating a new memory instance (like in constructor) we use the ones we already have and move values between them 
        // code begins
        if(this!=rhs)
        {
            delete num; 
            num=new DataType(*rhs.num); 
        }
        return *this; //value of something
        // code ends
    }

    MyNumber & operator= (MyNumber<DataType> && rhs)	// move assignment with a MyNumber instance; set our pointer to point at memory that other object owns; swap values between two objects 
    {   if (this!=&rhs) //if this isnt address of rhs 
        {   delete num; 
            num=rhs.num; 
            rhs.num=nullptr; 
        }
        return *this; 
    }

    ~MyNumber(void) // destructor
    {
        delete num; //when we delete num (pointer/var to value) we delete the value and the space the pointer occupy, you cant use delete on a non-pointer value since pointers are just variables to some memory 
        // code begins

        // code ends
    }
    
    DataType read(void) const
    {   
        return *num; //pointer to num; returns the actual value at address pointer points to 
        // code begins

        // code ends
    }

    void write(DataType rhs)
    {
        *num=rhs; //changes value at pointer num address to rhs
        // code begins

        // code ends
    }

  private:
    /* data */
    DataType *num;
};


#endif // __MYVECTOR_H__
