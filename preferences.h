#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = 0);
    ~Preferences();
    void updatePalette();

private slots:
    void on_buttonBox_accepted();

    void on_pal0_clicked();

    void on_pal1_clicked();

    void on_pal2_clicked();

    void on_pal3_clicked();

    void on_pal4_clicked();

    void on_pal6_clicked();

    void on_pal5_clicked();

    void on_pal7_clicked();

    void on_highlightBox_clicked(bool checked);

    void on_highlightColorButton_clicked();

private:
    Ui::Preferences *ui;
    int thePalette;
    int highlight;
    QColor highlightColor;
};

#endif // PREFERENCES_H
