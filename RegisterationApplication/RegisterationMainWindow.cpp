#include "RegisterationMainWindow.h"
#include "ui_RegisterationMainWindow.h"

#include <QMenuBar>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <QInputDialog>
#include <QAction>
#include <QListView>
#include <QStringListModel>
#include <QDebug>
#include <QSqlError>
#include <QFile>
#include <QSplitter>
#include <QTableView>
#include <QSqlRelationalTableModel>
#include <QDateTime>
#include <QScrollBar>
#include <QDir>
#include <QPushButton>

#include "GwTabWindow.h"
#include "GwInputDialog.h"
#include "RegistrationForm.h"
#include "GwGenaralFunctions.h"
#include "GwTabWidget.h"
#include "GwSortAndFilterDialog.h"


RegisterationMainWindow::RegisterationMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterationMainWindow)
{
    ui->setupUi(this);
    setGeometry(100, 60, 800, 600);
    setWindowTitle(QCoreApplication::applicationName());
    tabWidget_ = new GwTabWidget;
    tabWidget_->setStyleSheet("background-color: rgb(255, 255, 255);\ncolor: rgb(0, 0, 0)");
    setCentralWidget(tabWidget_);

    ui->menuControls->setEnabled(false);
    ui->registerAction->setVisible(false);
    ui->actionSortAndFiterMode->setVisible(false);
    btn_ = nullptr;
    QMenuBar *thisManuBar = menuBar();
    thisManuBar->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
    connect(ui->newManagerAction, &QAction::triggered, this, &RegisterationMainWindow::onTriggerNewManagerAction);
    connect(ui->openManagerAction, &QAction::triggered, this, &RegisterationMainWindow::onTriggerOpenManagerAction);
    connect(ui->registerAction, &QAction::triggered, this, &RegisterationMainWindow::onTriggerRegisterAction);
    connect(ui->deleteManagerAction, &QAction::triggered, this, &RegisterationMainWindow::onTriggerDeleteDatabaseAction);
    connect(ui->actionAboutRegistrationManager, &QAction::triggered, this, &RegisterationMainWindow::onTriggerAboutThisApp);
    connect(ui->actionAboutQt, &QAction::triggered, this, &RegisterationMainWindow::onTriggerAboutQt);
    connect(ui->actionSortAndFilterTable, &QAction::triggered, this, &RegisterationMainWindow::onTriggerSortAndFilterACtion);
    connect(this, &RegisterationMainWindow::tableSorted, [&](){
        ui->actionSortAndFiterMode->setVisible(true);
    });
    connect(ui->actionSortAndFiterMode, &QAction::triggered, this, &RegisterationMainWindow::restoreBack);
}

RegisterationMainWindow::~RegisterationMainWindow()
{
    delete ui;
    delete tabWidget_;
}

void RegisterationMainWindow::onTriggerNewManagerAction()
{
    QInputDialog dlg(this);
    dlg.setStyleSheet("background-color: rgb(255, 255, 250);\ncolor: rgb(0, 0, 0)");
    dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
    dlg.setLabelText("Select a School");
    QStringList l;
    l << "Nursery" << "Primary" << "Secondary";
    dlg.setComboBoxItems(l);
    int ret = dlg.exec();

    if(ret == 1){
        QString dbName = dlg.textValue();
        createNewManager(dbName);
    }

}

void RegisterationMainWindow::onTriggerOpenManagerAction()
{
    GwInputDialog dlg(this);
    readFile();
    dlg.setListItems(dbs().keys());
    dlg.setLabelText("Select the manager you want to work with and click ok. Or "
                         "double click on it.\n If you want to add more manager, "
                         "click on the add button.\nRemember that this application can "
                         "not handle more than three managers");


    GwDatabaseCreator dbc;
    QString dbName;

    int ret = dlg.exec();

    if(ret == QDialog::Rejected && dlg.inputDialogListText().isEmpty()){
        return;
    }else if(ret == QDialog::Accepted){
        dbName = dlg.listText();
        dbc.dbConnection(QString("%1/registration_%2.db").arg(projectRootPath(QDir::homePath()) + "/database").arg(dbName));
        ui->menuControls->setEnabled(true);
        ui->registerAction->setVisible(true);
        if(!dbc.db().isOpen()){
            QMessageBox::critical(this, QString(tr("Not Opened")), QString(tr("%1 counld not opend./n%2")).arg(dbName).arg(dbs()[dbName].db().lastError().text()));
            return;
        }

        auto *tabWindow = new GwTabWindow(dbc, this);
        tabWindow->tableView()->horizontalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
        tabWindow->tableView()->verticalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
        tabWindow->tableView()->horizontalHeader()->setStyleSheet("background-color: rgb(0, 41, 61);\ncolor: rgb(0, 41, 61);");
        tabWindow->tableView()->verticalHeader()->setStyleSheet("background-color: rgb(0, 41, 61);\ncolor: rgb(0, 41, 61);");
        statusBar()->showMessage(tr("%1 Registration Manager is Opened").arg(dbName), 5000);
        tabWindow->refresh("registrationT");
        if(!tabWidget_->contains(dbName)){
            tabWidget_->addTab(tabWindow, dbName);
            return;
        }else {
            tabWidget_->setCurrentIndex(tabWidget_->tabIndex(dbName));
            return;
        }
    }else {
        createNewManager(dlg.inputDialogListText());
    }
}

