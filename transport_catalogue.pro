TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        domain.cpp \
        geo.cpp \
        json.cpp \
        json_builder.cpp \
        json_reader.cpp \
        main.cpp \
        map_renderer.cpp \
        map_renderer.pb.cc \
        request_handler.cpp \
        serialization.cpp \
        svg.cpp \
        svg.pb.cc \
        tests.cpp \
        transport_catalogue.cpp \
        transport_catalogue.pb.cc \
        transport_router.cpp

HEADERS += \
  domain.h \
  geo.h \
  graph.h \
  json.h \
  json_builder.h \
  json_reader.h \
  map_renderer.h \
  map_renderer.pb.h \
  ranges.h \
  request_handler.h \
  router.h \
  serialization.h \
  svg.h \
  svg.pb.h \
  tests.h \
  transport_catalogue.h \
  transport_catalogue.pb.h \
  transport_router.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/package/lib/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/package/lib/ -lprotobufd

INCLUDEPATH += $$PWD/package/include
DEPENDPATH += $$PWD/package/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/package/lib/libprotobuf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/package/lib/libprotobufd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/package/lib/protobuf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/package/lib/protobufd.lib
