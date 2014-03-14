#include "qtoma2.h"
#include "ui_qtoma2.h"

extern Image iBuffer;
extern ImageBitmap iBitmap;
extern oma2UIData UIData;

extern char applicationPath[];		// this is the path to the directory that the program is running from
extern char contentsPath[];         // this is the path to the Contents directory


QtOma2::QtOma2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtOma2)
{
    ui->setupUi(this);
    lastReturn = 0;
    programmedText = 0;
    setUpUIData();

    //char text[NEW_PREFIX_CHPERLN];

    QString appPath =  qApp->applicationDirPath();
    QByteArray ba = appPath.toLocal8Bit();
    strlcpy(applicationPath ,ba.data(),CHPERLN);
    fprintf(stderr,"App path is:%s\n",applicationPath);

    QDesktopWidget widget;
    mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    window_placement.x = (mainScreenSize.x()+WINDOW_OFFSET);
    window_placement.y = (mainScreenSize.y()+WINDOW_OFFSET);
    status = new Status(this);
    status->setGeometry(COMMAND_WIDTH+WINDOW_OFFSET,mainScreenSize.y()+mainScreenSize.height()-WINDOW_HEIGHT,STATUS_WIDTH,WINDOW_HEIGHT);
    status->show();


    wraps = 1;
    windowRow = 0;
    numWindows = 0;
    currentDataWindow = -1;
    // this is one way to add menu items
    // probably a better way is to go to the action editor
    //      right click on the action name
    //      select go to slot
    //      select triggered -- OK
    //      this fills in on_actionName_triggered() functions in both the .h and .cpp files
    //connect( ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));

    //strlcpy(text,SETTINGSFILE,NEW_PREFIX_CHPERLN);
    //loadprefs(text);
    //addCString((char*)"OMA2>");

}

QtOma2::~QtOma2()
{
    delete ui;
}

void QtOma2::on_plainTextEdit_textChanged()
{
    if(programmedText) return;
    QString thetext = ui->plainTextEdit->toPlainText();
    QChar thechar = thetext.at(thetext.size()-1);
    char ch = thechar.toLatin1();
    if(ch == '\n'){
        QString command =  thetext.mid( lastReturn, thetext.size()-1-lastReturn);
        QByteArray ba = command.toLocal8Bit();
        strlcpy(oma2Command ,ba.data(),CHPERLN);
        lastReturn = thetext.size();
        int returnValue = comdec((char*) oma2Command);
        if(returnValue < GET_MACRO_LINE) addCString((char*)"OMA2>");
     }
}

void QtOma2::addCString(char* string)
{
    programmedText = 1;
    ui->plainTextEdit->insertPlainText(QString(string));
    lastReturn += strlen(string);
    programmedText = 0;
    ui->plainTextEdit->ensureCursorVisible();
    //QTextCursor cursor =ui->plainTextEdit->textCursor();
    //cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

}

void QtOma2::addForwardedCharacter(QString string)
{

    ui->plainTextEdit->insertPlainText(string);
    //lastReturn += string.length();


}


void QtOma2::keyPressEvent(QKeyEvent *event)
{
    extern int stopMacroNow;
    if(event->key() == Qt::Key_Escape)
    {
        //addCString((char*)"You pressed ESC\n");
        stopMacroNow = 1;
    }
}

void QtOma2::showPreferences(){
    prefs = new Preferences(this);
    prefs->show();
}

void QtOma2::updateStatus(){
    status->fillInLabels();
}

