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
        request_handler.cpp \
        svg.cpp \
        tests.cpp \
        transport_catalogue.cpp

HEADERS += \
  domain.h \
  geo.h \
  json.h \
  json_builder.h \
  json_reader.h \
  map_renderer.h \
  request_handler.h \
  svg.h \
  tests.h \
  transport_catalogue.h
