#include "datawindow.h"
#include "ui_datawindow.h"
#include "qtoma2.h"

extern Image iBuffer;
extern ImageBitmap iBitmap;


DataWindow::DataWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataWindow)
{
    ui->setupUi(this);
    mouseMoving = 0;
    thereIsALine = 0;
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
        fprintf(stderr,"%d %d\n",pos.x(),pos.y());
        startPoint = event->pos();
}

void DataWindow::mouseReleaseEvent(QMouseEvent *event)
{

}


void DataWindow::mouseMoveEvent(QMouseEvent *event)
{
        nextPoint = event->pos();
        mouseMoving = 1;
        update();

}

void DataWindow::paintEvent(QPaintEvent *event)
{
    if(mouseMoving) {
        QPainter painter( &pixmap);
        painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
        painter.setRenderHint(QPainter::Antialiasing, true);
        if(thereIsALine){
            painter.drawLine(oldP1,oldP2); // get rid of old one
        }
        painter.drawLine(startPoint,nextPoint); // draw the new
        oldP1 = startPoint;
        oldP2 = nextPoint;
        mouseMoving = 0;
        thereIsALine = 1;
        ui->label->setPixmap(pixmap);
    }
}

void DataWindow::closeEvent (QCloseEvent *event)
{
    fprintf(stderr,"closing\n");
}
