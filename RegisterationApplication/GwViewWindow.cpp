#include "GwViewWindow.h"
#include "ui_GwViewWindow.h"

#include <QSqlQuery>
#include <QDebug>
#include <QFileInfo>
#include <QImageReader>
#include <QTextFrame>
#include <QScrollBar>
#include <QTextTable>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>


GwViewWindow::GwViewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GwViewWindow)
{
    ui->setupUi(this);
}

GwViewWindow::GwViewWindow(const QString &regNum, const GwDatabaseCreator &dbCreator, QWidget *parent)
    :QMainWindow(parent), regNum_(regNum) ,dbCreator_(dbCreator), ui(new Ui::GwViewWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    textCursor_ = ui->textEdit->textCursor();
    ui->textEdit->setReadOnly(true);

    ui->textEdit->verticalScrollBar()->setStyleSheet("background-color: rgb(157, 157, 157);");
    ui->textEdit->horizontalScrollBar()->setStyleSheet("background-color: rgb(157, 157, 157);");

    textFormat_.setFontPointSize(12);
    txtFmt_ = textFormat_;
    txtFmt_.setFontItalic(true);
    txtFmt_.setFontFamily("Arial Black");
    txtFmt_.setForeground(QBrush(Qt::darkMagenta));

    titleCharFormat_.setFontWeight(10);
    titleCharFormat_.setFontItalic(true);
    titleCharFormat_.setFontUnderline(true);
    titleCharFormat_.setForeground(QBrush(Qt::darkGreen));
    titleCharFormat_.setFontPointSize(25);

    cellFormat_.setFontPointSize(12);
    createActions();
    prepareTextEditor();

}

GwViewWindow::~GwViewWindow()
{
    delete ui;
}

void GwViewWindow::writeTitle(const QString &title)
{
    textCursor_.insertBlock();

    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignCenter);
    textCursor_.insertBlock(blockFormat);

    QTextCharFormat subTitleCharFormat;
    subTitleCharFormat.setFontWeight(10);
    subTitleCharFormat.setFontItalic(true);
    subTitleCharFormat.setFontUnderline(true);
    subTitleCharFormat.setForeground(QBrush(Qt::blue));
    subTitleCharFormat.setFontPointSize(20);
    textCursor_.insertText(title, subTitleCharFormat);

    textCursor_.insertBlock();

}

void GwViewWindow::writeText(QSqlQuery q, const QString &fieldName, const QString &caption)
{
    textCursor_.insertBlock();
    textCursor_.insertText(tr("%1: ").arg(caption), textFormat_);
    textCursor_.insertText(tr("%1").arg(q.value(fieldName).toString()), txtFmt_);

}

void GwViewWindow::writeTextTable(QTextTable *table, int r, QSqlQuery q, const QString &fieldName, const QString &caption)
{
    table->appendRows(1);
    QTextTableCell tableCell = table->cellAt(r, 0);
    tableCell.setFormat(cellFormat_);
    textCursor_ = tableCell.lastCursorPosition();
    textCursor_.insertText(tr("%1: ").arg(caption));

    tableCell = table->cellAt(r, 1);
    tableCell.setFormat(cellFormat_);
    textCursor_ = tableCell.lastCursorPosition();
    textCursor_.insertText(tr("%1").arg(q.value(fieldName).toString()));
}

