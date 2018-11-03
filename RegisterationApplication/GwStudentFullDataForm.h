#ifndef GWSTUDENTFULLDATAFORM_H
#define GWSTUDENTFULLDATAFORM_H

#include <QDialog>
#include <QDateTime>

#include "GwStudentData.h"
#include"GwAddress.h"
#include "GwGenaralFunctions.h"

class QLabel;
class QLineEdit;

namespace Ui {
class GwStudentFullDataForm;
}

class GwStudentFullDataForm : public QDialog
{
    Q_OBJECT

public:
    explicit GwStudentFullDataForm(QWidget *parent = 0);
    ~GwStudentFullDataForm();

    void setRegNum(const QString &regNum);
    void setRegNumEditorReadOnly();
    void setSurname(const QString &name);
    void setSurnameEditorReadOnly();

    void setFirstName(const QString &name);
    void setFirstNameReadOnly();
    void setOtherNames(const QString &name);
    void setOtherNamesReadOnly();
    void setGender(const QString &gender);
    void setSchool(SCHOOL sch);
    void setClass(const QString &level);

    QString regNumber() const;
    QString surname() const;
    QString firstName() const;
    QString otherNames() const;
    QString level() const;
    QString gender() const;

    QString dateOfBirth() const;
    double stuHeight() const;
    double weigth() const;
    QString religion() const;
    QString bloodGroup() const;
    QString genotype() const;

    void setDateOfBirth(const QString &text);
    void setStuHeight(double t);
    void setWeigth(double t);
    void setReligion(const QString &text);
    void setBloodGroup(const QString &text);
    void setGenotype(const QString &text);

    void setResAddress(const QString &text);
    void setCity(const QString &text);
    void setResState(const QString &text);
    void setResLGA(const QString &text);

    void setPermAddress(const QString &text);
    void setPermState(const QString &text);
    void setPermLGA(const QString &text);
    void setNationality(const QString &text);

    void setPhoneNumber(const QString &text);
    void setEmail(const QString &text);

    QString resAddress() const;
    QString city() const;
    QString resState() const;
    QString resLGA() const;

    QString permAddress() const;
    QString permState() const;
    QString permLGA() const;
    QString nationality() const;

    QString phoneNumber() const;
    QString email() const;

    QString savedImagePath();

    QLabel *imageLabel();

    QLineEdit *regNumEditor();
    QLineEdit *surnameEditor();
    void deseableControls();
    void deseableAllControls();

    GwStudentData studentData();
    GwAddress residentialAddress() const;
    GwAddress permenentAddress() const;
    void setStudentData();

    void setLabelImage(const QString &imgPath);

private slots:
    void onStateComboChange(const QString &state);
    void onPermStateComboChange(const QString &state);
    void onClickSubmitButton();
    void onClickCancelButton();
    void onClickImageButton();
    void onTextChange(const QString &school);

private:
    void saveImage();

private:
    Ui::GwStudentFullDataForm *ui;

    QString upLoadPicturePath_;
    QString savedImagePath_;

    GwStudentData studentData_;
    GwAddress residentialAddress_;
    GwAddress permenentAddress_;
};

#endif // GWSTUDENTFULLDATAFORM_H
