#include "drawingwindow.h"
#include "ui_drawingwindow.h"
#include "qtoma2.h"

extern QtOma2* wPointer;
extern oma2UIData UIData;

DrawingWindow::DrawingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawingWindow)
{
    ui->setupUi(this);
    theRow = -1;
    theCol = -1;
    myDataWindow = 0;
    rowData = 0;
    colData = 0;
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

DrawingWindow::~DrawingWindow()
{
    delete ui;
    // leaving these in gives a crash on quit?
    //if(rowData) delete rowData;
    //if(colData) delete colData;
}

void DrawingWindow::setTheRow(int windowRow)
{
    theRow = windowRow;
}

int DrawingWindow::getTheRow()
{
    return theRow;
}

void DrawingWindow::setRowData(unsigned char* theData)
{
    if(rowData) delete rowData;
    rowData=theData;
}

void DrawingWindow::setTheCol(int windowCol)
{
    theCol = windowCol;
}

int DrawingWindow::getTheCol()
{
    return theCol;
}

void DrawingWindow::setColData(unsigned char* theData)
{
    if(colData) delete colData;
    colData=theData;
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

void DrawingWindow::setMyDataWindow(DataWindow* win){
    //if(win)
        myDataWindow = win;
}

DataWindow* DrawingWindow::getMyDataWindow(){
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
    if(colData){
        QPainter painter(this);
        int theDataCol = theCol*widthScale;

        if(theCol >= 0){
            char string[32];
            sprintf(string,"Column %d",theDataCol);
            painter.drawText(QPoint(10,15),QString(string));
        }
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

        QPointF pt1(0,h-*colData*scaley),pt2;
        for (int i=samplesPerPix; i< bytesPer;i+=samplesPerPix){
            pt2.setX((float)i*scalex);
            pt2.setY(h-*(colData+i)*scaley);
            painter.drawLine(pt1,pt2);
            pt1 = pt2;
        }
        if(isColor){
            painter.setPen(QColor("green"));
            pt1.setX(0);
            pt1.setY(h-*(colData+1)*scaley);
            for (int i=samplesPerPix; i< bytesPer;i+=samplesPerPix){
                pt2.setX((float)i*scalex);
                pt2.setY(h-*(colData+i+1)*scaley);
                painter.drawLine(pt1,pt2);
                pt1 = pt2;
            }

            painter.setPen(QColor("blue"));
            pt1.setX(0);
            pt1.setY(h-*(colData+2)*scaley);
            for (int i=samplesPerPix; i< bytesPer;i+=samplesPerPix){
                pt2.setX((float)i*scalex);
                pt2.setY(h-*(colData+i+2)*scaley);
                painter.drawLine(pt1,pt2);
                pt1 = pt2;
            }
        }
    }
}

void DrawingWindow::keyPressEvent(QKeyEvent *event){
    if(event->modifiers() == Qt::ControlModifier){
        if(event->key() == Qt::Key_F) wPointer->showPreferences();
        return;
    }
    extern int stopMacroNow;
    if(event->key() == Qt::Key_Escape)
    {
        //addCString((char*)"You pressed ESC\n");
        stopMacroNow = 1;
        return;
    }
    if(event->key() == Qt::Key_Backspace ||event->key() == Qt::Key_Delete){
        wPointer->deleteCharacter();
        return;
    }
    if(event->key() == Qt::Key_Up){
                wPointer->previousHistory();
                return;
    }
    if(event->key() == Qt::Key_Down){
                wPointer->nextHistory();
                return;
    }


      QString string = event->text();
      wPointer->addForwardedCharacter(string);
}

void DrawingWindow::closeEvent (QCloseEvent *event)
{
    // this needs work -- doesn't do book keeping and n isn't necessarily what you want
    //int n = wPointer->activeWindow();
    if(theRow == CLOSE_CLEANUP_DONE) return;
    int n = wPointer->whichDrawingWindow(this);

    if(n>=0){
        wPointer->eraseWindow(n);
        fprintf(stderr,"%d closing\n",n);
     }
}

void DrawingWindow::mousePressEvent(QMouseEvent *event)
{
        static float ovalue = 1;
       if(event->button() == Qt::RightButton){
            if (ovalue == 1) ovalue = UIData.alphaValue;
            else ovalue =1;
            this->setWindowOpacity(ovalue);
        }
}
