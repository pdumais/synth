#include "Sequencer.h"
#include <iostream>

Sequencer::Sequencer()
{
    for (auto it : this->midi.listDevices())
    {
        std::cout << it << "\r\n";
    }

    for (int i=0; i<16; i++) this->currentNote[i] = 0;
    for (int i=0; i<16; i++) this->tracks.push_back(EventList<MidiEvent*>());
    this->running = false;
    this->tpqn = 192;
    this->midi.useDevice(0);
}

Sequencer::~Sequencer()
{
}


void Sequencer::play()
{
    this->running = true;
    this->currentTick = 0;
}

void Sequencer::stop()
{
    this->running = false;
}

void Sequencer::setPosition(long tick)
{
    this->currentTick = tick;
}

long Sequencer::getCurrentTick()
{
    return this->currentTick;
}

Midi* Sequencer::getMidi()
{
    return &this->midi;
}

void Sequencer::setTrack(int trk)
{
    if (trk >= this->tracks.size()) return;
    this->stop();
    this->currentTrack = &this->tracks[trk];
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

    // Is there an event at this tick?
    auto evlist = this->currentTrack->get(this->currentTick);
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
    auto evlist = this->currentTrack->get(tick);
    for (auto ev : evlist) {
        if (ev->event == type && ev->channel == channel && ev->d1 == note)
        {
            this->currentTrack->remove(tick, ev);
        }
    }
}

void Sequencer::addEvent(long tick, MidiEvent* ev)
{
    if ((ev->event & 0xF0) != 0x90) return;
    this->currentTrack->insert(tick, ev);
}


void Sequencer::updateEvent(MidiEvent* original, MidiEvent* changed)
{
    for (auto ev : this->currentTrack->get(original->start))
    {
        if (ev->start == original->start && ev->channel == original->channel && ev->d1 == original->d1)
        {
            this->currentTrack->remove(original->start,ev);
            this->currentTrack->insert(changed->start, ev);
            ev->d1 = changed->d1;
            ev->channel = changed->channel;
            ev->start = changed->start;
            ev->duration = changed->duration;
            return;
        }
    }
}

void Sequencer::reset()
{
    for (auto trk : this->tracks)
    {
        for (auto slot : trk.getRaw())
        {
            for (auto ev : slot.second)
            {
                delete ev;
            }
        }
        trk.clear();
    }
}

void Sequencer::loadEvents(int trk, std::map<long, std::vector<MidiEvent*>> events)
{
    if (trk >= this->tracks.size()) return;

    this->setTrack(trk);
    this->currentTrack->clear();
    for (auto& it : events)
    {
        long tick = it.first;

        for (auto ev : it.second)
        {
            this->addEvent(tick, ev);
        }
    }

}

std::map<long, std::vector<MidiEvent*>> Sequencer::dumpEvents(int track /*=-1*/)
{
    if (track == -1) return this->currentTrack->getRaw();
    return this->tracks[track].getRaw();
}

