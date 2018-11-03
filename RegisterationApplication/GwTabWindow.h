#ifndef GWTABWINDOW_H
#define GWTABWINDOW_H

#include <QMainWindow>
#include "GwDatabaseCreator.h"

class QSplitter;
class QTableView;
class  QSqlQueryModel;
class GwTabWidget;

class GwTabWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GwTabWindow(const GwDatabaseCreator &dbCreator, QWidget *parent = nullptr);
    ~GwTabWindow();

    QSplitter *splitter() const;
    QTableView *tableView();
    QSqlQueryModel *model();
    GwDatabaseCreator &databaseCreator();

    void refresh(QString const &table);


signals:

public slots:


private slots:
    void onVHeaderContextMenuRequested(const QPoint &pos);
    void onViewContextMenuRequested(const QPoint &pos);

    void onTriggerAddData();
    void onTriggerStudentDetailData();
    void onTriggerStudentsBriefData();

    void onTriggerAddParentData();
    void onTriggerParentData();
    void onTriggerAllParentData();
    void onEditStudentData();
    void onEditParentData();
    void onViewCompleteStudentData();
    void onViewCompleteData();
    void onDeleteRecord();

private:
    GwDatabaseCreator dbCreator_;
    QSplitter *splitter_;
    QTableView *tableView_;
    QSqlQueryModel *model_;

    GwTabWidget *widgetTab_;

private:
     void studentBriefData(const QString &title, const QString &where = "");
     void setHeaders(QSqlQueryModel *model);
     void setPHeaders(QSqlQueryModel *model); // headers for parents' for view
     void setHeaderLables(const QStringList &labels, Qt::Orientation orientation,  QSqlQueryModel *model);

     void parentData(const QString &title, const QString &where = "");
};

#endif // GWTABWINDOW_H