void GwViewWindow::prepareTextEditor()
{
    textCursor_.movePosition(QTextCursor::Start);

    QTextFrame *topFrame = textCursor_.currentFrame();
    QTextFrameFormat topFrameFormat = topFrame->frameFormat();
    topFrameFormat.setLeftMargin(20);
    topFrameFormat.setRightMargin(20);
    topFrameFormat.setTopMargin(10);
    topFrameFormat.setBottomMargin(10);
    topFrame->setFrameFormat(topFrameFormat);

    QTextBlockFormat fmt;
    fmt.setAlignment(Qt::AlignCenter);

    GwWhereClause where("regNum", QVariant(regNum_));

    QSqlQuery query = dbCreator_.fetchData("registrationT", where);

    if(query.next()){
        textCursor_.insertBlock(fmt);
        textCursor_.insertText(tr("Informations of %1 %2")
             .arg(query.value("firstName").toString())
             .arg(query.value("surname").toString()), titleCharFormat_);
    }

    {
        QSqlQuery query1 = dbCreator_.fetchData("stuDetailDataT", where);
        textCursor_.insertBlock(fmt);
        textCursor_.insertBlock();
        QTextImageFormat imageFormat;
        bool yeah = query1.next();
        if(yeah){
            QFileInfo photoPath(query1.value("photoPath").toString());
            if(photoPath.exists()){
                imageFormat.setName(photoPath.filePath());
            }else{
                imageFormat.setName(":/images/images/person_placeholder.png");
            }
        }else{
            imageFormat.setName(":/images/images/person_placeholder.png");
        }
        imageFormat.setWidth(200);
        imageFormat.setHeight(200);
        textCursor_.insertImage(imageFormat);

        fmt.setAlignment(Qt::AlignLeft);
        textCursor_.insertBlock(fmt);
        textCursor_.movePosition(QTextCursor::NextBlock);
        textCursor_.insertBlock(fmt);
        textCursor_.movePosition(QTextCursor::Down);

        writeTitle("Personal Data");
        writeText(query, "regNum", "Reg. Number");
        writeText(query, "surname", "Surname");
        writeText(query, "firstName", "First Name");
        writeText(query, "otherNames", "Middle Name");
        writeText(query, "class", "Class");
        writeText(query, "gender", "Sex");
        writeText(query, "regDate", "Date of Registration");
        writeText(query, "firstPay", "First Payment Made");
        if(yeah){
            writeText(query1, "dateOfBirth", "Date of Birth");
            writeText(query1, "height", "Height");
            writeText(query1, "weight", "Weight");
            writeText(query1, "religion", "Religion");
            writeText(query1, "bloodGroup", "Bloood Group");
            writeText(query1, "genotype", "Blood Genotype");
        }
    }

    {
        QSqlQuery query2 = dbCreator_.fetchData("resAddressT", where);
        writeTitle("Home Address");
        if(query2.next()){
            writeText(query2, "address", "Address");
            writeText(query2, "resCity", "City");
            writeText(query2, "resLga", "Council Area");
            writeText(query2, "resState", "State");
            writeText(query2, "phoneNum", "Phone Number");
            writeText(query2, "email", "Email");
        }
    }

    {
        QSqlQuery query3 = dbCreator_.fetchData("permAddressT", where);
        writeTitle("Permenent Home Address");
        if(query3.next()){
            writeText(query3, "permHomeAddress", "Home Town");
            writeText(query3, "lgaOfOrigin", "Local Govt. of Origin");
            writeText(query3, "stateOfOrigin", "State of Origin");
            writeText(query3, "nationality", "Nationality");
        }
    }

    QSqlQuery qu = dbCreator_.fetchData("studentParentT", where);
    long long parentID = 0;
    if(qu.next()){
        parentID = qu.value("parentID").toLongLong();
    }

    {
        QSqlQuery query4 = dbCreator_.fetchData("parentsT", GwWhereClause("parentID", parentID));
        writeTitle("Parent Infomation");
        if(query4.next()){


            textCursor_.insertBlock();
            textCursor_.insertText("Name: ", textFormat_);
            textCursor_.insertText(tr("%1 %2").arg(query4.value("firstName").toString())
                           .arg(query4.value("lastName").toString()), txtFmt_);
            writeText(query4, "firstName", "Local Govt. of Origin");
            writeText(query4, "relationship", "Relationship");
            writeText(query4, "occupation", "Occupation");
        }
    }

    {
        QSqlQuery query2 = dbCreator_.fetchData("parentHomeAddressT", GwWhereClause("parentID", parentID));
        writeTitle("Parent's Address");
        if(query2.next()){
            writeText(query2, "homeAddress", "Address");
            writeText(query2, "homeCity", "City");
            writeText(query2, "homeLga", "Council Area");
            writeText(query2, "homeState", "State");
            writeText(query2, "homePhoneNumber", "Phone Number");
            writeText(query2, "homeEmail", "Email");
        }
    }

    {
        QSqlQuery query2 = dbCreator_.fetchData("parentOfficeAddressT", GwWhereClause("parentID", parentID));
        writeTitle("Parent's Office Address");
        if(query2.next()){
            writeText(query2, "officeAddress", "Address");
            writeText(query2, "officeCity", "City");
            writeText(query2, "officeLga", "Council Area");
            writeText(query2, "officeState", "State");
            writeText(query2, "officePhoneNumber", "Phone Number");
            writeText(query2, "officeEmail", "Email");
        }
    }


}

