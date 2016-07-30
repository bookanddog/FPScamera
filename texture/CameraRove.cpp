//=========================================================================
/**
*  @file      CameraRove.cpp
*
*  ��Ŀ������ ���������
*  �ļ�����:  ����ʵ���� 
*
*  ��������б����������¼����麯��
*																								
*	virtual bool Init();														
*		ִ�����еĳ�ʼ������������ɹ���������true							
*																			
*	virtual void Uninit();													
*		ִ�����е�ж�ع���										
*																			
*	virtual void Update(DWORD milliseconds);										
*		ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ
*																			
*	virtual void Draw();															
*		ִ�����еĻ��Ʋ���
*/
//=========================================================================

#include "CameraRove.h"						    


/** ����һ�������ʵ�� */
GLApp * GLApp::Create(const char * class_name)
{
	CameraRove * test = new CameraRove(class_name);
	return reinterpret_cast<GLApp *>(test);
}


/** ���캯�� */
CameraRove::CameraRove(const char * class_name) : GLApp(class_name)
{
  /// ��ʼ���û��Զ���ĳ������
	m_Fps = 0;
   
}

bool CameraRove::LoadTexture()
{
	
	if(!m_Texture.LoadBMP("image.bmp"))                   /**< ����λͼ�ļ� */
	{
		MessageBox(NULL,"װ��λͼ�ļ�ʧ�ܣ�","����",MB_OK);  /**< �������ʧ���򵯳��Ի��� */
		return false;
	}
	glGenTextures(1, &m_Texture.ID);                        /**< ����һ�������������� */
		
	glBindTexture(GL_TEXTURE_2D, m_Texture.ID);             /**< ������������ */
	
	/** �����˲� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	/** �������� */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_Texture.imageWidth,
					m_Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					m_Texture.image);

	/** ��������ӳ�� */
	glEnable(GL_TEXTURE_2D);

  	return true;
}

/** ���ù�Դ */
void CameraRove::SetLight()
{
    /** �����Դ������ֵ */
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 	/**< ��������� */
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };		/**< �������� */
	GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };	/**< �������� */
	GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };	/**< ��Դλ�� */

	/** ���ù�Դ������ֵ */
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		/**< ���û����� */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		/**< ��������� */
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);	/**< ��������� */
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);	/**< ���ù�Դλ�� */
	
	/** ���ù�Դ */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1); 
}

/** ��ʼ��OpenGL */
bool CameraRove::Init()									
{
/** �û��Զ���ĳ�ʼ������ */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);	                             /**< �ı�OpenGL���ڴ�С��ֱ�ӵ�������ĺ��� */
	
	/** ��ʼ������ */
	if(!m_Font.InitFont())
		MessageBox(NULL,"��ʼ������ʧ��!","����",MB_OK);
	
	/** �������� */
	if(!LoadTexture())
		MessageBox(NULL,"��������ʧ��!","����",MB_OK);
	
	/** ��������� */
	m_Camera.setCamera(0.0f,1.5f, 6.0f, 0.0f, 1.5f, 0.0f,0.0f, 1.0f, 0.0f);

	/** ���ù�Դ */
	SetLight();

	return true;                                        /**< �ɹ����� */
}

/** �û��Զ����ж�غ��� */
void CameraRove::Uninit()									
{
/** �û��Զ����ж�ع��� */
	m_Texture.FreeImage();              /** �ͷ�����ͼ��ռ�õ��ڴ� */
	glDeleteTextures(1, &m_Texture.ID); /**< ɾ���������� */

 }
/** ��������� */
void CameraRove::UpdateCamera()
{
	m_Camera.setViewByMouse();
	
	/** ���̰�����Ӧ */
	if(m_keys.IsPressed(VK_SHIFT))                      /**< ����SHIFT��ʱ���� */
	{
		m_Camera.setSpeed(0.6f);
	}
	if(!m_keys.IsPressed(VK_SHIFT))
	{
		m_Camera.setSpeed(0.2f);
	}
	if(m_keys.IsPressed(VK_UP) || m_keys.IsPressed('W'))   /**< ���Ϸ������'W'������ */
		m_Camera.moveCamera(m_Camera.getSpeed());          /**< �ƶ������ */

	if(m_keys.IsPressed(VK_DOWN) || m_keys.IsPressed('S')) /**< ���·������'S'������ */
		m_Camera.moveCamera(-m_Camera.getSpeed());         /**< �ƶ������ */

	if(m_keys.IsPressed(VK_LEFT) || m_keys.IsPressed('A')) /**< ���������'A'������ */
		m_Camera.yawCamera(-m_Camera.getSpeed());          /**< �ƶ������ */

	if(m_keys.IsPressed(VK_RIGHT) || m_keys.IsPressed('D')) /**< ���ҷ������'D'������ */
		m_Camera.yawCamera(m_Camera.getSpeed());            /**< �ƶ������ */

}
/** ������º��� */
void CameraRove::Update(DWORD milliseconds)						
{
	if (m_keys.IsPressed(VK_ESCAPE) == true)					/**< ��ESC�˳� */
	{
		TerminateApp();									
	}


	if (m_keys.IsPressed(VK_F1) == true)					/**< ��F1�ڴ��ں�ȫ�����л� */
	{
		ToggleFullscreen();									
	}

	/** ��������� */
	UpdateCamera();
	
	    
}

