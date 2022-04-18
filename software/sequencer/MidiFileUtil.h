#pragma once

#include "Midi.h"

class MidiFileUtil
{
public:
    MidiFileUtil();

    std::map<long, MidiEvent*> getEventsFromFile(std::string fname);
};

