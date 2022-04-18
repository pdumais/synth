#pragma once
#include <map>

#include "Midi.h"


class Sequencer
{
private:
    std::vector<MidiEvent*> events;
    Midi    midi;

public:
    Sequencer();
    virtual ~Sequencer();

    void play();
    void stop();

    void setEvent(MidiEvent ev, int pos=-1);
    void removeEvent(int pos);

    void loadEvents(std::map<long, MidiEvent*> events);
    std::map<long, MidiEvent*> dumpEvents();

    void onClockTick();
};
