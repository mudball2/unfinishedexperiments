/*
	File: CHowToPlayState.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To show a slideshow of instructions for the game.
*/

#pragma once
#include "IGameState.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class CHowToPlayState : public IGameState
{
public:
	static CHowToPlayState* GetInstance( void );

	// IGameState Interface
	virtual void Enter( void );
	virtual void Exit( void );

	virtual bool Input( void );
	virtual void Update( float fElapsedTime );
	virtual void Render( void );
private:
	CHowToPlayState(void);
	virtual ~CHowToPlayState(void);

	CHowToPlayState& operator=( const CHowToPlayState& );
	CHowToPlayState( const CHowToPlayState& );

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Assets
	int						m_nLettersID;
	int						m_nBackgroundMusicID;
	int						m_nBackgroundImage1ID;
	int						m_nBackgroundImage2ID;
	int						m_nBackgroundImage3ID;
	int						m_nBackgroundImage4ID;
	int						m_nBackgroundImage5ID;
	int						m_nBackgroundImage6ID;

	//Page counter
	int						m_nCurPage;

};

