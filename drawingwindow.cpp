#include "drawingwindow.h"
#include "ui_drawingwindow.h"

DrawingWindow::DrawingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawingWindow)
{
    ui->setupUi(this);
}

DrawingWindow::~DrawingWindow()
{
    delete ui;
}

//DrawingWindow::~DrawingWindow()

void DrawingWindow::setTheRow(int windowRow)
{
    theRow = windowRow;
}

void DrawingWindow::setRowData(unsigned char* theData)
{
    rowData=theData;
}

void DrawingWindow::setBytesPer(int nbytes){
    bytesPer = nbytes;
}

void DrawingWindow::setIsColor(int cflag){
    isColor = cflag;
}

void DrawingWindow::setHeightScale(float scale){
    heightScale=scale;
}

void DrawingWindow::setWidthScale(float scale){
    widthScale=scale;
}

void DrawingWindow::drawTheData(){

}
