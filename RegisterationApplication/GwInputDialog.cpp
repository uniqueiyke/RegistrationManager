#include "GwInputDialog.h"
#include "ui_GwInputDialog.h"
#include <QInputDialog>
#include <QModelIndex>
#include <QStringListModel>
#include <QDebug>

GwInputDialog::GwInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GwInputDialog)
{
    ui->setupUi(this);

    ui->label->setGeometry(0, size().width(), 0, size().height()/2);
    ui->listView->setFixedHeight(size().height()/3);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->okButton->setEnabled(false);
    model_ = nullptr;

    connect(ui->okButton, &QPushButton::clicked, this, &GwInputDialog::onClickOkButton);
    connect(ui->cancelButton, &QPushButton::clicked, this, &GwInputDialog::onClickCancelButton);
    connect(ui->addButton, &QPushButton::clicked, this, &GwInputDialog::onClickAddButton);
    connect(ui->listView, &QListView::doubleClicked, this, &GwInputDialog::onDoubleClickListItem);
    connect(ui->listView, &QListView::pressed, this, &GwInputDialog::onItemPressed);
}

GwInputDialog::~GwInputDialog()
{
    delete ui;
    delete model_;
}

void GwInputDialog::onClickOkButton()
{
    listText();
    accept();
}

void GwInputDialog::onClickCancelButton()
{
    reject();
}

void GwInputDialog::onClickAddButton()
{
    QInputDialog dlg(this);
    dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
    dlg.setLabelText("Select a School");
    QStringList l;
    l << "Nursery" << "Primary" << "Secondary";
    dlg.setComboBoxItems(l);
    int ret = dlg.exec();
    if(ret == 1){
        inputDialogListText_ = dlg.textValue();
        reject();
    }
}

void GwInputDialog::onDoubleClickListItem(const QModelIndex &index)
{
    Q_UNUSED(index);
    listText();
    accept();
}

void GwInputDialog::onItemPressed(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->okButton->setEnabled(true);
}

QString GwInputDialog::listText()
{
    return ui->listView->currentIndex().data().toString();
}

QString GwInputDialog::inputDialogListText() const
{
    return inputDialogListText_;
}

void GwInputDialog::setLabelText(const QString &text)
{
    ui->label->setText(text);
}

void GwInputDialog::setListItems(const QStringList &items)
{
    model_ = new QStringListModel(items, this);
    ui->listView->setModel(model_);
}
