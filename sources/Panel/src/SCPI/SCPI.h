#pragma once


/*
    Формат команды.
    1. Команда всегда начинается с символа ':'.
    2. Узлы разделяются символами ':'.
    3. Пробелы допускаются только перед параметром в количестве 1 шт.
    4. Команда должна заканчиваться символом с кодом 0x0D.
*/


class String;

typedef const char *(*FuncSCPI)(pCHAR);


// Структура, соотвествующая узлу дерева.
struct StructSCPI
{
    const char *key;            // Ключевое слово узла (морфема)

    const StructSCPI *strct;    // Если структура имеет тип Node, то здесь хранится массив потомков - StructSCPI *structs.

    FuncSCPI  func;             // Если структура имеет тип Leaf, то здесь хранится функция - обработчик листа типа FuncSCPI

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   // Структура является "узлом" дерева, нужно идти дальше по дереву через structs
    bool IsLeaf() const { return func != nullptr; };    // Стурктура является "листом" дерева, нужно выполнять функцию func
};


#define SCPI_NODE(key, strct)   {key, strct,   nullptr}
#define SCPI_LEAF(key, func) {key, nullptr, func}
#define SCPI_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG(t)  return t; } return nullptr;

#define SCPI_RUN_IF_END(func) if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG(end) }

#define SCPI_REQUEST(func)                          \
    const char *end = SCPI::BeginWith(buffer, "?"); \
    SCPI_RUN_IF_END(func)

#define SCPI_PROCESS_ARRAY(names, func)             \
    for(int i = 0; i < names[i][0] != 0; i++)       \
    {                                               \
        end = SCPI::BeginWith(buffer, names[i]);    \
        SCPI_RUN_IF_END(func)                       \
    }                                               \
    return nullptr;

#define SCPI_EXIT_ERROR()   LOG_WRITE("Ошибка теста SCPI %s:%d", __FILE__, __LINE__); return false;

#define SCPI_APPEND_STRING(string) SCPI::AppendNewData(string.c_str(), static_cast<int>(std::strlen(string.c_str()))); SCPI::Update()

#define SCPI_IF_BEGIN_WITH_THEN(begin, func) end = SCPI::BeginWith(buffer, begin); if (end) { SCPI_PROLOG(end); func; SCPI_EPILOG(end); }


namespace SCPI
{
    // Символ-разделить морфем команды
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(const char *buffer, int length);

    void Update();
    
    // Возвращает true, если указатель указывает на завершающую последовательность
    bool IsLineEnding(const char **bufer);
    
    // Послать ответ м в конце дописать 0x0D, если нет
    void SendAnswer(pCHAR message);
    void SendAnswer(const String &message);

    // Послать строку как есть - без завершающего символа
    void SendData(pCHAR message);
    void SendData(const String &message);

    // Послать измерение в SCPI - с заменой нечитаемых символов и единиц измерения
    void SendMeasure(const String &message);
    
    // Если строка buffer начинается с последовательности символов word, то возвращает указатель на символ, следующий за последним символом последовательности word.
    // Иначе возвращает nullptr.
    const char *BeginWith(const char *buffer, const char *word);
    
    // Послать сообщение об ошибочных символах, если таковые имеются
    void SendBadSymbols();

    bool Test();

    void ProcessHint(String *message, const char *const *names); //-V2504
};


namespace SCPI
{
    extern const StructSCPI head[];         //-V2504
}
