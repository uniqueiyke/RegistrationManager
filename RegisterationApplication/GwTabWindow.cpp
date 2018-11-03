#include "GwTabWindow.h"

#include <QSplitter>
#include <QAction>
#include <QToolBar>
#include <QTableView>
#include <QHeaderView>
#include <QMenu>
#include <QtSql>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QScrollBar>

#include "GwStudentFullDataForm.h"
#include "GwTabWidget.h"
#include "GwParentInfoForm.h"
#include "RegistrationForm.h"
#include "GwViewWidget.h"
#include "GwViewWindow.h"

GwTabWindow::GwTabWindow(const GwDatabaseCreator &dbCreator, QWidget *parent)
    : QMainWindow(parent)
{
    dbCreator_ = dbCreator;
    splitter_ = new QSplitter(Qt::Vertical);

    widgetTab_ = new GwTabWidget;

    tableView_ = new QTableView;
    model_ = new QSqlQueryModel(this);
    tableView_->setModel(model_);
    tableView_->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView_->setContextMenuPolicy(Qt::CustomContextMenu);

    QSizePolicy policy = tableView_->sizePolicy();
    policy.setVerticalStretch(3);
    tableView_->setSizePolicy(policy);

    QSizePolicy policy1 = widgetTab_->sizePolicy();
    policy1.setVerticalStretch(1);
    widgetTab_->setSizePolicy(policy1);

    splitter_->addWidget(tableView_);
    splitter_->addWidget(widgetTab_);

    widgetTab_->setVisible(false);

    setCentralWidget(splitter_);

    connect(tableView_->verticalHeader(), &QHeaderView::customContextMenuRequested, this, &GwTabWindow::onVHeaderContextMenuRequested);
    connect(tableView_, &QTableView::customContextMenuRequested, this, &GwTabWindow::onViewContextMenuRequested);
    connect(widgetTab_, &GwTabWidget::tabBarOpened, widgetTab_, &GwTabWidget::setVisible);
}

GwTabWindow::~GwTabWindow()
{
    delete model_;
    delete tableView_;
    delete widgetTab_;
    delete splitter_;
}

QTableView *GwTabWindow::tableView()
{
    return tableView_;
}

QSqlQueryModel *GwTabWindow::model()
{
    return model_;
}

GwDatabaseCreator &GwTabWindow::databaseCreator()
{
    return dbCreator_;
}

void GwTabWindow::refresh(const QString &table)
{
    model_->setQuery(dbCreator_.selectAll(table, "DESC"));
    QStringList l;
            l << "Reg. Number" << "Surname" << "First Name" << "Other Name" << "Gender"
              << "Class" << "First Payment" << "Date of Registration";
    setHeaderLables(l, Qt::Horizontal, model_);
    if (model_->lastError().isValid())
        QMessageBox::critical(this, QString(tr("failed")), QString(tr("%1")).arg(model_->lastError().text()));
}

void GwTabWindow::onVHeaderContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.setStyleSheet("background-color: rgb(236, 236, 236);\ncolor: rgb(0, 0, 0)");

    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record;
    if(index.isValid()){
        record = model()->record(index.row());
    }

    QString firstName = record.value("firstName").toString(),
            surname = record.value("surname").toString();

    QAction *showRecordAction = new QAction(tr("Brief infomation about %1 %2")
                      .arg(surname).arg(firstName), this);
    connect(showRecordAction, &QAction::triggered, this, &GwTabWindow::onTriggerStudentDetailData);
    menu.addAction(showRecordAction);

    QAction *addStudDataAction = new QAction(tr("Add %1's detailed data").arg(firstName), this);
    connect(addStudDataAction, &QAction::triggered, this, &GwTabWindow::onTriggerAddData);
    menu.addAction(addStudDataAction);

    QAction *addParentDataAction = new QAction(tr("Add Parent Data"), this);
    connect(addParentDataAction, &QAction::triggered, this, &GwTabWindow::onTriggerAddParentData);
    menu.addAction(addParentDataAction);

    QAction *viewParentDataAction = new QAction(tr("View %1's parent data").arg(firstName), this);
    connect(viewParentDataAction, &QAction::triggered, this, &GwTabWindow::onTriggerParentData);
    menu.addAction(viewParentDataAction);

    QAction *viewAllAction = new QAction(tr("%1's Data in printable form").arg(firstName), this);
    connect(viewAllAction, &QAction::triggered, this, &GwTabWindow::onViewCompleteData);
    menu.addAction(viewAllAction);

    QAction *viewAllAction2 = new QAction(tr("View %1's Data in Form style").arg(firstName), this);
    connect(viewAllAction2, &QAction::triggered, this, &GwTabWindow::onViewCompleteStudentData);
    menu.addAction(viewAllAction2);

    auto *editDataMenu = new QMenu(tr("Edit %1 %2's Rocord").arg(surname).arg(firstName));

    QAction *editStudentDataAction = new QAction(tr("Edit Student Record"), this);
    connect(editStudentDataAction, &QAction::triggered, this, &GwTabWindow::onEditStudentData);
    editDataMenu->addAction(editStudentDataAction);

    QAction *editParentDataAction = new QAction(tr("Edit Parent Record"), this);
    connect(editParentDataAction, &QAction::triggered, this, &GwTabWindow::onEditParentData);
    editDataMenu->addAction(editParentDataAction);

    QAction *deleteAction = new QAction(tr("Delete %1 %2 record").arg(surname).arg(firstName), this);
    connect(deleteAction, &QAction::triggered, this, &GwTabWindow::onDeleteRecord);
    menu.addAction(deleteAction);

    menu.addMenu(editDataMenu);
    menu.exec(tableView()->verticalHeader()->viewport()->mapToGlobal(pos));
}

