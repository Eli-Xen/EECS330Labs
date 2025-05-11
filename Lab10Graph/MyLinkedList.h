#ifndef _MY_LINKEDLIST_H_
#define _MY_LINKEDLIST_H_

#include <algorithm>
#include <iostream>

//this code gives a yes with commands (and also delete extra space from all given output_#.txt files): 
//g++ --std=c++11 MainTest.cpp -o Lab3 
//./Lab3 Inputs/input_0.txt > output_0.txt 
//python3 GradingScript.py output_0.txt Outputs/output_0.txt

template <typename DataType>
class MyLinkedList
{ private:
    
    struct Node
    {   DataType  data; //data held in node 
        Node   *prev; //previous node 
        Node   *next; //next node 
        
        //copy constructor for node 
        Node(const DataType &d = DataType{ }, Node *p = nullptr, Node *n = nullptr) : 
        data{d}, 
        prev{p}, 
        next{n} 
        { }
        
        //move constructor for node 
        Node(DataType&& d, Node* p = nullptr, Node* n = nullptr) : 
        data{std::move(d)}, 
        prev{p}, 
        next{n} 
        { }
    };

    int theSize;        // the number of elements that the linked list is currently holding
    Node *head;         // pointer to the head node; does not hold real data
    Node *tail;         // pointer to the tail note; does not hold real data

    void init( )
    {   theSize=0; 
        head=new Node; //empty node that doest hold data, just signifies where the start is 
        tail=new Node; //empty node that doest hold data, just signifies where the end is 
        head->next=tail; //initiates the head and tail to be connected 
        tail->prev=head; 
    
        return;
    }

  public:
    class const_iterator
    { protected:
        Node *current; //will be used by the iterators; current stores a pointer to the node that the iterator is currently pointing to 
        // returns a mutable object
        // defined as protected as we don't expect it to be called directly by other classess
        DataType& retrieve() const //returns data stored in the current node 
        {   return current->data;  }
        // constructor to be hidden from other classes; 
        // we don't expect direct construction of iterator form pointer
        const_iterator(Node *p) : //constructor for the const_iterator class, param passed is pointer to node current 
        current{p} 
        {}
        
        friend class MyLinkedList<DataType>; //friend class has access to private/protected members of const_iterator

      public:
        const_iterator() : 
        current{nullptr} 
        {}

        // add const to the return type and make the return value by retrieve() non-mutable
        const DataType& operator* () const
        {   return retrieve(); } //returns an immutable version of data in the node that * is acted upon 
        
        // move to the next data element
        // the prefix increment (e.g. ++ p) with 0 parameter
        const_iterator& operator++ () //returns updated iterator 
        {   current=current->next; 
            return *this; //returns pointer to the next element 
        }

        // move to the next data element
        // the suffix increment (e.g. p ++) with 1 int parameter
        const_iterator operator++ ( int ) //returns iterator before incriment; still incriments by 1, the int parameter is just to differentiation from the other operator++ above 
        {   const_iterator old=*this; //makes iterator of current 
            ++(*this); //incriments 
            return old; //returns the iterator we stored 
        }

        // move to the previous data element
        // the prefix decrement (e.g. -- p) with 0 parameter
        const_iterator& operator-- () //returns updated iterator 
        {   current=current->prev; //moves iterator to previous element 
            return *this; //returns the iterator 
        }

        // move to the previous data element
        // the suffix decrement (e.g. p --) with 1 int parameter
        const_iterator operator-- (int)
        {   const_iterator old=*this; //store current iterator state 
            --(*this); //incriment 
            return old; //return old iterator state 
        }

        // check whether two iterators point to the same data element
        bool operator== (const const_iterator& rhs) const
        {   return current==rhs.current; } //compare the two and return the bool of that 
        

        // check whether two iterators point to the same data element
        bool operator!= (const const_iterator & rhs) const
        {   return !(*this==rhs); } //compare if our current itarator isnt the same as same as the one given 
      
    };

    // define the iterator class; inherit from class const_iterator
    class iterator : public const_iterator
    { protected:
        iterator(Node *p) : 
        const_iterator{p}
        {}
        
        friend class MyLinkedList<DataType>;
      
      public:
        // default constructor
        iterator() //default constructor for iterator 
        {}

        // the return type is mutable
        DataType& operator* ()
        { return const_iterator::retrieve(); } //since iterator ingerits from const_iterator, it uses its retrieve function 
           
        // keep the return type as non-mutable
        const DataType& operator* () const
        {   return const_iterator::operator*();   } //uses const_iterator's overload * function 

        iterator & operator++ ()
        {   this->current=this->current->next; //makes the current node equal to its next node 
            return *this; 
        }

        iterator operator++ (int)
        {   iterator old=*this; //saves current iterator state 
            ++(*this); //incriments 
            return old; //still returns old iteratator 
        }

        iterator & operator-- ()
        {   this->current=this->current->prev; //makes the current iterator equal to the prev element  
            return *this; 
        }
        
        iterator operator-- (int)
        {   iterator old=*this; //saves state of current iterator 
            --(*this); //decrements current iterator 
            return old; //returns saved state of old iterator 
        }
    };
  
  public:    // defining the MyLinkedList class methods
    MyLinkedList() // default constructor
    {   init(); } //call to initializer 
  
    MyLinkedList(const MyLinkedList& rhs) // copy constructor
    {   init(); //call to initializer
        for (auto & x : rhs) //for every x in rhs; auto fills in the type for x and & uses referance 
            push_back(x); 
    }

