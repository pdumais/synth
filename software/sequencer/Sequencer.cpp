#include "Sequencer.h"

Sequencer::Sequencer()
{
    for (auto &it : this->midi.listDevices())
    {
        std::cout << it << "\r\n";
    }

    this->running = false;
    this->tpqn = 192;
    this->position = 0;
    this->midi.useDevice(0);
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

void Sequencer::onClockTick()
{
    if (!this->running) return;

    // Is there an even at this tick?
    if (this->events.count(this->currentTick))
    {
        auto ev = this->events[this->currentTick];
        while (ev) {
            if (ev->event == 0x90)
            {
                this->midi.sendNoteOn(ev->channel, ev->d1, ev->d2);
                long end = ev->duration + this->currentTick;

                MidiEvent* mev = new MidiEvent();
                mev->d1 = ev->d1;
                mev->event = 0x80;
                mev->channel = ev->channel;
                mev->start = end;
                mev->next = 0;
                if (!this->events.count(end))
                {
                    this->events[end] = mev;
                }
                else
                {
                    auto mev2 = this->events[end];
                    while (mev2) {
                        if (!mev2->next)
                        {
                            mev2->next = mev;
                            break;
                        }
                        mev2 = mev2->next;
                    }
                }
            }
            else if (ev->event == 0x80)
            {
                this->midi.sendNoteOff(ev->channel, ev->d1);
            }
            ev = ev->next;
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

void Sequencer::setEvent(MidiEvent ev, int pos/*=-1*/)
{
}

void Sequencer::removeEvent(int pos)
{
}


void Sequencer::loadEvents(std::map<long, MidiEvent*> events)
{
    this->events = events;
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

