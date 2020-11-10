#pragma once


struct EventType
{
    enum E
    {
        ModeTestChanged,    // Это событие генерирует ModeTest, когда изменяет своё состояние
        Count
    };
};


class Observer
{
public:
    virtual ~Observer() {}
    virtual void OnEvent(EventType::E) = 0;
};


class Subject
{
public:
    Subject() : numObservers(0)
    {
        for (int i = 0; i < numObservers; i++)
        {
            observers[i] = nullptr;
        }
    };
    void AddObserver(Observer *observer) { observers[numObservers++] = observer; }

protected:
    void Event(EventType::E event)
    {
        for (int i = 0; i < numObservers; i++)
        {
            observers[i]->OnEvent(event);
        }
    }

private:
    Observer *observers[10];
    int numObservers;
};
