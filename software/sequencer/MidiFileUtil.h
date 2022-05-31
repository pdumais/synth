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

    static void saveToFile(std::string fname, std::vector<std::map<long, std::vector<MidiEvent*>>> tracks, int tpq);
    std::vector<std::map<long, std::vector<MidiEvent*>>> getEventsFromFile();
    int getTicksPerQuarteNotes();
};

