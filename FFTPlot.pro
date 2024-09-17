QT       += core gui xml svg printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filewriterworkerfft.cpp \
    main.cpp \
    mainwindow.cpp \
    thirdparty/FFT/FftComplex.cpp \
    thirdparty/FFT/FftComplexTest.cpp \
    thirdparty/FFT/FftRealPair.cpp \
    thirdparty/FFT/FftRealPairTest.cpp \
    thirdparty/qcustomplot/qcustomplot.cpp

HEADERS += \
    filewriterworkerfft.h \
    mainwindow.h \
    thirdparty/FFT/FftComplex.hpp \
    thirdparty/FFT/FftRealPair.hpp \
    thirdparty/qcustomplot/qcustomplot.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    thirdparty/qcustomplot/GPL.txt \
    thirdparty/qcustomplot/changelog.txt
