#ifndef REGISTERATIONMAINWINDOW_H
#define REGISTERATIONMAINWINDOW_H

#include <QMainWindow>
#include <GwDatabaseCreator.h>

class GwTabWindow;
class GwTabWidget;
class QPushButton;

namespace Ui {
class RegisterationMainWindow;
}

class RegisterationMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterationMainWindow(QWidget *parent = 0);
    ~RegisterationMainWindow();

    QMap<QString, GwDatabaseCreator> dbs() const;

    void addDatabase(const QString &dbName, GwDatabaseCreator db);
    bool writeFile();
    bool readFile();
    GwTabWindow *currentTabWindow() const;

signals:
    void widgetAdded();
    void tableSorted();

private slots:
    void onTriggerNewManagerAction();
    void onTriggerOpenManagerAction();
    void onTriggerRegisterAction();
    void onTriggerDeleteDatabaseAction();
    void onTriggerAboutThisApp();
    void onTriggerAboutQt();
    void onTriggerSortAndFilterACtion();
    void restoreBack();


private:
    Ui::RegisterationMainWindow *ui;
    GwTabWidget *tabWidget_;
    QMap<QString, GwDatabaseCreator> dbs_;
    QPushButton *btn_;

private:
    void createNewManager(const QString &dbName);
    bool createTables(GwDatabaseCreator &db);
};

#endif // REGISTERATIONMAINWINDOW_H