void GwViewWindow::prepareTextEditorT()
{
    textCursor_.movePosition(QTextCursor::Start);

    QTextFrame *topFrame = textCursor_.currentFrame();
    QTextFrameFormat topFrameFormat = topFrame->frameFormat();
    topFrameFormat.setLeftMargin(20);
    topFrameFormat.setRightMargin(20);
    topFrameFormat.setTopMargin(10);
    topFrameFormat.setBottomMargin(10);
    topFrame->setFrameFormat(topFrameFormat);

    QTextBlockFormat fmt;
    fmt.setAlignment(Qt::AlignCenter);

    GwWhereClause where("regNum", QVariant(regNum_));
    QSqlQuery query = dbCreator_.fetchData("registrationT", where);

    if(query.next()){
        textCursor_.insertBlock(fmt);
        textCursor_.insertText(tr("Informations of %1 %2")
             .arg(query.value("firstName").toString())
             .arg(query.value("surname").toString()), titleCharFormat_);
    }

    {
        QSqlQuery query1 = dbCreator_.fetchData("stuDetailDataT", where);
        textCursor_.insertBlock(fmt);
        textCursor_.insertBlock();
        QTextImageFormat imageFormat;
        bool yeah = query1.next();
        if(yeah){
            QFileInfo photoPath(query1.value("photoPath").toString());
            if(photoPath.exists()){
                imageFormat.setName(photoPath.filePath());
            }else{
                imageFormat.setName(":/images/images/person_placeholder.png");
            }
        }else{
            imageFormat.setName(":/images/images/person_placeholder.png");
        }
        imageFormat.setWidth(200);
        imageFormat.setHeight(200);
        textCursor_.insertImage(imageFormat);

        fmt.setAlignment(Qt::AlignLeft);
        textCursor_.insertBlock(fmt);
        textCursor_.movePosition(QTextCursor::NextBlock);
        textCursor_.insertBlock(fmt);
        textCursor_.movePosition(QTextCursor::Down);

        writeTitle("Personal Data");
        QTextTable * table = tableHeader();
        writeTextTable(table, 1, query, "regNum", "Reg. Number");
        writeTextTable(table, 2, query, "surname", "Surname");
        writeTextTable(table, 3, query, "firstName", "First Name");
        writeTextTable(table, 4, query, "otherNames", "Middle Name");
        writeTextTable(table, 5, query, "class", "Class");
        writeTextTable(table, 6, query, "gender", "Sex");
        writeTextTable(table, 7, query, "regDate", "Date of Registration");
        writeTextTable(table, 8, query, "firstPay", "First Payment Made");
        if(yeah){
            writeTextTable(table, 9, query1, "dateOfBirth", "Date of Birth");
            writeTextTable(table, 10, query1, "height", "Height");
            writeTextTable(table, 11, query1, "weight", "Weight");
            writeTextTable(table, 12, query1, "religion", "Religion");
            writeTextTable(table, 13, query1, "bloodGroup", "Bloood Group");
            writeTextTable(table, 14, query1, "genotype", "Blood Genotype");
        }
    }

    textCursor_.movePosition(QTextCursor::NextBlock);
    {
        QSqlQuery query2 = dbCreator_.fetchData("resAddressT", where);
        writeTitle("Home Address");
        QTextTable * table = tableHeader();

        if(query2.next()){
            writeTextTable(table, 1, query2, "address", "Address");
            writeTextTable(table, 2, query2, "resCity", "City");
            writeTextTable(table, 3, query2, "resLga", "Council Area");
            writeTextTable(table, 4, query2, "resState", "State");
            writeTextTable(table, 5, query2, "phoneNum", "Phone Number");
            writeTextTable(table, 6, query2, "email", "Email");
        }
    }

    textCursor_.movePosition(QTextCursor::NextBlock);
    {
        QSqlQuery query3 = dbCreator_.fetchData("permAddressT", where);
        writeTitle("Permenent Home Address");

        if(query3.next()){

            QTextTable * table = tableHeader();

            writeTextTable(table, 1, query3, "permHomeAddress", "Home Town");
            writeTextTable(table, 2, query3, "lgaOfOrigin", "Local Govt. of Origin");
            writeTextTable(table, 3, query3, "stateOfOrigin", "State of Origin");
            writeTextTable(table, 4, query3, "nationality", "Nationality");
        }
    }

    QSqlQuery qu = dbCreator_.fetchData("studentParentT", where);
    long long parentID = 0;
    if(qu.next()){
        parentID = qu.value("parentID").toLongLong();
    }

    textCursor_.movePosition(QTextCursor::NextBlock);
    {
        QSqlQuery query4 = dbCreator_.fetchData("parentsT", GwWhereClause("parentID", parentID));
        writeTitle("Parent Infomation");
        if(query4.next()){
            QTextTable * table = tableHeader();
            writeTextTable(table, 1, query4,  "lastName", "Last Name");
            writeTextTable(table, 2, query4, "firstName", "First Name");
            writeTextTable(table, 3, query4, "relationship", "Relationship");
            writeTextTable(table, 4, query4, "occupation", "Occupation");
        }
    }

    textCursor_.movePosition(QTextCursor::NextBlock);
    {
        QSqlQuery query2 = dbCreator_.fetchData("parentHomeAddressT", GwWhereClause("parentID", parentID));
        writeTitle("Parent's Address");
        if(query2.next()){
            QTextTable * table = tableHeader();
            writeTextTable(table, 1, query2, "homeAddress", "Address");
            writeTextTable(table, 2, query2, "homeCity", "City");
            writeTextTable(table, 3, query2, "homeLga", "Council Area");
            writeTextTable(table, 4, query2, "homeState", "State");
            writeTextTable(table, 5, query2, "homePhoneNumber", "Phone Number");
            writeTextTable(table, 6, query2, "homeEmail", "Email");
        }
    }

    textCursor_.movePosition(QTextCursor::NextBlock);
    {
        QSqlQuery query2 = dbCreator_.fetchData("parentOfficeAddressT", GwWhereClause("parentID", parentID));
        writeTitle("Parent's Office Address");
        if(query2.next()){
            QTextTable * table = tableHeader();
            writeTextTable(table, 1, query2, "officeAddress", "Address");
            writeTextTable(table, 2, query2, "officeCity", "City");
            writeTextTable(table, 3, query2, "officeLga", "Council Area");
            writeTextTable(table, 4, query2, "officeState", "State");
            writeTextTable(table, 5, query2, "officePhoneNumber", "Phone Number");
            writeTextTable(table, 6, query2, "officeEmail", "Email");
        }
    }

}

