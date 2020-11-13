#pragma once


template<typename T>
class Stack
{
public:
    explicit Stack(int size);
    
    ~Stack();
    
    void Push(T elem);
    
    T Pop();
    
    int Size() const;
    
    bool Empty() const;
    
    // ¬озвращает количество нулей на дне стека
    int NumFirstZeros() const;

    T &operator [](const int index) const { return buffer[index]; }

    void Clear() { numElements = 0; };

    bool IsFull() const { return numElements == size; }

private:
    T *buffer;
    int size;
    int numElements;
};
