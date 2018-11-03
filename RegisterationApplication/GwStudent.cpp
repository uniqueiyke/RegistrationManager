#include "GwStudent.h"

GwStudent::GwStudent(const QString &surname, const QString &firstName, const QString &otherNames)
    :GwPerson(surname, firstName, otherNames)
{}

void GwStudent::setRegNum(const QString &regNum)
{
    regNum_ = matchString(regNum, stringRegExp("\\d{4}/ES\\d{3,4}/[NSP]S"));
}

void GwStudent::setGender(const QString &gender)
{
    gender_ = matchString(gender, QRegularExpression(QString("female|male"), QRegularExpression::CaseInsensitiveOption));
}

void GwStudent::setLevel(const QString &level)
{
    level_ = level;
}

QString GwStudent::regNum() const
{
    return regNum_;
}

QString GwStudent::gender() const
{
    return gender_;
}

QString GwStudent::level() const
{
    return level_;
}

QString GwStudent::studentData() const
{
    QString data = "Names: " + names();
    data += "\nReg. Number: " + regNum();
    data += "\nClass: " + level();
    data += "\nSex: " + gender();

    return data;
}

bool GwStudent::isRegistered() const
{
    if(regNum().isEmpty()){
        return false;
    }

    return true;
}
