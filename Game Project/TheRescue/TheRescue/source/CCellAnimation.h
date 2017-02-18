/*
	File: CCellAnimation.h
	Class: SGD 1212 
	Author: Gabriel Islas
	Purpose: To handle animations for the sprites in game. 
*/
#pragma once

#include <Windows.h>
#include "..\SGD Wrappers\CSGD_TextureManager.h"

// CELL ANIMATION
// - image holds multiple frames of animation
// - the current frame changes over time
class CCellAnimation
{
private:
	// Cell Algorithm
	int m_nFrameWidth;
	int m_nFrameHeight;
	int m_nNumCols;
	int m_nNumRows;

	// Animation
	int m_nCurrFrame;
	int m_nMaxFrame; 
	float m_fTimeWaited;
	float m_fTimePerFrame; 

	bool  m_bPlaying;
	bool  m_bLooping;
	float  m_fSpeed; 

	int m_nImageID;

	RECT CellAlgorithm( int id ) 
	{ 
		RECT rCell = { };

		rCell.left = (id% m_nNumCols) * m_nFrameWidth;
		rCell.top = (id/m_nNumCols) * m_nFrameHeight;

		rCell.right = rCell.left + m_nFrameWidth;
		rCell.bottom = rCell.top + m_nFrameHeight;

		return rCell; 
	}

public:
	void Initialize(int nFrameWidth, 
		int nFrameHeight, int nNumCols, int nNumRows,
		int nCurrFrame, int nMaxFrame, float fTimeWaited,
		float fTimePerFrame, bool bPlaying = false, bool bLooping = true,
		float fSpeed = 1.0f, int nImageID = -1) 
	{
		//Initialize variables
		m_nFrameWidth = nFrameWidth;
		m_nFrameHeight = nFrameHeight;
		m_nNumCols = nNumCols;
		m_nNumRows = nNumRows;

		//Initialize animation variables
		m_nCurrFrame = nCurrFrame;
		m_nMaxFrame = nMaxFrame;
		m_fTimeWaited = fTimeWaited;
		m_fTimePerFrame = fTimePerFrame;
		m_bPlaying = bPlaying;
		m_bLooping = bLooping;

		m_fSpeed = fSpeed;
		m_nImageID = nImageID;
	}

	void Update( float fElapsedTime )
	{
		// Exit if the animation is paused
		if( m_bPlaying == false )
			return;
		// Increase the animation timer
		m_fTimeWaited += fElapsedTime * m_fSpeed;

		// Ready for next frame?
		if( m_fTimeWaited >= m_fTimePerFrame )
		{
			m_fTimeWaited = 0;
			++m_nCurrFrame;

			// Have we reached the end?
			if( m_nCurrFrame == m_nMaxFrame )
			{
				// Should we loop?
				if( m_bLooping == true )
				{
					m_nCurrFrame = 0;
				}
				else
				{
					// Sopt on the last valid frame
					m_bPlaying = false;
					m_nCurrFrame = m_nMaxFrame-1;
				}
			}
		}
	}

	void Render( int nX, int nY, 
			float fScale, DWORD dwColor )
	{
		//Use the current frame as the id for the CellAlgorithm
		RECT rFrame = CellAlgorithm( m_nCurrFrame );

		// Draw the frame of animation
		CSGD_TextureManager::GetInstance()->Draw( 
				m_nImageID, nX, nY, 
				fScale, fScale, &rFrame, 0, 0, 0,
				dwColor );
	}

	// Controls
	void Play( bool bLooping, float fSpeed = 1.0f )
	{
		m_bLooping = bLooping;
		m_fSpeed = fSpeed; 

		m_nCurrFrame = 0;
		m_fTimeWaited = 0;
		m_bPlaying = true; 
	}

	void Pause( void )
	{
		m_bPlaying = false;
	}

	void Resume (void )
	{
		m_bPlaying = true;
	}

	bool IsStopped( void )
	{
		return m_bPlaying == false;
	}
};