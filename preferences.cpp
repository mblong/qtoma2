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


}
