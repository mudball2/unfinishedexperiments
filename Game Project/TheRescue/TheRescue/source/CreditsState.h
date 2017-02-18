/*
	File: CCreditsState.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To display scrolling credits of the game to the user.
*/
#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/SGD_String.h"
#include "BitmapFont.h"
#include <iostream>
#include <fstream>
using namespace std;

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class CCreditsState : public IGameState
{
public:
	static CCreditsState* GetInstance( void );

	// IGameState Interface
	virtual void Enter( void );
	virtual void Exit( void );

	virtual bool Input( void );
	virtual void Update( float fElapsedTime );
	virtual void Render( void );
private:
	CCreditsState(void);
	virtual ~CCreditsState(void);

	CCreditsState& operator=( const CCreditsState& );
	CCreditsState( const CCreditsState& );

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Assets
	int						m_nLettersID;
	int						m_nBackgroundMusicID;
	CBitmapFont				credits;

	//Scrolling variables
	int						m_nPosY;
	int						m_nVelY;
	string					s;
	string					cred[45];
	ifstream				output;

};

