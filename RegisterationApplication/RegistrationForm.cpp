#include "RegistrationForm.h"
#include "ui_RegistrationForm.h"
#include "GwGenaralFunctions.h"


#include <QDebug>
#include <QMessageBox>

RegistrationForm::RegistrationForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);

    setFixedSize(410, 415);

    ui->regNumEditor->setValidator(validators(stringRegExp("\\d{4}/ES\\d{3,4}/[NSP]S"), this));
    ui->surnameEditor->setValidator(validators(stringRegExp("^[a-z]+$"), this));
    ui->firstNameEditor->setValidator(validators(stringRegExp("^[a-z]+$"), this));
    ui->otherNamesEditor->setValidator(validators(stringRegExp("^[a-z ]+$"), this));
    connect(ui->schoolComboBox, &QComboBox::currentTextChanged, this, &RegistrationForm::onTextChange);
    connect(ui->clearButton, &QPushButton::clicked, this, &RegistrationForm::onClickClearButton);
    connect(ui->submitButton, &QPushButton::clicked, this, &RegistrationForm::onClickSubmitButton);
    connect(ui->cancelButton, &QPushButton::clicked, this, &RegistrationForm::onClickCancelButton);

}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

QString RegistrationForm::regNumber() const
{
    return matchString(ui->regNumEditor->text(), stringRegExp("\\d{4}/ES\\d{3,4}/[NSP]S")).toUpper();
}

QString RegistrationForm::surname() const
{
    return capitalizedEachWord(ui->surnameEditor->text());
}

QString RegistrationForm::firstName() const
{
    return capitalizedEachWord(ui->firstNameEditor->text());
}

QString RegistrationForm::otherNames() const
{
    return capitalizedEachWord(ui->otherNamesEditor->text());
}

QString RegistrationForm::gender() const
{
    if(ui->maleRadioButton->isChecked()){
        return QString("Male");
    }

    return QString("Female");
}

QString RegistrationForm::level() const
{
    return ui->classComboBox->currentText();
}

QString RegistrationForm::school() const
{
    return ui->schoolComboBox->currentText();
}

double RegistrationForm::firstPaymentAmount()
{
    return ui->firstPaymentDoubleSpinBox->value();
}

void RegistrationForm::setRegNumber(const QString &text)
{
    ui->regNumEditor->setText(text);
}

void RegistrationForm::setSurname(const QString &text)
{
    ui->surnameEditor->setText(text);
}

void RegistrationForm::setFirstName(const QString &text)
{
    ui->firstNameEditor->setText(text);
}

void RegistrationForm::setOtherNames(const QString &text)
{
    ui->otherNamesEditor->setText(text);
}

void RegistrationForm::setGender(const QString &text)
{
    if("Female" == text)
        ui->femaleRadioButton->setChecked(true);
    else
        ui->maleRadioButton->setChecked(true);
}

void RegistrationForm::setLevel(const QString &level)
{
    QStringList nurseryClasses, primaryClasses, secondaryClasses;
    nurseryClasses << "Select Your Class" << "Day care" << "Pre-nursery" << "Nursery 1" << "Nursery 2" << "Nursery 3";
    primaryClasses << "Select Your Class" << "Primary 1" << "Primary 2" << "Primary 3" << "Primary 4" << "Primary 5" << "Primary 6";
    secondaryClasses << "Select Your Class" << "JSS 1" << "JSS 2" << "JSS 3" << "SS 1" << "SS 2" << "SS 3";

    if(ui->schoolComboBox->currentText() == "Nursery School"){
        ui->classComboBox->clear();
        ui->classComboBox->addItems(nurseryClasses);
        ui->classComboBox->setCurrentText(level);
    }else if (ui->schoolComboBox->currentText() == "Primary School") {
        ui->classComboBox->clear();
        ui->classComboBox->addItems(primaryClasses);
        ui->classComboBox->setCurrentText(level);
    }else if (ui->schoolComboBox->currentText() == "Secondary School"){
        ui->classComboBox->clear();
        ui->classComboBox->addItems(secondaryClasses);
        ui->classComboBox->setCurrentText(level);
    }else {
        ui->classComboBox->clear();
        ui->classComboBox->addItem("Select Your Class");
    }
}

void RegistrationForm::setSchool(SCHOOL sch)
{
    if(sch == SCHOOL::NURSERY){
        ui->schoolComboBox->setCurrentIndex(1);
    }
    else if(sch == SCHOOL::PRYMARY){
        ui->schoolComboBox->setCurrentIndex(2);
    }else if(sch == SCHOOL::SECONDARY){
        ui->schoolComboBox->setCurrentIndex(3);
    }else {
        ui->schoolComboBox->setCurrentIndex(0);
    }
}

