QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addnewchapter.cpp \
    addnewreview.cpp \
    chapter.cpp \
    deleteuser.cpp \
    globals.cpp \
    main.cpp \
    mainwindow.cpp \
    manga.cpp \
    qpushlabel.cpp \
    review.cpp

HEADERS += \
    addnewchapter.h \
    addnewreview.h \
    chapter.h \
    deleteuser.h \
    globals.h \
    mainwindow.h \
    manga.h \
    qpushlabel.h \
    review.h

FORMS += \
    addnewchapter.ui \
    addnewreview.ui \
    chapter.ui \
    deleteuser.ui \
    mainwindow.ui \
    manga.ui \
    review.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