void GwTabWindow::onViewContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.setStyleSheet("background-color: rgb(236, 236, 236);\ncolor: rgb(0, 0, 0)");

    QAction *showRecordsAction = new QAction(tr("Brief infomation of students"), this);
    connect(showRecordsAction, &QAction::triggered, this, &GwTabWindow::onTriggerStudentsBriefData);
    menu.addAction(showRecordsAction);

    QAction *viewParentDataAction = new QAction(tr("Parents Brief information"), this);
    connect(viewParentDataAction, &QAction::triggered, this, &GwTabWindow::onTriggerAllParentData);
    menu.addAction(viewParentDataAction);

    menu.exec(tableView()->viewport()->mapToGlobal(pos));
}

void GwTabWindow::onTriggerAddData()
{

    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());
    QString regNum = record.value("regNum").toString();

    GwStudentFullDataForm dataForm(this);
    dataForm.setRegNum(regNum);
    dataForm.setRegNumEditorReadOnly();
    dataForm.setSurnameEditorReadOnly();
    dataForm.setSurname(QString("%1").arg(record.value("surname").toString()));
    dataForm.setFirstName(QString("%1").arg(record.value("firstName").toString()));
    dataForm.setFirstNameReadOnly();
    dataForm.setOtherNames(QString("%1").arg(record.value("otherNames").toString()));
    dataForm.setOtherNamesReadOnly();
    dataForm.setGender(QString("%1").arg(record.value("gender").toString()));
    QString cl = QString("%1").arg(record.value("class").toString());
    if(!cl.isEmpty() && (cl.startsWith("Nursery", Qt::CaseInsensitive)
                         || cl.startsWith("Day", Qt::CaseInsensitive)
                         || cl.endsWith("nursery", Qt::CaseInsensitive))){
        dataForm.setSchool(SCHOOL::NURSERY);
    }else if (!cl.isEmpty() && (cl.startsWith("Primary", Qt::CaseInsensitive))) {
        dataForm.setSchool(SCHOOL::PRYMARY);
    }else if (!cl.isEmpty() && ((cl.startsWith("JS", Qt::CaseInsensitive)) ||
                                cl.startsWith("SS", Qt::CaseInsensitive))) {
        dataForm.setSchool(SCHOOL::SECONDARY);
    }
    dataForm.setClass(QString("%1").arg(record.value("class").toString()));
    dataForm.deseableControls();
    int ret = dataForm.exec();

    if(1==ret){
        QMap<QString, QVariant> personalData;
        QMap<QString, QVariant> permHomeData;
        QMap<QString, QVariant> resHomeData;

        personalData["regNum"] = QVariant(dataForm.regNumber());
        personalData["dateOfBirth"] = QVariant(dataForm.dateOfBirth());
        personalData["height"] = QVariant(dataForm.stuHeight());
        personalData["weight"] = QVariant(dataForm.weigth());
        personalData["religion"] = QVariant(dataForm.religion());
        personalData["bloodGroup"] = QVariant(dataForm.bloodGroup());
        personalData["genotype"] = QVariant(dataForm.genotype());
        personalData["photoPath"] = QVariant(dataForm.savedImagePath());


        permHomeData["regNum"] = QVariant(dataForm.regNumber());
        permHomeData["permHomeAddress"] = QVariant(dataForm.permAddress());
        permHomeData["lgaOfOrigin"] = QVariant(dataForm.permLGA());
        permHomeData["stateOfOrigin"] = QVariant(dataForm.permState());
        permHomeData["nationality"] = QVariant(dataForm.nationality());

        resHomeData["regNum"] = QVariant(dataForm.regNumber());
        resHomeData["address"] = QVariant(dataForm.resAddress());
        resHomeData["resCity"] = QVariant(dataForm.city());
        resHomeData["resLga"] = QVariant(dataForm.resLGA());
        resHomeData["resState"] = QVariant(dataForm.resState());
        resHomeData["phoneNum"] = QVariant(dataForm.phoneNumber());
        resHomeData["email"] = QVariant(dataForm.email());

        databaseCreator().db().transaction();
        bool i = databaseCreator().insertData("stuDetailDataT", personalData);
        bool j = databaseCreator().insertData("permAddressT", permHomeData);
        bool k = databaseCreator().insertData("resAddressT", resHomeData);
        if(i && j && k && databaseCreator().db().commit()){
            QMessageBox::information(this, QString("Success"), QString("Data added"));
        }else {
            databaseCreator().db().rollback();
            QMessageBox::information(this, QString("failed"), QString("Is like this Student's "
                                      "record already exist.\nUse edit student record to make changes "
                                       "in to the record."));
        }
    }

}

