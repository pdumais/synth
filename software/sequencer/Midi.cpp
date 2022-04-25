#include "Midi.h"
#include <iostream>

Midi::Midi()
{
#ifndef AUTOTEST
    this->midiout = new RtMidiOut();
#endif
}

std::vector<std::string> Midi::listDevices()
{
    std::vector<std::string> ret;

#ifndef AUTOTEST
    unsigned int nPorts = this->midiout->getPortCount();
    std::string portName;
    for (int i = 0; i < nPorts; i++ ) 
    {
        ret.push_back(this->midiout->getPortName(i));
    }

#endif
    return ret;
}

void Midi::useDevice(int num)
{
#ifndef AUTOTEST
    midiout->openPort(num);
#endif
}

void Midi::sendNoteOn(int channel, int note, int level, bool poly/*=false*/)
{
    unsigned char msg[3];

    msg[0] = 0x90 | (channel&0xF);
    msg[1] = note;
    msg[2] = level;
#ifndef AUTOTEST
    this->midiout->sendMessage(msg,3);
#else
    std::string str((char*)&msg[0]);
    debugEvents.push_back(str);
#endif
    std::cout << "Note On: " << note << std::endl;
}

void Midi::sendNoteOff(int channel, int note)
{
    unsigned char msg[3];
    msg[0] = 0x80 | (channel&0xF);
    msg[1] = note;
#ifndef AUTOTEST
    this->midiout->sendMessage(msg,2);
#else
    std::string str((char*)&msg[0]);
    debugEvents.push_back(str);
#endif

    std::cout << "Note Off: " << note << std::endl;
//TODO: currentNote per channel!!!
}
