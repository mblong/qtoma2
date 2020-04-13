#include "videoRoutines.h"

#ifdef Qt_UI_Mac

extern QtOma2* wPointer;

static cv::VideoWriter outputVideo;
static cv::Mat frame;
static cv::Size frame_size;
static bool videoOpened = 0;

int vidAddFrame_q(int n,char* args){
    using namespace cv;

    QImage theImage = wPointer->getVideoFrame();
    if(videoOpened && theImage.bits() && theImage.width() && theImage.height()){
        frame = Mat(theImage.height(), theImage.width(), CV_8UC3, theImage.bits());
        outputVideo.write(frame);
        return NO_ERR;
    }
    beep();
    printf("Open video file first.\n");
    return FILE_ERR;
}

int vidOpenFile_q(int n,char* args){
    using namespace cv;
    int frames_per_second=15;
    char filename[CHPERLN];

    int narg = sscanf(args,"%d %s",&frames_per_second,filename);
    if(narg !=2){
        beep();
        printf("Two arguments needed: framesPerSecond filename\n");
        return CMND_ERR;
    }
    if(videoOpened){
        beep();
        printf("A video file is already open.\n");
        return CMND_ERR;
    }

    QImage theImage = wPointer->getVideoFrame();
    frame = Mat(theImage.height(), theImage.width(), CV_8UC3, theImage.bits());
    frame_size= Size(theImage.width(), theImage.height());

    //qDebug()<<theImage.bytesPerLine()<<" "<<theImage.height()<<endl;

    outputVideo = VideoWriter(fullname(filename,SAVE_DATA_NO_SUFFIX), VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                frames_per_second, frame_size, true);

    if(outputVideo.isOpened()){
        outputVideo.write(frame);
        videoOpened=1;
        return NO_ERR;
    } else {
        beep();
        printf("Could not open VideoWriter.\n");
        return FILE_ERR;
    }
}

int vidCloseFile_q(int n,char* args){
    using namespace cv;
    if(videoOpened){
        outputVideo.release();
        videoOpened=0;
        return NO_ERR;
    }
    beep();
    printf("No open video file.\n");
    return FILE_ERR;

}

#endif