void QtOma2::newData(char* name){
    extern int newWindowFlag;
    if(!newWindowFlag && (currentDataWindow >= 0)){
        updateData(); // dont put a new window, just use the current data window
        return;
     }

    // figure out where to place image
    // this is for possibly scaling down images that won't fit on screen
    int windowHeight = iBitmap.getheight();
    int windowWidth = iBitmap.getwidth();

    float scaleWidth = (float)windowWidth/(float)mainScreenSize.width();
    // leave a little space at the bottom of the sreen
    float scaleHeight = (float)windowHeight/(float)(mainScreenSize.height()-2*WINDOW_OFFSET);
    float scaleWindow = 1.0;
    if (scaleHeight > 1.0 || scaleWidth > 1.0) {
        if(scaleHeight > scaleWidth)
            scaleWindow = scaleHeight;
        else
            scaleWindow = scaleWidth;
        windowHeight /= scaleWindow;
        windowWidth /= scaleWindow;
        char txt[128];
        sprintf(txt," Window scaled by %f\n",scaleWindow);
        addCString(txt);
    }

    window_placement.width = windowWidth;
    window_placement.height = windowHeight;

    // now, figure out where to place the window
    if(window_placement.x == WINDOW_OFFSET+mainScreenSize.x()) {   // left column
        window_placement.y = mainScreenSize.y()+WINDOW_OFFSET+windowRow*(windowHeight+WINDOW_OFFSET);
    }

    if (window_placement.x+windowWidth > mainScreenSize.width()) {
        window_placement.x = (mainScreenSize.x()+WINDOW_OFFSET);
        windowRow++;
        if(window_placement.y + 2*(windowHeight) < mainScreenSize.height()){
            window_placement.y += (windowHeight + WINDOW_OFFSET) ;
        } else{
            wraps++;
            window_placement.y= mainScreenSize.y()+wraps*WINDOW_OFFSET; // wrap to top
            windowRow = 0;
        }
    }

    QRect placement(window_placement.x,window_placement.y,window_placement.width,window_placement.height);

    if(numWindows == MAX_WINDOW_COUNT){
        eraseWindow(0);
    }

    windowArray[numWindows].dataWindow = new DataWindow(this);
    windowArray[numWindows].dataWindow->setGeometry(placement);
    windowArray[numWindows].dataWindow->show();
    windowArray[numWindows].dataWindow->showData(name);
    windowArray[numWindows].type = DATA;
    currentDataWindow = numWindows;

    numWindows++;
    //ui->plainTextEdit->activateWindow();    // make the command window active
    window_placement.x += windowWidth+WINDOW_SPACE;            // increment for next one
}

void QtOma2::updateData(){
    if(currentDataWindow >=0)
        windowArray[currentDataWindow].dataWindow->showData(nil);
}

int QtOma2::activeWindow(){
    QWidget *activeWin = QApplication::activeWindow();
    int i=0;
    for(i=0; i< numWindows; i++){
        if(windowArray[i].dataWindow == activeWin || windowArray[i].drawingWindow == activeWin) break;
    }
    if(i == numWindows) i = -1;
    return i;
}

