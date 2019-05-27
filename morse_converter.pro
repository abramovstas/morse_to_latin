QT += qml quick widgets
TARGET = morse_conveter

CONFIG += c++11

SOURCES += \
    $$PWD/src/documenthandler.cpp \
    $$PWD/src/main.cpp


HEADERS += \
    $$PWD/src/documenthandler.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols/controls/texteditor
INSTALLS += target