void GwTabWindow::onTriggerStudentDetailData()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());

    QString regNum = "\"" + record.value("regNum").toString() + "\"";

    QString title = QString(tr("%1's brief info").arg(record.value("firstName").toString()));
    studentBriefData(title, regNum);
}

void GwTabWindow::onTriggerStudentsBriefData()
{
    studentBriefData("Info Summary");
}

void GwTabWindow::onTriggerAddParentData()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());
    QString regNum = record.value("regNum").toString();

    GwParentInfoForm dataForm(this);
    dataForm.setRegNumber(regNum);
    dataForm.setName(QString("%1 %2").arg(record.value("surname").toString())
                     .arg(record.value("firstName").toString()));
    int ret = dataForm.exec();
    if(1 == ret){
        databaseCreator().db().transaction();

        QMap<QString, QVariant> parentData;
        QMap<QString, QVariant> parentStudentRelationShip;
        QMap<QString, QVariant> parentHomeAddress;
        QMap<QString, QVariant> parentOfficeAddress;

        parentData["lastName"] = QVariant(dataForm.pLastName());
        parentData["firstName"] = QVariant(dataForm.pFirstName());
        parentData["relationship"] = QVariant(dataForm.pRelationship());
        parentData["occupation"] = QVariant(dataForm.pOccupation());

        bool a = databaseCreator().insertData("parentsT", parentData);
        QVariant parentID = databaseCreator().lastInsertedID();

        qDebug() << parentData;
        parentHomeAddress["homeAddress"] = QVariant(dataForm.pHAddress());
        parentHomeAddress["homeCity"] = QVariant(dataForm.pHCity());
        parentHomeAddress["homeLga"] = QVariant(dataForm.pHLga());
        parentHomeAddress["homeState"] = QVariant(dataForm.pHState());
        parentHomeAddress["homeEmail"] = QVariant(dataForm.pHEmail());
        parentHomeAddress["homePhoneNumber"] = QVariant(dataForm.pHPhoneNumber());
        parentHomeAddress["parentID"] =  parentID;

        parentOfficeAddress["officeAddress"] = QVariant(dataForm.pOAddress());
        parentOfficeAddress["officeCity"] = QVariant(dataForm.pOCity());
        parentOfficeAddress["officeLga"] = QVariant(dataForm.pOLga());
        parentOfficeAddress["officeState"] = QVariant(dataForm.pOState());
        parentOfficeAddress["officeEmail"] = QVariant(dataForm.pOEmail());
        parentOfficeAddress["officePhoneNumber"] = QVariant(dataForm.pOPhoneNumber());
        parentOfficeAddress["parentID"] = parentID;

        parentStudentRelationShip["parentID"] = parentID;
        parentStudentRelationShip["regNum"] = QVariant(regNum);

        bool b = databaseCreator().insertData("studentParentT", parentStudentRelationShip);
        bool c = databaseCreator().insertData("parentHomeAddressT", parentHomeAddress);
        bool d = databaseCreator().insertData("parentOfficeAddressT", parentOfficeAddress);


        if(a && b && c && d && databaseCreator().db().commit()){
            QMessageBox::information(this, QString("Success"), QString("Data added"));
        }else{
            databaseCreator().db().rollback();
            QMessageBox::critical(this, QString("Failed"), QString("Operation could not complete"));
        }
    }
}

