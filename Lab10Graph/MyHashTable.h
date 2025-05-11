#ifndef __MYHASHTABLE_H__
#define __MYHASHTABLE_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>

#include "MyVector.h"
#include "MyLinkedList.h"

//g++ --std=c++11 MainTest.cpp -o Lab6 

// for i in {0..9}; do
//     ./Lab6 Inputs/input_$i.txt > output_$i.txt
//     python3 GradingScript.py output_$i.txt Outputs/output_$i.txt
// done

// for i in {0..9}; do
//     printf $i" my output vs given output line be line:\n"
//     diff output_$i.txt Outputs/output_$i.txt
//     printf "\n\n"
// done


static const long long uh_param_a = 53;       // universal hash function parameter a
static const long long uh_param_b = 97;       // universal hash function parameter b
static const long long prime_digits = 19;     // parameter used for finding a Mersenne prime
static const long long mersenne_prime = (1 << prime_digits) - 1;  // the Mersenne prime for universal hashing

long long fastMersenneModulo(const long long n) // fast calculation of (n modulo mersenne_prime)
{   long long result=(n & mersenne_prime) + (n >> prime_digits); //binary multiplication and shift right 
    if (result>=mersenne_prime) result-=mersenne_prime;  //if result too large subtract the mersenne prime so result always <=mersenne prime 
    return result;
}

template <typename KeyType> // definition of the template hash function class
class HashFunc
{ public:
    long long univHash(const KeyType key, const long long table_size) const; //holds the universal hash 
};

template <>
class HashFunc<long long> // the hash function class that supports the hashing of the "long long" data type
{ public:
    long long univHash(const long long key, const long long table_size) const
    {   long long hv = fastMersenneModulo(static_cast<long long>(uh_param_a * key + uh_param_b));
        hv = hv % table_size; 
        return hv;
    }
};


