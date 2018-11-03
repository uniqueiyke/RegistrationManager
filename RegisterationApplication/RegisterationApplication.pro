#-------------------------------------------------
#
# Project created by QtCreator 2018-08-05T01:48:14
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport
TARGET = RegisterationApplication
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        RegisterationMainWindow.cpp \
    RegistrationForm.cpp \
    GwGenaralFunctions.cpp \
    GwStudentFullDataForm.cpp \
    GwParentInfoForm.cpp \
    GwDatabaseCreator.cpp \
    GwInputDialog.cpp \
    GwTabWindow.cpp \
    GwStudent.cpp \
    GwPerson.cpp \
    GwTabWidget.cpp \
    GwAddress.cpp \
    GwParent.cpp \
    GwStudentData.cpp \
    GwViewWidget.cpp \
    GwViewWindow.cpp \
    GwSortAndFilterDialog.cpp \
    GwSqlClauses.cpp

HEADERS += \
        RegisterationMainWindow.h \
    RegistrationForm.h \
    GwGenaralFunctions.h \
    GwStudentFullDataForm.h \
    GwParentInfoForm.h \
    GwDatabaseCreator.h \
    GwInputDialog.h \
    GwTabWindow.h \
    GwStudent.h \
    GwPerson.h \
    GwTabWidget.h \
    GwAddress.h \
    GwParent.h \
    GwStudentData.h \
    GwViewWidget.h \
    GwViewWindow.h \
    GwSortAndFilterDialog.h \
    GwSqlClauses.h

FORMS += \
        RegisterationMainWindow.ui \
    RegistrationForm.ui \
    GwStudentFullDataForm.ui \
    GwParentInfoForm.ui \
    GwInputDialog.ui \
    GwViewWindow.ui \
    GwSortAndFilterDialog.ui

RESOURCES += \
    images.qrc

QMAKE_CXXFLAGS += -std=gnu++14
