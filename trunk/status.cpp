#include "status.h"
#include "ui_status.h"
#include "qtoma2.h"

extern oma2UIData UIData;
extern Image   iBuffer;       // the image buffer
extern ImageBitmap iBitmap;   // the bitmap buffer
extern QtOma2*  wPointer;


Status::Status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

Status::~Status()
{
    delete ui;
}

void Status::fillInLabels(){
    char text[256];
    sprintf(text,"%g",UIData.cmin);
    ui->colorMinValue->setText(text);
    sprintf(text,"%g",UIData.cmax);
    ui->colorMaxValue->setText(text);

    sprintf(text,"Min: %g",UIData.min);
    ui->minLabel->setText(text);
    sprintf(text,"Max: %g",UIData.max);
    ui->maxLabel->setText(text);
    sprintf(text,"DX: %d",UIData.dx);
    ui->dxLabel->setText(text);
    sprintf(text,"DY: %d",UIData.dy);
    ui->dyLabel->setText(text);
    sprintf(text,"X0: %d",UIData.x0);
    ui->x0Label->setText(text);
    sprintf(text,"Y0: %d",UIData.y0);
    ui->y0Label->setText(text);

    sprintf(text,"Cols: %d",UIData.cols);
    ui->colLabel->setText(text);
    if(UIData.iscolor){
        sprintf(text,"Rows: %d x 3",UIData.rows/3);
        ui->rowLabel->setText(text);
     } else{
        sprintf(text,"Rows: %d",UIData.rows);
        ui->rowLabel->setText(text);
    }

    ui->updateBox->setChecked(UIData.autoupdate);
    ui->scaleBox->setChecked(UIData.autoscale);

    ui->plusButton->setDown(UIData.toolselected == CROSS);
    ui->boxButton->setDown(UIData.toolselected == SELRECT);
    ui->cboxButton->setDown(UIData.toolselected == CALCRECT);
    ui->rulerButton->setDown(UIData.toolselected == RULER);
    ui->graphButton->setDown(UIData.toolselected == LINEPLOT);

    sprintf(text,"Min/Max Increment: %d%%",UIData.cminmaxinc);
    ui->cMinMaxIncLabel->setText(text);
    ui->cMinMaxIncControl->setValue(UIData.cminmaxinc);
}

void Status::on_plusButton_clicked()
{
    UIData.toolselected = CROSS;
    fillInLabels();
}

void Status::on_boxButton_clicked()
{
    UIData.toolselected = SELRECT;
    fillInLabels();

}

void Status::on_cboxButton_clicked()
{
    UIData.toolselected = CALCRECT;
    fillInLabels();

}

void Status::on_rulerButton_clicked()
{
    UIData.toolselected = RULER;
    fillInLabels();

}

void Status::on_graphButton_clicked()
{
    UIData.toolselected = LINEPLOT;
    fillInLabels();

}

void Status::on_scaleBox_clicked(bool checked)
{
    UIData.autoscale = checked;

}

void Status::on_updateBox_clicked(bool checked)
{
    UIData.autoupdate = checked;
}

void Status::fillDataLabel1(int x, int y, DATAWORD z){
    char text[256];
    sprintf(text,"X: %d",x);
    ui->xLabel_1->setText(text);
    sprintf(text,"Y: %d",y);
    ui->yLabel_1->setText(text);
    sprintf(text,"Z: %g",z);
    ui->zLabel_1->setText(text);

}

void Status::fillDataLabel2(int x, int y, DATAWORD z){
    char text[256];
    if(x< 0){
        sprintf(text,"X:");
        ui->xLabel_2->setText(text);
        sprintf(text,"Y:");
        ui->yLabel_2->setText(text);
        sprintf(text,"Z:");
        ui->zLabel_2->setText(text);

    } else {
        sprintf(text,"X: %d",x);
        ui->xLabel_2->setText(text);
        sprintf(text,"Y: %d",y);
        ui->yLabel_2->setText(text);
        sprintf(text,"Z: %g",z);
        ui->zLabel_2->setText(text);
    }

}

void Status::on_cmaxMinus_clicked()
{
    UIData.cmax -= UIData.cminmaxinc/100.0*(UIData.max-UIData.min);
    fillInLabels();
    if(UIData.autoupdate){
        int saveAuatoscale = UIData.autoscale;
        UIData.autoscale = 0;
        iBitmap = iBuffer;
        wPointer->updateData();
        UIData.autoscale = saveAuatoscale;
    }
}

void Status::on_cmaxPlus_clicked()
{
    UIData.cmax += UIData.cminmaxinc/100.0*(UIData.max-UIData.min);
    fillInLabels();
    if(UIData.autoupdate){
        int saveAuatoscale = UIData.autoscale;
        UIData.autoscale = 0;
        iBitmap = iBuffer;
        wPointer->updateData();
        UIData.autoscale = saveAuatoscale;
    }

}

void Status::on_cminMinus_clicked()
{
    UIData.cmin -= UIData.cminmaxinc/100.0*(UIData.max-UIData.min);
    fillInLabels();
    if(UIData.autoupdate){
        int saveAuatoscale = UIData.autoscale;
        UIData.autoscale = 0;
        iBitmap = iBuffer;
        wPointer->updateData();
        UIData.autoscale = saveAuatoscale;
    }

}

void Status::on_cminPlus_clicked()
{
    UIData.cmin += UIData.cminmaxinc/100.0*(UIData.max-UIData.min);
    fillInLabels();
    if(UIData.autoupdate){
        int saveAuatoscale = UIData.autoscale;
        UIData.autoscale = 0;
        iBitmap = iBuffer;
        wPointer->updateData();
        UIData.autoscale = saveAuatoscale;
    }

}

void Status::on_cMinMaxIncControl_valueChanged(int value)
{
    char text[256];
    UIData.cminmaxinc = value;
    sprintf(text,"Min/Max increment %d%%",UIData.cminmaxinc);
    ui->cMinMaxIncLabel->setText(text);
}

void Status::keyPressEvent(QKeyEvent *event){
      if(event->modifiers() == Qt::ControlModifier){
          if(event->key() == Qt::Key_R) wPointer->newRowPlot();
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
      QString string = event->text();
      wPointer->addForwardedCharacter(string);
}

void Status::dragEnterEvent(QDragEnterEvent *ev)
{
    ev->accept();
}

void Status::dropEvent(QDropEvent *event){
        QList<QUrl> urls = event->mimeData()->urls();
        foreach(QUrl url, urls)
        {
            QString theFile = url.path();
            QByteArray ba = theFile.toLocal8Bit();
            char name[CHPERLN],ext[PREFIX_CHPERLN];
            strlcpy(name,ba.data(),CHPERLN);
            int i;
            for(i=strlen(name)-1; i >= 0 ;i--){
                if(name[i] == '.') break;
            }
            strlcpy(ext,&name[i+1],PREFIX_CHPERLN);
#ifdef Qt_UI_Win
            // need to get rid of the leading / in windows
            dropped_file(ext,&name[1]);
#else
            dropped_file(ext,name);
#endif

        }
}
