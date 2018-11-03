#include "GwPerson.h"

GwPerson::GwPerson(const QString &surname, const QString &firstName, const QString &otherNames)
{
    setNames(surname, firstName, otherNames);
}

void GwPerson::setSurname(const QString &surname)
{
    surname_ = matchString(surname, stringRegExp("^[a-z]+$"));
}

void GwPerson::setFirstName(const QString &firstName)
{
    firstName_ = matchString(firstName, stringRegExp("^[a-z]+$"));
}

void GwPerson::setOtherNames(const QString &otherNames)
{
    otherNames_ = matchString(otherNames, stringRegExp("^[a-z ]+$"));
}

void GwPerson::setNames(const QString &surname, const QString &firstName, const QString &otherNames)
{
    setSurname(surname);
    setFirstName(firstName);
    setOtherNames(otherNames);
}

QString GwPerson::surname() const {return surname_;}

QString GwPerson::firstName() const {return firstName_;}

QString GwPerson::otherNames() const {return otherNames_;}

QString GwPerson::names() const
{
    QString name = surname() + " " + firstName() + " " + otherNames();
    return name.trimmed();
}

bool GwPerson::isEmpty() const
{
    if(names().isEmpty()){
        return false;
    }

    return true;
}

