#include "videoRoutines.h"

extern QtOma2* wPointer;

static cv::VideoWriter outputVideo;
static cv::Mat frame;
static cv::Size frame_size;


int vidAddFrame_q(int n,char* args){
    using namespace cv;
    extern ImageBitmap iBitmap;
    //frame = Mat(iBitmap.getheight(), iBitmap.getwidth(), CV_8UC3, iBitmap.getBGRpixdata());

    QImage theImage = wPointer->getVideoFrame();
    frame = Mat(theImage.height(), theImage.width(), CV_8UC3, theImage.bits());

    outputVideo.write(frame);
    return NO_ERR;
}

int vidOpenFile_q(int n,char* args){
    using namespace cv;
    int frames_per_second=15;
    extern ImageBitmap iBitmap;
    char filename[CHPERLN];

    int narg = sscanf(args,"%d %s",&frames_per_second,filename);
    if(narg !=2){
        beep();
        printf("Two arguments needed: framesPerSecond filename\n");
        return CMND_ERR;
    }

    QImage theImage = wPointer->getVideoFrame();
    frame = Mat(theImage.height(), theImage.width(), CV_8UC3, theImage.bits());
    frame_size= Size(theImage.width(), theImage.height());

    //qDebug()<<theImage.bytesPerLine()<<" "<<theImage.height()<<endl;

    outputVideo = VideoWriter(fullname(filename,SAVE_DATA_NO_SUFFIX), VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                frames_per_second, frame_size, true);

    if(outputVideo.isOpened()){
        //frame = Mat(iBitmap.getheight(), iBitmap.getwidth(), CV_8UC3, iBitmap.getBGRpixdata());
        outputVideo.write(frame);
        return NO_ERR;
    } else {
        beep();
        printf("Could not open VideoWriter.\n");
        return FILE_ERR;
    }
}

int vidCloseFile_q(int n,char* args){
    using namespace cv;
    outputVideo.release();
    return NO_ERR;
}
