//
//  UI.h
//  oma2
//
//  Created by Marshall Long on 3/26/12.
//  Copyright (c) 2012 Yale University. All rights reserved.
//

#ifndef oma2_UI_h
#define oma2_UI_h

#include "ImageBitmap.h"
#include "image.h"
#include "commands_1.h"
#include "comdec.h"

// do this at compile time with -DQt_UI (for example)
//#define MacOSX_UI
//#define Qt_UI

#ifdef MacOSX_UI

#include "StatusController.h"
#include "AppController.h"

// In this exceptional case, define external variable in this header file
extern AppController *appController;
extern StatusController *statusController;

enum {CROSS,SELRECT,CALCRECT,RULER,LINEPLOT};


#define printf omaprintf
/*
 #define display_data [appController showDataWindow:(char*) args];
 #define erase_window [appController eraseWindow:(int) n];
 #define label_data [appController labelDataWindow:(char*) args];
 #define label_data_minMax [appController labelMinMax];
 */

#define checkEvents ;

#define WMODE   O_CREAT|O_WRONLY,0666
#define READMODE   O_RDONLY
#define READBINARY   O_RDONLY

#ifndef SETTINGSFILE
#define SETTINGSFILE "Contents/Resources/OMA Settings"
#define PALETTEFILE	"Contents/Resources/OMApalette.pa1"
#define PALETTEFILE2 "Contents/Resources/OMApalette2.pa1"
#define PALETTEFILE3 "Contents/Resources/OMApalette3.pa1"

#define HELPFILE "Contents/Resources/oma2help.txt"
#define HELPURL "Contents/Resources/LightOma2Help/index.html"
#endif


// try this so that the command thread doesn't mess with things that need to be in the main thread

#define display_data if(dispatch_get_main_queue() == dispatch_get_current_queue()) \
[appController showDataWindow:(char*) args]; \
else \
dispatch_sync(dispatch_get_main_queue(),^{[appController showDataWindow:(char*) args];});


#define erase_window if(dispatch_get_main_queue() == dispatch_get_current_queue()) \
[appController eraseWindow:(int) n]; \
else \
dispatch_sync(dispatch_get_main_queue(),^{[appController eraseWindow:(int) n];});

#define label_data if(dispatch_get_main_queue() == dispatch_get_current_queue()) \
[appController labelDataWindow:(char*) args]; \
else \
dispatch_sync(dispatch_get_main_queue(),^{[appController labelDataWindow:(char*) args];});

#define label_data_minMax if(dispatch_get_main_queue() == dispatch_get_current_queue()) \
[appController labelMinMax]; \
else \
dispatch_sync(dispatch_get_main_queue(),^{[appController labelMinMax];});



BOOL dropped_file(char*,char*);
void update_UI();
void alertSound(char*);

int omaprintf(const char* format, ...);
int pprintf(const char* format, ...);

#endif


#ifdef Qt_UI

#include <QApplication>
#include "qtoma2.h"

#define display_data displayData(args);
#define erase_window eraseWindow(n);
#define label_data ;
#define label_data_minMax ;
#define checkEvents QCoreApplication::processEvents();

#define pprintf omaprintf
#define printf omaprintf
#define nil 0

#define WMODE   O_CREAT|O_WRONLY,0666
#define READMODE   O_RDONLY
#define READBINARY   O_RDONLY

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wsign-compare"

// dcraw needs these
#define ABS(x) (((int)(x) ^ ((int)(x) >> 31)) - ((int)(x) >> 31))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#ifndef SETTINGSFILE
#define SETTINGSFILE "../Resources/OMA Settings"
#define PALETTEFILE	"../Resources/OMApalette.pa1"
#define PALETTEFILE2 "../Resources/OMApalette2.pa1"
#define PALETTEFILE3 "../Resources/OMApalette3.pa1"

#define HELPFILE "../Resources/oma2help.txt"
#define HELPURL "../Resources/LightOma2Help/index.html"
#endif


enum {CROSS,SELRECT,CALCRECT,RULER,LINEPLOT,NEWROW,NEWCOL};

typedef struct{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} RGBColor;