void QtOma2::newRowPlot(){
    int n = activeWindow();
    fprintf(stderr,"%d is active\n",n);
    if(n<0){
        //can't do this
        beep();
        return;
    }
    if(windowArray[n].type  != DATA){
        //can't do this
        beep();
        return;
    }
    if(windowArray[n].dataWindow->getHasRowPlot()){
        //can't do this there is one already
        beep();
        return;
    }

    int windowWidth = window_placement.width = windowArray[n].dataWindow->width();
    int windowHeight = window_placement.height = 256;

    // now, figure out where to place the window
    if(window_placement.x == WINDOW_OFFSET+mainScreenSize.x()) {   // left column
        window_placement.y = mainScreenSize.y()+WINDOW_OFFSET+windowRow*(windowHeight+WINDOW_OFFSET);
    }
    if (window_placement.x+windowWidth > mainScreenSize.width()) {
        window_placement.x = (mainScreenSize.x()+WINDOW_OFFSET);

        windowRow++;

        if(window_placement.y + 2*(windowHeight) < mainScreenSize.height()){
            window_placement.y += (windowHeight + WINDOW_OFFSET) ;
        } else{
            wraps++;
            window_placement.y= mainScreenSize.y()+wraps*WINDOW_OFFSET; // wrap to top
            windowRow = 0;
        }
    }
    QRect placement(window_placement.x,window_placement.y,window_placement.width,window_placement.height);
    if(numWindows == MAX_WINDOW_COUNT){
        eraseWindow(0);
    }

    int theWindowRow = windowArray[n].dataWindow->height()/2;  // this is the height of the data window
    windowArray[numWindows].drawingWindow = new DrawingWindow(this);
    windowArray[numWindows].drawingWindow->setMyDataWindow(windowArray[n].dataWindow);
    windowArray[numWindows].drawingWindow->setGeometry(placement);
    windowArray[numWindows].drawingWindow->setTheRow( theWindowRow );
    int pal = windowArray[n].dataWindow->getThePalette();
    int bytesPerRow;
    unsigned char* bytes = windowArray[n].dataWindow->getIntensity();    // the start of the data
    //float widthScale = windowArray[n].dataWindow->getDataCols()/windowArray[n].dataWindow->width();
    float heightScale = (float)windowArray[n].dataWindow->getDataRows()/windowArray[n].dataWindow->height();
    int theRow = theWindowRow * heightScale;
    if(pal >= 0) { // we have a monochrome image
        bytes += theRow * windowArray[n].dataWindow->getDataCols();
        bytesPerRow = windowArray[n].dataWindow->getDataCols();
        windowArray[numWindows].drawingWindow->setIsColor(0);
    } else {
        bytes += theRow * windowArray[n].dataWindow->getDataCols()*3;
        bytesPerRow = windowArray[n].dataWindow->getDataCols()*3;
        windowArray[numWindows].drawingWindow->setIsColor(1);
    }
    unsigned char* rowData = new unsigned char[bytesPerRow];
    memcpy(rowData,bytes,bytesPerRow);

    // tell the data window what it needs to know
    windowArray[n].dataWindow->setRowLine(theRow);
    windowArray[n].dataWindow->setHasRowPlot(windowArray[numWindows].drawingWindow);
    if(windowArray[n].dataWindow->getHasColPlot()) windowArray[n].dataWindow->setThereIsDrawing(NEWROW);
    windowArray[n].dataWindow->showLine(theRow);

    windowArray[numWindows].drawingWindow->setHeightScale(heightScale);
    windowArray[numWindows].drawingWindow->setRowData(rowData);
    windowArray[numWindows].drawingWindow->setBytesPer(bytesPerRow);
    windowArray[numWindows].type = LINE_DRAWING;
    windowArray[numWindows].drawingWindow->show();
    windowArray[numWindows].drawingWindow->update();

    numWindows++;

    //ui->plainTextEdit->activateWindow();    // make the command window active
    window_placement.x += windowWidth+WINDOW_SPACE;            // increment for next one
    UIData.toolselected = CROSS;
    fillInLabels();
}

void QtOma2::updateRowPlot(int theWindowRow, DrawingWindow* theWindow){
    //int n = whichDataWindow(theWindow->getMyDataWindow());
    DataWindow* theDataWindow = theWindow->getMyDataWindow();
    theWindow->setTheRow( theWindowRow );
    int pal = theDataWindow->getThePalette();
    int bytesPerRow;
    unsigned char* bytes = theDataWindow->getIntensity();    // the start of the data
    //float widthScale = theDataWindow->getDataCols()/theDataWindow->width();
    float heightScale = (float)theDataWindow->getDataRows()/theDataWindow->height();
    int theRow = theWindowRow * heightScale;
    if(pal >= 0) { // we have a monochrome image
        bytes += theRow * theDataWindow->getDataCols();
        bytesPerRow = theDataWindow->getDataCols();
        theWindow->setIsColor(0);
    } else {
        bytes += theRow * theDataWindow->getDataCols()*3;
        bytesPerRow = theDataWindow->getDataCols()*3;
        theWindow->setIsColor(1);
    }
    unsigned char* rowData = new unsigned char[bytesPerRow];
    memcpy(rowData,bytes,bytesPerRow);

    theWindow->setHeightScale(heightScale);
    theWindow->setRowData(rowData);
    theWindow->setBytesPer(bytesPerRow);
    theWindow->update();
}

