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
    hasRowPlot = 0;
    colLine = -1;
    hasColPlot = 0;
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
           iBitmap.getwidth()*3,
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

QPoint DataWindow::pixmapCoords(QPoint pos){
    if(pos.x() < 0)pos.setX(0);
    if(pos.y() < 0)pos.setY(0);
    if(pos.x() > width()-1)pos.setX(width()-1);
    if(pos.y() > height()-1)pos.setY(height()-1);

    float windowWidth = width();
    float pixWidth = ui->label->pixmap()->width();
    float windowHeight = height();
    float pixHeight = ui->label->pixmap()->height();
    float scaleWidth = pixWidth/windowWidth;
    float scaleHeight = pixHeight/windowHeight;
    QPoint pixPos = QPoint(pos.x()*scaleWidth,pos.y()*scaleHeight);
    return pixPos;
}

void DataWindow::mousePressEvent(QMouseEvent *event)
{
        QPoint pos = event->pos();
        pos = pixmapCoords(pos);  // drawing is in the pixmap
        //fprintf(stderr,"%d %d\n",pos.x(),pos.y());
        // not safe for resized windows
        wPointer->fillDataLabel1(pos.x(),pos.y(),iBuffer.getpix(pos.y(),pos.x()));

        startPoint = pos;
        static float ovalue = 1;
        if(event->button() == Qt::RightButton){
            if (ovalue == 1) ovalue = UIData.alphaValue;
            else ovalue =1;
            this->setWindowOpacity(ovalue);
        }
}

void DataWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint start,next;
    start = startPoint;
    next = nextPoint;

    // remove restriction on the way a rectangle is defined
    // previously, the assumption was that all rectangles were defined from the upper left to lower right

    int x = nextPoint.x();
    int y = nextPoint.y();

    if(nextPoint.x() < startPoint.x()){
        next.setX(start.x()) ;
        start.setX(x) ;
    }
    if(nextPoint.y() < startPoint.y()){
        next.setY(startPoint.y()) ;
        start.setY(y) ;
    }

    switch  (UIData.toolselected){
        case CALCRECT:
            // add calculation here
            point begin,end;
            begin.h = start.x();
            begin.v = start.y();
            end.h = next.x();
            end.v = next.y();
            calc(begin,end);
            //break;    // in this implementation, this does redefine the image rectangle
        case SELRECT:
            UIData.iRect.ul.h = start.x();
            UIData.iRect.ul.v = start.y();
            UIData.iRect.lr.h = next.x();
            UIData.iRect.lr.v = next.y();
            break;
        case RULER:{
            DATAWORD* buffervalues = iBuffer.getvalues();
            int* bufferspecs = iBuffer.getspecs();
            char* unit_text = iBuffer.getunit_text();
            float dist,dx,dy;
            extern char reply[];

            dx = -(start.x() - next.x());
            dy = -(start.y() - next.y());
            dist = sqrt( dx*dx + dy*dy);

            if( bufferspecs[HAS_RULER] ) {
                dist /= buffervalues[RULER_SCALE];
                dx /= buffervalues[RULER_SCALE];
                dy /= buffervalues[RULER_SCALE];
            }
            printf("dx:\t%g\tdy:\t%g\tL:\t%g",dx,dy,dist);
            if( bufferspecs[HAS_RULER]!= 0  && unit_text[0]!=0 ){
                printf("\t%s\n",unit_text);
            } else {
                printf("\n");
            }
            free( buffervalues);
            free( bufferspecs);
            free( unit_text);
            break;
        }
        case LINEPLOT:
            wPointer->newLinePlot(startPoint,nextPoint);
            break;

    }
}

void DataWindow::showLine(int theLine){
     nextPoint.setY(theLine);
     mouseMoving = 1;
     update();
}

void DataWindow::showColLine(int theLine){
     nextPoint.setX(theLine);
     mouseMoving = 1;
     update();
}

void DataWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos = pixmapCoords(pos);  // drawing is in the pixmap

    if(UIData.toolselected > CROSS )
        wPointer->fillDataLabel2(pos.x(),pos.y(),iBuffer.getpix(pos.y(),pos.x()));
    else
        wPointer->fillDataLabel1(pos.x(),pos.y(),iBuffer.getpix(pos.y(),pos.x()));

    nextPoint = pos;
    mouseMoving = 1;
    if(hasRowPlot){
        int row = event->pos().y();    //nextPoint.y();
        if(row<0) row = 0;
        if(row > height()-1) row = height()-1;
        wPointer->updateRowPlot(row,hasRowPlot);
    }
    update();

    if(hasColPlot){
        int col = event->pos().x();    //nextPoint.x();
        if(col<0) col = 0;
        if(col > width()-1) col = width()-1;
        wPointer->updateColPlot(col,hasColPlot);
    }
    update();
