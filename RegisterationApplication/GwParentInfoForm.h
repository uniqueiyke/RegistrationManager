#ifndef GWPARENTINFOFORM_H
#define GWPARENTINFOFORM_H

#include <QDialog>
#include "GwParent.h"

class QRegularExpressionValidator;

namespace Ui {
class GwParentInfoForm;
}

class GwParentInfoForm : public QDialog
{
    Q_OBJECT

public:
    explicit GwParentInfoForm(QWidget *parent = 0);
    ~GwParentInfoForm();

    void setPFirstName(const QString &text);
    void setPLastName(const QString &text);
    void setPRelationship(const QString &text);
    void setPOccupation(const QString &text);

    void setPHAddress(const QString &text);
    void setPHCity(const QString &text);
    void setPHPhoneNumber(const QString &text);
    void setPHLga(const QString &text);
    void setPHState(const QString &text);
    void setPHEmail(const QString &text);

    void setPOAddress(const QString &text);
    void setPOCity(const QString &text);
    void setPOPhoneNumber(const QString &text);
    void setPOLga(const QString &text);
    void setPOState(const QString &text);
    void setPOEmail(const QString &text);



    QString pFirstName();
    QString pLastName();
    QString pRelationship();
    QString pOccupation();

    QString pHAddress();
    QString pHCity();
    QString pHPhoneNumber();
    QString pHLga();
    QString pHState();
    QString pHEmail();

    QString pOAddress();
    QString pOCity();
    QString pOPhoneNumber();
    QString pOLga();
    QString pOState();
    QString pOEmail();

    bool isEmpty();
    bool hasEmpty();

    void setRegNumber(const QString &stuRegNumber , bool readOnly = true);
    void setName(const QString &name, bool readOnly = true);

    GwParent getParentData();
    void deseableControls();

private slots:
    void onClickSubmitButton();
    void onClickCancelButton();
    void onHomeStateComboChange(const QString &state);
    void onOfficeStateComboChange(const QString &state);

private:
    Ui::GwParentInfoForm *ui;

    GwParent  parent_;
};

#endif // GWPARENTINFOFORM_H