void QtOma2::newColPlot(){
    int n = activeWindow();
    //fprintf(stderr,"%d is active\n",n);
    if(n<0){
        //can't do this
        beep();
        return;
    }
    if(windowArray[n].type  != DATA){
        //can't do this
        beep();
        return;
    }
    if(windowArray[n].dataWindow->getHasColPlot()){
        //can't do this there is one already
        beep();
        return;
    }

    int windowWidth = window_placement.width = windowArray[n].dataWindow->height();
    int windowHeight = window_placement.height = 256;

    // now, figure out where to place the window
    if(window_placement.x == WINDOW_OFFSET+mainScreenSize.x()) {   // left column
        window_placement.y = mainScreenSize.y()+WINDOW_OFFSET+windowRow*(windowHeight+WINDOW_OFFSET);
    }
    if (window_placement.x+windowWidth > mainScreenSize.width()) {
        window_placement.x = (mainScreenSize.x()+WINDOW_OFFSET);

        windowRow++;

        if(window_placement.y + 2*(windowHeight) < mainScreenSize.height()){
            window_placement.y += (windowHeight + WINDOW_OFFSET) ;
        } else{
            wraps++;
            window_placement.y= mainScreenSize.y()+wraps*WINDOW_OFFSET; // wrap to top
            windowRow = 0;
        }
    }
    QRect placement(window_placement.x,window_placement.y,window_placement.width,window_placement.height);
    if(numWindows == MAX_WINDOW_COUNT){
        eraseWindow(0);
    }

    int theWindowCol = windowArray[n].dataWindow->width()/2;  // this is half the width of the data window
    windowArray[numWindows].drawingWindow = new DrawingWindow(this);
    windowArray[numWindows].drawingWindow->setMyDataWindow(windowArray[n].dataWindow);
    windowArray[numWindows].drawingWindow->setGeometry(placement);
    windowArray[numWindows].drawingWindow->setTheCol( theWindowCol );
    int pal = windowArray[n].dataWindow->getThePalette();
    int bytesPerCol;
    unsigned char* bytes = windowArray[n].dataWindow->getIntensity();    // the start of the data
    float widthScale = (float)windowArray[n].dataWindow->getDataCols()/windowArray[n].dataWindow->width();
    //float heightScale = (float)windowArray[n].dataWindow->getDataRows()/windowArray[n].dataWindow->height();
    int theCol = theWindowCol * widthScale;
    int bytesPerPix;
    if(pal >= 0) { // we have a monochrome image
        //bytes += theRow * windowArray[n].dataWindow->getDataCols();
        bytesPerCol = windowArray[n].dataWindow->getDataRows();
        windowArray[numWindows].drawingWindow->setIsColor(0);
        bytesPerPix = 1;
    } else {
        //bytes += theRow * windowArray[n].dataWindow->getDataCols()*3;
        bytesPerCol = windowArray[n].dataWindow->getDataRows()*3;
        windowArray[numWindows].drawingWindow->setIsColor(1);
        bytesPerPix = 3;
    }
    unsigned char* colData = new unsigned char[bytesPerCol];
    int cols = windowArray[n].dataWindow->getDataCols();
    int k=0;
    for(int i = 0; i< bytesPerCol/bytesPerPix;i++){
        for(int j=0; j < bytesPerPix;j++)
        *(colData+k++) = *(bytes + (theCol + i*cols)*bytesPerPix + j);
    }

    // tell the data window what it needs to know
    windowArray[n].dataWindow->setColLine(theCol);
    windowArray[n].dataWindow->setHasColPlot(windowArray[numWindows].drawingWindow);
    if(windowArray[n].dataWindow->getHasRowPlot())
        windowArray[n].dataWindow->setThereIsDrawing(NEWCOL);
    windowArray[n].dataWindow->showColLine(theCol);

    windowArray[numWindows].drawingWindow->setWidthScale(widthScale);
    windowArray[numWindows].drawingWindow->setColData(colData);
    windowArray[numWindows].drawingWindow->setBytesPer(bytesPerCol);
    windowArray[numWindows].type = LINE_DRAWING;
    windowArray[numWindows].drawingWindow->show();
    windowArray[numWindows].drawingWindow->update();

    numWindows++;

    //ui->plainTextEdit->activateWindow();    // make the command window active
    window_placement.x += windowWidth+WINDOW_SPACE;            // increment for next one
    UIData.toolselected = CROSS;
    fillInLabels();

}

