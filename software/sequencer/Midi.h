#pragma once
#include <map>
#include <vector>
#include <string>
#include "RtMidi.h"

class MidiEvent
{
public:
    long start;
    int duration;
    int channel;
    int event;
    int d1;
    int d2;

    MidiEvent* next;
};


class Midi
{
private:
    RtMidiOut *midiout;
    int currentNote;

public:
    Midi();

    std::vector<std::string> listDevices();
    void useDevice(int num);

    void sendNoteOn(int channel, int note, int level, bool poly=false);
};
