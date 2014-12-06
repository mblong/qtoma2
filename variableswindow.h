#ifndef VariablesWindow_H
#define VariablesWindow_H

#include <QDialog>

namespace Ui {
class VariablesWindow;
}

class VariablesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit VariablesWindow(QWidget *parent = 0);
    ~VariablesWindow();

    void fillInVariables();

private:
    Ui::VariablesWindow *ui;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // VariablesWindow_H
