/*
	File: CMainMenuState.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To handle all of the main menu actions
*/

#pragma once

#include "IGameState.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class CMainMenuState : public IGameState
{
public:
	static CMainMenuState* GetInstance( void );

	// IGameState Interface
	virtual void Enter( void );
	virtual void Exit( void );

	virtual bool Input( void );
	virtual void Update( float fElapsedTime );
	virtual void Render( void );

private:
	CMainMenuState(void);
	virtual ~CMainMenuState(void);

	CMainMenuState& operator=( const CMainMenuState& );
	CMainMenuState( const CMainMenuState& );

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Assets
	int						m_nMenuCursorID;
	int						m_nBackgroundMusicID;
	int						m_nSFXSelectID;
	int						m_nTitleFontID;
	int						m_nBackgroundImageID;
	int						m_nMenuItemsID;

	// cursor position
	int						m_nCursorY;
	float					m_fMusicVolume;
	int						m_nDispMusicVolume;

};

