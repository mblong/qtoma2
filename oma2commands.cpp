#include "oma2commands.h"
#include "qtoma2.h"

extern QtOma2* wPointer;

oma2Commands::oma2Commands(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QFont font;
    //font.setFamily("Monaco");
    //font.setStyleHint(QFont::Monospace);
    //font.setFixedPitch(true);
    font.setPointSize(FONT_SIZE);

    //QTextEdit* editor = new QTextEdit();
    this->setFont(font);

    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    this->setTabStopWidth(tabStop * metrics.width(' '));
}

void oma2Commands::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
            case Qt::Key_Up:
                wPointer->previousHistory();
                event->accept();
                return; 
            case Qt::Key_Down:
                wPointer->nextHistory();
                event->accept();
                return;
    }

     QPlainTextEdit::keyPressEvent(event);

}
