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

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(onApplicationFocusChanged(QWidget*,QWidget*)));

    QString appPath =  qApp->applicationDirPath();
    QByteArray ba = appPath.toLocal8Bit();
    strlcpy(applicationPath ,ba.data(),CHPERLN);
    fprintf(stderr,"App path is:%s\n",applicationPath);

    QDesktopWidget widget;
    mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    window_placement.x = (mainScreenSize.x()+WINDOW_OFFSET);
    window_placement.y = (mainScreenSize.y()+WINDOW_OFFSET);
    status = new Status(this);
    status->setGeometry(COMMAND_WIDTH+STATUS_OFFSET,mainScreenSize.y()+mainScreenSize.height()-WINDOW_HEIGHT,STATUS_WIDTH,WINDOW_HEIGHT);
    status->show();


    wraps = 1;
    windowRow = 0;
    numWindows = 0;
    currentDataWindow = -1;
    variablesWindow = 0;
    showVariables();
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

void QtOma2::on_omaCommands_textChanged()
{
    if(programmedText) return;
    QString thetext = ui->omaCommands->toPlainText();
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
    ui->omaCommands->insertPlainText(QString(string));
    lastReturn += strlen(string);
    programmedText = 0;
    ui->omaCommands->ensureCursorVisible();
}

void QtOma2::addForwardedCharacter(QString string)
{
    ui->omaCommands->insertPlainText(string);
}

void  QtOma2::deleteCharacter(){
    QTextCursor cursor =ui->omaCommands->textCursor();
    cursor.deletePreviousChar();
}

void QtOma2::keyPressEvent(QKeyEvent *event)
{
    extern int stopMacroNow;
    if(event->key() == Qt::Key_Escape)
    {
        stopMacroNow = 1;
    }
}