void GwTabWindow::onTriggerParentData()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());

    QString regNum = "\"" + record.value("regNum").toString() + "\"";

    QString title = QString(tr("%1's parent brief info").arg(record.value("firstName").toString()));
    parentData(title, regNum);
}

void GwTabWindow::onTriggerAllParentData()
{
    parentData("Parents Data");
}

void GwTabWindow::onEditParentData()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());

    QString regNum = record.value("regNum").toString();

    GwWhereClause regNumWhere("regNum", QVariant(regNum));
    QSqlQuery query = databaseCreator().fetchData("studentParentT", regNumWhere);
    if(query.next()) {
        long long id = query.value("parentID").toLongLong();

        GwWhereClause where("parentID", QVariant(id));
        QSqlQuery query1 = databaseCreator().fetchData("parentsT", where);
        QSqlQuery query2 = databaseCreator().fetchData("parentHomeAddressT", where);
        QSqlQuery query3 = databaseCreator().fetchData("parentOfficeAddressT", where);

        GwParentInfoForm dataForm(this);

        dataForm.setRegNumber(regNum);
        dataForm.setName(QString("%1 %2").arg(record.value("surname").toString())
                         .arg(record.value("firstName").toString()));

        if(query1.next()) {
            dataForm.setPFirstName(query1.value("firstName").toString());
            dataForm.setPLastName(query1.value("lastName").toString());
            dataForm.setPRelationship(query1.value("relationship").toString());
            dataForm.setPOccupation(query1.value("occupation").toString());
        }

        if(query2.next()){
            dataForm.setPHAddress(query2.value("homeAddress").toString());\
            dataForm.setPHCity(query2.value("homeCity").toString());
            dataForm.setPHState(query2.value("homeState").toString());
            dataForm.setPHLga(query2.value("homeLga").toString());
            dataForm.setPHPhoneNumber(query2.value("homePhoneNumber").toString());
            dataForm.setPHEmail(query2.value("homeEmail").toString());
        }

        if(query3.next()){
            dataForm.setPOAddress(query3.value("officeAddress").toString());
            dataForm.setPOCity(query3.value("officeCity").toString());
            dataForm.setPOState(query3.value("officeState").toString());
            dataForm.setPOLga(query3.value("officeLga").toString());
            dataForm.setPOPhoneNumber(query3.value("officePhoneNumber").toString());
            dataForm.setPOEmail(query3.value("officeEmail").toString());
        }

        int ret = dataForm.exec();

        if(1 == ret){
            databaseCreator().db().transaction();

            QMap<QString, QVariant> parentData;
            QMap<QString, QVariant> homeData;
            QMap<QString, QVariant> officeData;

            parentData["firstName"] = dataForm.pFirstName();
            parentData["lastName"] = dataForm.pLastName();
            parentData["relationship"] = dataForm.pRelationship();
            parentData["occupation"] = dataForm.pOccupation();

            homeData["homeAddress"] = dataForm.pHAddress();
            homeData["homeCity"] = dataForm.pHCity();
            homeData["homeState"] = dataForm.pHState();
            homeData["homeLga"] = dataForm.pHLga();
            homeData["homePhoneNumber"] = dataForm.pHPhoneNumber();
            homeData["homeEmail"] = dataForm.pHEmail();

            officeData["officeAddress"] = dataForm.pOAddress();
            officeData["officeCity"] = dataForm.pOCity();
            officeData["officeState"] = dataForm.pOState();
            officeData["officeLga"] = dataForm.pOLga();
            officeData["officePhoneNumber"] = dataForm.pOPhoneNumber();
            officeData["officeEmail"] = dataForm.pOEmail();

            QMap<GwDatabaseCreator::DATATYPES, QVariant> where;
            where[GwDatabaseCreator::DATATYPES::LONG] = QVariant(id);

            bool x = databaseCreator().updateData("parentsT", parentData, "parentID", where);
            bool y = databaseCreator().updateData("parentHomeAddressT", homeData, "parentID", where);
            bool z = databaseCreator().updateData("parentOfficeAddressT", officeData, "parentID", where);
            if(x && y && z && databaseCreator().db().commit()){
                QMessageBox::information(this, tr("Update Completed"), tr("The update was "
                                "successful. The data has been stored"));
            }else{
                databaseCreator().db().rollback();
                QMessageBox::critical(this, tr("Update Failed"), tr("The update was "
                                "not successful. No data stored"));
            }
        }
    }else{
        QMessageBox::information(this, tr("No Data"), tr("The parent data has not been supply"));
    }
}

