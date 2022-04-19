#pragma once
#include <map>

#include "Midi.h"


class Sequencer
{
private:
    std::map<long, MidiEvent*> events;
    std::map<long, MidiEvent*>::iterator eventsIterator;
    Midi    midi;
    int tpqn;
    int position;
    bool running;
    long currentTick;

public:
    Sequencer();
    virtual ~Sequencer();

    void setTicksPerQuarterNote(int tpqn);
    long getTicksPerQuarterNote();
    void play();
    void stop();

    void setEvent(MidiEvent ev, int pos=-1);
    void removeEvent(int pos);
    long getCurrentTick();
    void loadEvents(std::map<long, MidiEvent*> events);
    std::map<long, MidiEvent*> dumpEvents();

    void onClockTick();
};
