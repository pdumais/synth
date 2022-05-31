#include <QtTest>
#include "../Sequencer.h"
#include "../MidiFileUtil.h"

// add necessary includes here

class SequencerTest : public QObject
{
    Q_OBJECT

public:
    SequencerTest();
    ~SequencerTest();

private slots:
    void test_load_file();
    void test_case1();

};

SequencerTest::SequencerTest()
{

}

SequencerTest::~SequencerTest()
{

}

void SequencerTest::test_load_file()
{
    Sequencer* seq = new Sequencer();

    //Playing while empty does not crash
    seq->play();

    seq->stop();

    MidiFileUtil* mf = new MidiFileUtil("test.mid");

    auto events = mf->getEventsFromFile();

    int t = 0;
    for (auto trk : events)
    {
        seq->loadEvents(t, trk);
        t++;
    }
    seq->setTrack(0);
    seq->addEvent(1, new MidiEvent(1, 1000, 0, 0x90, 100, 127));
    
    seq->setTicksPerQuarterNote(127);

}

void SequencerTest::test_case1()
{
    Sequencer* seq = new Sequencer();
    seq->setTrack(0);

    //Playing while empty does not crash
    seq->play();

    seq->stop();

    seq->addEvent(0, new MidiEvent(0, 1000, 0, 0x90, 100, 127));
    seq->addEvent(500, new MidiEvent(500, 1000, 0, 0x90, 101, 127));
    seq->addEvent(1000, new MidiEvent(1000, 1000, 0, 0x90, 102, 127));
    seq->addEvent(200, new MidiEvent(200, 1000, 0, 0x90, 103, 127));
    seq->addEvent(100, new MidiEvent(100, 10, 0, 0x90, 104, 127));

    auto list = seq->dumpEvents();

    QCOMPARE(list.count(0), 1);
    QCOMPARE(list.count(1), 0);
    QCOMPARE(list.count(500), 1);
    QCOMPARE(list.count(1000), 1);
    QCOMPARE(list.count(200), 1);
    QCOMPARE(list.count(1500), 0);
    QCOMPARE(list.count(2000), 0);
    QCOMPARE(list.count(1200), 0);
    QCOMPARE(list.count(110), 0);
    QCOMPARE(list.size(), 5);
    QCOMPARE(list[1000].size(), 1);

    seq->setTicksPerQuarterNote(127);


    seq->play();
    QCOMPARE(seq->getCurrentTick(), 0);
    for (int i=0; i<1000; i++) seq->onClockTick();
    QCOMPARE(seq->getCurrentTick(), 1000);

    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[0][0], 0x90);
    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[0][1], 100);
    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[0][2], 127);
    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[1][0], 0x80);
    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[1][1], 100);
    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[2][0], 0x90);
    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[2][1], 104);
    QCOMPARE((unsigned char)seq->getMidi()->debugEvents[2][2], 127);

    seq->removeEvent(0, 0, 100);
    list = seq->dumpEvents();
    QCOMPARE(list[0].size(), 0);
    QCOMPARE(list[1000].size(), 1);




}

QTEST_APPLESS_MAIN(SequencerTest)

#include "tst_sequencertest.moc"
