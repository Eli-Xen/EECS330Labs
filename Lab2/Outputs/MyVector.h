#ifndef __MYVECTOR_H__
#define __MYVECTOR_H__

#include <vector>
#include <stdexcept> // For exception handling

template <typename DataType>
class MyVector
{
  private:
    size_t theSize;       // Number of data elements the vector is currently holding
    size_t theCapacity;   // Maximum data elements the vector can hold
    DataType *data;       // Address of the data storage

  public:
    static const size_t SPARE_CAPACITY = 16; // Initial capacity of the vector

    // Default constructor
    explicit MyVector(size_t initSize = 0) 
        : theSize{initSize}, theCapacity{initSize + SPARE_CAPACITY} 
    {
        data = new DataType[theCapacity];
    }

    // Copy constructor
    MyVector(const MyVector &rhs) 
        : theSize{rhs.theSize}, theCapacity{rhs.theCapacity} 
    {
        data = new DataType[theCapacity];
        for (size_t i = 0; i < theSize; i++)
            data[i] = rhs.data[i];
    }

    // Move constructor
    MyVector(MyVector&& rhs) noexcept
        : theSize{rhs.theSize}, theCapacity{rhs.theCapacity}, data{rhs.data} 
    {
        rhs.theSize = 0;
        rhs.theCapacity = 0;
        rhs.data = nullptr;
    }

    // Copy constructor from STL vector
    MyVector(const std::vector<DataType> &rhs) 
        : theSize{rhs.size()}, theCapacity{rhs.size() + SPARE_CAPACITY} 
    {
        data = new DataType[theCapacity];
        for (size_t i = 0; i < theSize; i++)
            data[i] = rhs[i];
    }

    // Destructor
    ~MyVector() 
    {
        delete[] data;
    }

    // Copy assignment
    MyVector &operator=(const MyVector &rhs) 
    {
        if (this != &rhs) 
        {
            delete[] data;
            theSize = rhs.theSize;
            theCapacity = rhs.theCapacity;
            data = new DataType[theCapacity];
            for (size_t i = 0; i < theSize; i++)
                data[i] = rhs.data[i];
        }
        return *this;
    }

    // Move assignment
    MyVector &operator=(MyVector &&rhs) noexcept
    {
        if (this != &rhs) 
        {
            delete[] data;
            theSize = rhs.theSize;
            theCapacity = rhs.theCapacity;
            data = rhs.data;

            rhs.theSize = 0;
            rhs.theCapacity = 0;
            rhs.data = nullptr;
        }
        return *this;
    }

    // Change the size of the array
    void resize(size_t newSize) 
    {
        if (newSize > theCapacity)
            reserve(newSize + SPARE_CAPACITY);
        theSize = newSize;
    }

    // Allocate more memory for the array
    void reserve(size_t newCapacity) 
    {
        if (newCapacity <= theCapacity)
            return;

        DataType *newData = new DataType[newCapacity];
        for (size_t i = 0; i < theSize; i++)
            newData[i] = data[i];

        delete[] data;
        data = newData;
        theCapacity = newCapacity;
    }

    // Data access operator (without bound checking)
    DataType &operator[](size_t index) 
    {
        return data[index];
    }

    const DataType &operator[](size_t index) const 
    {
        return data[index];
    }

    // Check if the vector is empty
    bool empty() const 
    {
        return (theSize == 0);
    }

    // Returns the size of the vector
    size_t size() const 
    {
        return theSize;
    }

    // Returns the capacity of the vector
    size_t capacity() const 
    {
        return theCapacity;
    }

    // Insert an element at the end of the vector
    void push_back(const DataType &x) 
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity);
        data[theSize++] = x;
    }

    void push_back(DataType &&x) 
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity);
        data[theSize++] = x; // Directly assign instead of `std::move(x)`
    }

    // Append a vector to the current vector
    MyVector<DataType> &append(MyVector<DataType> &&rhs) 
    {
        if (theSize + rhs.theSize > theCapacity)
            reserve(theSize + rhs.theSize + SPARE_CAPACITY);

        for (size_t i = 0; i < rhs.theSize; i++)
            data[theSize++] = rhs.data[i];

        rhs.theSize = 0;
        rhs.data = nullptr;

        return *this;
    }

    // Remove the last data element from the array
    void pop_back() 
    {
        if (theSize > 0)
            --theSize;
    }

    // Returns the last data element from the array
    const DataType &back() const 
    {
        if (empty())
            throw std::out_of_range("Vector is empty");
        return data[theSize - 1];
    }

    // Iterator implementation
    typedef DataType *iterator;
    typedef const DataType *const_iterator;

    iterator begin() 
    {
        return data;
    }

    const_iterator begin() const 
    {
        return data;
    }

    iterator end() 
    {
        return data + theSize;
    }

    const_iterator end() const 
    {
        return data + theSize;
    }
};

#endif // __MYVECTOR_H__
