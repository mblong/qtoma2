#include "status.h"
#include "ui_status.h"
#include "qtoma2.h"

extern oma2UIData UIData;


Status::Status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
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
    sprintf(text,"Min: %g",UIData.max);
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


/*
- (void) labelX0:(int) x Y0:(int) y Z0:(float) z{
    [XLabel setStringValue:[NSString stringWithFormat:@"X: %d",x]];
    [YLabel setStringValue:[NSString stringWithFormat:@"Y: %d",y]];
    [ZLabel setStringValue:[NSString stringWithFormat:@"Z: %g",z]];

}

- (void) labelX1:(int) x Y1:(int) y Z1:(float) z{
    if(x<0){
        [XLabel2 setStringValue:@" "];
        [YLabel2 setStringValue:@" "];
        [ZLabel2 setStringValue:@" "];
    } else {
        [XLabel2 setStringValue:[NSString stringWithFormat:@"X: %d",x]];
        [YLabel2 setStringValue:[NSString stringWithFormat:@"Y: %d",y]];
        [ZLabel2 setStringValue:[NSString stringWithFormat:@"Z: %g",z]];
     }
}
*/
