#include "defines.h"
#include "Stack.h"
#include <cstdlib>
#include <cstring>


template class Stack<int8>;
template class Stack<char>;
template class Stack<double>;
template class Stack<uint>;


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
    else
    {
        std::memcpy(buffer, buffer + 1, (numElements - 1) * sizeof(T));
        buffer[numElements - 1] = elem;
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


bool ConverterToInteger::IsInterger() const
{
    for (int i = 0; i < Size(); i++)
    {
        if ((*this)[i] >= '9' || (*this)[i] <= '0')
        {
            return false;
        }
    }

    return true;
}


int ConverterToInteger::GetValue() const
{
    int result = 0;

    int pow = 1;

    for (int i = Size() - 1; i >= 0; i--)
    {
        result += (*this)[i] * pow;
        pow *= 10;
    }

    return result;
}