QTextTable * GwViewWindow::tableHeader()
{
    QTextTable * table = textCursor_.insertTable(1, 2, tableFormat());
    QTextCharFormat fmt = cellFormat_;
    fmt.setFontItalic(true);
    fmt.setForeground(QBrush(Qt::darkRed));
    fmt.setFontWeight(20);

    QTextTableCell cellAt = table->cellAt(0,0);
    cellAt.setFormat(fmt);
    textCursor_ = cellAt.lastCursorPosition();
    textCursor_.insertText("Field");

    cellAt = table->cellAt(0,1);
    cellAt.setFormat(fmt);
    textCursor_ = cellAt.lastCursorPosition();
    textCursor_.insertText("Value");

    return table;
}

QTextTableFormat GwViewWindow::tableFormat()
{
    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1.5);
    tableFormat.setCellPadding(1);
    return tableFormat;
}

void GwViewWindow::onBalackThemeSelect(bool value)
{
    Q_UNUSED(value);
    ui->textEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(52, 52, 52);");
    txtFmt_.setForeground(QBrush(Qt::darkMagenta));
}

void GwViewWindow::onWhiteThemeSelect(bool value)
{
    Q_UNUSED(value);
    ui->textEdit->setStyleSheet("color: rgb(0, 0, 0);\nbackground-color: rgb(255, 255, 255);");
    txtFmt_.setForeground(QBrush(Qt::darkMagenta));
}

