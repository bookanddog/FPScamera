//==================================================================
/**
*  @file: imageLod.h
*  �ļ������� ����������
*
*
*
*
*
*/
//===================================================================


//bpmλͼ
#ifndef __IMAGELOAD_H__
#define __IMAGELOAD_H__

#include "stdafx.h"

#define BITMAP_ID 0x4D42  //λͼ�ļ���־

//λͼ������
class BMPLoad
{
	public:
		BMPLoad();
		~BMPLoad();

		bool LoadBMP(const char *filename);      //װ��bmp�ļ�
		void FreeImage();					//�ͷ�ͼ������
		bool Load(const char *filename);

		unsigned int ID;				//���������ID��
		int imageWidth,					//ͼ����
			imageHeight;
		unsigned char *image;			//ָ��ͼ�����ݵ�ָ��
		//unsigned int type;
};

//tga�ļ�������
class TGALoad
{
	public:
		//���캯��
		TGALoad();
		~TGALoad();

		bool LoadTGA(const char *file);     //����tga�ļ�
		void FreeImage();					//�ͷ��ڴ�
		bool Load(const char *filename);	//����tga�ʼ�λ����
		
		unsigned int ID;
		int imageWidth,
			imageHeight;
		unsigned char *image;
		unsigned int type;					//ͼ�����ͣ�GL_RGB����GL_RGBA

};


#endif