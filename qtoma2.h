#ifndef QTOMA2_H
#define QTOMA2_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QCloseEvent>
#include "./oma2UiIndependent/oma2.h"
#include "./oma2UiIndependent/UI.h"
#include "datawindow.h"
#include "status.h"
#include "preferences.h"


#define WINDOW_HEIGHT 376
#define COMMAND_WIDTH 640
#define STATUS_WIDTH 260
int comdec(char*);

// This isn't the right way to do this
#define TITLEBAR_HEIGHT 0

#define WINDOW_OFFSET 20

typedef struct{
    int x;
    int y;
    int width;
    int height;
} rectangle;


namespace Ui {
class QtOma2;
}

class QtOma2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtOma2(QWidget *parent = 0);
    ~QtOma2();
    void addCString(char* string);
    void newData(char* name);
    void eraseWindow(int n);
    void showPreferences();
    void fillInLabels();
    void fillDataLabel1(int x, int y, DATAWORD z);
    void fillDataLabel2(int x, int y, DATAWORD z);


protected:
    void keyPressEvent(QKeyEvent*);
    void closeEvent(QCloseEvent*);

private slots:
    void on_plainTextEdit_textChanged();
    //void fileOpen();
    void on_actionOpen_triggered();

    void on_actionTest_triggered();

private:
    Ui::QtOma2 *ui;
    int lastReturn;
    int programmedText;
    char oma2Command[CHPERLN];
    DataWindow* dwin[MAX_WINDOW_COUNT];
    Status *status;
    Preferences* prefs;
    QRect mainScreenSize;
    rectangle window_placement;
    int wraps;
    int windowRow;
    int numWindows;

};

#endif // QTOMA2_H
