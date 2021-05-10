#ifndef QTOMA2_H
#define QTOMA2_H

#include <QMainWindow>
//#include <QDesktopWidget>
#include <QScreen>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include "./oma2UiIndependent/oma2.h"
#include "./oma2UiIndependent/UI.h"
#include "datawindow.h"
#include "drawingwindow.h"
#include "status.h"
#include "preferences.h"
#include "variableswindow.h"

#define CLOSE_CLEANUP_DONE -10
#define WINDOW_HEIGHT 376

#define STATUS_WIDTH 260
int comdec(char*);

#ifdef Qt_UI_Mac

// This isn't the right way to do this
#define TITLEBAR_HEIGHT 0
#define WINDOW_OFFSET 20
#define WINDOW_SPACE 0
#define COMMAND_WIDTH 640
#define STATUS_OFFSET 20
#define FONT_SIZE 12

#endif

#ifdef Qt_UI_Win

// This isn't the right way to do this
#define TITLEBAR_HEIGHT 0
#define WINDOW_OFFSET 30
#define WINDOW_SPACE 14
#define COMMAND_WIDTH 640
#define STATUS_OFFSET 20
#define FONT_SIZE 10


#endif

#ifdef Qt_UI_Linux

// This isn't the right way to do this
#define TITLEBAR_HEIGHT 0
#define WINDOW_OFFSET 40
#define WINDOW_SPACE 0
#define COMMAND_WIDTH 640
#define STATUS_OFFSET 70
#define FONT_SIZE 10


#endif

typedef struct{
    int x;
    int y;
    int width;
    int height;
} rectangle;

 enum {DATA,LINE_DRAWING};

typedef struct{
    DataWindow* dataWindow;
    DrawingWindow* drawingWindow;
    int type;
} WindowArray;

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
    void updateData();
    void newRowPlot();
    void newColPlot();
    void newLinePlot(QPoint start, QPoint end);
    void addDataWindowLabel(char* string, int line);
    void addDataWindowMinMax();
    void eraseWindow(int n);
    void showPreferences();
    void showVariables();
    void fillInLabels();
    void fillDataLabel1(int x, int y, DATAWORD z);
    void fillDataLabel2(int x, int y, DATAWORD z);
    int activeWindow();
    void updateRowPlot(int theWindowRow, DrawingWindow* theWindow);
    void updateColPlot(int theWindowCol, DrawingWindow* theWindow);
    void updateStatus();
    void addForwardedCharacter(QString string);
    void deleteCharacter();
    void previousHistory();
    void nextHistory();
    int whichDataWindow(DataWindow* theWindow);
    int whichDrawingWindow(DrawingWindow* theWindow);
    void openDataFile();
    void saveDataFile();
    void setWindowAlpha(float newAlpha);

    void clearCommandWindowText();

    int fillInDataFromPixmap(QSqlDatabase db, char* tableName);    // for saving pixmap to sql database
    QImage getVideoFrame();                                 // save pixmap to video


protected:
    void keyPressEvent(QKeyEvent*);
    void closeEvent(QCloseEvent*);



private slots:
    void on_omaCommands_textChanged();
    //void fileOpen();
    void on_actionOpen_triggered();
    void on_actionPreferences_triggered();
    void on_actionShow_Variables_Window_triggered();
    void on_actionPlot_Rows_triggered();
    void on_actionPlot_Columns_triggered();
    void on_actionClose_triggered();
    void on_actionOma2_Help_triggered();
    void onApplicationFocusChanged(QWidget *old, QWidget *now);
    void moveCursorToEnd();
    void on_actionExit_triggered();
    void on_actionSave_Data_File_triggered();



    void on_actionSave_Settings_triggered();


private:
    Ui::QtOma2 *ui;
    int lastReturn;
    int programmedText;
    char oma2Command[CHPERLN*2+1];
    // the factor of 2 above is because strlcpy has been defined as strncpy for windows and linux (they work differently)
    // without this fullname would clobber the windowArray
    WindowArray windowArray[MAX_WINDOW_COUNT];
    Status *status;
    Preferences* prefs;
    VariablesWindow* variablesWindow;
    QRect mainScreenSize;
    rectangle window_placement;
    int wraps;
    int windowRow;      // not to be confused with row/colplots, this is for placement on the screen
    int numWindows;
    int currentDataWindow;

};

#endif // QTOMA2_H
