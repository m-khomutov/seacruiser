TEMPLATE = lib
CONFIG += staticlib

OBJECTS_DIR = ../build/kformat
DESTDIR += $$OBJECTS_DIR/lib
CONFIG += 14

HEADERS = kformat.h \
    utils.h \
    screen/basescreen.h \
    service/s_socket.h \
    service/s_poll.h \
    service/connection.h \
    service/baseservice.h \
    encoding/baseframe.h \
    encoding/jpegframe.h \
    protocol/baseprotocol.h \
    protocol/flvprotocol.h \
    protocol/httpapi.h


SOURCES = \
        utils.cpp \
        screen/basescreen.cpp \
        service/s_socket.cpp \
        service/connection.cpp \
        service/s_poll.cpp \
        service/baseservice.cpp \
        encoding/baseframe.cpp \
        encoding/jpegframe.cpp \
        protocol/baseprotocol.cpp \
        protocol/flvprotocol.cpp \
        protocol/httpapi.cpp


INCLUDEPATH += $$PWD
LIBS = -ljpeg
