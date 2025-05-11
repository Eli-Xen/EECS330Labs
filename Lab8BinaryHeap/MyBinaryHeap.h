#ifndef __MYBINARYHEAP_H__
#define __MYBINARYHEAP_H__

#include <iostream>
#include <cstdlib>

#include "MyVector.h"

// ComparableType should be comparable (<, >, ==, >=, <= operators implemented)
// ComparableType should also have +, +=, -, -= operators implemented to support priority adjustment

//g++ --std=c++11 MainTest.cpp -o Lab8 

// for i in {0..9}; do
//     ./Lab8 Inputs/input_$i.txt > output_$i.txt
//     python3 GradingScript.py output_$i.txt Outputs/output_$i.txt
// done

// for i in {0..9}; do
//     printf $i" my output vs given output line be line:\n"
//     diff output_$i.txt Outputs/output_$i.txt
//     printf "\n\n"
// done

//potential issue: removed -1 from p>data.size()-1 in percolate and increase/decreasekey becuase this stopped the out of range errors from being thrown

template <typename ComparableType>
class MyBinaryHeap 
{ private:
	MyVector<ComparableType> data;  //array that holds the data elements

    void percolateUp(const size_t p) // moves the data element at the pth position of the array up
    {   if (p>data.size()-1) //if data index is larger than index throw error 
        {   std::cerr << "invalid index " << p << " for percolateUp \tdata.size() = " << data.size() << "\n"; 
            throw std::out_of_range("percolateUp"); 
        }
        data[0]=std::move(data[p]); //otherwise store temporarily in index 0 (empty) 
        size_t hole=p; //mark where the hole/empty node is 
        for (; data[0]>data[hole/2]; hole/=2) //shoft nodes if theyre smaller than the one were percolating 
            data[hole]=std::move(data[hole/2]); 
        data[hole]=std::move(data[0]); //move our percolated element into correct position, there is now a an empty node for it 
        return; 
    }

    void percolateDown(const size_t p) // moves the data element at the pth position of the array down
    {   if (p>data.size()-1) //checks that index is in range of array 
            throw std::out_of_range("percolateDown"); 
        size_t hole=p, child; //mark the hole to be item were percolating 
        data[0]=std::move(data[p]); //remporarily move p element to index 0 
        for (; hole*2<=data.size()-1; hole=child) //iterate through bottom half of array and reset the hole to be the child every time 
        {   child=hole*2; //get child 
            if (child<data.size()-1 && data[child+1]>data[child]) 
                ++child; 
            if (data[0]<data[child]) //if item were percolating down is less than that child... 
                data[hole]=std::move(data[child]); //..then fill the hole with the child 
            else break; 
        }
        data[hole]=std::move(data[0]); //move the temp item we were percolating to the hole 
        return; 
    }

    void buildHeap() // reorders the data elements in the array to ensure heap property
    {   for (size_t i=(data.size()-1)/2; i>0; --i)
            percolateDown(i); 
        return; 
    }	

  public: 
    explicit MyBinaryHeap() : // default constructor
        data(1)     // reserve data[0]
    {    }

    MyBinaryHeap(const MyVector<ComparableType>& items) : // constructor from a set of data elements
        data(1)     // reserve data[0]
    {   for (size_t i=1; i<items.size(); i++) //go through other array/set and push them it our array, assume other array is just full of numbers so we push starting from index 1 
            data.push_back(items[i]); 
        buildHeap(); 
    }

    MyBinaryHeap(const MyBinaryHeap<ComparableType>& rhs) : // copy constructor
        data(rhs.data) //this just uses the copy constructor of MyVector to copy all items into it 
    {   buildHeap();    }

    
    MyBinaryHeap(MyBinaryHeap<ComparableType> && rhs) : // move constructor
        data(std::move(rhs.data)) //use move constructor of MyVector to move elements 
    {   buildHeap();    }

    MyBinaryHeap& operator=(const MyBinaryHeap<ComparableType>& rhs)   // copy assignment
    {   if (this!=&rhs) //if rhs isnt our tree, not passed our own tree to assignment 
        {   clear(); //clears current heap 
            data=rhs.data; //uses MyVector = operator to reset our data MyVector 
            buildHeap(); //or verifyHeapProperty() 
        }
        return *this; 
    }

