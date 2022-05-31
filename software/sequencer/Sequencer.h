#pragma once
#include <map>

#include "Midi.h"
#include "EventList.h"


class Sequencer
{
private:
    std::vector<EventList<MidiEvent*>> tracks;
    EventList<MidiEvent*> *currentTrack;
    Midi    midi;
    int tpqn;
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

    void reset();
    void setPosition(long tick);
    void removeEvent(long tick, int channel, int note, int type=0x90);
    long getCurrentTick();
    void loadEvents(int track, std::map<long, std::vector<MidiEvent*>> events);
    void setTrack(int trk);
    void addEvent(long tick, MidiEvent* event);
    void updateEvent(MidiEvent* original, MidiEvent* changed);
    Midi* getMidi();
    std::map<long, std::vector<MidiEvent*>> dumpEvents(int track = -1);

    void onClockTick();
};
