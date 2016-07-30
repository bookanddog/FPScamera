//========================================================
/**
*  @file      glWindows.cpp
*
*  ��Ŀ������ ����ӳ��
*  �ļ�����:  ���򴰿��� 
*/     
//========================================================

#include "GLWindow.h"	


//���캯��
GLWindow::GLWindow()
{
	m_windowPosX = 0;
	m_windowPosY = 0;
	m_windowWidth = 800;
	m_windowHeight = 600;
	m_screenWidth = 1366;
	m_screenHeight =768;
	m_bitPerPixel = 32;
	m_isFullScreen =false;

	m_hWnd = 0;
	m_hDC = 0;
	m_hRC = 0;
}

//���ش��ڵĴ�С
int GLWindow::GetWidth()
{
	if(m_isFullScreen == true)
		return m_screenWidth;
	else
		return m_windowWidth;
}

int GLWindow::GetHeight()
{
	if (m_isFullScreen == true)
	{
		return m_screenHeight;
	}
	else
	{
		return m_windowHeight;
	}
}

/** ���ô��ڵĴ�С */
void GLWindow::SetWidth(int width)
{
	if (m_isFullScreen == true)
	{
		m_screenWidth = width;
	}
	else
	{
		m_windowWidth = width;
	}
}
void GLWindow::SetHeight(int height)
{
	if (m_isFullScreen == true)
	{
		m_screenHeight = height;
	}
	else
	{
		m_windowHeight = height;
	}
}

/** ���ش������Ͻǵ�λ�� */
int GLWindow::GetPosX()
{
	if (m_isFullScreen == false)
	{
		return m_windowPosX;
	}
	return 0;
}
int GLWindow::GetPosY()
{
	if (m_isFullScreen == false)
	{
		return m_windowPosY;
	}
	return 0;
}

/** ���ô������Ͻǵ�λ�� */
void GLWindow::SetPosX(int x)
{
	if (m_isFullScreen == false)
	{
		m_windowPosX = x;
	}
}
void GLWindow::SetPosY(int y)
{
	if (m_isFullScreen == false)
	{
		m_windowPosY = y;
	}
}

//�����ڴ�С�ı�ʱ��֪ͨopengl�ı��С
void GLWindow::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//ȱʡֵʱ�ӵ�����ԭ���
	gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 1.0, 4000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/** �ı䴰�ڵ���ʾ���� */
bool GLWindow::ChangeScreenSetting()
{
	DEVMODE dmScreenSettings;
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
	dmScreenSettings.dmSize       = sizeof(DEVMODE);
	dmScreenSettings.dmPelsWidth  = GetWidth();
	dmScreenSettings.dmPelsHeight = GetHeight();
	dmScreenSettings.dmBitsPerPel = m_bitPerPixel;           //����λ��
	//dmScreenSettings.dmDisplayFrequency = 75;                           /**< ������Ļˢ���� */
	dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//| DM_DISPLAYFREQUENCY;Ƶ��

	//�ı���ʾģʽ//��ȱʡ��ʾ�豸�����øı�Ϊ��lpDevMode�趨��ͼ��ģʽ��Ҫ�ı�һ���ض���ʾ�豸�����ã���ʹ��ChangeDisplaySettingsEx������
	//0��������ǰ��Ļ��ͼ��ģʽҪ��̬�ظı�
	if(ChangeDisplaySettings(&dmScreenSettings, 0) != DISP_CHANGE_SUCCESSFUL) 
	{ 
		return false;
	}
	return true;
}

