#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTCore>
#include <QColorDialog>
#include <QInputDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionColor_triggered();
    void on_actionSave_Note_triggered();
    void on_actionClear_Jotpad_triggered();
    void on_actionFind_Notes_triggered();
    void on_actionNew_Note_triggered();
    void on_actionDisplay_All_triggered();
    void resetJotPad();

private:
    Ui::MainWindow *ui;
    QString curFormat;
    QString notePath;
    QString holdText;

    bool eventFilter(QObject *, QEvent *); //Filter to enable toolbar click events

    void setInitialProgramState(); //Sets up UI for MainWindow on launch

    void linkToolbarButtonsToEventFilter(); //Link toolbar buttons with event slots to change cursor

    void loadDefaultPath(); //Sets the default load/save path and creates the directory if it doesn't already exist

    void displayMessage(QString, bool); //(Message, Warning?) If Warning == true, holdText is used.

    void saveSettings(); //Save user defined settings

    void loadSettings(); //Load user defined settings

    void loadFontFamily(QString); //Changes the font for the entire text area.

    QString parseFileName(QString); //Parses filename into clear date time.
};


#endif // MAINWINDOW_H
