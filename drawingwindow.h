#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QDialog>
#include <QPainter>
#include <stdio.h>
#include <QKeyEvent>

namespace Ui {
class DrawingWindow;
}

class DrawingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DrawingWindow(QWidget *parent = 0);
    ~DrawingWindow();
    void setTheRow(int windowRow);
    void setRowData(unsigned char* theData);
    void setBytesPer(int nbytes);
    void setIsColor(int cflag);
    void setHeightScale(float scale);
    void setWidthScale(float scale);
    void setMyDataWindow(int wnum);
    int getMyDataWindow();

protected:
    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent *event);


private:
    Ui::DrawingWindow *ui;
    unsigned char* rowData;
    unsigned char* colData;
    int theRow;
    int theCol;
    int bytesPer;   // bytes in the line (or lines) to be plotted
    int isColor;
    float heightScale;
    float widthScale;
    int myDataWindow;
};


#endif // DRAWINGWINDOW_H
