#include "GwAddress.h"

GwAddress::GwAddress(const QString &address, const QString &city)
    :address_(address), city_(city)
{}

void GwAddress::setAddress(const QString &address)
{
    address_= address;
}

void GwAddress::setCity(const QString &city)
{
    city_ = city;
}

void GwAddress::setLga(const QString &lga)
{
    lga_ = lga;
}

void GwAddress::setState(const QString &state)
{
    state_ = state;
}

void GwAddress::setCountry(const QString &country)
{
    country_ = country;
}

void GwAddress::setPhoneNumber(const QString &phoneNumber)
{
    phoneNumber_ = phoneNumber;
}

void GwAddress::setEmail(const QString &email)
{
    email_ = email;
}

QString GwAddress::address() const
{
    return address_;
}

QString GwAddress::city() const
{
    return city_;
}

QString GwAddress::lga() const
{
    return lga_;
}

QString GwAddress::state() const
{
    return state_;
}

QString GwAddress::country() const
{
    return country_;
}

QString GwAddress::phoneNumber() const
{
    return phoneNumber_;
}

QString GwAddress::email() const
{
    return email_;
}

bool GwAddress::isEmpty()
{
    if(address().isEmpty())
        return true;
    return false;
}