void GwTabWindow::onViewCompleteStudentData()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());
    QString regNum = record.value("regNum").toString();
    GwWhereClause regNumWhere("regNum", QVariant(regNum));

    auto *dataForm = new GwStudentFullDataForm(this);
    {
        dataForm->setRegNum(regNum);
        dataForm->setSurname(QString("%1").arg(record.value("surname").toString()));
        dataForm->setFirstName(QString("%1").arg(record.value("firstName").toString()));
        dataForm->setOtherNames(QString("%1").arg(record.value("otherNames").toString()));
        dataForm->setGender(QString("%1").arg(record.value("gender").toString()));
        QString cl = QString("%1").arg(record.value("class").toString());
        if(!cl.isEmpty() && (cl.startsWith("Nursery", Qt::CaseInsensitive)
                             || cl.startsWith("Day", Qt::CaseInsensitive)
                             || cl.endsWith("nursery", Qt::CaseInsensitive))){
            dataForm->setSchool(SCHOOL::NURSERY);
        }else if (!cl.isEmpty() && (cl.startsWith("Primary", Qt::CaseInsensitive))) {
            dataForm->setSchool(SCHOOL::PRYMARY);
        }else if (!cl.isEmpty() && ((cl.startsWith("JS", Qt::CaseInsensitive)) ||
                                    cl.startsWith("SS", Qt::CaseInsensitive))) {
            dataForm->setSchool(SCHOOL::SECONDARY);
        }
        dataForm->setClass(QString("%1").arg(record.value("class").toString()));

        QSqlQuery query1 = databaseCreator().fetchData("stuDetailDataT", regNumWhere);
        QSqlQuery query2 = databaseCreator().fetchData("permAddressT", regNumWhere);
        QSqlQuery query3 = databaseCreator().fetchData("resAddressT", regNumWhere);

        while(query1.next()){
            dataForm->setDateOfBirth(query1.value("dateOfBirth").toString());
            dataForm->setStuHeight(query1.value("height").toDouble());
            dataForm->setWeigth(query1.value("weight").toDouble());
            dataForm->setReligion(query1.value("religion").toString());
            dataForm->setBloodGroup(query1.value("bloodGroup").toString());
            dataForm->setGenotype(query1.value("genotype").toString());
            dataForm->setLabelImage(query1.value("photoPath").toString());
        }

        while(query2.next()){
            dataForm->setPermAddress(query2.value("permHomeAddress").toString());
            dataForm->setPermState(query2.value("stateOfOrigin").toString());
            dataForm->setPermLGA(query2.value("lgaOfOrigin").toString());
            dataForm->setNationality(query2.value("nationality").toString());
        }

        while(query3.next()){
            dataForm->setResAddress(query3.value("address").toString());
            dataForm->setCity(query3.value("resCity").toString());
            dataForm->setResState(query3.value("resState").toString());
            dataForm->setResLGA(query3.value("resLga").toString());
            dataForm->setPhoneNumber(query3.value("phoneNum").toString());
            dataForm->setEmail(query3.value("email").toString());
        }
    }

    auto *dataForm1 = new GwParentInfoForm (this);
    {
        QSqlQuery query = databaseCreator().fetchData("studentParentT", regNumWhere);
        if(query.next()) {
            long long id = query.value("parentID").toLongLong();

            GwWhereClause where("parentID", QVariant(id));
            QSqlQuery query1 = databaseCreator().fetchData("parentsT", where);
            QSqlQuery query2 = databaseCreator().fetchData("parentHomeAddressT", where);
            QSqlQuery query3 = databaseCreator().fetchData("parentOfficeAddressT", where);

            dataForm1->setRegNumber(regNum);
            dataForm1->setName(QString("%1 %2").arg(record.value("surname").toString())
                             .arg(record.value("firstName").toString()));

            if(query1.next()) {
                dataForm1->setPFirstName(query1.value("firstName").toString());
                dataForm1->setPLastName(query1.value("lastName").toString());
                dataForm1->setPRelationship(query1.value("relationship").toString());
                dataForm1->setPOccupation(query1.value("occupation").toString());
            }

            if(query2.next()){
                dataForm1->setPHAddress(query2.value("homeAddress").toString());
                dataForm1->setPHCity(query2.value("homeCity").toString());
                dataForm1->setPHState(query2.value("homeState").toString());
                dataForm1->setPHLga(query2.value("homeLga").toString());
                dataForm1->setPHPhoneNumber(query2.value("homePhoneNumber").toString());
                dataForm1->setPHEmail(query2.value("homeEmail").toString());
            }

            if(query3.next()){
                dataForm1->setPOAddress(query3.value("officeAddress").toString());
                dataForm1->setPOCity(query3.value("officeCity").toString());
                dataForm1->setPOState(query3.value("officeState").toString());
                dataForm1->setPOLga(query3.value("officeLga").toString());
                dataForm1->setPOPhoneNumber(query3.value("officePhoneNumber").toString());
                dataForm1->setPOEmail(query3.value("officeEmail").toString());
            }
        }
    }

    auto *w = new GwViewWidget(dataForm, dataForm1);
    w->show();
}

