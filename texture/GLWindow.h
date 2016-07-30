//==================================
/**
*@file  : glWindows.h
*������ ������
*
*
*
*/
//=================================

#ifndef __GLWIDOW_H__
#define __GLWINDOW_H__

#include "stdafx.h"

//������
class GLWindow
{
	public:
		GLWindow();
		//~glWindows();

		//�������ڣ����ڱ��⣬�����֣����Ƿ�ȫ��������ǰʵ���������������������
		bool Create(LPCSTR window_title, LPCSTR class_name, bool fullscreen,HINSTANCE hinstance, LPVOID lpParam );

		//ɾ������
		void Destroy();

		//�ı䴰����ʾ����
		bool ChangeScreenSetting();

		//���ݴ��ڴ�С����opengl
		void ReshapeGL();

		//����������           ::ǰ�治����/�����ռ�ͱ�ʾ����ͨ�ռ䡰
		void SwapBuffers() {::SwapBuffers(m_hDC);}

		//���ô������Ͻ�λ��,�˴����أ���Ը������͵�x��y.    ����ȥӦ����Ҫ��һ������setPosXY��x,y������
		void SetPosX(int x);
		void SetPosX(unsigned short x) { SetPosX((signed short)x); }
		void SetPosY(int y);
		void SetPosY(unsigned short y) { SetPosY((signed short)y); }

		//���ش��ڴ�С
		int GetWidth();
		int GetHeight();

		//���ô��ڴ�С,  ����ȥ�Ǹ����ô��ںϲ��ĸ�����һ����λ��������
		void SetWidth(int width);
		void SetHeight(int height);

		//���ش������Ͻ�λ��
		int GetPosX();
		int GetPosY();

		//���ô��ڵ���ɫλ��
		void SetHiColor()   { m_bitPerPixel = 16; }
		void SetTrueColor() { m_bitPerPixel = 32; }

		//��������� ���ÿ�����GL_Window m_Window�������m_Window��Ϊ���ھ��ʹ�� 
		//���ص�������ת����������񣿣�
		operator HWND() { return m_hWnd; }

	private:
		HWND     m_hWnd;                 //���ھ��
		HDC      m_hDC;                  //�豸������
		HGLRC    m_hRC;                  //opengl��Ⱦ������

		int      m_windowPosX,           //�������Ͻ�λ��
				 m_windowPosY,
                 m_windowWidth,          //���ڿ��
				 m_windowHeight,
				 m_screenWidth,           //ȫ�����
				 m_screenHeight,
				 m_bitPerPixel;           //��ɫλ��
		bool     m_isFullScreen;          //�Ƿ�ȫ��

};

#endif