void RegistrationForm::setFirstPaymentAmount(double amount)
{
    ui->firstPaymentDoubleSpinBox->setValue(amount);
}

GwStudent RegistrationForm::student() const
{
    return student_;
}

void RegistrationForm::setStudent(const GwStudent &student)
{
    student_ = student;
}

void RegistrationForm::onTextChange(const QString &school)
{
    QStringList nurseryClasses, primaryClasses, secondaryClasses;
    nurseryClasses << "Select Your Class" << "Day care" << "Pre-nursery" << "Nursery 1" << "Nursery 2" << "Nursery 3";
    primaryClasses << "Select Your Class" << "Primary 1" << "Primary 2" << "Primary 3" << "Primary 4" << "Primary 5" << "Primary 6";
    secondaryClasses << "Select Your Class" << "JSS 1" << "JSS 2" << "JSS 3" << "SS 1" << "SS 2" << "SS 3";

    if(school.toLower() == "nursery school"){
        ui->classComboBox->clear();
        ui->classComboBox->addItems(nurseryClasses);
    }else if (school.toLower() == "primary school") {
        ui->classComboBox->clear();
        ui->classComboBox->addItems(primaryClasses);
    }else if (school.toLower() == "secondary school"){
        ui->classComboBox->clear();
        ui->classComboBox->addItems(secondaryClasses);
    }else {
        ui->classComboBox->clear();
        ui->classComboBox->addItem("Select Your Class");
    }
}

void RegistrationForm::onClickClearButton()
{
    ui->regNumEditor->clear();
    ui->surnameEditor->clear();
    ui->firstNameEditor->clear();
    ui->otherNamesEditor->clear();
    ui->maleRadioButton->setChecked(true);
    ui->femaleRadioButton->setChecked(false);

    QStringList sch;
    sch << "Select Your School" << "Nursery School" << "Primary School" << "Secondary School";
    ui->schoolComboBox->clear();
    ui->schoolComboBox->addItems(sch);

    ui->classComboBox->clear();
    ui->classComboBox->addItem("Select Your Class");
    ui->firstPaymentDoubleSpinBox->setValue(0.0);
}

void RegistrationForm::onClickSubmitButton()
{
    if(!isAllFieldFilled()){
        QMessageBox::critical(nullptr, QString(tr("All field needed")),
                              QString(tr("Please fill all the fields on the form\nThe"
                                         "data are essential")));
        return;
    }

    if(!isMatched(ui->regNumEditor->text(), stringRegExp("\\d{4}/ES\\d{3,4}/[NSP]S"))){
        QMessageBox::critical(nullptr, QString(tr("Incorrect reg number")),
                              QString(tr("%1 is not a correct registration number\n"
                                         "This reg number should be in this format 2017/ES0003/NS")).arg(ui->regNumEditor->text()));
        return;
    }

    if(!isMatched(ui->surnameEditor->text(), stringRegExp("^[a-z]{3,}$"))){
        QMessageBox::critical(nullptr, QString(tr("Incorrect name")),
                              QString(tr("%1 is an abbreviation\n"
                                         "The full name is needed")).arg(ui->surnameEditor->text()));
        return;
    }

    if(!isMatched(ui->firstNameEditor->text(), stringRegExp("^[a-z]{3,}$"))){
        QMessageBox::critical(nullptr, QString(tr("Incorrect name")),
                              QString(tr("%1 is an abbreviation\n"
                                         "The full name is needed")).arg(ui->firstNameEditor->text()));
        return;
    }

    setStudent();
    accept();
}

void RegistrationForm::onClickCancelButton()
{
    reject();
}

void RegistrationForm::setStudent()
{
    student_.setRegNum(regNumber());
    student_.setSurname(surname());
    student_.setFirstName(firstName());
    student_.setOtherNames(otherNames());
    student_.setGender(gender());
    student_.setLevel(level());
}

bool RegistrationForm::isAllFieldFilled()
{
    if(
        ui->regNumEditor->text().isEmpty() || ui->surnameEditor->text().isEmpty() ||
        ui->firstNameEditor->text().isEmpty() || (ui->firstPaymentDoubleSpinBox->value() == 0.0)
        || (!ui->maleRadioButton->isChecked() && !ui->femaleRadioButton->isChecked())||
        ui->classComboBox->currentText() == "Select Your Class" ||
        ui->schoolComboBox->currentText() == "Select Your School"
      ){
        return false;
    }
    return true;
}
