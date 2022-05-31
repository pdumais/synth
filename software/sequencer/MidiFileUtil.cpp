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

void MidiFileUtil::saveToFile(std::string fname, std::vector<std::map<long, std::vector<MidiEvent*>>> tracks, int tpq)
{
    smf::MidiFile midifile;

    midifile.setTPQ(tpq);
    midifile.addTracks(tracks.size()-1);
    for (int i = 0; i < tracks.size(); i++)
    {
        for (auto it : tracks[i])
        {
            for (auto ev : it.second)
            {
                int starttick = ev->start;
                int channel = ev->channel;
                int key = ev->d1;
                int vel = ev->d2;
                int endtick = starttick+ev->duration;
                midifile.addNoteOn (i, starttick, channel, key, vel);
                midifile.addNoteOff(i, endtick, channel, key);
            }
        }
    }
    midifile.sortTracks();
    midifile.removeEmpties();
    midifile.write(fname);
}

std::vector<std::map<long, std::vector<MidiEvent*>>> MidiFileUtil::getEventsFromFile()
{
    std::vector<std::map<long, std::vector<MidiEvent*>>> ret;
    for (int track=0; track<midifile.size(); track++)
    {
        std::map<long, std::vector<MidiEvent*>> trk;
        for (int event=0; event<midifile[track].size(); event++) {
            smf::MidiEvent* mev = &midifile[track][event];

            if (!mev->isNoteOn()) continue;
            if (mev->getVelocity() == 0) continue;

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
        }
        ret.push_back(trk);
     }

   return ret;
}
