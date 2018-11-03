#include "GwStudentFullDataForm.h"
#include "ui_GwStudentFullDataForm.h"
#include "GwGenaralFunctions.h"
#include <QLabel>
#include <QMessageBox>
#include <QImageReader>
#include <QFileDialog>
#include <QDebug>

GwStudentFullDataForm::GwStudentFullDataForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GwStudentFullDataForm)
{
    ui->setupUi(this);

    ui->stateComboBox->addItems(statesInNigeria());
    ui->permStateComboBox->addItems(statesInNigeria());
    ui->emailEditor->setValidator(validators(stringRegExp("^[a-z0-9_#$!`~*%+-]{5,35}@[a-z0-9]+[.a-z0-9]+$"), this));
    ui->phoneNumberEditor->setValidator(validators(stringRegExp("^\\+?\\(\\d{2,4}\\)\\s?\\d{3,}\\s?\\d{3,}?|\\+?\\d{2,}\\s?\\d{3,}?\\s?\\d{2,}?$"), this));
    ui->dateOfBirthEditor->setDate(QDate::currentDate());

    connect(ui->stateComboBox, &QComboBox::currentTextChanged, this, &GwStudentFullDataForm::onStateComboChange);
    connect(ui->permStateComboBox, &QComboBox::currentTextChanged, this, &GwStudentFullDataForm::onPermStateComboChange);
    connect(ui->submitButton, &QPushButton::clicked, this, &GwStudentFullDataForm::onClickSubmitButton);
    connect(ui->cancelButton, &QPushButton::clicked, this, &GwStudentFullDataForm::onClickCancelButton);
    connect(ui->imageButton, &QPushButton::clicked, this, &GwStudentFullDataForm::onClickImageButton);
    connect(ui->schoolComboBox, &QComboBox::currentTextChanged, this, &GwStudentFullDataForm::onTextChange);
}

GwStudentFullDataForm::~GwStudentFullDataForm()
{
    delete ui;
}

void GwStudentFullDataForm::setRegNum(const QString &regNum)
{
    ui->regNumEditor->setText(regNum);
}

void GwStudentFullDataForm::setRegNumEditorReadOnly()
{
    ui->regNumEditor->setReadOnly(true);
}

void GwStudentFullDataForm::setSurname(const QString &name)
{
    ui->surnameEditor->setText(name);
}

void GwStudentFullDataForm::setSurnameEditorReadOnly()
{
    ui->surnameEditor->setReadOnly(true);
}

void GwStudentFullDataForm::setFirstName(const QString &name)
{
    ui->firstNameEditor->setText(name);
}

void GwStudentFullDataForm::setFirstNameReadOnly()
{
    ui->firstNameEditor->setReadOnly(true);
}

void GwStudentFullDataForm::setOtherNames(const QString &name)
{
    ui->otherNamesEditor->setText(name);
}

void GwStudentFullDataForm::setOtherNamesReadOnly()
{
    ui->otherNamesEditor->setReadOnly(true);
}

void GwStudentFullDataForm::setGender(const QString &gender)
{
    if(gender.toLower() == "female"){
        ui->femaleRadioButton->setChecked(true);
    }else {
        ui->maleRadioButton->setChecked(true);
    }
}

void GwStudentFullDataForm::setSchool(SCHOOL sch)
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

void GwStudentFullDataForm::setClass(const QString &level)
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

QString GwStudentFullDataForm::regNumber() const
{
    return ui->regNumEditor->text();
}

QString GwStudentFullDataForm::surname() const
{
    return ui->surnameEditor->text();
}

QString GwStudentFullDataForm::firstName() const
{
    return ui->firstNameEditor->text();
}

QString GwStudentFullDataForm::otherNames() const
{
    return ui->otherNamesEditor->text();
}

QString GwStudentFullDataForm::level() const
{
    return ui->classComboBox->currentText();
}

QString GwStudentFullDataForm::gender() const
{
    if(ui->maleRadioButton->isChecked())
        return "Male";
    return "Female";
}

QString GwStudentFullDataForm::dateOfBirth() const
{
    return ui->dateOfBirthEditor->text();
}

double GwStudentFullDataForm::stuHeight() const
{
    return ui->heigthSpinBox->value();
}

double GwStudentFullDataForm::weigth() const
{
    return ui->weigthSpinBox->value();
}

QString GwStudentFullDataForm::religion() const
{
    return ui->religionComboBox->currentText();
}

QString GwStudentFullDataForm::bloodGroup() const
{
    return ui->bloodGroupComboBox->currentText();
}

QString GwStudentFullDataForm::genotype() const
{
    return ui->genotyprComboBox->currentText();
}

void GwStudentFullDataForm::setDateOfBirth(const QString &text)
{
    QStringList l = text.split('/');
    QString m = l.at(0);
    QString d = l.at(1);
    QString y = l.at(2);
    ui->dateOfBirthEditor->setDate(QDate(y.toInt(), m.toInt(), d.toInt()));
}

