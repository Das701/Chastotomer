#pragma once


#define LIMIT_BELOW(x, min)                 if((x) < (min)) { x = (min); }

#define LIMIT_ABOVE(x, max)                 if((x) > (max)) { x = (max); }

#define SET_MIN_IF_LESS(x, min)             if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)           if((x) > (max)) { (max) = (x); }

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };
// \todo ��� ��������� ����������� �������� � �������������� �������
#define ROUND(type, x) ((type)((x) + 0.5F))

#define SET_IF_LARGER(x, max, newValue)     if((x) > (max)) { (x) = (newValue); }

#define LIMITATION_ABOVE(var, value, max)   var = (value); if((var) > (max)) { (var) = (max); }

#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); } else  { var = var; };

#define IN_RANGE(x, min, max)               ((x) >= (min) && (x) <= (max))



namespace Math
{
    const double PI = 3.141592653589793238463;
    const float PI_F = 3.141592653589793F;
    // ���������� ����� �������� ����, ������� �������
    int LowSignedBit(uint value);
    // \brief ���������� ��� �����. ���������� true, ���� ����� ���������� ���� �� ����� �� �����, ��� �� epsilonPart. ��� ���� ��� ������� epsilonPart 
    // ������������ ������� � ������ ������ �����.
    bool FloatsIsEquals(float value0, float value1, float epsilonPart);

    bool IsEquals(float x, float y);

    void Smoothing(uint8 *data, int numPoints, int numSmooth);
    
    int MinFrom2Int(int val0, int val1);
 
    // \brief ���������� ���������� x ����������� �����, ���������� ����� (x0, y0), (x1, y1), � �������������� ������, ���������� ����� ����� � 
    // ��������� yHorLine.
    float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
    
    uint8 CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);

    void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
    
    // ����� ������ ������� �������, �� �������������� ���������
    int FindAnotherElement(const uint8 *data, uint8 value, int numElements);
    
    // ���������� ��������� ����� �� ��������� [min; max]
    float RandFloat(float min, float max);

    int DigitsInIntPart(float value);
    
    // \brief ��������� ����� � ��������� ������. numDigits - ������ ����� ������, �� ������� ������������ ����������.
    // ��������, RoundFloat(12.37137, 4) �������� �� 12.40)
    float RoundFloat(float value, int numDigits);
    
    // ���������� 10**pow.
    int Pow10(int pow);
    
    // ���������� ������� ���������� ������ �� ������� value0 � value1
    template<class T> void Swap(T *value0, T *value1);
    
    // ��������� ���������� value0 � value1 � ������� �����������
    template<class T> void Sort(T *value0, T *value1);

    template<class T> int  Sign(T x);

    template<class T> T    Abs(T x);

    template<class T> T    Min(T x1, T x2);

    //template<class T> T    Max(T x1, T x2);
    // ���������� ������������ �������� �� ���
    template<class T> T    Max(T val1, T val2, T val3);

    template<class T> void CircleIncrease(T *value, T min, T max);

    template<class T> void CircleDecrease(T *value, T min, T max);

    template<class T> T Add(T value, T delta, T min, T max);
    
    // ���������������� *value, �� �� ������, ��� max
    template<class T> void LimitationIncrease(T *value, T max);
    
    // ���������������� *value, �� �� ������, ��� min
    template<class T> void LimitationDecrease(T *value, T min);

    template<class T> void Limitation(T *value, T min, T max);

    template<class T> T    LimitationRet(T value, T min, T max);
    
    // ��������� � *value term � ����������, ���� ��������� ������� �� ������� [min, max]
    template<class T> void AdditionThisLimitation(T *value, int term, T min, T max);
};



// ��������� ��������� � ���� �������� � ��������� ��� ���������� �� ���.
// ���������� ��, ������� ������
struct Min2
{
    Min2() { Reset(); }
    void Add(uint16 value);
    void Reset() { moreMin = 0xFFFF; lessMin = 0xFFFF; }
    // �� ���� �������� ������������ �������� ���������� ��, ������� ������ (�� �������)
    uint16 Get() const { return moreMin; }

private:
    uint16 moreMin;    // ������� �������
    uint16 lessMin;    // ������� �������
};


// ��������� ��������� � ���� �������� � ��������� ��� ���������� �� ���.
// ���������� ��, ������� ������
struct Max2
{
    Max2() { Reset(); }
    void Reset() { moreMax = 0; lessMax = 0; }
    void Add(uint16 value);
    // �� ���� �������� ������������ �������� ���������� ��, ������� ������ (�� �������)
    uint16 Get() const { return lessMax; }

private:
    uint16 moreMax;
    uint16 lessMax;
};


// ��������� ������ ��� ���������� �������� � ��� ���������� �� ������������������, ���������� �� � ������� Add

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

