#pragma once

#include "Midi.h"
#include "midifile/MidiFile.h"


class MidiFileUtil
{
private:
    std::string fname;
    int tpqn;
    smf::MidiFile midifile;

public:
    MidiFileUtil(std::string fname);

    std::vector<std::map<long, MidiEvent*>> getEventsFromFile();
    int getTicksPerQuarteNotes();
};

