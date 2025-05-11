#ifndef __MYVECTOR_H__
#define __MYVECTOR_H__

#include <utility>
#include <vector>

//this code gives a yes with commands: 
//g++ --std=c++11 MainTest.cpp -o Lab2 
//./Lab2 Inputs/input_0.txt >output_0.txt 
//python3 GradingScript.py output_0.txt Outputs/output_0.txt

template <typename DataType>
class MyVector
{
  protected: //private changed to protected so we can access the data from other classes 
    size_t theSize;  // the number of data elements the vector is currently holding
    size_t theCapacity;  // maximum data elements the vector can hold
    DataType *data;  // address of the data storage

  public:
    
    static const size_t SPARE_CAPACITY = 16;   // initial capacity of the vector

    // default constructor
    explicit MyVector(size_t initSize = 0) : 
        theSize{initSize},
        theCapacity{initSize + SPARE_CAPACITY} //allocates that much memory 
    {   data=new DataType[theCapacity]; } //make new array called data (as defined in private) and allocate memory 

    // copy constructor
    MyVector(const MyVector & rhs) : 
        theSize{rhs.theSize},
        theCapacity{rhs.theCapacity}
    {   data=new DataType[theCapacity]; //make new array with the newly specified capacity and copy elements individuallay from other array 
        for (size_t i=0; i<theSize; i++)
            data[i]=rhs.data[i]; 
    }

    // move constructor
    MyVector(MyVector&& rhs):
        theSize{rhs.theSize},
        theCapacity{rhs.theCapacity},
        data{rhs.data}
    {   rhs.theSize=0; 
        rhs.theCapacity=0;  
        rhs.data=nullptr; 
    }

    // copy constructor from STL vector implementation
    MyVector(const std::vector<DataType> & rhs) : //this is us copying from a standard vector instead of from one of our own objects 
        theSize{rhs.size()},
        theCapacity{rhs.size() + SPARE_CAPACITY}
    {   data=new DataType[theCapacity]; 
        for (size_t i=0; i<theSize; i++)
            data[i]=rhs[i]; 
    }

    // destructor
    ~MyVector()
    {   delete[] data; };

    // copy assignment
    MyVector & operator= (const MyVector& rhs)
    {   if (this!=&rhs)
        {   delete[] data; 
            theSize=rhs.theSize; 
            theCapacity=rhs.theCapacity; 
            data=new DataType[theCapacity]; 
            for (size_t i=0; i<theSize; i++)
                data[i]=rhs.data[i]; 
        } 
        return *this; 
    }

    // move assignment
    MyVector & operator= (MyVector && rhs)
    {   if(this!=&rhs) 
        {   delete[] data; 
            theSize=rhs.theSize; 
            theCapacity=rhs.theCapacity; 
            data=rhs.data; 
            rhs.data=nullptr; 
            rhs.theSize=0; 
            rhs.theCapacity=0; 
            rhs.data=nullptr; 
        }
        return *this; 
    }

    // change the size of the array
    void resize(size_t newSize)
    {   if (newSize>theCapacity)
            reserve(newSize+SPARE_CAPACITY); 
        theSize=newSize; 
    }

    // allocate more memory for the array
    void reserve(size_t newCapacity)
    {   if (newCapacity<theCapacity) //possibly wrong newcapacity<=theCapacity then reuturn; 
            return; 
        else 
        {    DataType* newData=new DataType[newCapacity]; 
            for (size_t i=0; i<theSize; i++)
                newData[i]=data[i]; 

            delete[] data; 
            data=newData; 
            theCapacity=newCapacity; 
        }
    }

    // data access operator (without bound checking)
    DataType & operator[] (size_t index) //without bounch checking???? 
    {   return data[index];     }

    const DataType & operator[](size_t index) const
    {   return data[index]; } //is this just it??? like what else can i do for this? 

    // check if the vector is empty; return TURE if the vector is empty
    bool empty() const
    {   return (theSize==0); }

    // returns the size of the vector
    size_t size() const 
    {   return theSize; }

    // returns the capacity of the vector
    size_t capacity() const 
    {   return theCapacity;  }

    // insert an data element to the end of the vector
    void push_back(const DataType & x) //dont create anything new 
    {   if (theSize==theCapacity)
            reserve(2*theCapacity); 
        data[theSize]=x; 
        theSize+=1; 
    }

    void push_back(DataType && x) //take element from other data structure, moving it into our vector and deleting it elsewhere 
    {   if (theSize==theCapacity)
            reserve(2*theCapacity); 
        data[theSize]=std::move(x); 
        theSize+=1; 
    }

    // append a vector as indicated by the parameter to the current vector
    MyVector<DataType>& append(MyVector<DataType> && rhs) //taking segment (vector) and adding it to the back 
    {   if (theSize+rhs.theSize > theCapacity)
            reserve(2*(theSize+rhs.theSize));
        for (size_t i=0; i<rhs.theSize; i++)
            {   data[theSize]=rhs.data[i]; 
                theSize+=1; 
            } 
        rhs.theSize=0;
        rhs.theCapacity=0;
        rhs.data=nullptr;
        
        return *this;
    }

    // remove the last data element from the array
    void pop_back()
    {   if (theSize>0)
            theSize-=1; 
    }

    // returns the last data element from the array
    const DataType& back() const 
    {   if (empty()==false)
            return data[theSize-1]; 
    }

    // iterator implementation
    typedef DataType* iterator; //rename DataType pointer to iterator 
    typedef const DataType* const_iterator; //iterators let you move through data type like beads on a string; incriment the pointer 

    iterator begin()
    {   return data; } //return pointer, if we return array automatically points to first element 

    const_iterator begin() const
    {   return data;     }

    iterator end()
    {   return data+theSize;     }

    const_iterator end() const 
    {   return data+theSize;     }

};
#endif // __MYVECTOR_H__ 