void RegisterationMainWindow::onTriggerRegisterAction()
{
    RegistrationForm form(this);
    int ret = form.exec();
    if(1 == ret){
        QString sch = capitalizedEachWord(form.school().split(" ").at(0));
        QString s, schPreffix = form.regNumber().toUpper().split("/").at(2);

        if(schPreffix == "NS"){
            s = "Nursery";
        }else if(schPreffix == "PS"){
            s = "Primary";
        }if(schPreffix == "SS"){
            s = "Secondary";
        }

        QString tabText = tabWidget_->tabText(tabWidget_->currentIndex());
        if(s != tabText || sch != tabText){
            QMessageBox::critical(this, tr("Wrong School"),
                       tr("From the Registration Detail specified, "
                          "it show that this child is been registed in wrong "
                          "school.\nHe/She should be in %1 school "
                          "and not in %2 school\nFor that the registration "
                          "couldn't complete. Open the right Manager to "
                          "continue.  Thanks").arg(s).arg(sch));
            return;
        }

        readFile();
        if(dbs().contains(sch)){
            GwTabWindow *tabWindow = currentTabWindow();
            if(tabWindow){
                GwDatabaseCreator dbc = tabWindow->databaseCreator();
                QMap<QString, QVariant> mapData;
                mapData["regNum"] = form.regNumber();
                mapData["surname"] = form.surname();
                mapData["firstName"] = form.firstName();
                mapData["otherNames"] = form.otherNames();
                mapData["gender"] = form.gender();
                mapData["class"] = form.level();
                mapData["firstPay"] = form.firstPaymentAmount();
                mapData["regDate"] = QVariant(QDateTime::currentDateTime().toString("ddd, MMM d, yyyy h:mm:ss"));
                if(!dbc.db().isOpen() || !dbc.insertData("registrationT", mapData)){
                    dbc.errorMessage("Insert Error", dbc.dbErrors());
                }else{
                    statusBar()->showMessage(tr("%1 has been registered successfully").arg(form.firstName()), 5000);
                    tabWindow->refresh("registrationT");
                }
            }
        }else{
            qDebug() << form.student().studentData();
        }
    }
}

void RegisterationMainWindow::onTriggerDeleteDatabaseAction()
{
    QString tabText = tabWidget_->tabText(tabWidget_->currentIndex());

    int ret = QMessageBox::warning(nullptr, tr("Deleting Manager"), tr("You about to delete %1 Registration Manager.\n"
                                                           "This action will remove the database file and cannot be"
                                                             " undo.\nBe double sure that you know what you want to do.\n"
                                                             "If you are not sure please cancel the action.\n"
                                                             "Do you want to continue?").arg(tabText)
                         , QMessageBox::Yes | QMessageBox::Cancel);
    if(ret == QMessageBox::Yes){
        GwTabWindow *tabWindow = currentTabWindow();
        GwDatabaseCreator dbc = tabWindow->databaseCreator();
        if(dbc.db().isOpen()){
            if(dbc.deleteDatabase(QString("%1_registraion").arg(tabText))){
                QMessageBox::information(nullptr, tr("Database removed"), tr("Database deleted"));
            }else{
                QMessageBox::information(nullptr, tr("Database not removed"), tr("Database could not be delete\n%1").arg(dbc.db().lastError().text()));
            }
        }
    }
}