void GwTabWindow::onViewCompleteData()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());
    QString regNum = record.value("regNum").toString();

    QString cl = QString("%1").arg(record.value("class").toString());
    QString sch;
    if(!cl.isEmpty() && (cl.startsWith("Nursery", Qt::CaseInsensitive)
                         || cl.startsWith("Day", Qt::CaseInsensitive)
                         || cl.endsWith("nursery", Qt::CaseInsensitive))){
        sch = "Nursery";
    }else if (!cl.isEmpty() && (cl.startsWith("Primary", Qt::CaseInsensitive))) {
        sch = "Primary";
    }else if (!cl.isEmpty() && ((cl.startsWith("JS", Qt::CaseInsensitive)) ||
                                cl.startsWith("SS", Qt::CaseInsensitive))) {
        sch = "Secondary";
    }

    auto *win = new GwViewWindow(regNum, databaseCreator());
    win->show();
}

void GwTabWindow::onDeleteRecord()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());
    QString regNum = record.value("regNum").toString();
    GwWhereClause regNumWhere("regNum", QVariant(regNum));

    QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Delete Record"), tr("You are about to delete record of %1 %2 with Reg. Number %3.\n"
                                                        "Do you wish to continue with the delete operation?")
               .arg(record.value("surname").toString()).arg(record.value("firstName").toString()).arg(regNum));
    if(ret == QMessageBox::Yes){
        databaseCreator().db().transaction();
        QSqlQuery que = databaseCreator().fetchData("studentParentT", regNumWhere);
        QVariant parentID;
        if(que.next()){
            parentID = que.value("parentID");
        }
        bool a = databaseCreator().deleteRecord("registrationT", regNumWhere);
        bool b = databaseCreator().deleteRecord("parentsT", GwWhereClause("parentID", parentID));
        if(a && b && databaseCreator().db().commit()){
            QMessageBox::information(this, tr("Delete Record"), tr("Record Deleted"));
            refresh("registrationT");
        }else{
            databaseCreator().db().rollback();
            QMessageBox::critical(this, tr("Delete Record"), tr("Record not Deleted %1").arg(databaseCreator().db().lastError().text()));
        }
    }

}

