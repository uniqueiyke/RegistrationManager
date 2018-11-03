#ifndef GWADDRESS_H
#define GWADDRESS_H

#include <QString>
class GwAddress
{
public:
    GwAddress(const QString &address = QString(), const QString &city = QString());

    void setAddress(const QString &address);
    void setCity(const QString &city);
    void setLga(const QString &lga);
    void setState(const QString &state);
    void setCountry(const QString &country);
    void setPhoneNumber(const QString &phoneNumber);
    void setEmail(const QString &email);

    QString address() const;
    QString city() const;
    QString lga() const;
    QString state() const;
    QString country() const;
    QString phoneNumber() const;
    QString email() const;

    bool isEmpty();

private:
    QString address_;
    QString city_;
    QString lga_;
    QString state_;
    QString country_;
    QString phoneNumber_;
    QString email_;
};

#endif // GWADDRESS_H