void RegisterationMainWindow::onTriggerAboutThisApp()
{
    QMessageBox::information(this, tr("About Registration Manager"), tr("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Version 1.0</span></p>"
                                                                        "<p>This application is use strictly for registrion into Nursery, Primary and Secondary Schools.\n"
                                                                        " It stores the students' and their perants' information in an SQLite database.</p></body></html>"));
}

void RegisterationMainWindow::onTriggerAboutQt()
{
    qApp->aboutQt();
}

void RegisterationMainWindow::onTriggerSortAndFilterACtion()
{
    auto *wind = currentTabWindow();

    GwSortAndFilterDialog dlg(wind->model(), this);
    dlg.setFieldBoxItems();
    int ret = dlg.exec();

    if(QDialog::Accepted == ret){
        QSqlQuery query = wind->databaseCreator().sortAndFilter("registrationT", dlg.tableSortOrder(), dlg.tableFilters());
        wind->model()->setQuery(query);
        statusBar()->addAction(ui->actionSortAndFiterMode);
        btn_ = new QPushButton("sort and fiter mode");
        btn_->setIcon(QIcon(":/images/Filter.png"));
        connect(btn_, &QPushButton::clicked, this, &RegisterationMainWindow::restoreBack);
        statusBar()->addPermanentWidget(btn_);
        emit tableSorted();
    }
}

void RegisterationMainWindow::restoreBack()
{
    auto *wind = currentTabWindow();
    wind->refresh("registrationT");
    ui->actionSortAndFiterMode->setVisible(false);
    delete btn_;
    btn_ = nullptr;
}

void RegisterationMainWindow::createNewManager(const QString &dbName)
{
    GwDatabaseCreator dbc;
    readFile();
    dbc.dbConnection(QString("%1/registration_%2.db").arg(projectRootPath(QDir::homePath()) + "/database").arg(dbName));
    if(dbc.tables().isEmpty()){
        if(!createTables(dbc)) return;
        if(!dbs().contains(dbName)){
            addDatabase(dbName, dbc);
        }
        writeFile();
    }
    ui->menuControls->setEnabled(true);
    ui->registerAction->setVisible(true);
    auto *tabWindow = new GwTabWindow(dbc, this);
    tabWindow->tableView()->horizontalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
    tabWindow->tableView()->verticalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
    tabWindow->tableView()->horizontalHeader()->setStyleSheet("background-color: rgb(0, 41, 61);\ncolor: rgb(0, 41, 61);");
    tabWindow->tableView()->verticalHeader()->setStyleSheet("background-color: rgb(0, 41, 61);\ncolor: rgb(0, 41, 61);");
    if(!tabWidget_->contains(dbName)){
        tabWidget_->addTab(tabWindow, dbName);
    }else {
        tabWidget_->setCurrentIndex(tabWidget_->tabIndex(dbName));
    }
    tabWindow->refresh("registrationT");
    statusBar()->showMessage(tr("%1 Registration Manager Created").arg(dbName), 5000);
}