template <>
class HashFunc<std::string> // the hash function class that supports the hashing of the "std::string" data type
{ private:
    const int param_base = 37; //the base used for inflating each character so hash table isnt biased 
  public:    
    long long univHash(const std::string& key, const long long table_size) const
    {   long long hv = 0;
        for(size_t i = 0; i < key.length(); ++ i)
        {   hv = param_base * hv + static_cast<long long>(key[i]); }

        hv = fastMersenneModulo(static_cast<long long>(uh_param_a * hv + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};


template <typename KeyType, typename ValueType>
class HashedObj // definition of the template hashed object class
{ public:
    KeyType key;
    ValueType value;

    HashedObj()
    {   return; }

    HashedObj(const KeyType& k, const ValueType& v) :
        key(k),
        value(v)
    {   return; }
  
    HashedObj(KeyType && k, ValueType && v) :
        key(std::move(k)),
        value(std::move(v))
    {   return; }

    bool operator==(const HashedObj<KeyType, ValueType>& rhs)
    {   return (key == rhs.key); }    

    bool operator!=(const HashedObj<KeyType, ValueType>& rhs)
    {   return !(*this == rhs); }   

};


template <typename KeyType, typename ValueType>
class MyHashTable
{ private:
    size_t theSize; // the number of data elements stored in the hash table
    MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > hash_table;    // the hash table implementing the separate chaining approach
    MyVector<size_t> primes;    // a set of precomputed and sorted prime numbers

    void preCalPrimes(const size_t n) // pre-calculate a set of primes using the sieve of Eratosthenes algorithm; called if table doubling needs a larger prime for table size 
    {   if (!primes.empty() && primes.back()>=n) return; //if we already have primes >=n dont do anything 
        std::vector<bool> is_prime(n+1, true);
        is_prime[0]=is_prime[1]=false;

        for (size_t i=2; i*i<=n; ++i) 
        {   if (is_prime[i]) //if set to true then the multiple of that index is set to false 
            {   for (size_t j=i*i; j<=n; j+=i) 
                    is_prime[j] = false; 
            }
        }

        for (size_t i=2; i<=n; ++i) 
        {   if (is_prime[i]) //if its set to true the put it onto primes vector 
            {   if (i==7 || i==11 || i==13) continue; //wanted to have it skip 7 being prime 
                primes.push_back(i);
            }
                
        }
    }

    size_t nextPrime(const size_t n) // finding the smallest prime that is larger than or equal to n; perform binary search primes vector 
    {   size_t prime=primes.back(); //automate to largest possible prime we have 
        if (primes.empty() || n>primes.back()) preCalPrimes(n*2); //if no primes where n>= one then make more 
        size_t left=0; //left end 
        size_t right=primes.size()-1; //right end 
        while(left<=right) //until they both come to the middle of vector 
        {   size_t middle=(left+right)/2; //middle of the range we are searching 
            if (primes[middle]>=n) //if its greater than equal to current size... 
            {   prime=primes[middle]; //...save it 
                right=middle-1; //search left for smaller prime if possible 
            }
            else left=middle+1; //adjust boundary to search right for larger prime 
        }
        return prime; 
    }

    typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator find(const KeyType& key) //return iterator for particular key(thing) in LL; returns the end() iterator if not found
    {   HashFunc<KeyType> hash_func; 
        long long bucket=hash_func.univHash(key, static_cast<long long>(hash_table.size())); //computes the hash index value to be inserted at 
        for(auto itr=hash_table[bucket]->begin(); itr!=hash_table[bucket]->end(); ++itr)
        {   if ((*itr).key==key) //check this 
                return itr; 
        }
        return hash_table[bucket]->end(); 
    }

    void rehash(const size_t new_size) //rehashes all data elements in the hash table into a new hash table with new_size; note that the new_size can be either smaller or larger than the existing size; check prime list if double/halving 
    {   size_t oldSize=theSize; 
        MyVector<MyLinkedList<HashedObj<KeyType, ValueType>>*> tmp_hash_table(new_size); //make new hash table to move items to 
        for (size_t i=0; i<tmp_hash_table.size(); ++i) 
           tmp_hash_table[i]=new MyLinkedList<HashedObj<KeyType, ValueType>>(); //for each bucket make a new LL with a hash object 
                
        std::swap(hash_table, tmp_hash_table); //replace old hash table with new hash table 
        
        for (size_t i=0; i<tmp_hash_table.size(); ++i)
        {   for(auto itr=tmp_hash_table[i]->begin(); itr!=tmp_hash_table[i]->end(); ++itr)
            {   insert(std::move(*itr)); 
                //theSize=0; //this will avoid skipping 7, 7 will be present 
            }
        }
        
        theSize=oldSize; 
        return; 
    }

    void doubleTable() // doubles the size rehashes; new table size is smallest prime that is larger than the expected new table size (double of the old size)
    {   size_t new_size = nextPrime(2 * hash_table.size()); 
        //std::cout << "doubleTable() new_size: " << new_size << std::endl; 
        this->rehash(new_size);
        return;
    }

    void halveTable() // halves the size of the table and rehashes; new table size is smallest prime that is larger than the expected new table size (half of the old size)
    {   size_t new_size=nextPrime(ceil(hash_table.size() / 2));
        this->rehash(new_size);
        return;
    }

  public: 
    explicit MyHashTable(const size_t init_size = 3) // the default constructor; allocate memory if necessary
    {   preCalPrimes(100); //precompute more primes upon initilization 
        hash_table.resize(init_size); //resize the vector to be size 3  
        for (size_t i=0; i<init_size; ++i) //push LLs onto vector 3 times; vector has init size of 3 so we have 3 spaces al holding linked lists initalized with a hashed object 
            hash_table[i]=new MyLinkedList<HashedObj<KeyType, ValueType>>; 
        theSize=0; 
        return; 
    }

    ~MyHashTable() // the default destructor; collect memory if necessary 
    {   for (size_t i=0; i<hash_table.size(); ++i) //goes through and manually deletes all elements in the vector 
            delete hash_table[i]; 
        return; 
    }
    
    bool contains(const KeyType& key) // checks if the hash table contains the given key
    {   HashFunc<KeyType> hash_func; 
        long long bucket=hash_func.univHash(key, static_cast<long long>(hash_table.size())); 
        
        for (auto itr=hash_table[bucket]->begin(); itr!=hash_table[bucket]->end(); ++itr)
        {   if ((*itr).key==key) return true; } 
        return false; 
    }

    bool retrieve(const KeyType& key, HashedObj<KeyType, ValueType>& data) // retrieves the data element that has the specified key; returns true if the key is contained in the hash table otherrise false 
    {   HashFunc<KeyType> hash_func; 
        long long bucket=hash_func.univHash(key, static_cast<long long>(hash_table.size())); 
        for(auto itr=hash_table[bucket]->begin(); itr!=hash_table[bucket]->end(); ++itr) 
        {   if ((*itr).key==key)
            {   data=*itr; 
                return true; 
            }
        }
        return false; 
    }
    
    bool insert(const HashedObj<KeyType, ValueType>& x) // inserts the given data element into the hash table (copy); returns true if the key is not contained in the hash table, false otherwise 
    {   if (contains(x.key)) //this was missing 
        {   //std::cout << "\tKey " << x.key << " already in table." << std::endl;
            return false;
        }
        HashFunc<KeyType> hash_func; 
        long long bucket=hash_func.univHash(x.key, static_cast<long long>(hash_table.size())); //computes the hash index value to be inserted at 
        hash_table[bucket]->push_back(x); //put x to the end of the LL of the vector bucket 
        ++theSize; //incriment size since we added something 

        if (2*theSize>=hash_table.size()) 
        {   //std::cout << "\tTable size exceeded half. Doubling..." << std::endl;
            doubleTable(); //if capacity exceeds half then double 
        }
        return true; //insert sucessful 
    }
 
    bool insert(HashedObj<KeyType, ValueType> && x) // inserts the given data element into the hash table (move); returns false if the key is contained in the hash table
    {   if(contains(x.key)) //check that x isnt already in table 
        {   //std::cout << "\tKey " << x.key << " already in table." << std::endl; 
            return false; 
        }
        
        HashFunc<KeyType> hash_func; 
        long long bucket=hash_func.univHash(x.key, static_cast<long long>(hash_table.size())); //computes the hash index value to be inserted at 
        hash_table[bucket]->push_back(std::move(x)); //put x to the end of the LL of the vector bucket using move         
        ++theSize; //incriment size since we added something 

        if (2*theSize>=hash_table.size()) 
        {   //std::cout << "\tTable size exceeded half. Doubling..." << std::endl;
            doubleTable(); //if capacity exceeds half then double 
        }
        return true; //insert sucessful 
    }
    
    bool remove(const KeyType& key) // removes the data element that has the key from the hash table; returns true if the key is contained in the hash table and was deleted, false if item not in hash table 
    {   HashFunc<KeyType> hash_func; 
        long long bucket=hash_func.univHash(key, static_cast<long long>(hash_table.size())); 
        for(auto itr=hash_table[bucket]->begin(); itr!=hash_table[bucket]->end(); ++itr) 
        {   if ((*itr).key==key)
            {   //std::cout << "Trying to remove key " << key << " from bucket " << bucket << std::endl;
                hash_table[bucket]->erase(itr); //go to LL and call erase on the index of LL 
                --theSize; //decriment size since we deleted an item 
                if (8*theSize<hash_table.size()) halveTable(); //if capacity exceeds half then double 
                return true; 
            }
        }
        return false; //if item was not found in hash table 
    }

    
    size_t size() // returns the number of data elements stored in the hash table
    {   return theSize; }

    // returns the capacity of the hash table
    size_t capacity()
    {   return hash_table.size(); } //return the number of LL in vector 


    void keys(MyVector<KeyType>& keyVec) const //method suggested to support .keys(), returns all keys to vector passed 
    {
        //keyVec.clear(); // optional: clear the vector first
        for (size_t i = 0; i < hash_table.size(); ++i)
        {
            for (auto itr = hash_table[i]->begin(); itr != hash_table[i]->end(); ++itr)
            {
                keyVec.push_back((*itr).key);
            }
        }
    }

};


#endif // __MYHASHTABLE_H__
