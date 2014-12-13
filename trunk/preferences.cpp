#include "qtoma2.h"
#include "preferences.h"
#include "ui_preferences.h"

extern oma2UIData UIData;

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    ui->savePre->setText(QString(UIData.saveprefixbuf));
    ui->saveSuf->setText(QString(UIData.savesuffixbuf));

    ui->getPre->setText(QString(UIData.getprefixbuf));
    ui->getSuf->setText(QString(UIData.getsuffixbuf));

    ui->macroPre->setText(QString(UIData.macroprefixbuf));
    ui->macroSuf->setText(QString(UIData.macrosuffixbuf));

    ui->settingsPre->setText(QString(UIData.graphicsprefixbuf));
    ui->settingsSuf->setText(QString(UIData.graphicssuffixbuf));
    char num[16];
    sprintf(num,"%3d",(int)(100*(1-UIData.alphaValue)));
    ui->windowTransparency->setText(QString(num));
    ui->highlightBox->setChecked(UIData.highlightSaturated);
    highlight = UIData.highlightSaturated;
    thePalette = UIData.thepalette;
    updatePalette();
}

void Preferences::updatePalette(){
    ui->pal0->setDown(thePalette==0);
    ui->pal1->setDown(thePalette==1);
    ui->pal2->setDown(thePalette==2);
    ui->pal3->setDown(thePalette==3);
    ui->pal4->setDown(thePalette==4);
    ui->pal5->setDown(thePalette==5);
    ui->pal6->setDown(thePalette==6);
    ui->pal7->setDown(thePalette==7);

}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::on_buttonBox_accepted()
{
    strlcpy(UIData.saveprefixbuf,ui->savePre->text().toLocal8Bit().data(),PREFIX_CHPERLN);
    strlcpy(UIData.savesuffixbuf,ui->saveSuf->text().toLocal8Bit().data(),PREFIX_CHPERLN);

    strlcpy(UIData.getprefixbuf,ui->getPre->text().toLocal8Bit().data(),PREFIX_CHPERLN);
    strlcpy(UIData.getsuffixbuf,ui->getSuf->text().toLocal8Bit().data(),PREFIX_CHPERLN);

    strlcpy(UIData.macroprefixbuf,ui->macroPre->text().toLocal8Bit().data(),PREFIX_CHPERLN);
    strlcpy(UIData.macrosuffixbuf,ui->macroSuf->text().toLocal8Bit().data(),PREFIX_CHPERLN);

    strlcpy(UIData.graphicsprefixbuf,ui->settingsPre->text().toLocal8Bit().data(),PREFIX_CHPERLN);
    strlcpy(UIData.graphicssuffixbuf,ui->settingsSuf->text().toLocal8Bit().data(),PREFIX_CHPERLN);

    char num[16];
    strlcpy(num,ui->windowTransparency->text().toLocal8Bit().data(),16);
    int transparent;
    sscanf(num,"%d",&transparent);

    UIData.alphaValue = (float)(100-transparent)/100.;
    if(UIData.alphaValue>1.)UIData.alphaValue=1.;
    if(UIData.alphaValue<0.)UIData.alphaValue=0.;

    UIData.thepalette = thePalette;

    UIData.highlightSaturated = highlight;

}

void Preferences::on_pal0_clicked()
{
    thePalette = 0;
    updatePalette();
}

void Preferences::on_pal1_clicked()
{
    thePalette = 1;
    updatePalette();
}

void Preferences::on_pal2_clicked()
{
    thePalette = 2;
    updatePalette();
}

void Preferences::on_pal3_clicked()
{
    thePalette = 3;
    updatePalette();
}

void Preferences::on_pal4_clicked()
{
    thePalette = 4;
    updatePalette();
}

void Preferences::on_pal5_clicked()
{
    thePalette = 5;
    updatePalette();
}

void Preferences::on_pal6_clicked()
{
    thePalette = 6;
    updatePalette();
}

void Preferences::on_pal7_clicked()
{
    thePalette = 7;
    updatePalette();
}

void Preferences::on_highlightBox_clicked(bool checked)
{
    highlight = checked;

}