void QtOma2::updateColPlot(int theWindowCol, DrawingWindow* theWindow){
    //int n = whichDataWindow(theWindow->getMyDataWindow());
    DataWindow* theDataWindow = theWindow->getMyDataWindow();
    theWindow->setTheCol( theWindowCol );
    int pal = theDataWindow->getThePalette();
    int bytesPerCol;
    unsigned char* bytes = theDataWindow->getIntensity();    // the start of the data
    float widthScale = (float)theDataWindow->getDataCols()/theDataWindow->width();
    //float heightScale = (float)windowArray[n].dataWindow->getDataRows()/windowArray[n].dataWindow->height();
    int theCol = theWindowCol * widthScale;
    int bytesPerPix;
    if(pal >= 0) { // we have a monochrome image
        bytesPerCol = theDataWindow->getDataRows();
        theWindow->setIsColor(0);
        bytesPerPix = 1;
    } else {
        bytesPerCol = theDataWindow->getDataRows()*3;
        theWindow->setIsColor(1);
        bytesPerPix = 3;
    }
    unsigned char* colData = new unsigned char[bytesPerCol];
    int cols = theDataWindow->getDataCols();
    int k=0;
    for(int i = 0; i< bytesPerCol/bytesPerPix;i++){
        for(int j=0; j < bytesPerPix;j++)
            *(colData+k++) = *(bytes + (theCol + i*cols)*bytesPerPix + j);
    }

    theWindow->setWidthScale(widthScale);
    theWindow->setColData(colData);
    theWindow->setBytesPer(bytesPerCol);
    theWindow->update();
}


int QtOma2::whichDataWindow(DataWindow* theWindow){
    int i=-1;
    for(i=0; i<numWindows;i++){
        if(theWindow == windowArray[i].dataWindow) break;
    }
    if(i == numWindows) i = -1;
    return i;
}

int QtOma2::whichDrawingWindow(DrawingWindow* theWindow){
    int i=-1;
    if(theWindow == 0) return i;
    for(i=0; i<numWindows;i++){
        if(theWindow == windowArray[i].drawingWindow) break;
    }
    if(i == numWindows) i = -1;
    return i;
}

