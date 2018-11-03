#include "GwParentInfoForm.h"
#include "ui_GwParentInfoForm.h"
#include "GwGenaralFunctions.h"
#include <QMessageBox>

GwParentInfoForm::GwParentInfoForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GwParentInfoForm)
{
    ui->setupUi(this);

    ui->pHStateComboBox->addItems(statesInNigeria());
    ui->pOStateComboBox->addItems(statesInNigeria());

    ui->pLastNameEditor->setValidator(validators(stringRegExp("^[a-z]+$"), this));
    ui->pFirstNameEditor->setValidator(validators(stringRegExp("^[a-z]+$"), this));
    ui->pHPhoneNumEditor->setValidator(validators(stringRegExp("^\\+?\\(\\d{2,4}\\)\\s?\\d{3,}\\s?\\d{3,}?|\\+?\\d{2,}\\s?\\d{3,}?\\s?\\d{2,}?$"), this));
    ui->pOPhoneNumEditor->setValidator(validators(stringRegExp("^\\+?\\(\\d{2,4}\\)\\s?\\d{3,}\\s?\\d{3,}?|\\+?\\d{2,}\\s?\\d{3,}?\\s?\\d{2,}?$"), this));
    ui->pHEmailEditor->setValidator(validators(stringRegExp("^[a-z0-9_#$!`~*%+-]{5,35}@[a-z0-9]+[.a-z0-9]+$"), this));
    ui->pOEmailEditor->setValidator(validators(stringRegExp("^[a-z0-9_#$!`~*%+-]{5,35}@[a-z0-9]+[.a-z0-9]+$"), this));

    connect(ui->pHStateComboBox, &QComboBox::currentTextChanged, this, &GwParentInfoForm::onHomeStateComboChange);
    connect(ui->pOStateComboBox, &QComboBox::currentTextChanged, this, &GwParentInfoForm::onOfficeStateComboChange);

    connect(ui->submitButton, &QPushButton::clicked, this, &GwParentInfoForm::onClickSubmitButton);
    connect(ui->cancelButton, &QPushButton::clicked, this, &GwParentInfoForm::onClickCancelButton);
}

GwParentInfoForm::~GwParentInfoForm()
{
    delete ui;
}

void GwParentInfoForm::setPFirstName(const QString &text)
{
    ui->pFirstNameEditor->setText(text);
}

void GwParentInfoForm::setPLastName(const QString &text)
{
    ui->pLastNameEditor->setText(text);
}

void GwParentInfoForm::setPRelationship(const QString &text)
{
    ui->pRelationshipComboBox->setCurrentText(text);
}

void GwParentInfoForm::setPOccupation(const QString &text)
{
    ui->pOcupationEditor->setText(text);
}

void GwParentInfoForm::setPHAddress(const QString &text)
{
    ui->pHAddressEditor->setText(text);
}

void GwParentInfoForm::setPHCity(const QString &text)
{
    ui->pHCityEditor->setText(text);
}

void GwParentInfoForm::setPHPhoneNumber(const QString &text)
{
    ui->pHPhoneNumEditor->setText(text);
}

void GwParentInfoForm::setPHLga(const QString &text)
{
    ui->pHLgaComboBox->setCurrentText(text);
}

void GwParentInfoForm::setPHState(const QString &text)
{
    ui->pHStateComboBox->setCurrentText(text);
}

void GwParentInfoForm::setPHEmail(const QString &text)
{
    ui->pHEmailEditor->setText(text);
}

void GwParentInfoForm::setPOAddress(const QString &text)
{
    ui->pOAddressEditor->setText(text);
}

void GwParentInfoForm::setPOCity(const QString &text)
{
    ui->pOCityEditor->setText(text);
}

void GwParentInfoForm::setPOPhoneNumber(const QString &text)
{
    ui->pOPhoneNumEditor->setText(text);
}

void GwParentInfoForm::setPOLga(const QString &text)
{
    ui->pOLgaComboBox->setCurrentText(text);
}

void GwParentInfoForm::setPOState(const QString &text)
{
    ui->pOStateComboBox->setCurrentText(text);
}

void GwParentInfoForm::setPOEmail(const QString &text)
{
    ui->pOEmailEditor->setText(text);
}

