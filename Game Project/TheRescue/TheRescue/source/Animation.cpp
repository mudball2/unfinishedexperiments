/*
	File: Animation.cpp
	Author: Gabriel Islas
	Class: SGP
	Purpose: Collects and stores the frames of the animation and the overall image. 
*/
#include "Animation.h"
//#include "stdafx.h"
#include "Frame.h"
#include "Game.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

Animation::Animation(/*Frame* frame, */int eventID, const char* szFilePath) : m_bLooping(true), 
	m_nCurrFrame(0), m_fTimeWaited(0)
{
	// Setting the image ID
	//m_nImage = eventID;

	//Firstly, create an instance of the texture manager. 
	pTm = CSGD_TextureManager::GetInstance();

	//Set the filepath
	m_szFilePath << szFilePath;
	
	// Setting the image ID and loading the texture
	m_nImage = pTm->LoadTexture(m_szFilePath.str().c_str());

	//Clear the string stream
	//m_szFilePath.clear();
}

Animation::~Animation()
{
	//Cleans up the memory for the frames. 
	for(unsigned int i = 0; i <= Frames.size()-1; i++)
		delete Frames[i];
	Frames.clear();

}

//Function: PlayAnim
//Author: Gabriel Islas
//Parameters: int posX, int posY
//Returns: void
//Notes: Will play the animation that is being loaded into the system. 
void Animation::PlayAnim( int posX, int posY)
{
	
	////This is the algorithm of code used for Anchor point animation as demonstrated by Doug in SGD. 
	////Use this as a reference point to get the animation running correctly through anchor points with
	////respect to time.

	////Update the animation timer
	//m_fTimeWaited += Core().DeltaTime();

	//Core().GetTextureManager().Draw(m_nImage, 
	//	posX-(Frames[m_nCurrFrame]->GetAnchor().x-Frames[m_nCurrFrame]->GetImageRect().left), 
	//	posY-(Frames[m_nCurrFrame]->GetAnchor().y-Frames[m_nCurrFrame]->GetImageRect().top), 
	//	1.0f, 1.0f, &Frames[m_nCurrFrame]->GetImageRect());
	//
	//if( m_fTimeWaited >= Frames[m_nCurrFrame]->GetDuration())
	//{

	//	m_fTimeWaited = 0;
	//	++m_nCurrFrame;

	//	//The end of animation?
	//	if( m_nCurrFrame == Frames.size() )
	//	{
	//		//Loop?
	//		if(m_bLooping == true)
	//		{
	//			m_nCurrFrame = 0;
	//		}
	//		else
	//		{
	//			//Stop on the last frame
	//			m_nCurrFrame = Frames.size()-1;
	//		}
	//	}

	//}
}


//Function: AddFrame
//Author: Gabriel Islas
//Parameters: Frame* frame
//Returns void
//Notes: Will add a new frame to the animation after being loaded from the XML.
void Animation::AddFrame( Frame* frame)
{
	// Pushing the frame object onto the vector
	Frames.push_back( frame );
}

