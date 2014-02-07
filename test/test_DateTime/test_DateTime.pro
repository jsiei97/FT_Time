CONFIG += qtestlib debug console
TEMPLATE = app
TARGET =
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += Test_DateTime.cpp

# Code to test
DEPENDPATH += ../../FT_Time/
INCLUDEPATH += ../../FT_Time/
SOURCES += DateTime.cpp


