#include "datawindow.h"
#include "ui_datawindow.h"
#include "qtoma2.h"

extern Image iBuffer;
extern ImageBitmap iBitmap;
extern QtOma2* wPointer;
extern oma2UIData UIData;

DataWindow::DataWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataWindow)
{
    ui->setupUi(this);
    mouseMoving = 0;
    thereIsDrawing = 0;
    oldP1 = oldP2 = QPoint(0,0);
    rowLine = -1;
    hasRowPlot = -1;
    colLine = -1;
    hasColPlot = -1;


}

DataWindow::~DataWindow()
{
    delete ui;
    delete intensity;
}

void DataWindow::showData(char* name){
    extern int macflag,exflag;
    if(name){
        if(strlen(name))
            this->setWindowTitle(QString(name));
    }
    pixmap = QPixmap::fromImage(
       QImage(
           iBitmap.getpixdata(),
           //data,
           iBitmap.getwidth(),
           iBitmap.getheight(),
           QImage::Format_RGB888
       )
   );

    intensitySize = iBitmap.getheight()*iBitmap.getwidth();
    dataCols = iBitmap.getwidth();
    dataRows = iBitmap.getheight();
    thePalette = iBitmap.getpalette();
    if (iBitmap.getpalette() == -1) intensitySize *= 3;
    intensity = new unsigned char[intensitySize];
    memcpy(intensity,iBitmap.getintensitydata(),intensitySize);

   ui->label->setPixmap(pixmap);
   ui->label->show();
   //ui->label->update();
   if(macflag || exflag) QCoreApplication::processEvents();

}


void DataWindow::mousePressEvent(QMouseEvent *event)
{
        QPoint pos = event->pos();
        //fprintf(stderr,"%d %d\n",pos.x(),pos.y());
        // not safe for resized windows
        wPointer->fillDataLabel1(pos.x(),pos.y(),iBuffer.getpix(pos.y(),pos.x()));

        startPoint = event->pos();
        static float ovalue = 1;
        if(event->button() == Qt::RightButton){
            if (ovalue == 1) ovalue = .5;
            else ovalue =1;
            this->setWindowOpacity(ovalue);
        }
}

void DataWindow::mouseReleaseEvent(QMouseEvent *event)
{


}

void DataWindow::showLine(int theLine){
     nextPoint.setY(theLine);
     mouseMoving = 1;
     update();
}

void DataWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    //fprintf(stderr,"%d %d\n",pos.x(),pos.y());
    // not safe for resized windows
    if(UIData.toolselected > CROSS )
        wPointer->fillDataLabel2(pos.x(),pos.y(),iBuffer.getpix(pos.y(),pos.x()));
    else
        wPointer->fillDataLabel1(pos.x(),pos.y(),iBuffer.getpix(pos.y(),pos.x()));

    nextPoint = event->pos();
    mouseMoving = 1;
    if(hasRowPlot >=0){
        int row = nextPoint.y();
        if(row<0) row = 0;
        if(row > height()-1) row = height()-1;
        wPointer->updateRowPlot(row,hasRowPlot);
    }
    update();

}

void DataWindow::paintEvent(QPaintEvent *event)
{
    if(hasRowPlot >= 0 && mouseMoving){
        QPainter painter( &pixmap);
        painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
        painter.setRenderHint(QPainter::Antialiasing, true);

        switch(thereIsDrawing){
            case SELRECT:
                painter.drawRect(QRect(oldP1,oldP2));
                thereIsDrawing=0;
                break;
            case RULER:
                painter.drawLine(oldP1,oldP2); // get rid of old one
                thereIsDrawing=0;
                break;
        }
        startPoint.setX(0);
        nextPoint.setX(width()-1);
        startPoint.setY(nextPoint.y());

        painter.drawLine(startPoint,nextPoint);
        thereIsDrawing = RULER;
        oldP1 = startPoint;
        oldP2 = nextPoint;
        mouseMoving = 0;
        ui->label->setPixmap(pixmap);
        return;
    }

    if(mouseMoving ) {
        QPainter painter( &pixmap);
        painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
        painter.setRenderHint(QPainter::Antialiasing, true);

        switch(thereIsDrawing){
            case SELRECT:
                painter.drawRect(QRect(oldP1,oldP2));
                thereIsDrawing=0;
                break;
            case RULER:
                painter.drawLine(oldP1,oldP2); // get rid of old one
                thereIsDrawing=0;
                break;
        }
        if(UIData.toolselected == SELRECT || UIData.toolselected == CALCRECT){
            painter.drawRect(QRect(startPoint,nextPoint));
            thereIsDrawing = SELRECT;
        } else if(UIData.toolselected > CALCRECT){  // draw a line
            painter.drawLine(startPoint,nextPoint); // draw the new
            thereIsDrawing = RULER;
        }
        oldP1 = startPoint;
        oldP2 = nextPoint;
        mouseMoving = 0;
        ui->label->setPixmap(pixmap);
    }
}

int DataWindow::getThePalette(){
    return thePalette;
}

int DataWindow::getDataRows(){
    return dataRows;
}

int DataWindow::getDataCols(){
    return dataCols;
}

unsigned char* DataWindow::getIntensity(){
    return intensity;
}

void DataWindow::setRowLine(int windowRow){
    rowLine = windowRow;
}

void DataWindow::setHasRowPlot(int theWindowNumber){
    hasRowPlot = theWindowNumber;
}

void DataWindow::closeEvent (QCloseEvent *event)
{
    // this needs work -- doesn't do book keeping and n isn't necessarily what you want
    int n = wPointer->activeWindow();
    fprintf(stderr,"%d closing\n",n);
}

void DataWindow::keyPressEvent(QKeyEvent *event){
      if(event->modifiers() == Qt::ControlModifier){
          if(event->key() == Qt::Key_R) wPointer->newRowPlot();
          return;
      }
      QString string = event->text();
      wPointer->addForwardedCharacter(string);
}
