#pragma once


namespace SCPI
{
    struct SU
    {
        // ¬озвращает true, если указатель указывает на завершающую последовательность
        static bool IsLineEnding(pchar *bufer);

        // ≈сли строка buffer начинаетс€ с последовательности символов word, то возвращает указатель на символ,
        // следующий за последним символом последовательности word. »наче возвращает nullptr.
        static pchar BeginWith(pchar buffer, pchar word);
    };
}
