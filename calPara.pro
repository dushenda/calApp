QT       += core gui widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qwt

RC_FILE += version.rc
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS  QT_NO_DEBUG_OUTPUT #去除DEBUG打印信息

DEFINES += QT_DLL QWT_DLL


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include ( C:\Qt\Qwt-6.1.4\features\qwt.prf )
# QtCSV Lib
QTCSV_LOCATION = $$OUT_PWD

LIBS += -L$$QTCSV_LOCATION -lqtcsv

#LIBS += -L$$QTCSV_LOCATION -llibxl
#LIBS += $$PWD/lib/libxl.lib $$PWD/lib64/libxl.lib

INCLUDEPATH += $$PWD/../qtcsv/include

#INCLUDEPATH += $$PWD/../libxl-3.8.8.2/include_cpp



TARGET = calApp

SOURCES += \
    anglecal.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    rangedata.cpp \
    specconv.cpp

HEADERS += \
    anglecal.h \
    mainwindow.h \
    qcustomplot.h \
    rangedata.h \
    specconv.h \
    version.rc

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Img.qrc
