#pragma once
#include "Utils/String.h"


#define LOG_WRITE(...)          Log::Message(String(__VA_ARGS__).c_str())
#define LOG_WRITE_AND_SHOW(...) Log::Message(String(__VA_ARGS__).c_str()); Painter::EndScene()
#define LOG_ERROR(...)          Log::Message(__FILE__, __LINE__, String(__VA_ARGS__).c_str())


#define LOG_FUNC_ENTER()        Log::Message(String("%s %d enter", __FUNCTION__, __LINE__).c_str())
#define LOG_FUNC_LEAVE()        Log::Message(String("%s %d leave", __FUNCTION__, __LINE__).c_str())


#define LOG_TRACE               Log::Message(String("%s : %d", __FILE__, __LINE__).c_str())


struct Log
{
    static void Message(const char *message);
    static void Message(const char *file, int line, const char *message);
};
