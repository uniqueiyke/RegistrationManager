#ifndef GWVIEWWINDOW_H
#define GWVIEWWINDOW_H

#include <QMainWindow>
#include <QSqlRecord>
#include <QTextCursor>
#include "GwDatabaseCreator.h"

class QPrinter;

namespace Ui {
class GwViewWindow;
}

class GwViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GwViewWindow(QWidget *parent = 0);
    GwViewWindow(const QString &regNum, const GwDatabaseCreator &dbCreator,  QWidget *parent = 0);
    ~GwViewWindow();

    void writeTitle(const QString &title);
    void writeText(QSqlQuery q, const QString &fieldName, const QString &caption);
    void writeTextTable(QTextTable *table, int r, QSqlQuery q, const QString &fieldName, const QString &caption);
    void prepareTextEditor();
    void prepareTextEditorT();
    QTextTable * tableHeader();
    QTextTableFormat tableFormat();

signals:

protected slots:
    void onBalackThemeSelect(bool value);
    void onWhiteThemeSelect(bool value);
    void onMagnetaThemeSelect(bool value);
    void onGoldThemeSelect(bool value);
    void onSwitchView(bool value);
    void onPrintPreview();
    void printPreview(QPrinter *printer);
    void filePrint();
    void filePrintPdf();


private:
    QString regNum_;
    GwDatabaseCreator dbCreator_;
    QTextCursor textCursor_;
    QTextCharFormat textFormat_;
    QTextCharFormat txtFmt_;
    QTextCharFormat titleCharFormat_;
    QTextCharFormat cellFormat_;

    Ui::GwViewWindow *ui;


    void createActions();

};

#endif // GWVIEWWINDOW_H
