#include "qtimagesupport.h"
#include "oma2UiIndependent/ImageBitmap.h"
#include <QtGui>

int readTiff(char* filename,Image* im){
    QString name = QString(filename);
    QImage image = QImage(name,0);
    if(!image.isNull()){
        printf("Success!\n");
        int w = image.width();
        int h = image.height();
        int fmt = image.format();
        int depth = image.depth();

        printf("Format: %d\nDepth: %d\n",fmt,depth);
        DATAWORD *databuf,*datr,*datg,*datb;
        switch(fmt){
        case QImage::Format_Indexed8:
            *im = Image(h,w);
            databuf = im->data;
            im->specs[IS_COLOR] = 0;
            for(int i = 0; i < h; i++){
                unsigned char* byte = image.scanLine(i);
                for(int j = 0; j<w; j++){
                    *databuf++ = *byte++;
                }
            }
            return NO_ERR;

        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
            *im = Image(h*3,w);
            datr = im->data;
            datg = datr + w*h;
            datb = datg + w*h;
            im->specs[IS_COLOR] = 1;
            for(int i = 0; i < h; i++){
                unsigned char* byte = image.scanLine(i);
                for(int j = 0; j<w; j++){
                    *datb++ = *byte++;
                    *datg++ = *byte++;
                    *datr++ = *byte++;
                    byte++;
                }
            }
            return NO_ERR;

        }
    }

    *im = Image(50,50);
    return FILE_ERR;
}

// this is just the same as the routine above for reading tiff files for now
int readJpeg(char* filename,Image* im)
{
    QString name = QString(filename);
    QImage image = QImage(name,0);
    if(!image.isNull()){
        printf("Success!\n");
        int w = image.width();
        int h = image.height();
        int fmt = image.format();
        int depth = image.depth();

        printf("Format: %d\nDepth: %d\n",fmt,depth);
        DATAWORD *databuf,*datr,*datg,*datb;
        switch(fmt){
        case QImage::Format_Indexed8:
            *im = Image(h,w);
            databuf = im->data;
            im->specs[IS_COLOR] = 0;
            for(int i = 0; i < h; i++){
                unsigned char* byte = image.scanLine(i);
                for(int j = 0; j<w; j++){
                    *databuf++ = *byte++;
                }
            }
            return NO_ERR;


        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
            *im = Image(h*3,w);
            datr = im->data;
            datg = datr + w*h;
            datb = datg + w*h;
            im->specs[IS_COLOR] = 1;
            for(int i = 0; i < h; i++){
                unsigned char* byte = image.scanLine(i);
                for(int j = 0; j<w; j++){
                    *datb++ = *byte++;
                    *datg++ = *byte++;
                    *datr++ = *byte++;
                    byte++;
                }
            }
            return NO_ERR;

        }
    }

    *im = Image(50,50);
    return FILE_ERR;
}

int saveJpeg(char* filename){
    extern ImageBitmap iBitmap;
    QImage currentWindowImage = QImage(iBitmap.getpixdata(), iBitmap.getwidth(), iBitmap.getheight(),
        iBitmap.getwidth()*3, QImage::Format_RGB888);
    QImageWriter myImageWriter;
    myImageWriter.setFileName(QString(filename));
    myImageWriter.setQuality(80);
    myImageWriter.setFormat("jpg");
    return myImageWriter.write(currentWindowImage);
}

int savePdf(char* filename)
{
    beep();
    printf("Not implemented in QtOma2.\n");
  return CMND_ERR;

}

