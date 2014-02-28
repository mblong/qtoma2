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
    void setTheRow(int windowRow);
    void setRowData(unsigned char* theData);
    void setBytesPer(int nbytes);
    void setIsColor(int cflag);
    void setHeightScale(float scale);
    void setWidthScale(float scale);
    void drawTheData();

private:
    Ui::DrawingWindow *ui;
    unsigned char* rowData;
    unsigned char* colData;
    int theRow;
    int theCol;
    int bytesPer;
    int isColor;
    float heightScale;
    float widthScale;




};



#endif // DRAWINGWINDOW_H
