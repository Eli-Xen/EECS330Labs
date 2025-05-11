#ifndef _MY_DISJOINTSETS_H_
#define _MY_DISJOINTSETS_H_

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "MyVector_e602m203.h"

class MyDisjointSets 
{ public:
	explicit MyDisjointSets(const size_t size) // constructor
	{   theSize=size; //initalizes size 
        numSets=size; //number of sets is the same as size since vector is empty rn 
        for (size_t i=0; i<size; ++i)
            parents.push_back(-1); //fill the vector containing sets with -1 to indicate that the set has just the index 
        // // code begins

        // code ends
	}

    ~MyDisjointSets(void) // destructor
    {   
        // code begins

        // code ends
    }
	
    // finds the ID of the set that contains x
    // implements path compression
    int find(const int x) //possibly infinate recursion??? If parents[x] == x, then find(x) will keep calling find(x) forever 
    {   if (parents[x]<0) return x; //base case, if the parent at that item is -1 then return the index 
        if (parents[x] == x) {
            std::cerr << "[ERROR] Infinite recursion: parents[" << x << "] == " << x << std::endl;
            exit(1);
        }        
        return parents[x]=find(parents[x]); //path compression; value at index is now recursing to be the value of the parent 
        //code begins
        // int current=x; 
        // while (parents[current]>=0)
        //     current=parents[current]; //climb to the root 
        // parents[j]=x; 
        // return x; 
        // code ends
    }

    // unions the two sets represented by r1 and r2, respectively
    // (r1 and r2 should be roots)
    // implements the union-by-size approach
	void unionSets(const int x, const int y) 
	{   int r1 = find(x);  // Make sure x is a root
        int r2 = find(y);  // Make sure y is a root
        if (r1==r2) return; //if theyre the same set do nothing 
        if (parents[r1]<parents[r2]) //if tree 1 is larger 
        {   parents[r1]+=parents[r2]; //add parent of r2 to r1 
            parents[r2]=r1; //makes parent of r2 into r1 
        }
        else //if tree 2 is bigger 
        {   parents[r2]+=parents[r1]; //parent of r2 is not r2+r1 
            parents[r1]=r2; //parent of r1 is now r2 
        }
        --numSets; //decrease since we merged two sets 
        // code begins
        // if (parents[r1]<parent[r2] && parents[r1]<0 && parents[r2]<0) 
        // {   size_t temp=parents[r1]; 
        //     parents[r1]=parents[r2]; 
        //     parents[r2]=temp; 
        // }
        // parents[r2]=parents[r2]+parents[r1]; //update the set size; add two negative numbers togeter 
        // parents[r1]=parents[r2]; //attach the subtree 

        // code ends

        // if (parents[r2]<parents[r2]) //if r2 is larger 
        //     parents[r1]=r2; //make r2 new root 
        // else 
        // {   if (parents[r1]==parents[r2])
        //     --parents[r1]; //update heihg if same 
        //     parents[r2]=r1;// make r1 new root 
        // }
	}

    
    size_t setSize(const int x) // returns the size of the set that contains x
    {   return (-parents[find(x)]); //get root of x since it stores the negative of the size 
        // code begins

        // code ends
    }

    size_t size() // returns the total number of elements in the disjoint set
    {   return theSize;
        // code begins

        // code ends
    }

    size_t numDisjointSets() // returns the number of disjoint sets
    {   return numSets; 
        // code begins

        // code ends
    }

  private:
	size_t theSize;             // the number of elements in the disjoint sets
    size_t numSets;             // the number of disjoint sets maintained
    MyVector<int> parents;      // the info array; if the index is not the root, it contains the ID of its parent; otherwise it contains the size of the set as a negative number

};

#endif  // _MY_DISJOINTSETS_H_
