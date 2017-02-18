/*
	File: CBitmapFont.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To create and handle all bitmap fonts loaded in.
*/

#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include <Windows.h>
#include "../SGD Wrappers/CSGD_TextureManager.h"

class CBitmapFont
{
	// Cell algorithm
	int m_nCharWidth;
	int m_nCharHeight;	
	int m_nNumCols;	
	int m_nNumRows;

	int m_nImageID;	
	char m_cFirstChar;
	bool m_bOnlyUppercase;	
	
	CSGD_TextureManager* pTM;

public:
	void Initialize( int nCharWidth, int nCharHeight, int nNumCols, int nNumRows, int nImageID, 
		char cFirstChar = 32, 
		bool bOnlyUppercase = true)
	{
		pTM = CSGD_TextureManager::GetInstance();

		m_nCharWidth = nCharWidth;
		m_nCharHeight = nCharHeight;
		m_nNumCols = nNumCols;
		m_nNumRows = nNumRows;

		m_nImageID = nImageID;
		m_cFirstChar = cFirstChar;
		m_bOnlyUppercase = bOnlyUppercase;
	}

	void Print( const char* szText, int nX, int nY,
			float fScale, DWORD dwColor )
	{
		// Keep track of the starting x value( for newlines)
		int nColStart = nX;
		
		// Iterate through the string 1 character at a time
		int nLength = strlen( szText );
		
		for (int i = 0; i < nLength; i++)
		{
			//Get the character out of the string
			char ch = szText[i];

			//Is it a space?
			if( ch == ' ' || ch == '\t' )
			{
				//Skip over some pixels
				nX += m_nCharWidth * fScale;
				continue; //Actual keyword
			}
			else if ( ch == '\n' )
			{
				//Move down some pixels
				nY += m_nCharHeight * fScale;

				//Reset the X
				nX = nColStart;
				continue; //Actual keyword
			}
			
			// Calculate the tile id
			if (m_bOnlyUppercase == true )
				m_cFirstChar = toupper( m_cFirstChar );

			int id = ch - m_cFirstChar;

			// Use the CellAlgorithm to get the source rect
			RECT rChar = CellAlgorithm( id );

			// Draw the character
			pTM->Draw( m_nImageID, nX, nY, fScale, fScale, &rChar, 0, 0, 0, dwColor );

			// Move to the next location on screen
			nX += m_nCharWidth * fScale;
		}
	}

	RECT CellAlgorithm( int id )
	{
		RECT rCell = { };

		rCell.left = (id% m_nNumCols) * m_nCharWidth;
		rCell.top = (id/m_nNumCols) * m_nCharHeight;

		rCell.right = rCell.left + m_nCharWidth;
		rCell.bottom = rCell.top + m_nCharHeight;
	
		return rCell;
	}

	~CBitmapFont()
	{
		pTM = nullptr;
	}
};


#endif