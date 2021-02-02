#pragma once


// ������ ������������ ��� �����������, ���� ����� � HardFault_Handler() ����������� ��� ����������
//#define START_PROFILING_US Debug::StartProfilingUS()
//#define POINT_PROFILING_US Debug::PointProfilingUS(__FILE__, __LINE__)
//
//#define START_PROFILING_MS Debug::StartProfilingMS()
//#define POINT_PROFILING_MS Debug::PointProfilingMS(__FILE__, __LINE__)
//
#define DEBUG_POINT(x) Debug::line[x] = __LINE__; Debug::file[x] = __FILE__;

#define DEBUG_POINT_0  DEBUG_POINT(0)

namespace Debug
{
    void StartProfilingUS();
    uint PointProfilingUS(const char *file, int line);

    void StartProfilingMS();
    uint PointProfilingMS(const char *file, int line);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    extern int line[10];
    extern const char *file[10];
};
