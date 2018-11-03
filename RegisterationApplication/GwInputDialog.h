#ifndef GWINPUTDIALOG_H
#define GWINPUTDIALOG_H

#include <QDialog>
class QListView;
class QLabel;
class QStringListModel;

namespace Ui {
class GwInputDialog;
}

class GwInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GwInputDialog(QWidget *parent = 0);
    ~GwInputDialog();

    void onClickOkButton();
    void onClickCancelButton();
    void onClickAddButton();
    void onDoubleClickListItem(const QModelIndex &index);
    void onItemPressed(const QModelIndex &index);

    QString listText();
    QString inputDialogListText() const;

    void setLabelText(const QString &text);
    void setListItems(const QStringList &items);

private:
    Ui::GwInputDialog *ui;
    QString inputDialogListText_;
    QStringListModel *model_;
};

#endif // GWINPUTDIALOG_H
