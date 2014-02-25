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


}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::showData(char* name){
    extern int macflag,exflag;
    if(strlen(name))
        this->setWindowTitle(QString(name));
    pixmap = QPixmap::fromImage(
       QImage(
           iBitmap.getpixdata(),
           //data,
           iBitmap.getwidth(),
           iBitmap.getheight(),
           QImage::Format_RGBA8888
       )
   );
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
}

void DataWindow::mouseReleaseEvent(QMouseEvent *event)
{

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
    update();

}

void DataWindow::paintEvent(QPaintEvent *event)
{
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

void DataWindow::closeEvent (QCloseEvent *event)
{
    fprintf(stderr,"closing\n");
}
