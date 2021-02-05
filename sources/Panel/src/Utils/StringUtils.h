#pragma once
#include "Utils/String.h"


struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


class SU
{
public:
    // Эта команда сразу преобразует к верхенму регистру слово.
    static bool GetWord(const char *string, Word *word, const int numWord);

    static bool WordEqualZeroString(Word *word, pCHAR string);

    // Сравнивает две строки. Число символов берётся из str1
    static bool EqualsStrings(const char *str1, const char *str2);

    static bool EqualsStrings(void *str1, void *str2, int size);

    static bool EqualsStrings(uint8 *str1, const char * const str2, int size);

    static bool EqualsZeroStrings(char *str1, char *str2);

    // Возвращает позицию первого символа, который не является цифрой либо точкой
    static int FirstNotNumeral(char *buffer);

    // Преобразует строку в float. Обрабатываются символы до первого нецифрового символа
    static float StringToFloat(const char *string);

    static char *ToUpper(char *str, int size);

    static char *ToUpper(char *str);

    static char ToUpper(char symbol);

    static char ToLower(char symbol);

    static char *DoubleToString(double value);

    static bool IsDigit(char symbol);

    static bool StringToDouble(double *value, const char *buffer);

    static bool String2Int(const char *buffer, int *value, char **end);
 
    static String Int2String(int n);

    // Оставляет в строке buffer размером size ровно digits цифр. Первые нули тоже учитываются
    static void LeaveFewDigits(char *buffer, int size, int digits);

    // Оставляет в строке buffer размером size ровно digits значащих цифр. Без учёта первых незначащих нулей
    static void LeaveFewSignedDigits(char *buffer, int size, int digits);

    // Заменить в строке все символы old на new
    static void ReplaceSymbol(char *string, char oldSymbol, char newSymbol);

    // Возвращает true, если в строке существует символ symbol
    static bool ExistSymbol(char *string, char symbol);
};


#ifndef WIN32

int strcpy_s(char *dest, uint dest_size, const char *src);

#endif
