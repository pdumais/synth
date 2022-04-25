QT += testlib
QT += gui
QT += widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -DAUTOTEST

TEMPLATE = app

SOURCES +=  tst_sequencertest.cpp \
    ../EventList.cpp \
    ../Midi.cpp \
    ../MidiFileUtil.cpp \
    ../MidiRoll.cpp \
    ../Sequencer.cpp \
    ../midifile/Binasc.cpp \
    ../midifile/MidiEvent.cpp \
    ../midifile/MidiEventList.cpp \
    ../midifile/MidiFile.cpp \
    ../midifile/MidiMessage.cpp

HEADERS += \
    ../EventList.h \
    ../Midi.h \
    ../MidiFileUtil.h \
    ../MidiRoll.h \
    ../Sequencer.h \
    ../midifile/Binasc.h \
    ../midifile/MidiEvent.h \
    ../midifile/MidiEventList.h \
    ../midifile/MidiFile.h \
    ../midifile/MidiMessage.h
