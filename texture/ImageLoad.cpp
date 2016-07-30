//========================================================
/*
*@file:			imageLoad.cpp
*项目描述纹理映射
*文件描述载入各类纹理类，目前提供bmp和tga类
*
*
*
*/
//=========================================================


#include "ImageLoad.h"

//bmp类

//构造函数
BMPLoad::BMPLoad()
{
	//初始化成员值为0
	image = 0;
	imageWidth = 0;
	imageHeight = 0;
}

//析构函数
BMPLoad::~BMPLoad()
{
	FreeImage();			//释放图像数据占据的内存
}

//装载一个位图文件
bool BMPLoad::LoadBMP(const char *filename)
{
	FILE *pFile = 0;  //文件指针

	//创建位图文件信息和位图文件头结构
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;

	unsigned char textureColors = 0;   //将颜色从bgr转换到rgb

	//打开文件并检查错误
	fopen_s(&pFile ,filename, "rb");
	if(pFile == 0)return false;

	//读入位图文件头信息
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);

	//检查该文件是否是位图文件
	if(header.bfType != BITMAP_ID)
	{
		fclose(pFile);
		return false;
	}

	//读入位图文件信息
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	//保存图像的宽高
	imageWidth = bitmapInfoHeader.biWidth;
	imageHeight = bitmapInfoHeader.biHeight;

	//确保读取数据的大小,当格式为BI_RGB格式是可为0
	if(bitmapInfoHeader.biSizeImage == 0)
	{
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	}

	//将指针移到数据开始的位置
	fseek(pFile, header.bfOffBits, SEEK_SET);

	//分配内存
	image = new unsigned char[bitmapInfoHeader.biSizeImage];

	//检查内存分配是否成功
	if(!image)
	{
		delete[] image;
		fclose(pFile);
		return false;
	}

	//读取图像数据
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	//将图像颜色格式交换，由bgr转换为RGB
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index += 3)
	{
		textureColors = image[index];
		image[index] = image[index + 2];
		image[index + 2] = textureColors;
	}

	fclose(pFile);
	return true;
}

//载入位图文件并创建纹理
bool BMPLoad::Load(const char *filename)
{
	if(!LoadBMP(filename))
	{
		MessageBox(NULL, TEXT("载入位图文件失败"),TEXT("错误"), MB_OK);
		exit(0);
	}
	//生成纹理对象名称
	glGenTextures(1, &ID);
	//创建纹理对象
	glBindTexture(GL_TEXTURE_2D, ID);

	//控制滤波，纹理过滤与环绕,  控制纹理参数 
	/*==============================
	*这里改动会比较多做个记号
	*/
	//==================================
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	//创建纹理mip贴图
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);

	return true;
}

//释放内存
void BMPLoad::FreeImage()
{
	if(image)
	{
		delete[] image;
		image = 0;
	}
}


//tga类

/** 构造函数 */
TGALoad::TGALoad()
{
  /** 设置为默认值 */
  image = 0;
  type = 0;
  ID = -1;
  imageWidth = 0;
  imageHeight = 0;
}

/** 析构函数 */
TGALoad::~TGALoad()
{
	FreeImage();
}

//载入TGA文件
bool TGALoad::LoadTGA(const char *file)
{
	FILE *pFile;
	unsigned char tempColor;                              //交换颜色分量
	unsigned char bitCount;								//没像素的bit位数
	int colorMode;										//颜色模式
	long TGASize;										//TGA文件大小
	unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //未压缩TGA文件头
	unsigned char TGAHeader[12];                      //文件头
	unsigned char header[6];                          //文件头前6个字节

	//检查文件是否为空
	if(!file)
	{
		return false;
	}

	//打开文件
	fopen_s(&pFile, file, "rb");
	if(!pFile) return false;

	//读取文件头前12个字节
	fread(TGAHeader, 1, sizeof(TGAHeader), pFile);

	//比较文件是否为未压缩文件//=======================
	/*
	*搞不懂unCompressHeader就只有一个2的值，肯定不跟TGAHeader一样吧
	*搞不懂标记先
	*/
	//===================================================
	if(memcmp(unCompressHeader, TGAHeader, sizeof(unCompressHeader)) != 0)
	   {
		   MessageBox(NULL,TEXT("文件类型错误!"),TEXT("错误"),MB_OK);
		   fclose(pFile);
		   return false;
	   }
	
	//读取6个字节
	fread(header, 1, sizeof(header), pFile);

	//计算图像的宽度和高度
	imageWidth = header[1] * 256 + header[0];
    imageHeight = header[3] * 256 + header[2];

   /** 获取每象素的bit位数 */
   bitCount = header[4];

   /**　计算颜色模式和图像大小 */
   colorMode = bitCount / 8;
   TGASize = imageWidth * imageHeight * colorMode;

    /** 分配内存 */
   image = new unsigned char[sizeof(unsigned char) * TGASize];

      /** 读取数据 */
   fread(image, sizeof(unsigned char), TGASize, pFile);

      /** 将BGA格式转化为RGA格式 */
   for(long index = 0; index < TGASize; index += colorMode)
	   {
		   tempColor = image[index];
		   image[index] = image[index + 2];
		   image[index + 2] = tempColor;
	   }

      /** 关闭文件 */
   fclose(pFile);

   /** 设置图象类型 */
   if(colorMode == 3) 
	   type = GL_RGB;
   else 
	   type = GL_RGBA;

   return true;
}

/** 载入TGA文件并创建纹理 */
bool TGALoad::Load(const char* fileName)
{
	if(!LoadTGA(fileName))
	{
		MessageBox(NULL,TEXT("载入TGA文件失败!"),TEXT("错误"),MB_OK);
		exit(0);
	}
	/** 生成纹理对象名称 */
	glGenTextures(1, &ID);
   
    /** 创建纹理对象 */
    glBindTexture(GL_TEXTURE_2D, ID);
	
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   
	/** 创建纹理 */
   	gluBuild2DMipmaps(GL_TEXTURE_2D, type, imageWidth,
	                  imageHeight, type, GL_UNSIGNED_BYTE,
	                  image);
   return true;
}

void TGALoad::FreeImage()
{
   /** 释放内存 */
   if(image)
      {
         delete[] image;
         image = 0;
      }
}

