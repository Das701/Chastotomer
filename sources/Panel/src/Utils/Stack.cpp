#include "defines.h"
#include "Stack.h"
#include <cstdlib>


template class Stack<uint>;
template class Stack<int8>;
template class Stack<uint8>;


template <typename T>
Stack<T>::Stack(int _size) : buffer(0), size(_size), numElements(0U)
{
    buffer = static_cast<T *>(std::malloc(sizeof(T) * _size));
}


template <typename T>
Stack<T>::~Stack()
{
    std::free(buffer);
}


template <typename T>
void Stack<T>::Push(T elem)
{
    if(numElements < size)
    {
        buffer[numElements] = elem;
        numElements++;
    }
}


template <typename T>
T Stack<T>::Pop()
{
    if(numElements != 0)
    {
        numElements--;
        return buffer[numElements];
    }

    return (T)0;
}


template <typename T>
int Stack<T>::Size() const
{
    return numElements;
}


template <typename T>
bool Stack<T>::Empty() const
{
    return Size() == 0;
}


template <typename T>
int Stack<T>::NumFirstZeros() const
{
    int result = 0;

    for(int i = 0; i < size; i++)
    {
        if(buffer[i] != 0)
        {
            break;
        }
        result++;
    }

    return result;
}
