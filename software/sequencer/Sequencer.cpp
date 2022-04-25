#include "Sequencer.h"
#include <iostream>

Sequencer::Sequencer()
{
    for (auto it : this->midi.listDevices())
    {
        std::cout << it << "\r\n";
    }

    for (int i=0; i<16; i++) this->currentNote[i] = 0;
    this->running = false;
    this->tpqn = 192;
    this->position = 0;
    this->midi.useDevice(1);
}

Sequencer::~Sequencer()
{
}


void Sequencer::play()
{
    this->running = true;
    this->position = 0;
    this->currentTick = 0;
}

void Sequencer::stop()
{
    this->running = false;
}

long Sequencer::getCurrentTick()
{
    return this->currentTick;
}

Midi* Sequencer::getMidi()
{
    return &this->midi;
}

void Sequencer::onClockTick()
{
    if (!this->running) return;

    for (int i = 0; i< 16; i++)
    {
        auto *cn = this->currentNote[i];
        if (!cn) continue;
        if (this->currentTick >= (cn->start+cn->duration))
        {
            this->midi.sendNoteOff(cn->channel, cn->d1);
            this->currentNote[i] = 0;
        }
    }


    // Is there an even at this tick?
    auto evlist = this->events.get(this->currentTick);
    for (auto ev : evlist) {
        if (ev->event == 0x90)
        {
            auto *cn = this->currentNote[ev->channel];
            if (cn)
            {
                this->midi.sendNoteOff(cn->channel, cn->d1);
            }
            this->midi.sendNoteOn(ev->channel, ev->d1, ev->d2);
            this->currentNote[ev->channel] = ev;
        }
        else if (ev->event == 0x80)
        {
            this->midi.sendNoteOff(ev->channel, ev->d1);
            this->currentNote[ev->channel] = 0;
        }
    }

    this->currentTick++;
}

void Sequencer::setTicksPerQuarterNote(int tpqn)
{
    this->tpqn = tpqn;
}

long Sequencer::getTicksPerQuarterNote()
{
    return this->tpqn;
}

void Sequencer::removeEvent(long tick, int channel, int note, int type/*=0x90*/)
{
    auto evlist = this->events.get(tick);
    for (auto ev : evlist) {
        if (ev->event == type && ev->channel == channel && ev->d1 == note)
        {
            this->events.remove(tick, ev);
        }
    }
}

void Sequencer::addEvent(long tick, MidiEvent* ev)
{
    this->events.insert(tick, ev);
}


void Sequencer::loadEvents(std::map<long, std::vector<MidiEvent*>> events)
{
    this->events.clear();
    for (auto& it : events)
    {
        long tick = it.first;

        for (auto ev : it.second)
        {
            this->addEvent(tick, ev);
        }
    }

}

std::map<long, std::vector<MidiEvent*>> Sequencer::dumpEvents()
{
    return this->events.getRaw();
}

