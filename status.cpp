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
    ui->boxButton->setDown(UIData.toolselected == RECT);
    ui->cboxButton->setDown(UIData.toolselected == CALCRECT);
    ui->rulerButton->setDown(UIData.toolselected == RULER);
    ui->graphButton->setDown(UIData.toolselected == LINEPLOT);




}
/*
      [self setMinMaxInc:UIData.cminmaxinc];
    [slide_val setIntValue:UIData.cminmaxinc];
    [slide_label setIntValue:[slide_val intValue]];

    //[minMaxIncSetting setIntValue:UIData.cminmaxinc];


    [ColorMaxLabel setStringValue:[NSString stringWithFormat:@"%g",UIData.cmax]];
    [ColorMinLabel setStringValue:[NSString stringWithFormat:@"%g",UIData.cmin]];

    [MinLabel setStringValue:[NSString stringWithFormat:@"Min: %g",UIData.min]];
    [MaxLabel setStringValue:[NSString stringWithFormat:@"Max: %g",UIData.max]];
    if(UIData.iscolor)
        [RowsLabel setStringValue:[NSString stringWithFormat:@"Rows: %d x 3",UIData.rows/3]];
    else
        [RowsLabel setStringValue:[NSString stringWithFormat:@"Rows: %d",UIData.rows]];
    [ColsLabel setStringValue:[NSString stringWithFormat:@"Cols: %d",UIData.cols]];
    [DXLabel setStringValue:[NSString stringWithFormat:@"DX: %d",UIData.dx]];
    [DYLabel setStringValue:[NSString stringWithFormat:@"DY: %d",UIData.dy]];
    [X0Label setStringValue:[NSString stringWithFormat:@"X0: %d",UIData.x0]];
    [Y0Label setStringValue:[NSString stringWithFormat:@"Y0: %d",UIData.y0]];

    appController.tool = UIData.toolselected;
    [self setTool_selected:UIData.toolselected];
    [[self toolSelected] selectCellAtRow:0 column:UIData.toolselected];

 */

void Status::on_plusButton_clicked()
{
    UIData.toolselected = CROSS;
    fillInLabels();
}

void Status::on_boxButton_clicked()
{
    UIData.toolselected = RECT;
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
