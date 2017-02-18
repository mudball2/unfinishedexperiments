/*
	File: CHighScoresState.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To display a set of high scores to the player.
*/

#pragma once
#include "IGameState.h"

#include "Entity.h"


#include <sstream>
#include <fstream>

#include <string>
using namespace std;

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class CHighScoresState : public IGameState
{
public:
	static CHighScoresState* GetInstance( void );

	// IGameState Interface
	virtual void Enter( void );
	virtual void Exit( void );

	virtual bool Input( void );
	virtual void Update( float fElapsedTime );
	virtual void Render( void );

private:
	CHighScoresState(void);
	virtual ~CHighScoresState(void);

	CHighScoresState& operator=( const CHighScoresState& );
	CHighScoresState( const CHighScoresState& );

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Assets
	int						m_nFontID;
	
	ostringstream			ossUser; 

	// high score variables
	string					m_szNames[10];
	int						m_nScores[10];
	string					m_szUserInput;
	CEntity*				m_pPlayer;


};

