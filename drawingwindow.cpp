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

/*
 *     if(rowData){
        // theRow is the data window row
        // the data window height is
        // the data height is
        //iBitmap.getheight()/self.frame.size.height;
        int theDataRow = theRow*heightScale;

        NSString *label =[NSString stringWithFormat:@"Row %d",theDataRow];
        NSPoint startPoint;
        startPoint.x = 10;
        startPoint.y = dirtyRect.size.height  - TITLEBAR_HEIGHT;
        [label drawAtPoint:startPoint withAttributes:NULL];

        NSBezierPath *path = [NSBezierPath bezierPath];
        if (isColor) {
            [[NSColor redColor] setStroke];
            samplesPerPix = 3;
        } else {
            [[NSColor blackColor] setStroke];
            samplesPerPix = 1;
        }

        [path setLineWidth:1.0];
        unsigned char* rowData_ = (unsigned char*)[rowData bytes];

        float scalex = self.window.frame.size.width/(float)bytesPerRow/pixPerPt;
        float scaley = (self.window.frame.size.height-TITLEBAR_HEIGHT)/(256.);

        NSPoint pt;
        pt.x = 0.;
        pt.y = *rowData_*scaley;
        [path moveToPoint:pt];
        for (int i=samplesPerPix; i< bytesPerRow;i+=samplesPerPix){
            pt.x = (float)i*scalex*pixPerPt;
            pt.y = *(rowData_+i)*scaley;
            [path lineToPoint:pt];
        }

        [path stroke];
        if (isColor) {
            NSBezierPath *path2 = [NSBezierPath bezierPath];
            [[NSColor greenColor] setStroke];
            pt.x = 0.;
            pt.y = (*rowData_+1)*scaley;
            [path2 moveToPoint:pt];
            for (int i=samplesPerPix; i< bytesPerRow;i+=samplesPerPix){
                pt.x = (float)i*scalex*pixPerPt;
                pt.y = *(rowData_+i+1)*scaley;
                [path2 lineToPoint:pt];
            }
            [path2 stroke];
*/
