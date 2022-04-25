#include "MidiFileUtil.h"

MidiFileUtil::MidiFileUtil(std::string fname)
{
    midifile.read(fname);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
}

int MidiFileUtil::getTicksPerQuarteNotes()
{
    return midifile.getTicksPerQuarterNote();
}


std::vector<std::map<long, std::vector<MidiEvent*>>> MidiFileUtil::getEventsFromFile()
{
    std::vector<std::map<long, std::vector<MidiEvent*>>> ret;
    for (int track=0; track<midifile.size(); track++)
    {
        std::map<long, std::vector<MidiEvent*>> trk;
        for (int event=0; event<midifile[track].size(); event++) {
            smf::MidiEvent* mev = &midifile[track][event];

            MidiEvent *ev = new MidiEvent();
            long t = mev->tick;
            if (!trk.count(t))
            {
                trk[t] = std::vector<MidiEvent*>();
            }
            trk[t].push_back(ev);

            ev->start = mev->tick;
            ev->duration = mev->getTickDuration();
            ev->channel = mev->getChannel();
            ev->event = mev->getCommandByte() & 0xF0;

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
        ret.push_back(trk);
     }

   return ret;
}
