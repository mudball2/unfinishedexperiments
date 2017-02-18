/*
	File: GameplayState.h
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To handle gameplay
*/
#pragma once
#include "IGameState.h"

#include "Entity.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/CSGD_ObjectFactory.h"

#include "Clover.h"


////////////////////////////////////////////////////
// SGD Wrappers
//	- forward declarations reduce header dependencies
//	- can ONLY make pointers & references to the type
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;
class CSGD_MessageSystem;
class IMessage;


class CObjectManager;


typedef CSGD_ObjectFactory< TSTRING, CEntity >	CSGD_Factory;

class CGameplayState : public IGameState
{
public:

	////////////////////////////////////////////////
	// SINGLETON
	static	CGameplayState*	GetInstance( void );

	
	////////////////////////////////////////////////
	// IGameState Interface
	virtual void Enter	( void );
	virtual void Exit	( void );

	virtual bool Input	( void );
	virtual void Update	( float fElapsedTime );
	virtual void Render	( void );

	int GetFinalScore( void ) { return m_nFinalScore; }

private:
	CGameplayState(void);
	virtual ~CGameplayState(void);

	CGameplayState( const CGameplayState& );
	CGameplayState& operator= ( const CGameplayState& );

	static void MessageProc( IMessage* pMsg );
	
		
	
	////////////////////////////////////////////////
	// SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CSGD_Factory*			m_pOF;
	CSGD_MessageSystem*		m_pMS;

	CObjectManager*			m_pOM;


	
	////////////////////////////////////////////////
	// Assets
	int						m_nCloverImageID;

	int						m_nSoldierImageID;

	int						m_nSentinelImageID;

	int						m_nBackgroundImageID;

	int						m_nBackgroundMusicID;
	
	int						m_nSilencedShotSfxID;

	int						m_nBulletImageID;

	int						m_nPauseImageID;

	int						m_nPauseFontID;

	int						m_nPauseCursorID;

	int						m_nTooEasySfxID;
	
	float					m_fMusicVolume;


	// FPS
	float					m_fFPSTimer;
	unsigned int			m_unFPS;
	unsigned int			m_unFrames;

	//Other variables
	bool					m_bIsPaused;
	bool					m_bYouWin;
	bool					m_bYouLose;
	int						m_nCursorY;
	int						m_nFinalScore;

	
	CEntity*				m_pPlayer;
};

