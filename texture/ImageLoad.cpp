//========================================================
/*
*@file:			imageLoad.cpp
*��Ŀ��������ӳ��
*�ļ�����������������࣬Ŀǰ�ṩbmp��tga��
*
*
*
*/
//=========================================================


#include "ImageLoad.h"

//bmp��

//���캯��
BMPLoad::BMPLoad()
{
	//��ʼ����ԱֵΪ0
	image = 0;
	imageWidth = 0;
	imageHeight = 0;
}

//��������
BMPLoad::~BMPLoad()
{
	FreeImage();			//�ͷ�ͼ������ռ�ݵ��ڴ�
}

//װ��һ��λͼ�ļ�
bool BMPLoad::LoadBMP(const char *filename)
{
	FILE *pFile = 0;  //�ļ�ָ��

	//����λͼ�ļ���Ϣ��λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;

	unsigned char textureColors = 0;   //����ɫ��bgrת����rgb

	//���ļ���������
	fopen_s(&pFile ,filename, "rb");
	if(pFile == 0)return false;

	//����λͼ�ļ�ͷ��Ϣ
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);

	//�����ļ��Ƿ���λͼ�ļ�
	if(header.bfType != BITMAP_ID)
	{
		fclose(pFile);
		return false;
	}

	//����λͼ�ļ���Ϣ
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	//����ͼ��Ŀ��
	imageWidth = bitmapInfoHeader.biWidth;
	imageHeight = bitmapInfoHeader.biHeight;

	//ȷ����ȡ���ݵĴ�С,����ʽΪBI_RGB��ʽ�ǿ�Ϊ0
	if(bitmapInfoHeader.biSizeImage == 0)
	{
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	}

	//��ָ���Ƶ����ݿ�ʼ��λ��
	fseek(pFile, header.bfOffBits, SEEK_SET);

	//�����ڴ�
	image = new unsigned char[bitmapInfoHeader.biSizeImage];

	//����ڴ�����Ƿ�ɹ�
	if(!image)
	{
		delete[] image;
		fclose(pFile);
		return false;
	}

	//��ȡͼ������
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	//��ͼ����ɫ��ʽ��������bgrת��ΪRGB
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index += 3)
	{
		textureColors = image[index];
		image[index] = image[index + 2];
		image[index + 2] = textureColors;
	}

	fclose(pFile);
	return true;
}

//����λͼ�ļ�����������
bool BMPLoad::Load(const char *filename)
{
	if(!LoadBMP(filename))
	{
		MessageBox(NULL, TEXT("����λͼ�ļ�ʧ��"),TEXT("����"), MB_OK);
		exit(0);
	}
	//���������������
	glGenTextures(1, &ID);
	//�����������
	glBindTexture(GL_TEXTURE_2D, ID);

	//�����˲�����������뻷��,  ����������� 
	/*==============================
	*����Ķ���Ƚ϶������Ǻ�
	*/
	//==================================
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	//��������mip��ͼ
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);

	return true;
}

//�ͷ��ڴ�
void BMPLoad::FreeImage()
{
	if(image)
	{
		delete[] image;
		image = 0;
	}
}


//tga��

/** ���캯�� */
TGALoad::TGALoad()
{
  /** ����ΪĬ��ֵ */
  image = 0;
  type = 0;
  ID = -1;
  imageWidth = 0;
  imageHeight = 0;
}

/** �������� */
TGALoad::~TGALoad()
{
	FreeImage();
}

//����TGA�ļ�
bool TGALoad::LoadTGA(const char *file)
{
	FILE *pFile;
	unsigned char tempColor;                              //������ɫ����
	unsigned char bitCount;								//û���ص�bitλ��
	int colorMode;										//��ɫģʽ
	long TGASize;										//TGA�ļ���С
	unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //δѹ��TGA�ļ�ͷ
	unsigned char TGAHeader[12];                      //�ļ�ͷ
	unsigned char header[6];                          //�ļ�ͷǰ6���ֽ�

	//����ļ��Ƿ�Ϊ��
	if(!file)
	{
		return false;
	}

	//���ļ�
	fopen_s(&pFile, file, "rb");
	if(!pFile) return false;

	//��ȡ�ļ�ͷǰ12���ֽ�
	fread(TGAHeader, 1, sizeof(TGAHeader), pFile);

	//�Ƚ��ļ��Ƿ�Ϊδѹ���ļ�//=======================
	/*
	*�㲻��unCompressHeader��ֻ��һ��2��ֵ���϶�����TGAHeaderһ����
	*�㲻�������
	*/
	//===================================================
	if(memcmp(unCompressHeader, TGAHeader, sizeof(unCompressHeader)) != 0)
	   {
		   MessageBox(NULL,TEXT("�ļ����ʹ���!"),TEXT("����"),MB_OK);
		   fclose(pFile);
		   return false;
	   }
	
	//��ȡ6���ֽ�
	fread(header, 1, sizeof(header), pFile);

	//����ͼ��Ŀ�Ⱥ͸߶�
	imageWidth = header[1] * 256 + header[0];
    imageHeight = header[3] * 256 + header[2];

   /** ��ȡÿ���ص�bitλ�� */
   bitCount = header[4];

   /**��������ɫģʽ��ͼ���С */
   colorMode = bitCount / 8;
   TGASize = imageWidth * imageHeight * colorMode;

    /** �����ڴ� */
   image = new unsigned char[sizeof(unsigned char) * TGASize];

      /** ��ȡ���� */
   fread(image, sizeof(unsigned char), TGASize, pFile);

      /** ��BGA��ʽת��ΪRGA��ʽ */
   for(long index = 0; index < TGASize; index += colorMode)
	   {
		   tempColor = image[index];
		   image[index] = image[index + 2];
		   image[index + 2] = tempColor;
	   }

      /** �ر��ļ� */
   fclose(pFile);

   /** ����ͼ������ */
   if(colorMode == 3) 
	   type = GL_RGB;
   else 
	   type = GL_RGBA;

   return true;
}

/** ����TGA�ļ����������� */
bool TGALoad::Load(const char* fileName)
{
	if(!LoadTGA(fileName))
	{
		MessageBox(NULL,TEXT("����TGA�ļ�ʧ��!"),TEXT("����"),MB_OK);
		exit(0);
	}
	/** ��������������� */
	glGenTextures(1, &ID);
   
    /** ����������� */
    glBindTexture(GL_TEXTURE_2D, ID);
	
	/** �����˲� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   
	/** �������� */
   	gluBuild2DMipmaps(GL_TEXTURE_2D, type, imageWidth,
	                  imageHeight, type, GL_UNSIGNED_BYTE,
	                  image);
   return true;
}

void TGALoad::FreeImage()
{
   /** �ͷ��ڴ� */
   if(image)
      {
         delete[] image;
         image = 0;
      }
}