void QtOma2::eraseWindow(int n){
    if(n < 0){  // close all
        for(int i=0; i< numWindows; i++){
          if(windowArray[i].type == DATA){
              windowArray[i].dataWindow->setRowLine(CLOSE_CLEANUP_DONE);
              windowArray[i].dataWindow-> close();
          } else{
              windowArray[i].drawingWindow->setRowData(0);  // free the memory
              windowArray[i].drawingWindow->setTheRow(CLOSE_CLEANUP_DONE);
              windowArray[i].drawingWindow->close();
           }
        }
        window_placement.x = (mainScreenSize.x()+WINDOW_OFFSET);
        window_placement.y = (mainScreenSize.y()+WINDOW_OFFSET);
        wraps = 1;
        windowRow = 0;
        numWindows = 0;
        currentDataWindow = -1;
        return;
    }
    if (n < numWindows){
        if(n == currentDataWindow) currentDataWindow  = -1;
        if(currentDataWindow > n) currentDataWindow--;
        //dwin[n]->close();
        if(windowArray[n].type == DATA){
            // maybe this has a row or column plot that needs to know of our imminent demise
            int theRowPlotWindowNumber = whichDrawingWindow(windowArray[n].dataWindow->getHasRowPlot());
            if(theRowPlotWindowNumber >=0){
                windowArray[theRowPlotWindowNumber].drawingWindow->setMyDataWindow(0);
                windowArray[theRowPlotWindowNumber].drawingWindow->setTheRow(-1);
            }
            int theColPlotWindowNumber = whichDrawingWindow(windowArray[n].dataWindow->getHasColPlot());
            if(theColPlotWindowNumber >=0){
                windowArray[theColPlotWindowNumber].drawingWindow->setMyDataWindow(0);
                windowArray[theColPlotWindowNumber].drawingWindow->setTheCol(-1);
            }

            windowArray[n].dataWindow->setRowLine(CLOSE_CLEANUP_DONE);
            windowArray[n].dataWindow->close();
        }else{ //getting rid of row or column plot
            int dataWindowNumber = whichDataWindow(windowArray[n].drawingWindow->getMyDataWindow());
            // what kind of drawing window is this anyway?
            if(windowArray[n].drawingWindow->getTheRow() >=0) {
                // this is a row plot -- clear information from the data window
                if(dataWindowNumber >=0){
                    windowArray[dataWindowNumber].dataWindow->setHasRowPlot(0);
                    windowArray[dataWindowNumber].dataWindow->setRowLine(-1);
                }
                windowArray[n].drawingWindow->setRowData(0);  // free the memory
            } else{
                // column plot stuff
                // clear information from the data window
                if(dataWindowNumber >=0){
                    windowArray[dataWindowNumber].dataWindow->setHasColPlot(0);
                    windowArray[dataWindowNumber].dataWindow->setColLine(-1);
                }
                windowArray[n].drawingWindow->setColData(0);  // free the memory
            }

            windowArray[n].drawingWindow->setTheRow(CLOSE_CLEANUP_DONE);
            windowArray[n].drawingWindow->close();
        }

        for(int i=n; i<numWindows-1; i++){
            //dwin[i]=dwin[i+1];
            windowArray[i] = windowArray[i+1];
        }
        numWindows--;
    }

    if(numWindows == 0){
        window_placement.x = (mainScreenSize.x()+WINDOW_OFFSET);
        window_placement.y = (mainScreenSize.y()+WINDOW_OFFSET);
        wraps = 1;
        windowRow = 0;
        numWindows = 0;
        currentDataWindow = -1;
    }
    /*
        if (n < 0) {            // erase everything
            for (id thewindow in windowArray){
                if ([thewindow isKindOfClass:[DataWindowController class]]){
                    [thewindow setHasRowPlot:CLOSE_CLEANUP_DONE];
                    // this is for communication with [dataWindowController windowWillClose]
                    // we don't need to do any similar thing with hasColPlot, since the whole dataWindowController and imageView go away
                }
                if ([thewindow isKindOfClass:[DrawingWindowController class]]){
                    [thewindow setDrawingType:CLOSE_CLEANUP_DONE];
                    // see above
                }
                [[thewindow window ] close];
            }
            [windowArray removeAllObjects];
            wraps=1;
            window_placement.origin.x = screenRect.origin.x+WINDOW_OFFSET;
            window_placement.origin.y = screenRect.size.height;
            return;
        }

        if (n < [windowArray count]) {
            id thewindowController = windowArray[n];
            if ([thewindowController isKindOfClass:[DataWindowController class]]){
                // erasing a data window
                // check to see if this has any row or column plots
                if([thewindowController hasRowPlot] >=0){
                    // this data window is going away, so don't leave the pointer laying around
                    [[[(DataWindowController*)thewindowController imageView] rowWindowController] setDataWindowController:NULL];
                }
                if([thewindowController hasColPlot] >=0){
                    // this data window is going away, so don't leave the pointer laying around
                    [[[(DataWindowController*)thewindowController imageView] colWindowController] setDataWindowController:NULL];
                }
                // signal that we are done with the housekeeping
                [thewindowController setHasRowPlot:CLOSE_CLEANUP_DONE];

            }

            if ([thewindowController isKindOfClass:[DrawingWindowController class]]){
                // if we are erasing a row or column plot, let the data window know they are gone
                if([thewindowController drawingType] == ROW_DRAWING){
                    [[thewindowController dataWindowController] setHasRowPlot:-1];
                    [[[thewindowController dataWindowController] imageView] setRowLine:-1];
                    [[[thewindowController dataWindowController] imageView] setRowWindowController:NULL];
                }
                if([thewindowController drawingType] == COL_DRAWING){
                    [[thewindowController dataWindowController] setHasColPlot:-1];
                    [[[thewindowController dataWindowController] imageView] setColLine:-1];
                    [[[thewindowController dataWindowController] imageView] setColWindowController:NULL];
                }
                // signal that we are done with the housekeeping
                [thewindowController setDrawingType:CLOSE_CLEANUP_DONE];

                [[[thewindowController dataWindowController] imageView] setNeedsDisplay:YES];
            }

            [[thewindowController window ] close];
            [windowArray removeObjectAtIndex:n];

        }
        if([windowArray count] == 0){
            wraps=1;
            window_placement.origin.x = screenRect.origin.x+WINDOW_OFFSET;
            window_placement.origin.y = screenRect.size.height;
        }

    }
  */
}


