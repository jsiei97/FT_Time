CONFIG += qtestlib debug 
QT     -= gui 
TEMPLATE = app
TARGET =
DEFINES += private=public

# Build with gcov coverage (gcov and gcovr)
QMAKE_CXXFLAGS += -g -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS   += -g -fprofile-arcs -ftest-coverage -O0
QMAKE_CLEAN += *.gcda *.gcno *.gcov

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += Test_DateTime.cpp

# Code to test
DEPENDPATH += ../../FT_Time/
INCLUDEPATH += ../../FT_Time/
SOURCES += DateTime.cpp


