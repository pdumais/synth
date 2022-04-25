QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

!win32 {
QMAKE_CXXFLAGS += -D__LINUX_ALSA__ -D__RTMIDI_DEBUG__
LIBS += -lpthread -lasound
}

win32 {
QMAKE_CXXFLAGS += -D__WINDOWS_MM__ -D__RTMIDI_DEBUG__
LIBS += -lwinmm
#LIBS += -lpthread -lasound
}
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    EventList.cpp \
    Midi.cpp \
    MidiFileUtil.cpp \
    MidiRoll.cpp \
    RtMidi.cpp \
    Sequencer.cpp \
    TrackWidget.cpp \
    main.cpp \
    mainwindow.cpp \
    midifile/Binasc.cpp \
    midifile/MidiEvent.cpp \
    midifile/MidiEventList.cpp \
    midifile/MidiFile.cpp \
    midifile/MidiMessage.cpp \
    rtmidi_c.cpp

HEADERS += \
    EventList.h \
    Midi.h \
    MidiFileUtil.h \
    MidiRoll.h \
    RtMidi.h \
    Sequencer.h \
    TrackWidget.h \
    mainwindow.h \
    midifile/Binasc.h \
    midifile/MidiEvent.h \
    midifile/MidiEventList.h \
    midifile/MidiFile.h \
    midifile/MidiMessage.h \
    rtmidi_c.h

FORMS += \
    TrackWidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
