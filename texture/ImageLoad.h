//==================================================================
/**
*  @file: imageLod.h
*  文件描述： 载入纹理类
*
*
*
*
*
*/
//===================================================================


//bpm位图
#ifndef __IMAGELOAD_H__
#define __IMAGELOAD_H__

#include "stdafx.h"

#define BITMAP_ID 0x4D42  //位图文件标志

//位图载入类
class BMPLoad
{
	public:
		BMPLoad();
		~BMPLoad();

		bool LoadBMP(const char *filename);      //装载bmp文件
		void FreeImage();					//释放图像数据
		bool Load(const char *filename);

		unsigned int ID;				//生成纹理的ID号
		int imageWidth,					//图像宽高
			imageHeight;
		unsigned char *image;			//指向图像数据的指针
		//unsigned int type;
};

//tga文件载入类
class TGALoad
{
	public:
		//构造函数
		TGALoad();
		~TGALoad();

		bool LoadTGA(const char *file);     //载入tga文件
		void FreeImage();					//释放内存
		bool Load(const char *filename);	//载入tga问价位纹理
		
		unsigned int ID;
		int imageWidth,
			imageHeight;
		unsigned char *image;
		unsigned int type;					//图像类型：GL_RGB或者GL_RGBA

};


#endif