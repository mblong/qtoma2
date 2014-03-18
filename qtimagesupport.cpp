#include "qtimagesupport.h"

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
            break;
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
            break;
        }
    }

    *im = Image(50,50);
    return FILE_ERR;
}

// this is just the same as the routine above for rading tiff fils for now

int read_jpeg(char* filename,int thecolor,Image* im)
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
            break;

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
            break;
        }
    }

    *im = Image(50,50);
    return FILE_ERR;
}