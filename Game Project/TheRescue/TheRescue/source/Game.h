/*
	File:		Game.h
	Course:		SGD 1212
	Author:		Gabriel Islas
	Purpose:	CGame class controls the SGD Wrappers
*/


#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


// SGD Wrappers
// - forward declarations reduce header dependencies
// - can ONLY make pointers & references to the type
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

// Game state;
class IGameState;

class CGame
{
public:
	//SINGLETON
	// - Only have ONE instance can be created
	static CGame* GetInstance( void );
	static void   DeleteInstance( void );


	// Setup:
	void Initialize( HWND hWnd, HINSTANCE hInstance, 
					 int nWidth, int nHeight,
					 bool bWindowed );

	// Play:
	bool Main( void );

	// Cleanup:
	void Shutdown( void );


	// Game State Machine: 
	void ChangeState( IGameState* pNewState );

	// ACCESSORS
	int	GetWidth	( void ) const	{	return m_nScreenWidth;	}
	int GetHeight	( void ) const	{	return m_nScreenHeight;	}




private: //To make a singleton, you must make the destructor and ctor "private."
	//SINGLETON
	// - static data member for dynamically-allocated instance
	// - hidden constructors / destructor / operator=
	static CGame* s_Instance;
	CGame(void);
	~CGame(void);

	CGame( const CGame& );				//copy constructor
	CGame& operator=( const CGame& );	//assignment operator

	// Helper methods
	bool Input( void );
	void Update( void );
	void Render( void );

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Window Attributes
	int						m_nScreenWidth;
	int						m_nScreenHeight;
	bool					m_bIsWindowed;

	// Current time 
	DWORD					m_dwCurrTime;

	// Current game state 
	IGameState*				m_pCurrState;
};

