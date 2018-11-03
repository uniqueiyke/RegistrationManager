#ifndef GWVIEWWIDGET_H
#define GWVIEWWIDGET_H

#include <QWidget>

class GwParentInfoForm;
class GwStudentFullDataForm;

class GwViewWidget : public QWidget
{
    Q_OBJECT
public:
    GwViewWidget(GwStudentFullDataForm *stuData, GwParentInfoForm *parentData, QWidget *parent = nullptr);

signals:

public slots:
};

#endif // GWVIEWWIDGET_H
