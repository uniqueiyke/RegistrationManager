#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QDialog>
#include <GwStudent.h>

class QRegularExpressionValidator;

namespace Ui {
class RegistrationForm;
}

class RegistrationForm : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationForm(QWidget *parent = 0);
    ~RegistrationForm();

    QString regNumber() const;
    QString surname() const;
    QString firstName() const;
    QString otherNames() const;
    QString gender() const;
    QString level() const;
    QString school() const;
    double firstPaymentAmount();

    void setRegNumber(const QString &text);
    void setSurname(const QString &text);
    void setFirstName(const QString &text);
    void setOtherNames(const QString &text);
    void setGender(const QString &text);
    void setLevel(const QString &level);
    void setSchool(SCHOOL sch);
    void setFirstPaymentAmount(double amount);

    GwStudent student() const;
    void  setStudent(const GwStudent &student);
    bool isAllFieldFilled();

private slots:
    void onTextChange(const QString &school);
    void onClickClearButton();
    void onClickSubmitButton();
    void onClickCancelButton();
private:
    Ui::RegistrationForm *ui;

    GwStudent student_;

     void  setStudent();
};

#endif // REGISTRATIONFORM_H
