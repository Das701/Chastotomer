#pragma once
#include "SCPI/StringUtils.h"


/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/


class String;
class Switch;

typedef pchar (*FuncSCPI)(pchar);


// ���������, �������������� ���� ������.
struct StructSCPI
{
    pchar key;            // �������� ����� ���� (�������)

    const StructSCPI *strct;    // ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             // ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   // ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
    bool IsLeaf() const { return func != nullptr; };    // ��������� �������� "������" ������, ����� ��������� ������� func
};


#define SCPI_NODE(key, strct)   {key, strct,   nullptr}
#define SCPI_LEAF(key, func) {key, nullptr, func}
#define SCPI_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::SU::IsLineEnding(&t)) { SCPI::Answer::SendBadSymbols();
#define SCPI_EPILOG_REQUEST(t) return t; } return nullptr;
#define SCPI_EPILOG(t)         return t; }

#define SCPI_RUN_IF_END_REQUEST(func) if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG_REQUEST(end) }
#define SCPI_RUN_IF_END(func)         if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG(end) }

#define SCPI_REQUEST(func)                              \
    pchar end = SCPI::SU::BeginWith(buffer, "?");       \
    SCPI_RUN_IF_END_REQUEST(func)

#define SCPI_PROCESS_ARRAY(names, func)                 \
    for(int i = 0; i < names[i][0] != 0; i++)           \
    {                                                   \
        end = SCPI::SU::BeginWith(buffer, names[i]);    \
        SCPI_RUN_IF_END(func)                           \
    }                                                   \
    return nullptr;

#define SCPI_EXIT_ERROR()   LOG_WRITE("������ ����� SCPI %s:%d", __FILE__, __LINE__); return false;

#define SCPI_APPEND_STRING(string) SCPI::AppendNewData(string.c_str(), static_cast<int>(std::strlen(string.c_str()))); SCPI::Update()

#define SCPI_IF_BEGIN_WITH_THEN(begin, func) end = SCPI::BeginWith(buffer, begin); if (end) { SCPI_PROLOG(end); func; SCPI_EPILOG(end); }


namespace SCPI
{
    // ������-��������� ������ �������
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(pchar buffer, int length);

    void Update();
   
    // ������� ����� � � ����� �������� 0x0D, ���� ���
    void SendAnswer(pchar message);

    // ������� ������ ��� ���� - ��� ������������ �������
    void SendData(pchar message);
    void SendData(const String &message);

    // ������� ��������� � SCPI - � ������� ���������� �������� � ������ ���������
    void SendMeasure(const String &message);
       
    void ProcessHint(String *message, pchar const *names); //-V2504

    namespace Answer
    {
        // ������� ��������� �� ��������� ��������, ���� ������� �������
        void SendBadSymbols();

        void CurrentChannelHasNotParameter();

        void InvalidParameter();

        void ThisModeCannotBeSetForTheCurrentChannel();
    }

    // ����� ������� ��������� ������� �������� �������
    pchar ProcessSimpleParameter(pchar buffer, const pchar choice[], Switch * const sw);
};


namespace SCPI
{
    extern const StructSCPI data[];
    extern const StructSCPI head[];
    extern const StructSCPI input[];
    extern const StructSCPI measure[];
    extern const StructSCPI set[];
}
