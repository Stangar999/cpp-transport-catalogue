TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        domain.cpp \
        geo.cpp \
        input_reader.cpp \
        json.cpp \
        json_reader.cpp \
        main.cpp \
        request_handler.cpp \
        stat_reader.cpp \
        tests.cpp \
        transport_catalogue.cpp

HEADERS += \
  domain.h \
  geo.h \
  input_reader.h \
  json.h \
  json_reader.h \
  request_handler.h \
  stat_reader.h \
  tests.h \
  transport_catalogue.h