void GwTabWindow::onEditStudentData()
{
    QModelIndex index = tableView()->verticalHeader()->currentIndex();
    QSqlRecord record = model()->record(index.row());
    QString regNum = record.value("regNum").toString();
    GwWhereClause regNumWhere("regNum", QVariant(regNum));

    GwStudentFullDataForm dataForm(this);
    dataForm.setRegNum(regNum);
    dataForm.setSurname(QString("%1").arg(record.value("surname").toString()));
    dataForm.setFirstName(QString("%1").arg(record.value("firstName").toString()));
    dataForm.setOtherNames(QString("%1").arg(record.value("otherNames").toString()));
    dataForm.setGender(QString("%1").arg(record.value("gender").toString()));
    QString cl = QString("%1").arg(record.value("class").toString());
    if(!cl.isEmpty() && (cl.startsWith("Nursery", Qt::CaseInsensitive)
                         || cl.startsWith("Day", Qt::CaseInsensitive)
                         || cl.endsWith("nursery", Qt::CaseInsensitive))){
        dataForm.setSchool(SCHOOL::NURSERY);
    }else if (!cl.isEmpty() && (cl.startsWith("Primary", Qt::CaseInsensitive))) {
        dataForm.setSchool(SCHOOL::PRYMARY);
    }else if (!cl.isEmpty() && ((cl.startsWith("JS", Qt::CaseInsensitive)) ||
                                cl.startsWith("SS", Qt::CaseInsensitive))) {
        dataForm.setSchool(SCHOOL::SECONDARY);
    }
    dataForm.setClass(QString("%1").arg(record.value("class").toString()));

    QSqlQuery query1 = databaseCreator().fetchData("stuDetailDataT", regNumWhere);
    QSqlQuery query2 = databaseCreator().fetchData("permAddressT", regNumWhere);
    QSqlQuery query3 = databaseCreator().fetchData("resAddressT", regNumWhere);

    while(query1.next()){
        dataForm.setDateOfBirth(query1.value("dateOfBirth").toString());
        dataForm.setStuHeight(query1.value("height").toDouble());
        dataForm.setWeigth(query1.value("weight").toDouble());
        dataForm.setReligion(query1.value("religion").toString());
        dataForm.setBloodGroup(query1.value("bloodGroup").toString());
        dataForm.setGenotype(query1.value("genotype").toString());
        dataForm.setLabelImage(query1.value("photoPath").toString());
    }

    while(query2.next()){
        dataForm.setPermAddress(query2.value("permHomeAddress").toString());
        dataForm.setPermState(query2.value("stateOfOrigin").toString());
        dataForm.setPermLGA(query2.value("lgaOfOrigin").toString());
        dataForm.setNationality(query2.value("nationality").toString());
    }

    while(query3.next()){
        dataForm.setResAddress(query3.value("address").toString());
        dataForm.setCity(query3.value("resCity").toString());
        dataForm.setResState(query3.value("resState").toString());
        dataForm.setResLGA(query3.value("resLga").toString());
        dataForm.setPhoneNumber(query3.value("phoneNum").toString());
        dataForm.setEmail(query3.value("email").toString());
    }

    int ret = dataForm.exec();

    if(1==ret){
        QMap<QString, QVariant> mapData;
        QMap<QString, QVariant> personalData;
        QMap<QString, QVariant> permHomeData;
        QMap<QString, QVariant> resHomeData;


        mapData["regNum"] = dataForm.regNumber();
        mapData["surname"] = dataForm.surname();
        mapData["firstName"] = dataForm.firstName();
        mapData["otherNames"] = dataForm.otherNames();
        mapData["gender"] = dataForm.gender();
        mapData["class"] = dataForm.level();

        personalData["regNum"] = dataForm.regNumber();
        personalData["dateOfBirth"] = dataForm.dateOfBirth();
        personalData["height"] = dataForm.stuHeight();
        personalData["weight"] = dataForm.weigth();
        personalData["religion"] = dataForm.religion();
        personalData["bloodGroup"] = dataForm.bloodGroup();
        personalData["genotype"] = dataForm.genotype();
        personalData["photoPath"] = dataForm.savedImagePath();

        permHomeData["regNum"] = dataForm.regNumber();
        permHomeData["permHomeAddress"] = dataForm.permAddress();
        permHomeData["lgaOfOrigin"] = dataForm.permLGA();
        permHomeData["stateOfOrigin"] = dataForm.permState();
        permHomeData["nationality"] = dataForm.nationality();

        resHomeData["regNum"] = dataForm.regNumber();
        resHomeData["address"] = dataForm.resAddress();
        resHomeData["resCity"] = dataForm.city();
        resHomeData["resLga"] = dataForm.resLGA();
        resHomeData["resState"] = dataForm.resState();
        resHomeData["phoneNum"] = dataForm.phoneNumber();
        resHomeData["email"] = dataForm.email();

        QMap<GwDatabaseCreator::DATATYPES, QVariant> where;
        where[GwDatabaseCreator::DATATYPES::LONG] = QVariant(regNum);
        databaseCreator().db().transaction();
        bool h = databaseCreator().updateData("registrationT", mapData, "regNum");
        bool i = databaseCreator().updateData("stuDetailDataT", personalData, "regNum");
        bool j = databaseCreator().updateData("permAddressT", permHomeData, "regNum");
        bool k = databaseCreator().updateData("resAddressT", resHomeData, "regNum");
        if(h && i && j && k && databaseCreator().db().commit()){
            refresh("registrationT");
            QMessageBox::information(this, QString("Success"), QString("Data Updated"));
        }else {
            databaseCreator().db().rollback();
            QMessageBox::information(this, QString("failed"), QString("%1").arg(databaseCreator().db().lastError().text()));
        }
    }
}

