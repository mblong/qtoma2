#ifndef OMA2COMMANDS_H
#define OMA2COMMANDS_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class oma2Commands : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit oma2Commands(QWidget *parent = 0);

signals:

protected:
    void keyPressEvent(QKeyEvent*);

public slots:

};

#endif // OMA2COMMANDS_H
