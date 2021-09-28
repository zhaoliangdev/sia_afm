#include "marktarget.h"

markTarget::markTarget()
{

}

/////生成图像/////
//图像框中显示文件读取的图像
//原始图像、图像框的宽、高
QImage markTarget::getShowImg(QImage img, int width, int height)
{
    QImage showImg;

    int imgWidth = img.width(); //原始图像的宽
    int imgHeight = img.height(); //原始图像的高

    //展示图像的宽和高
    int showWidth;
    int showHeight;

    //根据最大宽/高计算的缩放比例
    double zoomData;

    //图像宽和高比较判断
    if(imgWidth >= imgHeight) //宽大于等于高
    {
        //计算缩放系数
        zoomData = double(width)/imgWidth;

        showWidth = width; //缩放后图像的宽 = 图像框的宽
        showHeight = int(zoomData * imgHeight);
    }
    else if(imgWidth < imgHeight) //高大于宽
    {
        //计算缩放系数
        zoomData = double(height)/imgHeight;

        showHeight = height; //缩放后图像的高 = 图像框的高
        showWidth = int(zoomData * imgWidth);
    }

    //根据展示图像的宽和高 缩放得到新图像
    //缩放模式 平滑缩放
    showImg = img.scaled(showWidth,showHeight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    return showImg;
}

//根据原始图像、需要拷贝原始图像部分的左上顶点、新图像的宽、高
//返回值是一幅新的图像
QImage markTarget::getCopyImg(QImage img, int leftTopX, int leftTopY, int width, int height)
{
    QImage tmpImg;

    //从原始图像拷贝
    tmpImg = img.copy(leftTopX,leftTopY,width,height);

    return tmpImg;
}

//根据原始图像、宽、高生成新的图像
//返回值是一幅新的图像
QImage markTarget::getTargetImg(QImage img, int width, int height)
{
    QImage tmpImg;

    //scale缩放
    //缩放模式 选择清晰度高的平滑缩放模式
    tmpImg = img.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    return tmpImg;
}