//��������
bool GLWindow::Create(LPCSTR window_title, LPCSTR class_name, bool fullscreen, HINSTANCE hinstance, LPVOID lpParam)
{
	m_isFullScreen = fullscreen;
	int nX = 0,
		nY = 0;
	PIXELFORMATDESCRIPTOR pfd = 
	{                                           //�������������ṹ
		sizeof(PIXELFORMATDESCRIPTOR),         //���ؽṹ�Ĵ�С
		1,                                     //�汾��
		PFD_DRAW_TO_WINDOW |                  //���컺�������ԣ��������������ʾ��һ��������
		PFD_SUPPORT_OPENGL |                  //                 ������֧��opengl��ͼ
		PFD_STEREO         |                  //                ��ɫ�����������建��
		PFD_DOUBLEBUFFER,                     //                 ��ɫ��������˫����
		PFD_TYPE_RGBA,                       //ʹ��rgba��ɫ��ʽ
		m_bitPerPixel,                       //��ɫ����������ɫֵ��ռ��λ��
		0, 0, 0, 0, 0, 0,                   //ʹ��Ĭ�ϵ���ɫ����
		0,                                   //��alpha����
		0,                                  //��ɫ��������alpha�ɷֵ���λ����
		0,									/**< ���ۼƻ����� */
		0, 0, 0, 0,							/**< �ۼƻ���������λ *///���ۻ�������rgbaλ��
		32,                                //��Ȼ�����λ��
		0,					     			/**< ���ɰ滺�� *///ģ�建����λ��
		0,						    		/**< �޸��������� *///��������������
		PFD_MAIN_PLANE,                    // ����ΪPFD_MAIN_PLANE������Ϊ����ͼ�� */���ѷ�������
		0,                                  //��ʾOpenGLʵ����֧�ֵ��ϲ���²�ƽ������� */�²�ƽ���͸����ɫ
		0, 0, 0					           /**< ��ʱ���Ѳ���ʹ�� *///
	};

	///**< �������Ǵ������ͣ�ʹ�ó����趨��ȥ����󻯰�ť�������ܸı䴰���С */
	DWORD windowStyle = WS_OVERLAPPEDWINDOW; //& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX;  //WS_OVERLAPPEDWINDOW &
	DWORD windowExtendStyle = WS_EX_APPWINDOW;

	if(m_isFullScreen == true)
	{
		if(ChangeScreenSetting() == false)
		{
			MessageBox(HWND_DESKTOP, TEXT("ģʽװ��ʧ�ܡ�\n�ڴ���ģʽ�����С�"), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
			m_isFullScreen = false;
		}
		else
		{
			ShowCursor(true);
			windowStyle = WS_POPUP;                    //���ô���ģʽΪ���㴰��
			windowExtendStyle |= WS_EX_TOPMOST; 
		}
	}

	//�����������Ĵ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
	RECT windowRect = {GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight()};
	if(m_isFullScreen = false)          //����ģʽ
	{
		windowExtendStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;// | WS_EX_CLIENTEDGE;        //���ھ���3d���
		int wid = GetSystemMetrics(SM_CXSCREEN);                    //��ȡ��ǰ��Ļ���
		int hei = GetSystemMetrics(SM_CYSCREEN);
		nX = (wid - GetWidth()) / 2;                    /**< ���㴰�ھ����� */
		nY = (hei - GetHeight()) / 2;			//��Ļ���ĵ�

		/// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С��������Ĵ��ھ��������Դ��͸�CreateWindowEx���������ڴ���һ���ͻ��������С�Ĵ��ڡ�
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendStyle);
		/// �жϴ��ڵ����Ͻ��Ƿ�������������
		if(windowRect.left < 0)
		{
			windowRect.right -= windowRect.left;						
			windowRect.left = 0;	
		}
		if (windowRect.top < 0)											/**< �������Y����Ϊ�����ƶ����굽0�������������ڵ�λ�� */
		{
			windowRect.bottom -= windowRect.top;						
			windowRect.top = 0;											
		}
	}

	//��������
	m_hWnd = CreateWindowExA(windowExtendStyle,                        //���ڵ���չ���
		                    class_name,                              //���ڵ�����
							window_title,                             //���ڱ���
							windowStyle,                             //���ڷ��
							nX,nY,                                  /**< ���ڵ����Ͻ�λ�� *///��ò���Ǵ�������λ�ð�
							                                       //Ӧ����windowRect.left,windowRect.top
							windowRect.right - windowRect.left,			/**< ���ڵĿ�� */
							windowRect.bottom - windowRect.top,			/**< ���ڵĸ߶� */
							HWND_DESKTOP,								/**< ���ڵĸ�����Ϊ���� */
							0,											/**< �޲˵� */
							hinstance,									/**< ���봰�ڵ�ʵ����� */
							lpParam);									/**< ������������ */

	while (m_hWnd != 0)
	{
		m_hDC = GetDC(m_hWnd);//�����豸������
		if (m_hDC == 0)
		{
			break;
		}

		GLuint pixelFormat = ChoosePixelFormat(m_hDC, &pfd);         /**< ����һ�����ݵ����ظ�ʽ */
		if (pixelFormat == 0)
		{
			break;
		}
		if (SetPixelFormat(m_hDC, pixelFormat, &pfd) == false)        /**< �������ظ�ʽ */
		{
			break;
		}

		m_hRC = wglCreateContext(m_hDC);                             /**< ����OpenGL����Ⱦ������ */
		if (m_hRC == 0)													/**< ���Ϊ�� */
		{																/**< ʧ�� */
			break;														
		}
		if (wglMakeCurrent(m_hDC, m_hRC) == false)                     /**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��ǰ�Ĵ��� */
		{																/**< ʧ�� */
			break;														
		}
		
		//�ú�������ָ�����ڵ���ʾ״̬
		ShowWindow(m_hWnd, SW_NORMAL);
		ReshapeGL();
		return true;													/**< �ɹ����� */
	}
	Destroy();															/**< �ͷ���Դ */
	return false;														/**< ����ʧ�� */
}

/** ɾ��OpenGL���� */
void GLWindow::Destroy()	
	{
	if (m_hWnd != 0)										/**< ���ھ���Ƿ���� */
	{
		if (m_hDC != 0)										/**< �����豸�������Ƿ���� */
		{
			wglMakeCurrent(m_hDC, 0);						/**< ���õ�ǰ��OpenGL����Ⱦ����Ϊ��NULL */
			if (m_hRC != 0)									/**< OpenGL����Ⱦ�������Ƿ���� */
			{
				wglDeleteContext(m_hRC);					/**< �ͷ�OpenGL����Ⱦ������ */
				m_hRC = 0;									/**< ����OpenGL����Ⱦ������Ϊ0 */
			}
			ReleaseDC(m_hWnd, m_hDC);						/**< �ͷŴ��ڵ��豸������ */
			m_hDC = 0;										/**< ���ô��ڵ��豸������Ϊ0 */
		}
		DestroyWindow(m_hWnd);								/**< ɾ������ */
		m_hWnd = 0;											/**< ���ô��ھ��Ϊ0 */
	}

	if (m_isFullScreen)										/**< ���Ϊȫ��ģʽ���ڳ�������󣬱任������ģʽ������ʾ��� */
	{
		ChangeDisplaySettings(NULL, 0);						/**< �任������ģʽ */
		ShowCursor(true);									/**< ��ʾ��� */
	}
}
