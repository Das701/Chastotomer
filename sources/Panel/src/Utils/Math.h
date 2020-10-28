#pragma once


#define LIMIT_BELOW(x, min)                 if((x) < (min)) { x = (min); }

#define LIMIT_ABOVE(x, max)                 if((x) > (max)) { x = (max); }

#define SET_MIN_IF_LESS(x, min)             if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)           if((x) > (max)) { (max) = (x); }

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };
// \todo Ёто выражение неправильно работает с отрицательными числами
#define ROUND(type, x) ((type)((x) + 0.5F))

#define SET_IF_LARGER(x, max, newValue)     if((x) > (max)) { (x) = (newValue); }

#define LIMITATION_ABOVE(var, value, max)   var = (value); if((var) > (max)) { (var) = (max); }

#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); } else  { var = var; };

#define IN_RANGE(x, min, max)               ((x) >= (min) && (x) <= (max))



namespace Math
{
    const double PI = 3.141592653589793238463;
    const float PI_F = 3.141592653589793F;
    // ¬озвращает номер младшего бита, равного единице
    int LowSignedBit(uint value);
    // \brief —равнивает два числа. ¬озвращает true, если числа отличаютс€ друг от друга не более, чем на epsilonPart. ѕри этом дл€ расчЄта epsilonPart 
    // используетс€ большее в смысле модул€ число.
    bool FloatsIsEquals(float value0, float value1, float epsilonPart);

    bool IsEquals(float x, float y);

    void Smoothing(uint8 *data, int numPoints, int numSmooth);
    
    int MinFrom2Int(int val0, int val1);
 
    // \brief ¬озвращает координату x пересечени€ линии, проход€щей через (x0, y0), (x1, y1), с горизонтальной линией, проход€щей через точку с 
    // ординатой yHorLine.
    float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
    
    uint8 CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);

    void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
    
    // Ќайти первый элемент массива, не соотвествующий заданному
    int FindAnotherElement(const uint8 *data, uint8 value, int numElements);
    
    // ¬озвращает случайное число из диапазона [min; max]
    float RandFloat(float min, float max);

    int DigitsInIntPart(float value);
    
    // \brief ќкругл€ет число с плавающей точкой. numDigits - полное число знаков, по которым производитс€ округление.
    // Ќапример, RoundFloat(12.37137, 4) округлит до 12.40)
    float RoundFloat(float value, int numDigits);
    
    // ¬ычисление 10**pow.
    int Pow10(int pow);
    
    // ќбменивает местами содержимое пам€ти по адресам value0 и value1
    template<class T> void Swap(T *value0, T *value1);
    
    // –азмещает переменные value0 и value1 в пор€дке возрастани€
    template<class T> void Sort(T *value0, T *value1);

    template<class T> int  Sign(T x);

    template<class T> T    Abs(T x);

    template<class T> T    Min(T x1, T x2);

    //template<class T> T    Max(T x1, T x2);
    // ¬озвращает максимальное значение из трЄх
    template<class T> T    Max(T val1, T val2, T val3);

    template<class T> void CircleIncrease(T *value, T min, T max);

    template<class T> void CircleDecrease(T *value, T min, T max);

    template<class T> T Add(T value, T delta, T min, T max);
    
    // »нкрементировать *value, но не больше, чем max
    template<class T> void LimitationIncrease(T *value, T max);
    
    // ƒекрементировать *value, но не меньше, чем min
    template<class T> void LimitationDecrease(T *value, T min);

    template<class T> void Limitation(T *value, T min, T max);

    template<class T> T    LimitationRet(T value, T min, T max);
    
    // ѕрибавить к *value term и ограничить, если результат выходит за границы [min, max]
    template<class T> void AdditionThisLimitation(T *value, int term, T min, T max);
};



// —труктура принимает в себ€ значени€ и сохран€ет два наименьших из них.
// ¬озвращает то, которое больше
struct Min2
{
    Min2() { Reset(); }
    void Add(uint16 value);
    void Reset() { moreMin = 0xFFFF; lessMin = 0xFFFF; }
    // »з двух хранимых минимиальных значений возвращает то, которое больше (не крайнее)
    uint16 Get() const { return moreMin; }

private:
    uint16 moreMin;    // Ѕольшее меньшее
    uint16 lessMin;    // ћеньшее меньшее
};


// —труктура принимает в себ€ значени€ и сохран€ет два наибольших из них.
// ¬озвращает то, которое меньше
struct Max2
{
    Max2() { Reset(); }
    void Reset() { moreMax = 0; lessMax = 0; }
    void Add(uint16 value);
    // »з двух хранимых максимальных значений возвращает то, которое меньше (не крайнее)
    uint16 Get() const { return lessMax; }

private:
    uint16 moreMax;
    uint16 lessMax;
};


// —труктура хранит два наименьших значени€ и два наибольших из последовательности, подаваемой ей в функцию Add

struct MinMax2
{
    MinMax2()              { Reset(); }
    void Add(uint16 value) { min.Add(value); max.Add(value); count++; }
    void Reset()           { min.Reset(); max.Reset(); count = 0; }
    uint16 Min()           { return min.Get(); }
    uint16 Max()           { return max.Get(); }
    int Count()            { return count; }

private:
    Min2 min;
    Max2 max;

    int count;
};

