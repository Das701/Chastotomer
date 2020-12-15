#include "defines.h"
#include "Buffer.h"
#include <cstdlib>
#include <cstring>



Buffer::Buffer(int s)
{
    Malloc(s);
}


Buffer::~Buffer()
{
    Free();
}


void Buffer::Realloc(int _size)
{
    Free();
    Malloc(_size);
}


void Buffer::Fill(uint8 value)
{
    if (size)
    {
        std::memset(data, value, static_cast<uint>(size));
    }
}


void Buffer::Free()
{
    std::free(data); //-V2511
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(int s)
{
    if (s > 0)
    {
        data = static_cast<uint8 *>(std::malloc(static_cast<uint>(s))); //-V2511
        size = (data) ? s : 0;

        if(!data)
        {
//            LOG_ERROR("Нет памяти");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}
