#ifndef MARKTARGET_H
#define MARKTARGET_H

#include <QImage>

class markTarget
{
public:
    markTarget();

public:
    /////生成图像/////
    //图像框中显示文件读取的图像
    //原始图像、图像框的宽、高
    QImage getShowImg(QImage img,int width,int height);

    //根据原始图像、需要拷贝的图像左上顶点、新图像的宽、高
    //返回值是一幅新的图像
    QImage getCopyImg(QImage img,int leftTopX,int leftTopY,int width,int height);

    //根据原始图像、宽、高生成新的图像
    //返回值是一幅新的图像
    QImage getTargetImg(QImage img,int width,int height);

};

#endif // MARKTARGET_H
