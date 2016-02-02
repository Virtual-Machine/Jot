#include "mainwindow.h"
#include "ui_mainwindow.h"

#define WindowConstructorDeconstructorBegin {

// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
    setInitialProgramState();
    linkToolbarButtonsToEventFilter();
    loadDefaultPath();
}

// Deconstuctor
MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}


#define WindowConstructorDeconstructorEnd }

#define WindowActionSlotsBegin {
void MainWindow::on_actionColor_triggered()
{
    const QColor color = QColorDialog::getColor( QColor("#00A0FB"), this, "Select Color");
    if (color.isValid()) {
        QString style = "background-color: #333; color: rgb(%1, %2, %3);";
        ui->textBrowser->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
        MainWindow::curFormat = ui->textBrowser->styleSheet();
    }
}

void MainWindow::on_actionSave_Note_triggered()
{
    MainWindow::curFormat = ui->textBrowser->styleSheet();

    QString timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz-ap");
    QString saveName = MainWindow::notePath + "/" + timeString + ".note";
    QFile f(saveName);

    if(!f.open(QFile::WriteOnly | QFile::Text))
    {
        this->displayMessage("Unable To Save Note!", true);
        return;
    }

    QTextStream out(&f);
    out << ui->textBrowser->toPlainText();

    f.flush();
    f.close();

    this->displayMessage("Note Saved Successfully!", false);
}

void MainWindow::on_actionClear_Jotpad_triggered()
{
    ui->textBrowser->setPlainText("");
}

void MainWindow::on_actionFind_Notes_triggered()
{
    QString response = QInputDialog::getText(this, "Find Note", "Enter some text to search within your past notes");

    QString result = "";
    QString dir = MainWindow::notePath;
    QDirIterator it(dir, QStringList() << "*.note", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile file(it.next());
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        QString line = in.readAll();
        if (line != "" && line.indexOf(response,0,Qt::CaseInsensitive) >= 0){
            result += "<a style='color:#555; font-size: 14px;' href='";
            result += file.fileName();
            result += "'>";
            result += parseFileName(it.fileName());
            result += ":</a> ";
            result += line;
            result += "<br/>";
        }
        file.close();
    }

    ui->textBrowser->setReadOnly(true);
    ui->textBrowser->setHtml(result);
}

void MainWindow::on_actionDisplay_All_triggered()
{
    QString result = "";
    QString dir = MainWindow::notePath;
    QDirIterator it(dir, QStringList() << "*.note", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile file(it.next());
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        QString line = in.readAll();
        if (line != ""){
            result += "<a style='color:#555; font-size: 14px;' href='";
            result += file.fileName();
            result += "'>";
            result += parseFileName(it.fileName());
            result += ":</a> ";
            result += line;
            result += "<br/>";
        }
        file.close();
    }
    ui->textBrowser->setReadOnly(true);
    ui->textBrowser->setHtml(result);
}

void MainWindow::on_actionNew_Note_triggered()
{
    if (ui->textBrowser->toPlainText().length() > 0){
        QTextCursor c = ui->textBrowser->textCursor();
        c.setPosition(0);
        c.setPosition(1, QTextCursor::KeepAnchor);
        ui->textBrowser->setTextCursor(c);
    }
    MainWindow::resetJotPad();
}

#define WindowActionSlotsEnd }

#define WindowHelpersBegin {

void MainWindow::resetJotPad()
{
    ui->textBrowser->setPlainText(MainWindow::holdText);
    MainWindow::holdText = "";
    ui->textBrowser->setStyleSheet(MainWindow::curFormat);
    ui->textBrowser->setReadOnly(false);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() == QEvent::Enter)
    {
        QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
    }
    else if (e->type() == QEvent::Leave)
    {
        QApplication::restoreOverrideCursor();
    }
    return QMainWindow::eventFilter(obj, e);
}


void MainWindow::setInitialProgramState()
{
    this->setWindowIcon(QIcon(":/Jot.ico"));
    this->setWindowTitle("Jot");

    MainWindow::holdText = "";

    this->setStyleSheet("QMainWindow { background-color: #111;}");
    ui->toolBar->setStyleSheet("background : #222; border: none; color:#ddd; padding:2px;");
    MainWindow::curFormat = ui->textBrowser->styleSheet();
}

void MainWindow::linkToolbarButtonsToEventFilter()
{
    // Modify cursor when hovering over toolbar events to indicate button like status.
    QList<QWidget*> widgets = ui->toolBar->findChildren<QWidget*>();
    foreach (QWidget* widget, widgets)
    {
        widget->installEventFilter(this);
    }
}

void MainWindow::loadDefaultPath()
{
    QString notePath = QDir::homePath() + "/JotNotes";
    if(!QDir(notePath).exists())
    {
        QDir().mkdir(notePath);
    }
    MainWindow::notePath = notePath;
}

void MainWindow::displayMessage(QString str, bool warn)
{
    ui->textBrowser->setReadOnly(true);
    if (warn)
    {
        MainWindow::holdText = ui->textBrowser->toPlainText();
        ui->textBrowser->setStyleSheet("background-color: #333; color: rgb(255, 0, 120);");
    } else {
        ui->textBrowser->setStyleSheet("background-color: #333; color: rgb(0, 255, 120);");
    }
    ui->textBrowser->setPlainText(str);
    QTimer::singleShot(1000, this, SLOT(resetJotPad()));
}

void MainWindow::saveSettings()
{
    QSettings userSettings("TechAsteroid", "Jot");

    userSettings.beginGroup("MainWindow");
    userSettings.setValue("position", this->geometry());
    userSettings.endGroup();

    userSettings.beginGroup("Text");
    userSettings.setValue("style", ui->textBrowser->styleSheet());
    userSettings.setValue("family", ui->textBrowser->fontFamily());
    userSettings.endGroup();
}

void MainWindow::loadSettings()
{
    QSettings userSettings("TechAsteroid", "Jot");

    userSettings.beginGroup("MainWindow");
    QRect mWindow = userSettings.value("position", QRect(QPoint(200,200),QPoint(500,500))).toRect();
    setGeometry(mWindow);
    userSettings.endGroup();

    userSettings.beginGroup("Text");

    QString userStyleSheet = userSettings.value("style", "background-color: #333; color: #00A0FB;").toString();
    ui->textBrowser->setStyleSheet(userStyleSheet);

    QString fontFamily = userSettings.value("family", "Menlo").toString();
    loadFontFamily(fontFamily);
    userSettings.endGroup();
}

void MainWindow::loadFontFamily(QString font)
{
    QString curText = ui->textBrowser->toPlainText();
    ui->textBrowser->setPlainText("");
    ui->textBrowser->setFontFamily(font);
    ui->textBrowser->setPlainText(curText);
}

QString MainWindow::parseFileName(QString filename)
{
    QString endResult = filename.split("_")[0];
    endResult += " @ ";
    endResult += filename.split("_")[1].left(8).replace("-", ":");
    endResult += filename.split(".")[0].right(2);
    return endResult;
}


#define WindowHelpersEnd }