    MyLinkedList (MyLinkedList&& rhs) :    // move constructor
    theSize(rhs.theSize), //moves the size 
    head(rhs.head), //moves the head 
    tail(rhs.tail) //moves the tail
    {   rhs.theSize=0; //make other's size null
        rhs.head=nullptr; //make other's head null
        rhs.tail=nullptr; //make other's tail null
    }

    // destructor
    ~MyLinkedList() //clears/deletes all elements and then deletes head and tail 
    {   clear(); 
        delete head; 
        delete tail; 
    }

    MyLinkedList & operator= (const MyLinkedList& rhs) // copy assignment
    {   MyLinkedList copy=rhs; //saves other LL to copy var 
        std::swap(*this, copy); //uses sawp swap on out LL and copy 
        return *this; //return pointer 
    }
  
    MyLinkedList & operator= (MyLinkedList && rhs) // move assignment; swap each element 
    {   std::swap(theSize, rhs.theSize); 
        std::swap(head, rhs.head); 
        std::swap(tail, rhs.tail); 
        return *this; 
    }

    
    iterator begin() //returns first element 
    {   return (head->next);     }

    const_iterator begin() const //returns const of first element 
    {   return (head->next);     }
  
    iterator end() //returns the tail which doesnt hold anything 
    {   return tail;     }

    const_iterator end() const
    {   return tail;     }
    
    int size() const // gets the size of the linked list
    {   return theSize;     }

    bool empty( ) const // checks if the linked list is empty; return True if empty, false otherwise
    {   return size()==0;    }

    void clear( ) // deletes all nodes excepts the head and tail
    {   while(!empty())
            pop_front(); 
    }

    DataType& front() // return the first data element as mutable
    {   return *begin();     }

    const DataType& front() const // return the first data element as non-mutable
    {   return *begin();     }

    DataType& back() // return the last data element as mutable
    {   return *--end(); }

    const DataType& back() const // return the last data element as non-mutable
    {   return *--end();    }

    iterator insert(iterator itr, const DataType& x) // insert const of x before itr; return iterator pointing to the newly inserted data element
    {   Node *p=itr.current; //saves current node 
        theSize++; //incriments the size 
        return {p->prev=p->prev->next=new Node(x, p->prev, p)}; //new node is made with x, previous's next is updated to be the new new 
    }

    iterator insert(iterator itr, DataType&& x) // insert x before itr; return iterator pointing to the newly inserted data element
    {   Node *p=itr.current; 
        theSize++; 
        return {p->prev=p->prev->next=new Node(std::move(x), p->prev, p)}; 
    }

    iterator erase(iterator itr) // delete the data element pointed by itr; return the iterator pointing to the data element next to the one being deleted
    {   Node *p=itr.current; //saves current iterator element 
        iterator retVal(p->next); //iterator taht stores current's next 
        p->prev->next=p->next; //update previous element's next to be current's next, skipping over an element 
        p->next->prev=p->prev; //update next elements prev to be current's prev, skipping over an element 
        delete p; //after detaching all pointers from it, we delete temp 
        theSize--; //decrement the size 
        return retVal; //return the next iterator 
    }

    // delete the data elements from iterator "from" (inclusive) to iterator "to" (exclusive)
    // return the iterator "to"
    iterator erase(iterator from, iterator to)
    {   for(iterator itr=from; itr!=to;) //go from "from" to "to"; while from!=to do erase 
            itr=erase(itr); //earase moves itr forward for us so we dont have to incriment 
        return to; //return iterator so caller has referance after elements are erased 
    }

    void push_front(const DataType& x) // insert data element x to the very begining of the linked list
    {   insert(begin(),x);     } //copy vs ...
  
    void push_front(DataType&& x) //move assignment 
    {   insert(begin(),std::move(x));     }
    
    void push_back(const DataType& x) // insert data element x to the very end of the linked list
    {   insert(end(),x);     }
    
    void push_back(DataType&& x)
    {   insert(end(),std::move(x));     }
    
    void pop_front() // delete the very first data element
    {   erase(begin());     }
    
    void pop_back( ) // delete the very last data element
    {   erase(--end()); }
    

    void reverseList()
    {   Node *current=head; //save the head to a node
        while (current!=nullptr) //until we reach nullptr...
        {   Node *temp=current->next; //save state of next 
            current->next=current->prev; //switch next and prev pointers 
            current->prev=temp; //prev is now next 
            if (temp==nullptr) //extra check so my code doesnt explode; checks for end of list 
            {   tail=head; //switch head and tail 
                head=current; //and head will be current which is at the end (tail)
            }
            current=temp; //go to next node so loop continues properly 
        }
    }

    MyLinkedList<DataType>& appendList(MyLinkedList<DataType>&& rlist)
    {   for (auto &x : rlist) //for x in rlist (auto autofills the type of x) 
            push_back(x); //push x onto our own list 
        return *this; //return pointer to our own list 
    }

    bool swapAdjElements(iterator& itr)
    {   Node *p=itr.current; //use node instead of iterator because iterator was breaking stuff 
        if (p->next==tail) //check for end tail 
            return false; //if it is the tail dont do anything and return false since we didnt swap 
        Node *nextNode=p->next; //make node to hold the next node 
        p->prev->next=nextNode; //previous node will point to nextNode 
        nextNode->next->prev=p; //prev of node after will point to current node 
        p->next=nextNode->next; //current's next will skip over next node 
        nextNode->prev=p->prev; //nextNode's prev will skip over current node 
        p->prev=nextNode; //current's prev will be at nextNode 
        nextNode->next=p; //nextNode's next will be at current node 
        return true; //swapping complete 
    }

};
#endif // __MYLIST_H__