void GwStudentFullDataForm::setStuHeight(double t)
{
    ui->heigthSpinBox->setValue(t);
}

void GwStudentFullDataForm::setWeigth(double t)
{
    ui->weigthSpinBox->setValue(t);
}

void GwStudentFullDataForm::setReligion(const QString &text)
{
    ui->religionComboBox->setCurrentText(text);
}

void GwStudentFullDataForm::setBloodGroup(const QString &text)
{
    ui->bloodGroupComboBox->setCurrentText(text);
}

void GwStudentFullDataForm::setGenotype(const QString &text)
{
    ui->genotyprComboBox->setCurrentText(text);
}

void GwStudentFullDataForm::setResAddress(const QString &text)
{
    ui->resAddressEditor->setText(text);
}

void GwStudentFullDataForm::setCity(const QString &text)
{
    ui->resCityEditor->setText(text);
}

void GwStudentFullDataForm::setResState(const QString &text)
{
    ui->stateComboBox->setCurrentText(text);
}

void GwStudentFullDataForm::setResLGA(const QString &text)
{
    ui->lgaComboBox->setCurrentText(text);
}

void GwStudentFullDataForm::setPermAddress(const QString &text)
{
    ui->permHomeAddressEditor->setText(text);
}

void GwStudentFullDataForm::setPermState(const QString &text)
{
    ui->permStateComboBox->setCurrentText(text);
}

void GwStudentFullDataForm::setPermLGA(const QString &text)
{
    ui->permLgaComboBox->setCurrentText(text);
}

void GwStudentFullDataForm::setNationality(const QString &text)
{
    ui->nationalityEditor->setText(text);
}

void GwStudentFullDataForm::setPhoneNumber(const QString &text)
{
    ui->phoneNumberEditor->setText(text);
}

void GwStudentFullDataForm::setEmail(const QString &text)
{
    ui->emailEditor->setText(text);
}

QString GwStudentFullDataForm::resAddress() const
{
    return ui->resAddressEditor->text();
}

QString GwStudentFullDataForm::city() const
{
    return ui->resCityEditor->text();
}

QString GwStudentFullDataForm::resState() const
{
    return ui->stateComboBox->currentText();
}

QString GwStudentFullDataForm::resLGA() const
{
    return ui->lgaComboBox->currentText();
}

QString GwStudentFullDataForm::permAddress() const
{
    return ui->permHomeAddressEditor->text();
}

QString GwStudentFullDataForm::permState() const
{
    return ui->permStateComboBox->currentText();
}

QString GwStudentFullDataForm::permLGA() const
{
    return ui->permLgaComboBox->currentText();
}

QString GwStudentFullDataForm::nationality() const
{
    return ui->nationalityEditor->text();
}

QString GwStudentFullDataForm::phoneNumber() const
{
    if(isMatched(ui->phoneNumberEditor->text(), stringRegExp("^\\+?\\(\\d{2,4}\\)\\s?\\d{3,}\\s?\\d{3,}?|\\+?\\d{2,}\\s?\\d{3,}?\\s?\\d{2,}?$"))){
        return ui->phoneNumberEditor->text();
    }
    return "";
}

QString GwStudentFullDataForm::email() const
{
    if(isMatched(ui->emailEditor->text(), stringRegExp("^[a-z0-9_#$!`~*%+-]{5,35}@[a-z0-9]+[.a-z0-9]+$")))
        return ui->emailEditor->text();
    return "";
}

QString GwStudentFullDataForm::savedImagePath()
{
    return savedImagePath_;
}

QLabel *GwStudentFullDataForm::imageLabel()
{
    return ui->imageLabel;
}

QLineEdit *GwStudentFullDataForm::regNumEditor()
{
    return ui->regNumEditor;
}

QLineEdit *GwStudentFullDataForm::surnameEditor()
{
    return ui->surnameEditor;
}

void GwStudentFullDataForm::deseableControls()
{
    ui->schoolComboBox->setEnabled(false);
    ui->classComboBox->setEnabled(false);
    ui->groupBox_6->setEnabled(false);
}

