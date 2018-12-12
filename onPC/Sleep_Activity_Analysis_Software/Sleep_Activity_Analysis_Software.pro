#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T09:29:28
#
#-------------------------------------------------

QT       += core gui printsupport
QT += sql
QT+=serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH +=   $$PWD/poppler
win32: LIBS += -L$$PWD/poppler -llibpoppler
win32: LIBS += -L$$PWD/poppler -llibpoppler-qt5

TARGET = Sleep_Activity_Analysis_Software
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DataAnalysis/dataanalysis.cpp \
    DataManagement/datamanagement.cpp \
    ExportReport/exportreport.cpp \
    graphplot/enlargeplot.cpp \
    graphplot/markframe.cpp \
    graphplot/Myplot.cpp \
    graphplot/mytimescale.cpp \
    graphplot/qcustomplot.cpp \
    DataContainer/datacontainer.cpp \
    opration/compressanddealdata.cpp \
    opration/dataanalyze.cpp \
    opration/ksprogressbar.cpp \
    opration/obtaindevicemeasuredata.cpp \
    opration/openfilegetdata.cpp \
    opration/openfilegetdata_delegate.cpp \
    opration/openfilegetdata_proxy.cpp \
    opration/qwaitdialog.cpp \
    opration/readfiledata.cpp \
    opration/restoredevicedefaultdata.cpp \
    SystemConfig/systemconfig.cpp \
    graphplot/onetable.cpp \
    graphplot/statistics.cpp \
    Login/login.cpp \
    Login/loginsecondwidget.cpp \
    patient/patientinfo.cpp \
    graphplot/titlebar.cpp \
    patient/switchpatient.cpp \
    patient/currentpatient.cpp \
    DataBase/databaseconnect.cpp \
    DataBase/infodaoimp.cpp \
    DataBase/qmysqlquerymodel.cpp \
    print/myprintpreviewdialog.cpp \
    ExportReport/doctoradvice.cpp \
    SystemConfig/managetipsinfo.cpp \
    ExportReport/pdfandprint.cpp \
    DataManagement/buttondelegate.cpp \
    DataManagement/tablemodel.cpp \
    DataManagement/exportlist.cpp \
    opration/movefile.cpp \
    DataManagement/openpdf.cpp \
    HomePage/homepage.cpp \
    DataContainer/flagcontainer.cpp \
    SystemConfig/usartcommunication.cpp

HEADERS  += mainwindow.h \
    DataAnalysis/dataanalysis.h \
    DataManagement/datamanagement.h \
    ExportReport/exportreport.h \
    graphplot/enlargeplot.h \
    graphplot/markframe.h \
    graphplot/Myplot.h \
    graphplot/mytimescale.h \
    graphplot/qcustomplot.h \
    DataContainer/datacontainer.h \
    opration/compressanddealdata.h \
    opration/dataanalyze.h \
    opration/ksprogressbar.h \
    opration/obtaindevicemeasuredata.h \
    opration/openfilegetdata.h \
    opration/openfilegetdata_delegate.h \
    opration/openfilegetdata_proxy.h \
    opration/qwaitdialog.h \
    opration/readfiledata.h \
    opration/restoredevicedefaultdata.h \
    SystemConfig/systemconfig.h \
    graphplot/onetable.h \
    graphplot/statistics.h \
    Login/login.h \
    Login/loginsecondwidget.h \
    patient/patientinfo.h \
    graphplot/titlebar.h \
    patient/switchpatient.h \
    patient/currentpatient.h \
    DataBase/databaseconnect.h \
    DataBase/infodaoimp.h \
    DataBase/qmysqlquerymodel.h \
    print/myprintpreviewdialog.h \
    ExportReport/doctoradvice.h \
    SystemConfig/managetipsinfo.h \
    ExportReport/pdfandprint.h \
    DataManagement/buttondelegate.h \
    DataManagement/tablemodel.h \
    DataManagement/exportlist.h \
    opration/movefile.h \
    DataManagement/openpdf.h \
    HomePage/homepage.h \
    DataContainer/flagcontainer.h \
    SystemConfig/usartcommunication.h

#FORMS    += mainwindow.ui

RESOURCES += \
    pic.qrc