QString GwParentInfoForm::pFirstName()
{
    if(isMatched(ui->pFirstNameEditor->text(), stringRegExp("^[a-z]{3,}$")))
        return capitalizedEachWord(ui->pFirstNameEditor->text());
    return QString();
}

QString GwParentInfoForm::pLastName()
{
    if(isMatched(ui->pFirstNameEditor->text(), stringRegExp("^[a-z]{3,}$")))
        return capitalizedEachWord(ui->pLastNameEditor->text());
    return QString();
}

QString GwParentInfoForm::pRelationship()
{
    if(ui->pRelationshipComboBox->currentText() == "Parent Relationship")
        return QString();
    return ui->pRelationshipComboBox->currentText();
}

QString GwParentInfoForm::pOccupation()
{
    return capitalizedEachWord(ui->pOcupationEditor->text());
}

QString GwParentInfoForm::pHAddress()
{
    return capitalizedEachWord(ui->pHAddressEditor->text());
}

QString GwParentInfoForm::pHCity()
{
    return capitalizedEachWord(ui->pHCityEditor->text());
}

QString GwParentInfoForm::pHPhoneNumber()
{
    if(isMatched(ui->pHPhoneNumEditor->text(), stringRegExp("^\\+?\\(\\d{2,4}\\)\\s?\\d{3,}\\s?\\d{3,}?|\\+?\\d{2,}\\s?\\d{3,}?\\s?\\d{2,}?$")))
        return ui->pHPhoneNumEditor->text();
    return "";
}

QString GwParentInfoForm::pHLga()
{
    return ui->pHLgaComboBox->currentText();
}

QString GwParentInfoForm::pHState()
{
    return ui->pHStateComboBox->currentText();
}

QString GwParentInfoForm::pHEmail()
{
    if(isMatched(ui->pHEmailEditor->text(), stringRegExp("^[a-z0-9_#$!`~*%+-]{5,35}@[a-z0-9]+[.a-z0-9]+$")))
        return ui->pHEmailEditor->text();
    return "";
}

QString GwParentInfoForm::pOAddress()
{
    return ui->pOAddressEditor->text();
}

QString GwParentInfoForm::pOCity()
{
    return capitalizedEachWord(ui->pOCityEditor->text());
}

QString GwParentInfoForm::pOPhoneNumber()
{
    if(isMatched(ui->pOPhoneNumEditor->text(), stringRegExp("^\\+?\\(\\d{2,4}\\)\\s?\\d{3,}\\s?\\d{3,}?|\\+?\\d{2,}\\s?\\d{3,}?\\s?\\d{2,}?$")))
        return ui->pOPhoneNumEditor->text();
    return "";
}

QString GwParentInfoForm::pOLga()
{
    return ui->pOLgaComboBox->currentText();
}

QString GwParentInfoForm::pOState()
{
    return ui->pOStateComboBox->currentText();
}

QString GwParentInfoForm::pOEmail()
{
    if(isMatched(ui->pOEmailEditor->text(), stringRegExp("^[a-z0-9_#$!`~*%+-]{5,35}@[a-z0-9]+[.a-z0-9]+$")))
        return ui->pOEmailEditor->text();
    return "";
}

bool GwParentInfoForm::isEmpty()
{
    if(pLastName().isEmpty() || pFirstName().isEmpty() || pHPhoneNumber().isEmpty())
        return true;
    return false;
}

bool GwParentInfoForm::hasEmpty()
{
    if(
            ui->regNumEditor->text().isEmpty() || ui->pLastNameEditor->text().isEmpty()
            || ui->pFirstNameEditor->text().isEmpty() || ui->pRelationshipComboBox->currentText() == "Parent Relationship"
            || ui->pHAddressEditor->text().isEmpty() || ui->pHPhoneNumEditor->text().isEmpty() || ui->pHCityEditor->text().isEmpty()
      ) return true;
    return false;
}

void GwParentInfoForm::setRegNumber(const QString &stuRegNumber, bool readOnly)
{
    ui->regNumEditor->setText(stuRegNumber);
    ui->regNumEditor->setReadOnly(readOnly);
}

void GwParentInfoForm::setName(const QString &name, bool readOnly)
{
    ui->nameEditor->setText(name);
    ui->nameEditor->setReadOnly(readOnly);
}