void GwTabWindow::studentBriefData(const QString &title, const QString &where)
{
    auto *model = new QSqlQueryModel(this);
    auto *view = new QTableView;
    view->verticalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
    view->horizontalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
    QString queryStr = QString(
                "SELECT r.regNum, r.surname, d.dateOfBirth, d.height, d.weight, d.bloodGroup, d.genotype, a.address, a.resCity, a.phoneNum, a.email, r.firstPay "
                "FROM registrationT AS r "
                "INNER JOIN stuDetailDataT AS d ON r.regNum = d.regNum "
                "INNER JOIN resAddressT AS a ON r.regNum = a.regNum ");
    if(!where.isEmpty()){
        queryStr += QString("WHERE r.regNum = %1").arg(where);
    }

    QSqlQuery query(queryStr, databaseCreator().db());
    model->setQuery(query);
    setHeaders(model);
    if(model->lastError().isValid()){
        QMessageBox::critical(this, QString("failed"), QString("%1").arg(model->lastError().text()));
    }else {
        view->setModel(model);
        widgetTab_->addTab(view, tr("%1").arg(title));
    }
}

void GwTabWindow::setHeaders(QSqlQueryModel *model)
{
    QStringList labels;
    labels << "Reg. Number" << "Surname" << "Date Of Birth" << "Height" << "Weight"
           << "Blood Group" << "Blood Genotype" << "Address" << "City" << "Phone Number" << "Email" << "First Payment";
    setHeaderLables(labels, Qt::Horizontal, model);
}

void GwTabWindow::setPHeaders(QSqlQueryModel *model)
{
    QStringList labels;
    labels << "Parent Name" << "Student" << "Relationship" << "Phone Number" << "Email";
    setHeaderLables(labels, Qt::Horizontal, model);
}

void GwTabWindow::setHeaderLables(const QStringList &labels, Qt::Orientation orientation, QSqlQueryModel *model)
{
    int counter = 0;
    for(auto label : labels){
        model->setHeaderData(counter++, orientation, QString(tr("%1")).arg(label));
    }
}

void GwTabWindow::parentData(const QString &title, const QString &where)
{
    auto *model = new QSqlQueryModel(this);
    auto *view = new QTableView;
    view->horizontalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
    view->verticalScrollBar()->setStyleSheet("background-color: rgb(180, 180, 180);\ncolor: rgb(0, 0, 0)");
    QString queryStr = QString(
                "SELECT p.firstName || ' ' || p.lastName AS ParentName, r.firstName || ' ' || r.surname AS Student, p.relationship, pa.homePhoneNumber, pa.homeEmail "
                "FROM studentParentT AS sp "
                "INNER JOIN registrationT AS r ON r.regNum = sp.regNum "
                "INNER JOIN parentsT AS p ON p.parentID = sp.parentID "
                "INNER JOIN parentHomeAddressT AS pa ON pa.parentID = p.parentID ");
    if(!where.isEmpty()){
        queryStr += QString("WHERE sp.regNum = %1").arg(where);
    }

    QSqlQuery query(queryStr, databaseCreator().db());
    model->setQuery(query);
    setPHeaders(model);
    if(model->lastError().isValid()){
        QMessageBox::critical(this, QString("failed"), QString("%1").arg(model->lastError().text()));
    }else {
        view->setModel(model);
        widgetTab_->addTab(view, tr("%1").arg(title));
    }
}

QSplitter *GwTabWindow::splitter() const
{
    return splitter_;
}