void GwStudentFullDataForm::deseableAllControls()
{
    ui->regNumEditor->setReadOnly(true);
    ui->surnameEditor->setReadOnly(true);
    ui->firstNameEditor->setReadOnly(true);
    ui->otherNamesEditor->setReadOnly(true);
    ui->classComboBox->setEnabled(false);
    ui->dateOfBirthEditor->setEnabled(false);
    ui->maleRadioButton->setEnabled(false);
    ui->femaleRadioButton->setEnabled(false);

    if(ui->maleRadioButton->isChecked()){
        ui->femaleRadioButton->setVisible(false);
    }else{
        ui->maleRadioButton->setVisible(false);
    }
    ui->weigthSpinBox->setEnabled(false);
    ui->heigthSpinBox->setEnabled(false);
    ui->schoolComboBox->setEnabled(false);
    ui->groupBox_6->setTitle("Gender");
    ui->bloodGroupComboBox->setEnabled(false);
    ui->genotyprComboBox->setEnabled(false);

    ui->permHomeAddressEditor->setReadOnly(true);
    ui->permStateComboBox->setEnabled(false);
    ui->permLgaComboBox->setEnabled(false);
    ui->phoneNumberEditor->setReadOnly(true);
    ui->emailEditor->setEnabled(false);

    ui->religionComboBox->setEnabled(false);
    ui->nationalityEditor->setReadOnly(true);
    ui->resAddressEditor->setReadOnly(true);
    ui->resCityEditor->setReadOnly(true);
    ui->stateComboBox->setEnabled(false);
    ui->lgaComboBox->setEnabled(false);

    ui->label->setText(tr("<html><head/><body><p align=\"center\"><span style=\" font-size:20pt; font-weight:600;\">%1 %2's Data </span></p></body></html>")
                       .arg(firstName()).arg(surname()));
    ui->groupBox_2->setTitle("Residential Address");
    ui->groupBox_3->setTitle("Permenent Home Address");
    setStyleSheet("background-color: rgb(255, 255, 255);\nfont: 75 10pt \"MS Shell Dlg 2\";\ncolor: rgb(0, 0, 0);");

    ui->submitButton->setVisible(false);
    ui->cancelButton->setVisible(false);
    ui->imageButton->setVisible(false);
}

GwStudentData GwStudentFullDataForm::studentData()
{
    return studentData_;
}

GwAddress GwStudentFullDataForm::residentialAddress() const
{
    return residentialAddress_;
}

GwAddress GwStudentFullDataForm::permenentAddress() const
{
    return permenentAddress_;
}

void GwStudentFullDataForm::setStudentData()
{
    studentData_.setDateOfBirth(dateOfBirth());
    studentData_.setReligion(religion());
    studentData_.setBloodGroup(bloodGroup());
    studentData_.setGenotype(genotype());
    studentData_.setStudHeight(stuHeight());
    studentData_.setStudWeight(weigth());
    studentData_.setPhotoPath(savedImagePath());

    permenentAddress_.setAddress(permAddress());
    permenentAddress_.setState(permState());
    permenentAddress_.setLga(permLGA());
    permenentAddress_.setCountry(nationality());

    residentialAddress_.setAddress(resAddress());
    residentialAddress_.setCity(city());
    residentialAddress_.setState(resState());
    residentialAddress_.setLga(resLGA());
    residentialAddress_.setPhoneNumber(phoneNumber());
    residentialAddress_.setEmail(email());
}

void GwStudentFullDataForm::setLabelImage(const QString &imgPath)
{
    QFileInfo imagePath(imgPath);
    if(imagePath.exists()){
        ui->imageLabel->setPixmap(QPixmap(imgPath));
        upLoadPicturePath_ = imgPath;
    }
}

void GwStudentFullDataForm::onStateComboChange(const QString &state)
{
    onComboBoxTextChange(ui->lgaComboBox, state);
}

void GwStudentFullDataForm::onPermStateComboChange(const QString &state)
{
    onComboBoxTextChange(ui->permLgaComboBox, state);
}

void GwStudentFullDataForm::onClickSubmitButton()
{
    saveImage();
    accept();
}

void GwStudentFullDataForm::onClickCancelButton()
{
    reject();
}

void GwStudentFullDataForm::onClickImageButton()
{
    QSize ls = QSize(200,200);
    QString imageName = QFileDialog::getOpenFileName();
    QImageReader *imgReader = new QImageReader(imageName);
    imgReader->setScaledSize(ls);
    QPixmap image = QPixmap::fromImageReader(imgReader);
    ui->imageLabel->setPixmap(image);
    upLoadPicturePath_ = imageName;
}

void GwStudentFullDataForm::onTextChange(const QString &school)
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

void GwStudentFullDataForm::saveImage()
{
    const QPixmap *img = ui->imageLabel->pixmap();

    QString imgFileName = ui->regNumEditor->text().split("/").at(1) + "_" + ui->surnameEditor->text();
    QString pathStr = projectRootPath(QDir::homePath()) + "/img";

    if(!QFileInfo::exists(pathStr)){
        QDir path(pathStr);
        path.mkdir(pathStr);
    }

    QFileInfo imageInfomation(upLoadPicturePath_);
    QString imgExt = imageInfomation.suffix();

    QString imgPath = QString("%1/%2.%3").arg(pathStr).arg(imgFileName).arg(imgExt);
    img->save(imgPath);
    savedImagePath_ = imgPath;
}
