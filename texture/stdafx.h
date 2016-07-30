
/************************************
/
/
/
/        头文件依赖
/
/
/
/************************************/
//如果没有定义stdafx.h，定义
#ifndef __STDAFX_H__
#define __STDAFX_H__

//包含常用头文件
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Mmsystem.h>

//包含opengl头文件
#include <gl/GL.h>
#include <gl/GLU.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <glut.h>
#include <glaux.h>
#include <glext.h>

//包含opengl链接库文件
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "WINMM.lib")

//禁止出现类型转换警告
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)
#pragma warning(disable: 4018)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)


#endif