#include "RegisterationMainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <GwSortAndFilterDialog.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Registration Manager");
    QCoreApplication::setOrganizationName("Emak God's Own Schools");
    QCoreApplication::setApplicationVersion("1.0");
    QCommandLineParser parser;
    parser.setApplicationDescription(QString("%1\nThis application is use strictly for registrion"
                        " into Nursery, Primary and Secondary Schools.\n"
                       "It stores the students' and their perants' information in an SQLite"
                       " database").arg(QCoreApplication::applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(a);

    RegisterationMainWindow f;
    f.show();

    return a.exec();
}
