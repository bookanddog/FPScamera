
/************************************
/
/
/
/        ͷ�ļ�����
/
/
/
/************************************/
//���û�ж���stdafx.h������
#ifndef __STDAFX_H__
#define __STDAFX_H__

//��������ͷ�ļ�
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Mmsystem.h>

//����openglͷ�ļ�
#include <gl/GL.h>
#include <gl/GLU.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <glut.h>
#include <glaux.h>
#include <glext.h>

//����opengl���ӿ��ļ�
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "WINMM.lib")

//��ֹ��������ת������
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)
#pragma warning(disable: 4018)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)


#endif