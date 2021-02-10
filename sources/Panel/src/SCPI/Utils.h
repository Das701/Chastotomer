#pragma once


namespace SCPI
{
    struct SU
    {
        // Возвращает true, если указатель указывает на завершающую последовательность
        static bool IsLineEnding(pchar *bufer);

        // Если строка buffer начинается с последовательности символов word, то возвращает указатель на символ,
        // следующий за последним символом последовательности word. Иначе возвращает nullptr.
        static pchar BeginWith(pchar buffer, pchar word);

        // Считывает целое число до первого нецифрового символа. Если что-то считано, возвращает true, иначае
        // возвращает false. В outEnd возвращает указатель на символ, следующий за последним считанных
        static bool ReadIntegerValue(pchar buffer, int *outValue, pchar *outEnd);
    };
}
