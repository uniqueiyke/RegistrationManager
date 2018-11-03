#ifndef GWPARENT_H
#define GWPARENT_H

#include "GwPerson.h"
#include "GwAddress.h"

class GwParent : public GwPerson
{
public:
    GwParent() = default;
    GwParent(const QString &surname, const QString &firstName, const QString &otherNames = "");

    QString relationship() const;
    QString occupation() const;
    GwAddress homeAddress() const;
    GwAddress officeAddress() const;

    void setRelationship(const QString &relationship);
    void setOccupation(const QString &occupation);  
    void setHomeAddress(const GwAddress &address);
    void setOfficeAddress(const GwAddress &address);

private:
    QString relationship_;
    QString occupation_;
    GwAddress homeAddress_;
    GwAddress officeAddress_;
};

#endif // GWPARENT_H