void GwViewWindow::onMagnetaThemeSelect(bool value)
{
    Q_UNUSED(value);
    ui->textEdit->setStyleSheet("background-color: rgb(94, 189, 140);\ncolor: rgb(154, 0, 0);");
    titleCharFormat_.setForeground(QBrush(QColor(23, 23, 23)));
}

void GwViewWindow::onGoldThemeSelect(bool value)
{
    Q_UNUSED(value);
    ui->textEdit->setStyleSheet("background-color: rgb(211, 211, 0);\ncolor: rgb(0, 56, 0);");
    titleCharFormat_.setForeground(QBrush(QColor(23, 23, 23)));
}

void GwViewWindow::onSwitchView(bool value)
{
    Q_UNUSED(value);
    if(ui->actionTabularForm->isChecked()){
        ui->textEdit->clear();
        prepareTextEditorT();
    }else{
        ui->textEdit->clear();
        prepareTextEditor();
    }
}

void GwViewWindow::onPrintPreview()
{
#if QT_CONFIG(printpreviewdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &GwViewWindow::printPreview);
    preview.exec();
#endif
}

void GwViewWindow::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    ui->textEdit->print(printer);
#endif
}

void GwViewWindow::filePrint()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (ui->textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
        ui->textEdit->print(&printer);
    delete dlg;
#endif
}

void GwViewWindow::filePrintPdf()
{
#ifndef QT_NO_PRINTER
    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    ui->textEdit->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"")
                             .arg(QDir::toNativeSeparators(fileName)));
#endif
}

void GwViewWindow::createActions()
{
    auto themeGroup = new QActionGroup(this);
    themeGroup->addAction(ui->actionWhite);
    themeGroup->addAction(ui->actionBlack);
    themeGroup->addAction(ui->actionMagneta);
    themeGroup->addAction(ui->actionGold);
    ui->actionBlack->setChecked(true);

    connect(ui->actionBlack, &QAction::triggered, this, &GwViewWindow::onBalackThemeSelect);
    connect(ui->actionMagneta, &QAction::triggered, this, &GwViewWindow::onMagnetaThemeSelect);
    connect(ui->actionWhite, &QAction::triggered, this, &GwViewWindow::onWhiteThemeSelect);
    connect(ui->actionGold, &QAction::triggered, this, &GwViewWindow::onGoldThemeSelect);
    connect(ui->actionTabularForm, &QAction::toggled, this, &GwViewWindow::onSwitchView);
    connect(ui->actionPrint, &QAction::triggered, this, &GwViewWindow::filePrint );
    connect(ui->actionPrintPreview, &QAction::triggered, this, &GwViewWindow::onPrintPreview);
    connect(ui->actionPrintPDF, &QAction::triggered, this, &GwViewWindow::filePrintPdf);
}