GwParent GwParentInfoForm::getParentData()
{
    parent_.setNames(pLastName(), pFirstName());
    parent_.setRelationship(pRelationship());
    parent_.setOccupation(pOccupation());

    GwAddress homeAddress;
    GwAddress officeAddress;

    officeAddress.setAddress(pOAddress());
    officeAddress.setCity(pOCity());
    officeAddress.setState(pOState());
    officeAddress.setLga(pOLga());
    officeAddress.setPhoneNumber(pOPhoneNumber());
    officeAddress.setEmail(pOEmail());

    homeAddress.setAddress(pHAddress());
    homeAddress.setCity(pHCity());
    homeAddress.setState(pHState());
    homeAddress.setLga(pHLga());
    homeAddress.setPhoneNumber(pHPhoneNumber());
    homeAddress.setEmail(pHEmail());

    parent_.setHomeAddress(homeAddress);
    parent_.setOfficeAddress(officeAddress);

    return parent_;
}

void GwParentInfoForm::deseableControls()
{
    ui->pFirstNameEditor->setReadOnly(true);
    ui->pLastNameEditor->setReadOnly(true);
    ui->pRelationshipComboBox->setEnabled(false);
    ui->pOcupationEditor->setReadOnly(true);

    ui->pHAddressEditor->setReadOnly(true);
    ui->pHCityEditor->setReadOnly(true);
    ui->pHEmailEditor->setReadOnly(true);
    ui->pHPhoneNumEditor->setReadOnly(true);
    ui->pHStateComboBox->setEnabled(false);
    ui->pHLgaComboBox->setEnabled(false);

    ui->pOAddressEditor->setReadOnly(true);
    ui->pOCityEditor->setReadOnly(true);
    ui->pOEmailEditor->setReadOnly(true);
    ui->pOPhoneNumEditor->setReadOnly(true);
    ui->pOStateComboBox->setEnabled(false);
    ui->pOLgaComboBox->setEnabled(false);

    ui->submitButton->setVisible(false);
    ui->cancelButton->setVisible(false);
    if(ui->nameEditor->text().isEmpty()){
        ui->label->setText(tr("<html><head/><body><p align=\"center\"><span style=\" font-size:16pt; font-weight:600; text-decoration: underline; color:#000000;\">There is No Parent Data</span></p></body></html>"));
    }else{
        ui->label->setText(tr("<html><head/><body><p align=\"center\"><span style=\" font-size:16pt; font-weight:600; text-decoration: underline; color:#000000;\">%1's Parent Data</span></p></body></html>").arg(ui->nameEditor->text()));
    }

    ui->label_5->setVisible(false);
    ui->regNumEditor->setVisible(false);
    ui->label_6->setVisible(false);
    ui->nameEditor->setVisible(false);
    setStyleSheet("background-color: rgb(255, 255, 255);\nfont: 75 10pt \"MS Shell Dlg 2\";\ncolor: rgb(0, 0, 0);");
    ui->groupBox->setStyleSheet("background-color: rgb(255, 255, 255);");
    ui->groupBox_2->setStyleSheet("background-color: rgb(255, 255, 255);");
    ui->groupBox_3->setStyleSheet("background-color: rgb(255, 255, 255);");
}

void GwParentInfoForm::onClickSubmitButton()
{
    if(hasEmpty()){
        QMessageBox::critical(nullptr, QString(tr("Incomplete form")), QString(tr("This form "
             "is not completely filled.\nThe following infomations are highly needed:"
              "\nComplete Name, Reletionship with the student, Home Address and Phone Number")));
        return;
    }

    if(isEmpty()){
        QMessageBox::critical(nullptr, QString(tr("Incorrect data")), QString(tr("There some "
             "that has incorrect data format.\nPlease check the following fields:"
              "\nThe Names and the Phone Number")));
        return;
    }
    accept();
}

void GwParentInfoForm::onClickCancelButton()
{
    reject();
}

void GwParentInfoForm::onHomeStateComboChange(const QString &state)
{
    onComboBoxTextChange(ui->pHLgaComboBox, state);
}

void GwParentInfoForm::onOfficeStateComboChange(const QString &state)
{
    onComboBoxTextChange(ui->pOLgaComboBox, state);
}
