#pragma once
#include <map>

#include "Midi.h"
#include "EventList.h"


class Sequencer
{
private:
    EventList<MidiEvent*> events;
    Midi    midi;
    int tpqn;
    int position;
    bool running;
    long currentTick;
    MidiEvent* currentNote[16];

public:
    Sequencer();
    virtual ~Sequencer();

    void setTicksPerQuarterNote(int tpqn);
    long getTicksPerQuarterNote();
    void play();
    void stop();

    void removeEvent(long tick, int channel, int note, int type=0x90);
    long getCurrentTick();
    void loadEvents(std::map<long, std::vector<MidiEvent*>> events);
    void addEvent(long tick, MidiEvent* event);
    Midi* getMidi();
    std::map<long, std::vector<MidiEvent*>> dumpEvents();

    void onClockTick();
};
