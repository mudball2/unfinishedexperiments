/*
	File: COptionsState.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To handle all of the configuration settings for the game.
*/


#pragma once
#include "IGameState.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class COptionsState : public IGameState
{
public:
	//SINGLETON
	static COptionsState* GetInstance(void);

	// IGameState Interface
	virtual void Enter( void );
	virtual void Exit( void );

	virtual bool Input( void );
	virtual void Update( float fElapsedTime );
	virtual void Render( void );

	//Accessors

private:
	COptionsState(void);
	virtual ~COptionsState(void);

	COptionsState& operator=( const COptionsState& );
	COptionsState( const COptionsState& );

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Assets
	int						m_nFontID;
	int						m_nCursorID;
	int						m_nBackgroundMusicID;
	int						m_nSFXID;

	// volume varaibles
	int						m_nDispVolumeMusic;
	int						m_nDispVolumeSfx;
	float					m_fMusicVolume;
	float					m_fSfxVolume;

	// other variables
	int						m_nCursorY;



};

