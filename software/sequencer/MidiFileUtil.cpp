#include "MidiFileUtil.h"
#include "midifile/MidiFile.h"

MidiFileUtil::MidiFileUtil()
{

}


std::map<long, MidiEvent*> MidiFileUtil::getEventsFromFile(std::string fname)
{
    std::map<long, MidiEvent*> ret;
    smf::MidiFile midifile;
    midifile.read(fname);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    midifile.joinTracks();

   //midifile.getTicksPerQuarterNote()
    for (int event=0; event<midifile[0].size(); event++) {
        smf::MidiEvent* mev = &midifile[0][event];

        MidiEvent *ev = new MidiEvent();
        ev->next = 0;
        long t = mev->tick;
        if (!ret.count(t))
        {
            ret[t] = ev;
        }
        else
        {
            MidiEvent *e = ret[t];
            while (e->next != 0) e = e->next;
            e->next = ev;
        }

        ev->channel = mev->getChannel();
        ev->event = mev->getCommandByte();
         if (mev->isNoteOn())
        {
            ev->d1 = mev->getKeyNumber();
            ev->d2 = mev->getVelocity();
         }
        else if (mev->isNoteOff())
        {
            ev->d1 = mev->getKeyNumber();
        }
    }

   return ret;
}
