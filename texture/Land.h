//========================================================
/**
*  @file      Terrain.h
*
*  ��Ŀ������ ������պ͵���
*  �ļ�����:  ������               
*
*/     
//========================================================

#ifndef __TERRAIN_H__
#define __TERRAIN_H__


#include "stdafx.h"
#include "CBMPLoader.h"
#include "camera.h"


/** ������ */
class CTerrain
{
public:
	
	/** ���캯�� */
	CTerrain();

	~CTerrain();

	/** ��ʼ������ */
	bool init();

	/** ��Ⱦ���� */
	void render();

	/** �����Ƿ�ʹ��ϸ�������־ */ 
	void setDetail(bool _bDetail) { m_bDetail = _bDetail;}
		
	/** ��õ���߶� */
	float getAveHeight(float x, float z);
	
	/** �õ���ǰTerrainָ�� */
	static CTerrain* GetTerrainPointer() { return m_pTerrain;}
	

private:

	/** ���õ��εĴ�С */
	void setSize(unsigned  int width, unsigned  int cell); 

	/** ����'.raw'�߶�ͼ */
	bool loadRawFile(const char* fileName);

	/** װ������ */
	bool loadTexture();

	/** ��õ�(x,y)�ĸ߶���Ϣ */
	int getHeight(int x, int y);

	/** ������������ */
	void setTexCoord(float x, float z);

	/** ������Ч */
	void initFog();

                      
	
public:
	static CTerrain*  m_pTerrain;        /**< ��ǰTerrainָ�� */
	unsigned  int     m_nWidth;          /**< ���������� */
	unsigned  int     m_nCellWidth;      /**< ÿһ���� */
   	BYTE*             m_pHeightMap;      /**< ��Ÿ߶���Ϣ */
	CBMPLoader        m_texture[2];      /**< ���������ϸ������ */
	bool              m_bDetail;         /**< �Ƿ�ʹ��ϸ�������־ */
	int               m_DetailScale;     /**< ���ű��� */
                                         	
};

#endif //__TERRAIN_H__

