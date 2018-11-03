#ifndef GWPERSON_H
#define GWPERSON_H


#include <QString>
#include "GwGenaralFunctions.h"

class GwPerson
{
public:
    GwPerson() = default;
    GwPerson(const QString &surname, const QString &firstName, const QString &otherNames = QString());

    void setNames(const QString &surname, const QString &firstName, const QString &otherNames = QString());

    void setSurname(const QString &surname);
    void setFirstName(const QString &firstName);
    void setOtherNames(const QString &otherNames);

    QString surname() const;
    QString firstName() const;
    QString otherNames() const;
    QString names() const;

    bool isEmpty() const;

private:
    QString surname_;
    QString firstName_;
    QString otherNames_;


};

#endif // GWPERSON_H
