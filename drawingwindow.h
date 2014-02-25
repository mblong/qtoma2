#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QDialog>

namespace Ui {
class DrawingWindow;
}

class DrawingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DrawingWindow(QWidget *parent = 0);
    ~DrawingWindow();

private:
    Ui::DrawingWindow *ui;
};

#endif // DRAWINGWINDOW_H
