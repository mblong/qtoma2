#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QtSql>

#include <QCloseEvent>

#include "oma2UiIndependent/oma2.h"

class DrawingWindow;

namespace Ui {
class DataWindow;
}

class DataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataWindow(QWidget *parent = 0);
    ~DataWindow();
    void showData(char*);
    int getThePalette();
    int getDataRows();
    int getDataCols();
    int getMyDataWindow();
    unsigned char* getIntensity();

    void setRowLine(int windowRow);
    void setHasRowPlot(DrawingWindow* theWindow);    // this is the row plot drawing window
    DrawingWindow* getHasRowPlot();    // this is the row plot drawing window

    void setColLine(int windowRow);
    void setHasColPlot(DrawingWindow* theWindow);    // this is the row plot drawing window
    DrawingWindow* getHasColPlot();    // this is the row plot drawing window

    void setThereIsDrawing(int type);
    void showLine(int theLine);
    void showColLine(int theLine);
    QPoint pixmapCoords(QPoint pos);

    void labelMinMax();
    void label(char* string);

    friend class QtOma2;



protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent (QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::DataWindow *ui;
    QString title;
    QPoint startPoint;
    QPoint nextPoint;
    QPoint oldP1,oldP2;
    QPixmap pixmap;
    int thereIsDrawing;
    DrawingWindow* hasRowPlot;     // this is the  row plot drawing window
    int rowLine;
    DrawingWindow* hasColPlot;
    int colLine;
    int mouseMoving;
    int         thePalette;
    unsigned char*   intensity;
    int         intensitySize;
    int         dataRows;
    int         dataCols;
    char minMaxString[PREFIX_CHPERLN];
    char labelString[PREFIX_CHPERLN];

};

#endif // DATAWINDOW_H
