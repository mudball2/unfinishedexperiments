/*
	File: CIntroState.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To display an intro based on a timer. Can be skipped with any 
*/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/SGD_String.h"
#include "BitmapFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class CIntroState : public IGameState
{
public:
	static CIntroState* GetInstance( void );

	// IGameState Interface
	virtual void Enter( void );
	virtual void Exit( void );

	virtual bool Input( void );
	virtual void Update( float fElapsedTime );
	virtual void Render( void );
private:
	CIntroState(void);
	virtual ~CIntroState(void);

	CIntroState& operator=( const CIntroState& );
	CIntroState( const CIntroState& );

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Assets
	int						m_nFontID;


	//Other variables
	int						m_nTextOpacity; 

};