/** ����֡�� */
void CameraRove::CaculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	     /**< ������ʾ֡�� */	
    static float lastTime			= 0.0f;	     /**< ��¼�ϴ�ʱ�� */						
    float currentTime = GetTickCount() * 0.001f; /**< ��õ�ǰʱ�� */	 			

	++framesPerSecond;                           /**< ��ʾ֡������1 */
    /** ���ʱ������1.0�� */
	if( currentTime - lastTime > 1.0f )          
    {
		
	    lastTime = currentTime;                   /**< ���浱ǰʱ�� */
		m_Fps = framesPerSecond;                  /**< ��ǰ֡������m_Fps */
        framesPerSecond = 0;                      /**< ��֡������ */                    
    }
}

/** ���������Ϣ */
void  CameraRove::PrintText()
{
	char string[128];                               /**< ���ڱ�������ַ��� */
	glPushAttrib(GL_CURRENT_BIT);                   /**< ����������ɫ������Ϣ */
	glColor3f(0.0f,1.0f,0.0f);                      /**< ����������ɫ */
	sprintf(string,"��ǰλ��:X=%3.1f  Y=%3.1f Speed =%3.1f ",   
		     m_Camera.getView().x,-m_Camera.getView().z ,m_Camera.getSpeed()); /**< �ַ�����ֵ */
	m_Font.PrintText(string,-5.0f,3.5f);

	/** ���֡�� */
	CaculateFrameRate();                               /**< ����֡�� */
    sprintf(string,"FPS:%3.0f",m_Fps);                 /**< �ַ�����ֵ */
	m_Font.PrintText(string, -5.0f,3.0f);              /**< ����ַ��� */
	glPopAttrib();
		
}

/** ����������� */
void CameraRove::DrawGrid()
{
    /** ��ó�����һЩ״̬  */
	GLboolean  lp,tp;
	glGetBooleanv(GL_LIGHTING,&lp);  
	glGetBooleanv(GL_TEXTURE_2D,&tp);
	
	/** �ر������͹��� */
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
	
	/** ���ƹ��� */
	glPushAttrib(GL_CURRENT_BIT);   /**< ���浱ǰ���� */
    glPushMatrix();                 /**< ѹ���ջ */
	glTranslatef(0.0f,0.0f,0.0f);  
	glColor3f(0.0f, 0.0f, 1.0f);    /**< ������ɫ */

	/** ��X,Zƽ���ϻ������� */
	for(float i = -50; i <= 50; i += 1)
	{
		/** ������ */
		glBegin(GL_LINES);

		    /** X�᷽�� */
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			/** Z�᷽�� */
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

		glEnd();
	}
	glPopMatrix();
	glPopAttrib();
	
	/** �ָ�����״̬ */
	if(tp)
	  glEnable(GL_TEXTURE_2D);
	if(lp)
      glEnable(GL_LIGHTING);
}

/** �������� */
void CameraRove::DrawSphere()
{
	/** ���ò������� */
	GLfloat mat_ambient[] = { 0.9f, 0.5f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.9f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
	/** �����������״̬ */
	GLboolean tp;
	glGetBooleanv(GL_TEXTURE_2D,&tp);
    glDisable(GL_TEXTURE_2D);                   /**< �ر����� */
    
	/** ���ƹ��� */
	glPushMatrix();
    glTranslatef(-5.0f,2.0f,-10.0f);
    GLUquadricObj * sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricNormals(sphere,GLU_SMOOTH);
	gluSphere(sphere,2.0,50,50);
	gluDeleteQuadric(sphere);
    glPopMatrix();
    
	/** �ָ�״̬ */
	if(tp)
	   glEnable(GL_TEXTURE_2D);
	
}

/** ����ľ�� */
void CameraRove::DrawBox()
{
	/** ���ò������� */
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glPushMatrix();
	glTranslatef(5.0f,2.0f,-10.0f);
	glScalef(2.0f,2.0f,2.0f);
	
	/** ѡ������ */
	glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
	
	/** ��ʼ�����ı��� */
	glBegin(GL_QUADS);												
		
	    /// ǰ����
		glNormal3f( 0.0f, 0.0f, 1.0f);								/**< ָ������ָ��۲��� */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		
		/// �����
		glNormal3f( 0.0f, 0.0f,-1.0f);								/**< ָ�����߱���۲��� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		
		/// ����
		glNormal3f( 0.0f, 1.0f, 0.0f);								/**< ָ���������� */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		
		/// ����
		glNormal3f( 0.0f,-1.0f, 0.0f);								/**< ָ�����߳��� */
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		
		/// �Ҳ���
		glNormal3f( 1.0f, 0.0f, 0.0f);								/**< ָ�����߳��� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		
		/// �����
		glNormal3f(-1.0f, 0.0f, 0.0f);								/**< ָ�����߳��� */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
	glEnd();
	glPopMatrix();
}

/** ���ƺ��� */
void CameraRove::Draw()											
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();	
	
	/** ��������� */	
	m_Camera.setLook();
	
	/** ���ƹ��� */
	DrawGrid();
    DrawSphere();
	DrawBox();
	
	/** ���������Ϣ */
	PrintText();
	
	/** ǿ��ִ�����е�OpenGL���� */	
	glFlush();							    
}