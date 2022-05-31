#pragma once
#include <map>
#include <vector>

template <class T>
class EventList
{
private:
    std::map<long, std::vector<T>> events;

public:
    void insert(long slot, T item)
    {
        if (!this->events.count(slot))
        {
            this->events[slot] = std::vector<T>();
        }
        this->events[slot].push_back(item);
    }

    void remove(long slot, T item)
    {

        if (!this->events.count(slot)) return;

        std::vector<T>& slotlist = this->events[slot];
        for (typename std::vector<T>::iterator it = slotlist.begin(); it != slotlist.end(); it++)
        {
            if (*it == item)
            {
                slotlist.erase(it);
                return;
            }
        }
    }

    void clear()
    {
        for (auto& slot : this->events)
        {
            slot.second.clear();
        }
        this->events.clear();
        int a = 1;
    }


    std::vector<T> get(long slot)
    {

        if (this->events.count(slot)) return this->events[slot];

        return std::vector<T>();
    }

    std::map<long, std::vector<T>> getRaw()
    {
        return events;
    }


};


