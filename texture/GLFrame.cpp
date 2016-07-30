//======================================================================
/**
*  @file      GLFrame.cpp
*
*  ��Ŀ������ ����ӳ��
*  �ļ�����:  ������ͳ������� 
*	
*  �����ڼ̳�����������º�����ʵ��
*																			
*  GLApplication * GLApplication::Create(const char * class_name)		
*		������������һ��ʵ��		
*																			
*  bool Init();														
*		ִ�����еĳ�ʼ������������ɹ���������true							
*																			
*  void Uninit();													
*		ִ�����е�ж�ع���										
*																			
*  void Update(DWORD milliseconds);										
*		ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ
*																			
*  void Draw();															
*		ִ�����еĻ��Ʋ���
*/																			
//======================================================================

#include "stdafx.h"
#include "GLFrame.h"	

//���������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	GLApp * app1 = GLApp::Create("opengl");              //����������
	if(app1 != 0)
	{
		ret = app1->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow); //ָ�������ѭ��
		delete app1;        ///**< ɾ�������ࣨ�ڼ̳����У�ʹ��GL_Example * example = new GL_Example(class_name);������һ���ڴ棩*/
	}
	else
	{
		MessageBoxA(HWND_DESKTOP, "�����������", "Error", MB_ICONEXCLAMATION);
	}
	return ret;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);/**< �����û��Զ����32λ�ĳ��򸽼�ֵ */
	if (user_data == 0)
	{
		//��������һ������
		//WM_CREATE��һ��Ӧ�ó���ͨ��CreateWindowEx��������CreateWindow�������󴴽�����ʱ���ʹ���Ϣ��(����Ϣ�ں�������֮ǰ����)��
       //����ʱ���Ǵ��ڴ���֮����ʾ֮ǰ��ͬʱWM_CREATEҲ�����ǲ���������Ϣ��
		if (uMsg == WM_CREATE)
		{
			/// ���ش��ڽṹ��ָ�룬������մ����ĳ���ʵ������
			CREATESTRUCT  *creation = reinterpret_cast<CREATESTRUCT *>(lParam);

			//��ó���ʵ����ָ��
			GLApp *app1 = reinterpret_cast<GLApp *>(creation->lpCreateParams);

			//�������ʵ����ָ��Ϊ�û��Զ���ĳ���ĳ��򸽼�ֵ
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(app1));
			app1->m_isVisible = true;         //���ó���ɼ�
			return 0;
		}
	}
	else
	{
		//������ǵ�һ�δ��ڣ����س���ʵ��ָ��
		GLApp *app1 = reinterpret_cast<GLApp *>(user_data);
		return app1->Message(hWnd, uMsg, wParam, lParam);  //���ó���ʵ���Լ�����Ϣ������

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);        ///**< ����Ĭ�ϵĴ�����Ϣ������ */
}

GLApp::GLApp(LPCSTR class_name)
{
	m_className = class_name;											/**< �������� */
	m_isProgramLooping = true;											/**< ���ó���ѭ��Ϊtrue */
	m_createFullScreen = true;											/**< ʹ��ȫ��ģʽ */
	m_isVisible = false;												/**< ���ɼ� */
	m_resizeDraw = false;												/**< �ڴ��ڸı��С��ʱ�򣬲��ɻ��� */
	m_lastTickCount = 0;
}

void GLApp::ToggleFullscreen()             //�л�  <ȫ�� / ����> ģʽ
{
	PostMessage(m_window, WM_TOGGLEFULLSCREEN, 0, 0);   //�����Զ����л���Ϣ
}

void GLApp::TerminateApp()                                //��������
{
	PostMessage(m_window, WM_QUIT, 0, 0);                 //�����˳���Ϣ
	m_isProgramLooping = false;
}

//��Ϣѭ��
LRESULT GLApp::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_SYSCOMMAND:                          /**< �ػ�ϵͳ���� */
		switch(wParam)
		{
		case SC_SCREENSAVE:                        /**< �ػ���Ļ������������ */
		case SC_MONITORPOWER:                      /**< �ػ���ʾ��ʡ��ģʽ�������� */
			return 0;                          //������������������
		    break;
		}
		break;

	case WM_CLOSE:                              //�رմ���
		TerminateApp();                       //���ú���
		return 0;
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		m_lastTickCount = GetTickCount();       //���¼�������ֵ
		return 0;
		break;

	case WM_MOVE:
			m_window.SetPosX(LOWORD(lParam));							/**< ������������ */
			m_window.SetPosY(HIWORD(lParam));							
			return 0;
		break;

	case WM_PAINT:
			if (m_resizeDraw == true)									/**< �����Ҫ�ػ� */
			{
				m_window.ReshapeGL();									/**< �������ô��ڵĴ�С */
				Draw();													/**< ���»��� */
				m_window.SwapBuffers();									/**< ����ǰ��֡���� */
			}
		break;

	case WM_SIZING:													/**< �������ڸı��С */
		{
			RECT * rect = (RECT *)lParam;
			m_window.SetWidth(rect->right - rect->left);				/**< ���ô��ڿ�� */
			m_window.SetHeight(rect->bottom - rect->top);				/**< ���ô��ڸ߶� */
			return TRUE;
		}
		break;

	case WM_SIZE:													/**< ���ڸı��С�� */
			switch (wParam)												/**< ����ͬ�Ĵ���״̬ */
			{
				case SIZE_MINIMIZED:									/**< �Ƿ���С��? */
					m_isVisible = false;								/**< ����ǣ������ò��ɼ� */
					return 0;											
				break;

				case SIZE_MAXIMIZED:									/**< �����Ƿ����? */
				case SIZE_RESTORED:										/**< ���ڱ���ԭ? */
					m_isVisible = true;									/**< ����Ϊ�ɼ� */
					m_window.SetWidth(LOWORD(lParam));					/**< ���ô��ڿ�� */
					m_window.SetHeight(HIWORD(lParam));					/**< ���ô��ڸ߶� */
					m_window.ReshapeGL();								/**< �ı䴰�ڴ�С */
					m_lastTickCount = GetTickCount();					/**< ���¼�������ֵ */
					return 0;											
				break;
			}
		break;			

	case WM_KEYDOWN:												/**< ���°�����Ϣ */
			m_keys.SetPressed(wParam);									
			return 0;													
		break;

	case WM_KEYUP:													/**< �����ͷż���Ϣ */
			m_keys.SetReleased(wParam);									
			return 0;													
		break;

	case WM_TOGGLEFULLSCREEN:										/**< �л� ȫ��/����ģʽ *///Ϊʲô�л���Ļ�˻������˳���Ϣ
			m_createFullScreen = !m_createFullScreen;                 //��һ��ѭ�����رմ����ǳ���û��ֹͣ�����������´���  get
			PostMessage(hWnd, WM_QUIT, 0, 0);                         //������û����������ʹ��ھ��޷��رգ�����û������Ϣ�����д���*/
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< ����Ĭ�ϵĴ�����Ϣ������ */
}

