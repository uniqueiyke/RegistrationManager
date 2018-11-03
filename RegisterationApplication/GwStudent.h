#ifndef GWSTUDENT_H
#define GWSTUDENT_H
#include "GwPerson.h"

class GwStudent : public GwPerson
{
public:
    GwStudent() = default;
    GwStudent(const QString &surname, const QString &firstName, const QString &otherNames = "");

    void setRegNum(const QString &regNum);
    void setGender(const QString &gender);
    void setLevel(const QString &level);

    QString regNum() const;
    QString gender()const;
    QString level()const;

    QString studentData() const;
    bool isRegistered() const;

private:
    QString regNum_;
    QString gender_;
    QString level_;
};

#endif // GWSTUDENT_H
