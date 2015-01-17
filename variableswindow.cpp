#include "variableswindow.h"
#include "ui_variableswindow.h"
#include "oma2UiIndependent/comdec.h"
#include "qtoma2.h"

extern QtOma2*  wPointer;

VariablesWindow::VariablesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VariablesWindow)
{
    ui->setupUi(this);
}

VariablesWindow::~VariablesWindow()
{
    delete ui;
}

void VariablesWindow::fillInVariables(){
    std::string variables("Defined Variables:\n");
    variables = getVariablesString(variables);
    variables = getTempImagesString(variables);

    ui->variableList->setText(variables.c_str());
}

void VariablesWindow::keyPressEvent(QKeyEvent *event){
      if(event->modifiers() == Qt::ControlModifier){
          if(event->key() == Qt::Key_R) wPointer->newRowPlot();
          if(event->key() == Qt::Key_F) wPointer->showPreferences();
          return;
      }
      extern int stopMacroNow;
      if(event->key() == Qt::Key_Escape)
      {
          //addCString((char*)"You pressed ESC\n");
          stopMacroNow = 1;
          return;
      }
      if(event->key() == Qt::Key_Backspace ||event->key() == Qt::Key_Delete){
          wPointer->deleteCharacter();
          return;
      }
      QString string = event->text();
      wPointer->addForwardedCharacter(string);
}