///** �������ѭ�� */
int GLApp::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/// ע��һ������
	WNDCLASSEXA windowClass;                                             /**< ������ */
	ZeroMemory(&windowClass, sizeof(WNDCLASSEXA));                        /**< ��սṹΪ0 */
	windowClass.cbSize			= sizeof(WNDCLASSEXA);					/**< ���ڽṹ�Ĵ�С */
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	/**< ���ô�������Ϊ���ƶ�ʱ�ػ�����Ϊ����ȡ��DC */
	windowClass.lpfnWndProc		= (WNDPROC)(WndProc);				    /**< WndProc������Ϣ */
	windowClass.hInstance		= hInstance;							/**< ����ʵ�� */
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			/**< ���ñ��� */
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			/**< ������ */
	windowClass.lpszClassName	= m_className;							/**< �������� */

	if (RegisterClassExA(&windowClass) == 0)								/**< ����ע�ᴰ���� */
	{																	/**< NOTE: Failure, Should Never Happen */
		MessageBoxA(HWND_DESKTOP, "ע�ᴰ��ʧ��!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														/**< �˳�������FALSE */
	}

	// ѯ���Ƿ���ȫ��״̬������?
	if (MessageBoxA(HWND_DESKTOP, "������ȫ��״̬������ô ?", "��������ģʽ", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		m_createFullScreen = false;										/**< m_CreateFullScreen��¼��ǰ����ʾģʽΪ���� */
	}
	
	while (m_isProgramLooping)											/**< ѭ��ֱ��WM_QUIT�˳����� */
	{																	
		/// ����һ������
		if (m_window.Create("OpenGL 3D��Ϸ��̡�������ӳ��", m_className, m_createFullScreen, hInstance, this) == true)
		{	
			
			/// �����ʼ��ʧ�ܣ����˳�
			if (Init() == false)									    /**< �����Զ���ĳ�ʼ������ */
			{															/**< ʧ�� */
				TerminateApp();							         		/**< �رմ����˳����� */
			}
			else														/**< �ɹ���ʼ��Ϣѭ�� */
			{															 
				MSG msg;												/**< Window��Ϣ�ṹ */ 
				bool isMessagePumpActive = true;						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
				m_lastTickCount = GetTickCount();						/**< ���ص�ǰ�ļ�ʱ����ֵ *///�Ӳ���ϵͳ�����������ĺ����������ķ���ֵ��DWORD��
				m_keys.Clear();											/**< ������еİ�����Ϣ */
				while (isMessagePumpActive == true)						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
				{														/**< �ɹ��������ڣ���ⴰ����Ϣ */
					if (PeekMessage(&msg, m_window, 0, 0, PM_REMOVE) != 0)
					{													
						/// ����Ƿ�ΪWM_QUIT��Ϣ
						if (msg.message != WM_QUIT)						
						{
							DispatchMessage(&msg);						/**< �������,������Ϣ����Ϣ�ص������д��� */
						}
						else											
						{
							isMessagePumpActive = false;				/**< ����ǣ����˳� */
						}
					}
					/// ���û����Ϣ
					else												
					{
						if (m_isVisible == false)						/**< ������ڲ��ɼ� */
						{
							WaitMessage();								/**< ��ͣ���򣬵ȴ���Ϣ */
						}
						else											/**< ������ڿɼ� */
						{												/**< ִ����Ϣѭ�� */
							DWORD tickCount = GetTickCount();			/**< ���ؼ�ʱ���ĵ�ǰֵ */
							Update(tickCount - m_lastTickCount);		/**< �����û��Զ���ĸ��º��� */
							m_lastTickCount = tickCount;				/**< ����������һ�Σ���������ֵ */
							Draw();										/**< �����û��Զ���Ļ��ƺ��� */
							m_window.SwapBuffers();						/**< ����ǰ��֡���� */
						}
					}
				}														/**< ���isMessagePumpActive == true����ѭ�� */
			}															
																		/**< ������� */
			Uninit();												/**< �û��Զ����ж�غ��� */
			m_window.Destroy();											/**< ɾ������ */
		}
		else															/**< �����������ʧ�� */
		{																
			MessageBox(HWND_DESKTOP, TEXT("����OpenGL���ڴ���"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
			m_isProgramLooping = false;									/**< ֹͣ����ѭ�� */
		}
	}																	

	UnregisterClassA(m_className, hInstance);							/**< ȡ��ע��Ĵ��� */
	return 0;
}