typedef struct{
    int h;
    int v;
} Point;

typedef char* Ptr;

typedef char BOOL;
typedef char Boolean;
#define NO 0
#define YES 1

int omaprintf(const char* format, ...);
void alertSound(char*);
void beep();
void displayData(char*);
void eraseWindow(int);
BOOL dropped_file(char*,char*);

#endif

#ifdef Qt_UI_Win

#include <QApplication>
#include "qtoma2.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>

#define display_data displayData(args);
#define erase_window eraseWindow(n);
#define label_data ;
#define label_data_minMax ;
#define checkEvents QCoreApplication::processEvents();

#define pprintf omaprintf
#define printf omaprintf
#define nil 0

// dcraw needs these
#define ABS(x) (((int)(x) ^ ((int)(x) >> 31)) - ((int)(x) >> 31))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define DJGPP 1

#define WMODE   _O_CREAT|_O_WRONLY|_O_BINARY|_O_TRUNC,S_IWUSR|S_IRUSR
#define READMODE   O_RDONLY
#define READBINARY   O_RDONLY|O_BINARY


#ifndef SETTINGSFILE
#define SETTINGSFILE "./OMA Settings"
#define PALETTEFILE	"./OMApalette.pa1"
#define PALETTEFILE2 "./OMApalette2.pa1"
#define PALETTEFILE3 "./OMApalette3.pa1"

#define HELPFILE "./oma2help.txt"
#define HELPURL "./LightOma2Help/index.html"
#endif
/*
#pragma gcc diagnostic ignored "-Wsign-compare"
#pragma gcc diagnostic ignored "-Wwrite-strings"
#pragma gcc diagnostic ignored "-Wunused-variable"
#pragma gcc diagnostic ignored "-Wunused-but-set-variable"
#pragma gcc diagnostic ignored "-Wcomment"
#pragma gcc diagnostic ignored "-Wtype-limits"
*/


enum {CROSS,SELRECT,CALCRECT,RULER,LINEPLOT,NEWROW,NEWCOL};

typedef struct{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} RGBColor;

typedef struct{
    int h;
    int v;
} Point;

typedef char* Ptr;

typedef int BOOL;
typedef char Boolean;
#define NO 0
#define YES 1
#define strlcpy strncpy
#define strlcat strncat

int omaprintf(const char* format, ...);
void alertSound(char*);
void beep();
void displayData(char*);
void eraseWindow(int);
BOOL dropped_file(char*,char*);

#endif


#ifdef Qt_UI_Linux

#include <QApplication>
#include "qtoma2.h"

#define display_data displayData(args);
#define erase_window eraseWindow(n);
#define label_data ;
#define label_data_minMax ;
#define checkEvents QCoreApplication::processEvents();

#define pprintf omaprintf
#define printf omaprintf
#define nil 0

#define strlcpy strncpy
#define strlcat strncat

#define _H_INTTYPES
#define _ALL_SOURCE

#define WMODE   O_CREAT|O_WRONLY,0666
#define READMODE   O_RDONLY
#define READBINARY   O_RDONLY

#define ANDOR_


#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wtype-limits"


// dcraw needs these
#define ABS(x) (((int)(x) ^ ((int)(x) >> 31)) - ((int)(x) >> 31))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#ifndef SETTINGSFILE
#define SETTINGSFILE "./OMA Settings"
#define PALETTEFILE	"./OMApalette.pa1"
#define PALETTEFILE2 "./OMApalette2.pa1"
#define PALETTEFILE3 "./OMApalette3.pa1"

#define HELPFILE "./oma2help.txt"
#define HELPURL "./LightOma2Help/index.html"
#endif

typedef struct{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} RGBColor;

typedef struct{
    int h;
    int v;
} Point;

typedef char* Ptr;

typedef char BOOL;
typedef char Boolean;
#define NO 0
#define YES 1
enum {CROSS,SELRECT,CALCRECT,RULER,LINEPLOT,NEWROW,NEWCOL};

int omaprintf(const char* format, ...);
void alertSound(char*);
void beep();
void displayData(char*);
void eraseWindow(int);
BOOL dropped_file(char*,char*);


#endif

#endif