bool RegisterationMainWindow::createTables(GwDatabaseCreator &db)
{
    db.db().transaction();
    bool a = db.createTable("CREATE TABLE registrationT(regNum VARCHAR(20) PRIMARY KEY  NOT NULL, "
                           "surname VARCHAR(30) NOT NULL, firstName VARCHAR(30) NOT NULL, otherNames VARCHAR(30) NULL, "
                           "gender VARCHAR(6) NOT NULL, class VARCHAR(20) NOT NULL, firstPay DECIMAL NOT NULL, regDate VARCHAR(35) )");

    bool b = db.createTable("CREATE TABLE stuDetailDataT(dataID INTEGER PRIMARY KEY, regNum VARCHAR(20) UNIQUE NOT NULL, "
                   "dateOfBirth VARCHAR(15) NULL, height VARCHAR(10) NULL, weight VARCHAR(10) NULL, "
                   "religion VARCHAR(6) NULL, bloodGroup VARCHAR(5) NULL, genotype VARCHAR(5) NULL, "
                   "photoPath VARCHAR(255) Null, FOREIGN KEY(regNum) REFERENCES registrationT(regNum) ON DELETE CASCADE ON UPDATE CASCADE)");

    bool c = db.createTable("CREATE TABLE resAddressT(resAddID INTEGER PRIMARY KEY, regNum VARCHAR(20) UNIQUE NOT NULL, "
                   "address VARCHAR(255) NULL, resCity VARCHAR(255) NULL, resLga VARCHAR(255) NULL, "
                   "resState VARCHAR(25) NULL, phoneNum VARCHAR(25) NULL, email VARCHAR(50) NULL, "
                   "FOREIGN KEY(regNum) REFERENCES registrationT(regNum) ON DELETE CASCADE ON UPDATE CASCADE)");

    bool d = db.createTable("CREATE TABLE permAddressT(permAddID INTEGER PRIMARY KEY, regNum VARCHAR(20) UNIQUE NOT NULL, "
                   "permHomeAddress VARCHAR(255) NULL, lgaOfOrigin VARCHAR(255) NULL, stateOfOrigin VARCHAR(25) NULL, "
                   "nationality VARCHAR(25) NULL, "
                   "FOREIGN KEY(regNum) REFERENCES registrationT(regNum) ON DELETE CASCADE ON UPDATE CASCADE)");

    bool e = db.createTable("CREATE TABLE parentsT(parentID INTEGER PRIMARY KEY NOT NULL, "
                   "lastName VARCHAR(30) NOT NULL, firstName VARCHAR(30) NOT NULL,"
                   "relationship VARCHAR(10) NOT NULL, occupation VARCHAR(20) NOT NULL)");

    bool f = db.createTable("CREATE TABLE studentParentT(parentID INTEGER NOT NULL, regNum VARCHAR(20) UNIQUE NOT NULL,"
                   "FOREIGN KEY(parentID) REFERENCES parentsT(parentID) ON DELETE CASCADE ON UPDATE CASCADE, "
                   "FOREIGN KEY(regNum) REFERENCES registrationT(regNum) ON DELETE CASCADE ON UPDATE CASCADE)");

    bool g = db.createTable("CREATE TABLE parentHomeAddressT(homeAddID INTEGER PRIMARY KEY, homeAddress VARCHAR(255) NULL ,"
                   "homeCity VARCHAR(255) NULL, homeLga VARCHAR(255) NULL, homeState VARCHAR(25) NULL, "
                   "homeEmail VARCHAR(50) NULL, homePhoneNumber VARCHAR(20) NULL , parentID INTEGER NOT NULL,"
                   "FOREIGN KEY(homeAddID) REFERENCES parentsT(parentID) ON DELETE CASCADE ON UPDATE CASCADE)");

    bool h = db.createTable("CREATE TABLE parentOfficeAddressT(officeAddID INTEGER PRIMARY KEY, officeAddress VARCHAR(255) NULL ,"
                   "officeCity VARCHAR(255) NULL, officeLga VARCHAR(255) NULL, officeState VARCHAR(25) NULL, "
                   "officeEmail VARCHAR(50) NULL, officePhoneNumber VARCHAR(20) NULL , parentID INTEGER NOT NULL,"
                   "FOREIGN KEY(officeAddID) REFERENCES parentsT(parentID) ON DELETE CASCADE ON UPDATE CASCADE)");

    if(a && b && c && d && e && f && g && h && db.db().commit()){
        QMessageBox::information(this, QString(tr("Success")), QString(tr("Manager added")));
    }else{
        QMessageBox::critical(this, QString(tr("Failed")), QString(tr("Could not create Manager")));
        db.db().rollback();
        return false;
    }
    return true;
}

void RegisterationMainWindow::addDatabase(const QString &dbName, GwDatabaseCreator db)
{
    if(dbs_.size() < 3){
        dbs_[dbName] = db;
    }else{
        QMessageBox::critical(this, QString(tr("Limit Exceeded")), QString(tr("This application can not handle more than "
                                                        "three different schools")));
    }
}

QMap<QString, GwDatabaseCreator> RegisterationMainWindow::dbs() const
{
    return dbs_;
}

bool RegisterationMainWindow::writeFile()
{
    QFile file(QString("%1/db_config.rem").arg(projectRootPath(QDir::homePath())));
    if(file.open(QFile::WriteOnly)){
        QDataStream byteArr(&file);
        byteArr << dbs_;
        return true;
    }

    return false;
}

bool RegisterationMainWindow::readFile()
{
    QFile file(QString("%1/db_config.rem").arg(projectRootPath(QDir::homePath())));
    if(file.open(QFile::ReadOnly)){
        QDataStream byteArr(&file);
        byteArr >> dbs_;
        return true;
    }
    return false;
}

GwTabWindow *RegisterationMainWindow::currentTabWindow() const
{
     return dynamic_cast<GwTabWindow *>(tabWidget_->currentWidget());
}
