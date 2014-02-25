#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QPainter>
#include <QRubberBand>

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

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent (QCloseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Ui::DataWindow *ui;
    QString title;
    QPoint startPoint;
    QPoint nextPoint;
    QPoint oldP1,oldP2;
    QPixmap pixmap;
    int thereIsDrawing;
    int hasRowPlot;
    int rowLine;
    int hasColPlot;
    int colLine;
    int mouseMoving;
};

#endif // DATAWINDOW_H
