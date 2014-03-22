#include "oma2commands.h"
#include "qtoma2.h"

extern QtOma2* wPointer;

oma2Commands::oma2Commands(QWidget *parent) :
    QPlainTextEdit(parent)
{

}

void oma2Commands::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
            case Qt::Key_Up:
                wPointer->previousHistory();
                event->accept();
                return;
                break;
            case Qt::Key_Down:
                wPointer->nextHistory();
                event->accept();
                return;
                break;

    }

     QPlainTextEdit::keyPressEvent(event);

}