void QtOma2::previousHistory(){
    extern char cmnd_history[];
    extern int hist_index;
    extern int selected_hist_index;
    char gets_string[CHPERLN];

    if(selected_hist_index > 0) {
        selected_hist_index-=2;
        while(selected_hist_index >= 0 && cmnd_history[selected_hist_index] !=0) {
            selected_hist_index--;
        }
        selected_hist_index++;
        strcpy(gets_string,&cmnd_history[selected_hist_index]);
        QString thetext = ui->omaCommands->toPlainText();

        if (lastReturn == thetext.size() ) {
            // we are at the end of the text -- just add the last command
            addCString(gets_string);
            lastReturn -= strlen(gets_string);
        } else {
            // need to get rid of the last bit that hasn't been treated as a command
            QTextCursor cursor =ui->omaCommands->textCursor();
            cursor.setPosition(lastReturn);
            cursor.setPosition(thetext.size(), QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            // then add the text
            addCString(gets_string);
            lastReturn -= strlen(gets_string);
        }
    }
    return;
}

void QtOma2::nextHistory(){
    extern char cmnd_history[];
    extern int hist_index;
    extern int selected_hist_index;
    char gets_string[CHPERLN];

    if(selected_hist_index < hist_index) {

        while(cmnd_history[selected_hist_index] !=0) {
            selected_hist_index++;
        }
        selected_hist_index++;
        if(selected_hist_index < hist_index){
            QString thetext = ui->omaCommands->toPlainText();
            strcpy(gets_string,&cmnd_history[selected_hist_index]);
            // need to get rid of the last bit that hasn't been treated as a command
            QTextCursor cursor =ui->omaCommands->textCursor();
            cursor.setPosition(lastReturn);
            cursor.setPosition(thetext.size(), QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            // then add the text
            addCString(gets_string);
            lastReturn -= strlen(gets_string);
        }
    }
    return;

}

void QtOma2::showPreferences(){
    prefs = new Preferences(this);
    prefs->show();
}

void QtOma2::showVariables(){
    variablesWindow = new VariablesWindow(this);
    variablesWindow->setGeometry(COMMAND_WIDTH+STATUS_OFFSET*2 + STATUS_WIDTH,mainScreenSize.y()+mainScreenSize.height()-WINDOW_HEIGHT,STATUS_WIDTH,WINDOW_HEIGHT);
    variablesWindow->show();
    variablesWindow->fillInVariables();
}

void QtOma2::updateStatus(){
    status->fillInLabels();
    if(variablesWindow){
        variablesWindow->fillInVariables();
    }
    QCoreApplication::sendPostedEvents(Q_NULLPTR,QEvent::DeferredDelete); // try to force garbage collection???
    QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents;
    QCoreApplication::processEvents(flags);
}

void QtOma2::newData(char* name){
    extern float windowScaleFactor;

    if(!UIData.newwindowflag && (currentDataWindow >= 0)){
        updateData(); // dont put a new window, just use the current data window
        return;
    }
    // figure out where to place image
    // this is for possibly scaling down images that won't fit on screen
    int windowHeight = iBitmap.getheight()*windowScaleFactor;
    int windowWidth = iBitmap.getwidth()*windowScaleFactor;

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
    //ui->omaCommands->activateWindow();    // make the command window active
    window_placement.x += windowWidth+WINDOW_SPACE;            // increment for next one
}

void QtOma2::updateData(){
    if(currentDataWindow >=0)
        windowArray[currentDataWindow].dataWindow->clearLabels();
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

    //ui->omaCommands->activateWindow();    // make the command window active
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

    //ui->omaCommands->activateWindow();    // make the command window active
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

void QtOma2::newLinePlot(QPoint start, QPoint end){
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
    if(windowArray[n].dataWindow->getHasRowPlot()){
        //can't do this there is one already
        beep();
        return;
    }

    float widthScale = (float)windowArray[n].dataWindow->getDataCols()/windowArray[n].dataWindow->width();
    float heightScale = (float)windowArray[n].dataWindow->getDataRows()/windowArray[n].dataWindow->height();

    //int windowWidth = window_placement.width = windowArray[n].dataWindow->height();
    int windowWidth = window_placement.width = sqrt(powf((start.x()-end.x())/widthScale,2)+powf((start.y()-end.y())/heightScale,2));
    int dataWidth = sqrt(powf((start.x()-end.x()),2)+powf((start.y()-end.y()),2));

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

    //int theWindowCol = windowArray[n].dataWindow->width()/2;  // this is half the width of the data window
    windowArray[numWindows].drawingWindow = new DrawingWindow(this);
    windowArray[numWindows].drawingWindow->setMyDataWindow(windowArray[n].dataWindow);
    windowArray[numWindows].drawingWindow->setGeometry(placement);
    windowArray[numWindows].drawingWindow->setTheCol( -1 );
    int pal = windowArray[n].dataWindow->getThePalette();
    int bytesPerLine;
    unsigned char* bytes = windowArray[n].dataWindow->getIntensity();    // the start of the data

    //int theCol = theWindowCol * widthScale;
    int bytesPerPix;
    if(pal >= 0) { // we have a monochrome image
        //bytes += theRow * windowArray[n].dataWindow->getDataCols();
        bytesPerLine = dataWidth;
        windowArray[numWindows].drawingWindow->setIsColor(0);
        bytesPerPix = 1;
    } else {
        //bytes += theRow * windowArray[n].dataWindow->getDataCols()*3;
        bytesPerLine = dataWidth*3;
        windowArray[numWindows].drawingWindow->setIsColor(1);
        bytesPerPix = 3;
    }
    unsigned char* lineData = new unsigned char[bytesPerLine];
    int cols = windowArray[n].dataWindow->getDataCols();
    int k=0;
    float dx = (float)(end.x() - start.x())/dataWidth;
    float dy = (float)(end.y() - start.y())/dataWidth;
    int x,y;
    for(int i = 0; i< bytesPerLine/bytesPerPix;i++){
        x = start.x() + dx*i;
        y = start.y() + dy*i;
        for(int j=0; j < bytesPerPix;j++)
            *(lineData+k++) = *(bytes + (x + y*cols)*bytesPerPix + j);
        //*(colData+k++) = *(bytes + (theCol + i*cols)*bytesPerPix + j);
    }

    // tell the data window what it needs to know
    //windowArray[n].dataWindow->setColLine(theCol);
    //windowArray[n].dataWindow->setHasColPlot(windowArray[numWindows].drawingWindow);
    //if(windowArray[n].dataWindow->getHasRowPlot())
    //    windowArray[n].dataWindow->setThereIsDrawing(NEWCOL);
    //windowArray[n].dataWindow->showColLine(theCol);

    windowArray[numWindows].drawingWindow->setWidthScale(widthScale);
    windowArray[numWindows].drawingWindow->setHeightScale(heightScale);
    windowArray[numWindows].drawingWindow->setColData(lineData);
    windowArray[numWindows].drawingWindow->setBytesPer(bytesPerLine);
    windowArray[numWindows].type = LINE_DRAWING;
    windowArray[numWindows].drawingWindow->show();
    windowArray[numWindows].drawingWindow->update();

    numWindows++;
    window_placement.x += windowWidth+WINDOW_SPACE;            // increment for next one

}

void QtOma2::addDataWindowLabel(char* string, int line){
    int n = activeWindow();
    if(n<0 || windowArray[n].type  != DATA){
        //can't do this
        beep();
        return;
    }
    windowArray[n].dataWindow->label(string,line);
}

void QtOma2::addDataWindowMinMax(){
    int n = activeWindow();
    if(n<0 || windowArray[n].type  != DATA){
        //can't do this
        beep();
        return;
    }
    windowArray[n].dataWindow->labelMinMax();
}

void QtOma2::eraseWindow(int n){
    if(n < 0){  // close all
        for(int i=0; i< numWindows; i++){
            if(windowArray[i].type == DATA){
                windowArray[i].dataWindow->setRowLine(CLOSE_CLEANUP_DONE);
                windowArray[i].dataWindow-> close();
                //delete windowArray[i].dataWindow;
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
    saveprefs(txt);
    //fprintf(stderr,"closing the app with status %d\n",err);
}

void QtOma2::on_actionPreferences_triggered()
{
    showPreferences();
}

void QtOma2::on_actionShow_Variables_Window_triggered()
{
    showVariables();
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
    int n = activeWindow();
    if( n >= 0 )
        eraseWindow(n);
}

void QtOma2::on_actionOma2_Help_triggered()
{
    QDesktopServices::openUrl(QUrl("http://oma-x.org/LightOma2Help"));
}

// these next two things ensure that when clicking on the commands window when it doesn't have focus, the cursor will be at the end
void QtOma2::onApplicationFocusChanged(QWidget *old, QWidget *now){
    if( now == ui->omaCommands){
        QTimer::singleShot(50, this, SLOT(moveCursorToEnd()));   // this gives enough time so that the event that places the cursor is done
    }
}

void QtOma2::moveCursorToEnd(){
    ui->omaCommands->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}


void QtOma2::on_actionExit_triggered()
{
    closeEvent(NULL);
    exit(0);
}

void QtOma2::on_actionOpen_triggered()
{
    openDataFile();
}

void QtOma2::openDataFile()
{
    QString fn = QFileDialog::getOpenFileName(NULL, tr("Open File..."),
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

void QtOma2::on_actionSave_Data_File_triggered()
{
    saveDataFile();
}

void QtOma2::saveDataFile()
{
    extern Image iBuffer;
    extern oma2UIData UIData;
    QString fn = QFileDialog::getSaveFileName(NULL, tr("Save Data File"),
                                              QString(UIData.saveprefixbuf)+QString("oma2Data"),

                                              tr("Data Files (*.o2d);;All Files (*)"));
    if (!fn.isEmpty()){
        QByteArray ba = fn.toLocal8Bit();
        printf("File is: %s\nOMA2>",ba.data());

        iBuffer.saveFile(ba.data(),LONG_NAME);
        if(iBuffer.err())
            printf("Could not save file.\nOMA2>");
    }
}

int QtOma2::fillInDataFromPixmap( QSqlDatabase db, char* tableName){

    //extern int isMySql;
    //extern char* tableName;

    int windowNumber = activeWindow();

    if(windowNumber<0){
        //can't do this
        beep();
        return -1;
    }
    if(windowArray[windowNumber].type != DATA){
        //can't do this
        beep();
        return -1;
    }

    QSqlQuery query = QSqlQuery( db );

    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    windowArray[windowNumber].dataWindow->pixmap.save( &inBuffer, "JPG" ); // write inPixmap into inByteArray in JPG format

    // Insert image bytes into the database
    // Note: make sure to wrap the :placeholder in parenthesis

    //qDebug() << QString(tableName)<<" is table name\n";
    query.prepare( "INSERT INTO "+QString(tableName)+" (imagedata) VALUES (:imageData)" );
    query.bindValue( ":imageData", inByteArray );

    inBuffer.close();
    inByteArray.clear();

    if( !query.exec() ){
        qDebug() << "Error inserting image into table:\n" << query.lastError();
        return -1;
    }

    return NO_ERR;
}

QImage QtOma2::getVideoFrame(){
    int windowNumber = activeWindow();
    QImage theImage;
    if(windowNumber<0){
        //can't do this
        beep();
        return theImage;
    }
    if(windowArray[windowNumber].type != DATA){
        //can't do this
        beep();
        return theImage;
    }

    theImage = (windowArray[windowNumber].dataWindow->pixmap).toImage();    // convert to image

    // the data is evidently BGRX BGRX in the image -- we need BGR BGR
    uchar* data=theImage.bits();
    int j=0;
    for(int i=0; i<theImage.height()*theImage.width()*4; i++ ){
        *(data+j++)=*(data+i++);
        *(data+j++)=*(data+i++);
        *(data+j++)=*(data+i++);
    }
    return theImage;
}

void QtOma2::on_actionSave_Settings_triggered()
{
    QString fn = QFileDialog::getSaveFileName(nullptr, tr("Save Settings File"),
                                              QString(UIData.graphicsprefixbuf)+QString("oma2Settings"),
                                              tr("Settings Files (*.o2s);;All Files (*)"));
    if (!fn.isEmpty()){
        QByteArray ba = fn.toLocal8Bit();
        printf("File is: %s\nOMA2>",ba.data());
        int err = saveprefs(ba.data());
        if(err)
            printf("Could not save file.\nOMA2>");
    }

}
