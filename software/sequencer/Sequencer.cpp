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
    this->eventsIterator = this->events.begin();
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

    this->currentTick++;
    //this->position += this->tpqn;

//TODO: detect if this timesout the duration of an event and we must send noteOff

    while (1)
    {
        auto it = this->eventsIterator;
        it++;
        if (it == this->events.end())
        {
         //  TODO: this->stop();
            break;
        }

        /*auto start = it->first;
        if (start <= this->position)
        {
            auto nextEvent = it->second;
            while (nextEvent) {
                if (nextEvent->event == 0x90)
                {
                    this->midi.sendNoteOn(nextEvent->channel, nextEvent->d1, nextEvent->d2);
                }
                else if (nextEvent->event == 0x80)
                {
                    this->midi.sendNoteOff(nextEvent->channel, nextEvent->d1);
                }
                nextEvent = nextEvent->next;
            }
            this->eventsIterator++;*/
        } else {
            break;
        }
    }
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

