#include "GwViewWidget.h"
#include "GwStudentFullDataForm.h"
#include "GwParentInfoForm.h"

#include <QVBoxLayout>
#include <QSplitter>

GwViewWidget::GwViewWidget(GwStudentFullDataForm *stuData, GwParentInfoForm *parentData, QWidget *parent) : QWidget(parent)
{
    auto *spliter = new QSplitter(Qt::Horizontal);
    spliter->addWidget(stuData);
    spliter->addWidget(parentData);

    stuData->deseableAllControls();
    parentData->deseableControls();
    auto *vlayout = new QVBoxLayout;

    setWindowTitle(tr("%1 %2").arg(stuData->surname()).arg(stuData->firstName()));
    vlayout->addWidget(spliter);

    setLayout(vlayout);
}
