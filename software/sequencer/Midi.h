#pragma once
#include <map>
#include <vector>
#include <string>
#ifndef AUTOTEST
#include "RtMidi.h"
#endif

class MidiEvent
{
public:
    MidiEvent()
    {
    }

    MidiEvent(long s, int d, int c, int e, int d1, int d2)
    {
        this->start = s;
        this->duration = d;
        this->channel = c;
        this->event = e;
        this->d1 = d1;
        this->d2 = d2;
    }


    long start;
    int duration;
    int channel;
    int event;
    int d1;
    int d2;
};


class Midi
{
private:
#ifndef AUTOTEST
    RtMidiOut *midiout;
#endif

public:
    Midi();

    std::vector<std::string> listDevices();
    void useDevice(int num);

    void sendNoteOn(int channel, int note, int level, bool poly=false);
    void sendNoteOff(int channel, int note);

#ifdef AUTOTEST
    std::vector<std::string> debugEvents;
#endif
};
