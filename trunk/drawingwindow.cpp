#include "drawingwindow.h"
#include "ui_drawingwindow.h"
#include "qtoma2.h"

extern QtOma2* wPointer;

DrawingWindow::DrawingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawingWindow)
{
    ui->setupUi(this);
}

DrawingWindow::~DrawingWindow()
{
    delete ui;
    // leaving these in gives a crash on quit?
    //if(rowData) delete rowData;
    //if(colData) delete colData;
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

void DrawingWindow::setMyDataWindow(int wnum){
    myDataWindow = wnum;
}

int DrawingWindow::getMyDataWindow(){
    return myDataWindow;
}


void DrawingWindow::paintEvent(QPaintEvent*){
    int samplesPerPix;
    if(rowData){
        QPainter painter(this);
        int theDataRow = theRow*heightScale;

        char string[32];
        sprintf(string,"Row %d",theDataRow);
        painter.drawText(QPoint(10,15),QString(string));
        painter.setRenderHint(QPainter::Antialiasing, true);

        if (isColor) {
            painter.setPen(QColor("red"));
            samplesPerPix = 3;
        } else {
            painter.setPen(QColor("black"));
            samplesPerPix = 1;
        }
        float scalex = width()/(float)bytesPer;
        float scaley = height()/(256.);
        float h = height();



        QPointF pt1(0,h-*rowData*scaley),pt2;
        for (int i=samplesPerPix; i< bytesPer;i+=samplesPerPix){
            pt2.setX((float)i*scalex);
            pt2.setY(h-*(rowData+i)*scaley);
            painter.drawLine(pt1,pt2);
            pt1 = pt2;
        }
        if(isColor){
            painter.setPen(QColor("green"));
            pt1.setX(0);
            pt1.setY(h-*(rowData+1)*scaley);
            for (int i=samplesPerPix; i< bytesPer;i+=samplesPerPix){
                pt2.setX((float)i*scalex);
                pt2.setY(h-*(rowData+i+1)*scaley);
                painter.drawLine(pt1,pt2);
                pt1 = pt2;
            }

            painter.setPen(QColor("blue"));
            pt1.setX(0);
            pt1.setY(h-*(rowData+2)*scaley);
            for (int i=samplesPerPix; i< bytesPer;i+=samplesPerPix){
                pt2.setX((float)i*scalex);
                pt2.setY(h-*(rowData+i+2)*scaley);
                painter.drawLine(pt1,pt2);
                pt1 = pt2;
            }
        }


    }
}

void DrawingWindow::keyPressEvent(QKeyEvent *event){
      QString string = event->text();
      wPointer->addForwardedCharacter(string);
}