    MyBinaryHeap& operator=(MyBinaryHeap<ComparableType> && rhs) // move assignment
    {   if (this!=&rhs)
        {   clear(); //clears our current heap 
            data=std::move(rhs.data); //moves data using MyVector = operator but for move instead of copy 
        }
        buildHeap(); 
        return *this;
    }

	void enqueue(const ComparableType& x) // inserts x into the priority queue (copy)
    {   data.push_back(x); //insert it to our vector at the end 
        percolateUp(data.size()-1); //then percolate up 
        return; 
    } 

    void enqueue(ComparableType && x) // inserts x into the priority queue (move)
    {   data.push_back(std::move(x)); //insert it to our vector at the end 
        percolateUp(data.size()-1); //then percoalte up 
        return; 
    }

	const ComparableType& front() // accesses the data element with the highest priority; aka findBest 
    {   return data[1];     } //return first element 

    void dequeue() // deletes the data element with the highest priority from the queue; aka deleteBest 
    {   if (empty()) //if array is empty throw error 
            throw std::out_of_range("DeleteBest/dequeue"); 
        data[1]=data[data.size()-1]; //replace root (deleted element) with the last element 
        data.pop_back(); //delete last element 
        if (!empty()) //if this doesnt make the heap empty... 
            percolateDown(1); //move the now last element at the root down since it was just temporary 
        return; 
    }

    bool verifyHeapProperty(void) // verifies whether the array satisfies the heap property
    {   int n=data.size();
        for (size_t i=1; i<(n-1)/2; ++i) 
        {   size_t leftChild=2*i; //this assumes root is at 0 but here i think we have it at 1 2i+2
            size_t rightChild=2*i+1; //this assumes root is at 0 but here i think we have it at 1 2i+3
        
            if (leftChild<n && data[i]<data[leftChild]) //LST within bounds and parent is smaller than LST 
                return false;
            if (rightChild<n && data[i]<data[rightChild]) //RST within bounds and parent is smaller than RST 
                return false;
        }
        return true;
    }

    void disruptHeapProperty(void) // disrupts heap property by random shuffling
    {   if(data.size() <= 3)
            return;
        for(size_t i = 0; i < 1000; ++ i)
        {   size_t p = 1 + ((int) rand()) % (data.size() - 1);
            size_t q = 1 + ((int) rand()) % (data.size() - 1);
            std::swap(data[p], data[q]);
        }
        return;
    }

    MyBinaryHeap& merge(MyBinaryHeap<ComparableType> && rhs) // merges two heaps; the second heap can be destructed after the merge
    {   for (size_t i=1; i<rhs.size()+1; i++) //go through given heap starting from index 1 to the last element 
            this->enqueue(rhs.data[i]); //add to our heap using out enqueue 
        rhs.clear(); //clear other heap 
        buildHeap(); //rebuild the heap to organize elements 
        return *this; 
    }

	void increaseKey(const size_t p, const unsigned int d) // increases the priority measure of an element at a specific position and reorder the heap
    {   if (p>data.size()-1) //makes sure index given is in range of array 
            throw std::out_of_range("increaseKey"); 
        data[p]=data[p]+d; //value of node is increased by incriment d so its higher priority 
        percolateUp(p); //percolate it up since priority of p increased 
        return; 
    }

	void decreaseKey(const size_t p, const unsigned int d) // decreases the priority measure of an element at a specific position and reorder the heap; if the current priority is smaller than the requested decrement, assign priority 0 
    {   if (p>data.size()-1) 
            throw std::out_of_range("decreaseKey"); 
        if (data[p]>d) //if element at p is larger than decrement then decrement that element by that amount of decremenet 
            data[p]=data[p]-d; 
        else 
            data[p]=0; //pushes all the way to the bottom (i think)
        percolateDown(p); 
        return; 
    }

    bool empty() // checks whether the heap is empty
    {   return data.size()<=1;     }

    void clear() // removes all data elements from the heap
    {   data=MyVector<ComparableType>(); }//sets current data array to a newly-made empty one, is this bad memory managment? 

    size_t size() // returns the size of the heap
    {   return (data.size()<=1) ? 0 : (data.size()-1);     }

    void print() // prints the data in the array
    {   const char delim = '\t';
        for (size_t i = 1; i < data.size(); ++ i) {
            std::cout << data[i] << delim;
        }
        std::cout << std::endl;
        return;
    }

};

#endif // __MYBINARYHEAP_H__