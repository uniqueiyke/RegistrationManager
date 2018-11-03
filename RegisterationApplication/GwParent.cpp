#include "GwParent.h"

GwParent::GwParent(const QString &surname, const QString &firstName, const QString &otherNames)
    :GwPerson(surname, firstName, otherNames)
{}

QString GwParent::relationship() const
{
    return relationship_;
}

QString GwParent::occupation() const
{
    return occupation_;
}

GwAddress GwParent::homeAddress() const
{
    return homeAddress_;
}

GwAddress GwParent::officeAddress() const
{
    return officeAddress_;
}

void GwParent::setRelationship(const QString &relationship)
{
    relationship_= relationship;
}

void GwParent::setOccupation(const QString &occupation)
{
    occupation_ =occupation;
}

void GwParent::setHomeAddress(const GwAddress &address)
{
    homeAddress_ = address;
}

void GwParent::setOfficeAddress(const GwAddress &address)
{
    officeAddress_ = address;
}
