#include "Midi.h"

Midi::Midi()
{
    this->midiout = new RtMidiOut();
    this->currentNote = -1;
}

std::vector<std::string> Midi::listDevices()
{
    std::vector<std::string> ret;

    unsigned int nPorts = this->midiout->getPortCount();
    std::string portName;
    for (int i = 0; i < nPorts; i++ ) 
    {
        ret.push_back(this->midiout->getPortName(i));
    }

    return ret;
}

void Midi::useDevice(int num)
{
    midiout->openPort(num);
}

void Midi::sendNoteOn(int channel, int note, int level, bool poly/*=false*/)
{
    unsigned char msg[3];
    if (!poly && this->currentNote != -1)
    {
        // Send note-off for other note if we are not poly
        msg[0] = 0x80 | (channel&0xF);
        msg[1] = this->currentNote;
        this->midiout->sendMessage(msg,2);
    }

    msg[0] = 0x90 | (channel&0xF);
    msg[1] = note;
    msg[2] = level;
    this->midiout->sendMessage(msg,3);
    std::cout << "Note On: " << note << std::endl;
    this->currentNote = note;
}

void Midi::sendNoteOff(int channel, int note)
{
    unsigned char msg[3];
    msg[0] = 0x90 | (channel&0xF);
    msg[1] = note;
    this->midiout->sendMessage(msg,2);

    std::cout << "Note Off: " << note << std::endl;
//TODO: currentNote per channel!!!
    this->currentNote = -1;
}