void QtOma2::on_actionOpen_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("Data Files (*.dat *.o2d);;All Files (*)"));
    if (!fn.isEmpty()){
        QByteArray ba = fn.toLocal8Bit();

        fprintf(stderr,"File is: %s\n",ba.data());
        char name[CHPERLN],ext[PREFIX_CHPERLN];
        strlcpy(name,ba.data(),CHPERLN);
        int i;
        for(i=strlen(name)-1; i >= 0 ;i--){
            if(name[i] == '.') break;
        }
        strlcpy(ext,&name[i+1],PREFIX_CHPERLN);
        fprintf(stderr,"Ext: %s\n",ext);
        dropped_file(ext,name);
    }
}


// Some Sample Code That Should Be Helpful
/*
 * void TextEdit::fileNew()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFileName(QString());
    }
}

void TextEdit::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("HTML-Files (*.htm *.html);;All Files (*)"));
    if (!fn.isEmpty())
        load(fn);
}

bool TextEdit::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success)
        textEdit->document()->setModified(false);
    return success;
}

bool TextEdit::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)) )
        fn += ".odt"; // default
    setCurrentFileName(fn);
    return fileSave();
}

void TextEdit::filePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted) {
        textEdit->print(&printer);
    }
    delete dlg;
#endif
}

void TextEdit::filePrintPreview()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));
    preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit->print(printer);
#endif
}
*/

void QtOma2::closeEvent(QCloseEvent *event)
{
    numWindows=0;
    char txt[CHPERLN];
    strlcpy(txt,SETTINGSFILE,CHPERLN);
    int err = saveprefs(txt);
    fprintf(stderr,"closing the app with status %d\n",err);
}

void QtOma2::on_actionTest_triggered()
{
    showPreferences();
}

void QtOma2::fillInLabels(){
    status->fillInLabels();
}

void QtOma2::fillDataLabel1(int x, int y, DATAWORD z){
    status->fillDataLabel1( x,  y,  z);
}

void QtOma2::fillDataLabel2(int x, int y, DATAWORD z){
    status->fillDataLabel2( x,  y,  z);
}

void QtOma2::on_actionPlot_Rows_triggered()
{
    newRowPlot();
}

void QtOma2::on_actionPlot_Columns_triggered()
{
    newColPlot();
}

void QtOma2::on_actionClose_triggered()
{
    eraseWindow(numWindows-1);
}

void QtOma2::on_actionOma2_Help_triggered()
{
    QDesktopServices::openUrl(QUrl("http://oma-x.org"));
}
