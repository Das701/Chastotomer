#pragma once


namespace SCPI
{
    struct SU
    {
        // ¬озвращает true, если указатель указывает на завершающую последовательность
        static bool IsLineEnding(pchar *bufer);
    };
}
