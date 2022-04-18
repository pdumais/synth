#include "Sequencer.h"

Sequencer::Sequencer()
{
    for (auto &it : this->midi.listDevices())
    {
        std::cout << it << "\r\n";
    }

    this->midi.useDevice(1);
    this->midi.sendNoteOn(0,64,32);
}

Sequencer::~Sequencer()
{
}


void Sequencer::play()
{
}

void Sequencer::stop()
{
}

void Sequencer::onClockTick()
{
}


void Sequencer::setEvent(MidiEvent ev, int pos/*=-1*/)
{
}

void Sequencer::removeEvent(int pos)
{
}


void Sequencer::loadEvents(std::map<long, MidiEvent*> events)
{
    for (auto &it : events)
    {
        std::cout << it.first << ": " << it.second->event << std::endl;
    }
}

std::map<long, MidiEvent*> Sequencer::dumpEvents()
{
    std::map<long, MidiEvent*> ret;
    return ret;
}