/* this is now done in mouseRelease
    if(UIData.toolselected == SELRECT || UIData.toolselected == CALCRECT){
        UIData.iRect.ul.h = startPoint.x();
        UIData.iRect.ul.v = startPoint.y();
        UIData.iRect.lr.h = nextPoint.x();
        UIData.iRect.lr.v = nextPoint.y();
        point start = UIData.iRect.ul;
        point end = UIData.iRect.lr;

        // remove restriction on the way a rectangle is defined
        // previously, the assumption was that all rectangles were defined from the upper left to lower right
        if(end.h < start.h){
            UIData.iRect.lr.h = start.h;
            UIData.iRect.ul.h = end.h;
        }
        if(end.v < start.v){
            UIData.iRect.lr.v = start.v;
            UIData.iRect.ul.v = end.v;
        }
    }
*/
}

void DataWindow::paintEvent(QPaintEvent *event)
{
    if(hasRowPlot && mouseMoving){
        QPainter painter( &pixmap);
        painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
        painter.setRenderHint(QPainter::Antialiasing, true);
        if(hasColPlot){ // we have both row and column plots -- things work differently now
            // the row information is in the y coordinate
            // get rid of the old row line
            QPoint P1,P2;
            P1 = oldP1;
            P2 = oldP2;
            P1.setX(0);
            P2.setX((ui->label->pixmap()->width()-1));
            if(thereIsDrawing != NEWROW)
                painter.drawLine(P1,P2); // get rid of old one
            // now draw the new row line
            startPoint.setY(nextPoint.y());
            P1 = startPoint;
            P2 = nextPoint;
            P1.setX(0);
            P2.setX((ui->label->pixmap()->width()-1));
            painter.drawLine(P1,P2); // draw the new one
            if(thereIsDrawing != NEWCOL)
                thereIsDrawing = RULER;
            oldP1.setY(startPoint.y());
            oldP2.setY(nextPoint.y());
            mouseMoving = 1;
            ui->label->setPixmap(pixmap);
        } else {
            // row plot only case
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
            nextPoint.setX(ui->label->pixmap()->width()-1);
            startPoint.setY(nextPoint.y());

            painter.drawLine(startPoint,nextPoint);
            thereIsDrawing = RULER;
            oldP1 = startPoint;
            oldP2 = nextPoint;
            mouseMoving = 0;
            ui->label->setPixmap(pixmap);
        }
    }
    if(hasColPlot && mouseMoving){
        QPainter painter( &pixmap);
        painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
        painter.setRenderHint(QPainter::Antialiasing, true);
        if(hasRowPlot){ // we have both row and column plots -- things work differently now
            // the col information is in the x coordinate
            // get rid of the old col line
            QPoint P1,P2;
            P1 = oldP1;
            P2 = oldP2;
            P1.setY(0);
            P2.setY((ui->label->pixmap()->height()-1));
            if(thereIsDrawing != NEWCOL)
                painter.drawLine(P1,P2); // get rid of old one
            // now draw the new col line
            startPoint.setX(nextPoint.x());
            P1 = startPoint;
            P2 = nextPoint;
            P1.setY(0);
            P2.setY((ui->label->pixmap()->height()-1));
            painter.drawLine(P1,P2); // draw the new one
            thereIsDrawing = RULER;
            oldP1.setX(startPoint.x());
            oldP2.setX(nextPoint.x());
            mouseMoving = 0;
            ui->label->setPixmap(pixmap);

        } else {
            // column plot only case
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
            startPoint.setY(0);
            nextPoint.setY(ui->label->pixmap()->height()-1);
            startPoint.setX(nextPoint.x());

            painter.drawLine(startPoint,nextPoint);
            thereIsDrawing = RULER;
            oldP1 = startPoint;
            oldP2 = nextPoint;
            mouseMoving = 0;
            ui->label->setPixmap(pixmap);
        }
    }
    if(hasRowPlot || hasColPlot) return;

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

void DataWindow::setHasRowPlot(DrawingWindow* theWindow){
    hasRowPlot = theWindow;
}

DrawingWindow* DataWindow::getHasRowPlot(){
    return hasRowPlot;
}

void DataWindow::setColLine(int windowCol){
    colLine = windowCol;
}

void DataWindow::setHasColPlot(DrawingWindow* theWindow){
    hasColPlot = theWindow;
}

DrawingWindow* DataWindow::getHasColPlot(){
    return hasColPlot;
}

void DataWindow::setThereIsDrawing(int type){
    thereIsDrawing = type;
}

void DataWindow::closeEvent (QCloseEvent *event)
{
    // this needs work -- doesn't do book keeping and n isn't necessarily what you want
    //int n = wPointer->activeWindow();
    if(rowLine == CLOSE_CLEANUP_DONE) return;
    int n = wPointer->whichDataWindow(this);

    if(n>=0){
        wPointer->eraseWindow(n);
        fprintf(stderr,"%d closing\n",n);
     }
}

void DataWindow::keyPressEvent(QKeyEvent *event){
      if(event->modifiers() == Qt::ControlModifier){
          if(event->key() == Qt::Key_R) wPointer->newRowPlot();
          if(event->key() == Qt::Key_T) wPointer->newColPlot();
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

