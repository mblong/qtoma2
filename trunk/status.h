#ifndef STATUS_H
#define STATUS_H

#include <QDialog>
//#include "./oma2UiIndependent/oma2.h"
//#include "./oma2UiIndependent/UI.h"


namespace Ui {
class Status;
}

class Status : public QDialog
{
    Q_OBJECT

public:
    explicit Status(QWidget *parent = 0);
    ~Status();
    void fillInLabels();
    void fillDataLabel1(int x, int y, float z);
    void fillDataLabel2(int x, int y, float z);

private slots:
    void on_plusButton_clicked();

    void on_boxButton_clicked();

    void on_cboxButton_clicked();

    void on_rulerButton_clicked();

    void on_graphButton_clicked();

    void on_scaleBox_clicked(bool checked);

    void on_updateBox_clicked(bool checked);


    void on_cmaxMinus_clicked();

    void on_cmaxPlus_clicked();

    void on_cminMinus_clicked();

    void on_cminPlus_clicked();

    void on_cMinMaxIncControl_valueChanged(int value);

private:
    Ui::Status *ui;
};

#endif // STATUS_H
