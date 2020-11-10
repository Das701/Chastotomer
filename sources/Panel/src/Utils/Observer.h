#pragma once


struct EventType
{
    enum E
    {
        ModeTestChanged
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
    Subject() : numObservers(0) {};
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
