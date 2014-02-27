#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QDialog>

namespace Ui {
class DrawingWindow;
}

class DrawingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DrawingWindow(QWidget *parent = 0);
    ~DrawingWindow();

private:
    Ui::DrawingWindow *ui;
    unsigned char* rowData;
    unsigned char* colData;
    int theRow;
    int theCol;
    int bytesPerRow;
    int pixPerPt;
    int isColor;
    float heightScale;
    float widthScale;
};

#endif // DRAWINGWINDOW_H
