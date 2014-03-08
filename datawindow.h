#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QKeyEvent>

#include <QCloseEvent>


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
    void setHasRowPlot(int theWindowNumber);    // this is the number of the row plot drawing window
    void showLine(int theLine);
    QPoint pixmapCoords(QPoint pos);

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
    int hasRowPlot;     // this is the number of the row plot drawing window
    int rowLine;
    int hasColPlot;
    int colLine;
    int mouseMoving;
    int         thePalette;
    unsigned char*   intensity;
    int         intensitySize;
    int         dataRows;
    int         dataCols;

};

#endif // DATAWINDOW_H
