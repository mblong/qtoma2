#ifndef oma2_commands_1_h
#define oma2_commands_1_h


#include "image.h"
#include "ImageBitmap.h"
#include "UI.h"

#ifdef MacOSX_UI
extern "C" {
#include "tiffio.h"
}
#endif

// Commands

int null_c(int,char*);

int addfile_c(int,char*);
int addtmp_c(int,char*);
int accumulate_c(int,char*);
int acdelete_c(int,char*);
int acget_c(int,char*);
int acadd_c(int,char*);
int addRGB_c(int,char*);
int absolute_c(int,char*);
int acmevelocity_c(int,char*);

int bit8_c(int,char*);
int bit16_c(int,char*);
int binarguments_c(int,char*);
int binextension_c(int,char*);
int boundbox_c(int, char*);
int bleed_c(int, char*);

int colorflag_c(int,char*);
int compositefile_c(int,char*);
int comtmp_c(int n, char* args);
int croprectangle_c(int,char*);
int calc_cmd_c(int,char*);
int calcall_c(int,char*);
int clip_c(int,char*);
int clipfraction_c(int,char*);
int clipbottom_c(int,char*);
int clipfbottom_c(int,char*);
int clearbad_c(int,char*);
int cclearbad_c(int,char*);
void colorClearBad(Image* image);
int createfile_c(int n,char* args);
int concatfile_c(int n,char* args);
int closefile_c(int n,char* args);
int columns_c(int n,char* args);
int c2rgb_c(int n,char* args);

int dcrawarg_c(int,char*);
int divfile_c(int,char*);
int diffx_c(int,char*);
int diffy_c(int,char*);
int divtmp_c(int,char*);
int divide_c(int,char*);
int delay_c(int,char*);
int dx_c(int n,char* args);
int dy_c(int n,char* args);
int doc2rgb_c(int n, char* args);
int doc2color_c(int n, char* args);
int divRGB_c(int,char*);
int disp2rgb_c(int,char*);
int demosaic_c(int,char*);
int decodeHobj_c(int,char*);
int dsaturate_c(int,char*);

int echo_c(int,char*);
int extra_c(int,char*);
int exposure_c(int,char*);

int fopen_c (int,char*);
int fclose_c (int,char*);
int fecho_c (int,char*);
int frame_c(int n, char* args);
int framecntr_c(int n, char* args);
int ftemp_c(int, char*);
int findbad_c(int,char*);
int fwdatm_c(int n,char* args);
int fold_c(int n, char* args);
#if defined(MacOSX_UI)
    int flippid_c(int n, char* args);
#endif

int getfile_c(int,char*);
int getFileNames_c(int,char*);
int getNext_c(int,char*);
int getfun_c(int,char*);
int gaussian_c(int,char*);
int grey2rgb_c(int,char*);
int gmacro_c(int,char*);
int gradient_c(int,char*);
int gsmooth_c(int, char*);
int gtemp_c(int, char*);
int getmatch_c(int, char*);
int gnoise_c(int, char*);
int getbin_c(int,char*);
int getangle_c(int,char*);

int hdrAccumulate_c(int,char*);
int hdrAcadd_c(int,char*);
int hdrAcdelete_c(int,char*);
int hdrAcget_c(int,char*);
int hdrNumget_c(int,char*);
int hobjSettings_c(int,char*);


int invert_c(int,char*);
int integrate_c(int,char*);
int intfill_c(int,char*);
int integrateFill(int,char*,int);
int im2Sequence_c(int,char*);

int killBox_c(int,char*);

int list_c(int,char*);
int ltemp_c(int,char*);
int lookup_c(int,char*);
int ln_c(int,char*);

int minus_c(int,char*);
int mirror_c(int,char*);
int mulfile_c(int,char*);
int multmp_c(int,char*);
int multiply_c(int,char*);
int mulRGB_c(int,char*);
int maskGreater_c(int,char*);
int maskLess_c(int,char*);
int match_c(int, char*);

#if defined(Qt_UI)  || defined(Qt_UI_Win) || defined(Qt_UI_Linux)
int mySqServer_q(int, char*);
int mySqTable_q(int, char*);
#endif

int newWindow_c(int,char*);
int nextFile_c(int,char*);
int nextPrefix_c(int,char*);
int noise_c(int, char*);
int nan2zero_c(int,char*);

int openfile_c(int,char*);

int pixSize_c(int,char*);
int palette_c(int,char*);
int plus_c(int,char*);
int power_c(int,char*);
int positive_c(int, char*);
int powRGB_c(int,char*);
int pixValue_c(int,char*);

int rectan_c(int,char*);
int resize_c(int,char*);
int rgb2grey_c(int,char*);
int rgb2red_c(int,char*);
int rgb2green_c(int,char*);
int rgb2blue_c(int,char*);
int rgb2color_c(int,char*);
int rotate_c(int,char*);
int readbad_c(int,char*);
int ramp_c(int,char*);
int roundUp_c(int,char*);
int roundoff_c(int,char*);
int rows_c(int,char*);
int ruler_c(int, char*);
int remap_c(int, char*);

int setcminmax_c(int,char*);
int size_c(int,char*);
int sinGrid_c(int, char*);
int smooth_c(int,char*);
int stemp_c(int, char*);
int stringmacro_c(int,char*);
int subfile_c(int,char*);
int subtmp_c(int, char*);
int savefile_c(int,char*);
int satiff_c(int,char*);
int saveJpg_c(int,char*);
int savePdf_c(int,char*);
int satiffscaled_c(int,char*);
int sysCommand_c(int,char*);
int seq2hdr_c(int,char*);
int seq2Image_c(int,char*);
int subRGB_c(int,char*);
int snr_c(int,char*);
int shotnoise_c(int,char*);
int say_c(int,char*);
int scatter_c(int,char*);

#if defined(Qt_UI)  || defined(Qt_UI_Win) || defined(Qt_UI_Linux)
int sqlAdd_q(int,char*);
int sqlNew_q(int,char*);
int sqlOpen_q(int,char*);
int sqlClose_q(int,char*);
#endif


int tsmooth_c(int,char*);

int uprefix_c(int n,char* args);
int unfold_c(int n,char* args);

#if defined(Qt_UI)
int vidAddFrame_q(int,char*);
int vidOpenFile_q(int,char*);
int vidCloseFile_q(int,char*);
#endif

int warpar_c(int, char*);
int warp_c(int, char*);
int writebad_c(int,char*);

int x0_c(int n,char* args);
int y0_c(int n,char* args);

// Others

int temp_image_index (char* name,int define);
int loadmatch(char* name);
int loadwarp(char* name);
float xwarp(float,float),ywarp(float,float);

void update_UI();    

char* fullname(char* fnam,int  type);
int calc(point,point);

float tabfun(int,float);

float ranfGauss();
void decodeHobj(Image*,int,int);

#endif
