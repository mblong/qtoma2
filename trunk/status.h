#ifndef STATUS_H
#define STATUS_H

#include <QDialog>

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

private slots:
    void on_plusButton_clicked();

    void on_boxButton_clicked();

    void on_cboxButton_clicked();

    void on_rulerButton_clicked();

    void on_graphButton_clicked();

    void on_scaleBox_clicked(bool checked);

    void on_updateBox_clicked(bool checked);

private:
    Ui::Status *ui;
};

#endif // STATUS_H
