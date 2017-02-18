/*
	File: Animation.h
	Author: Gabriel Islas
	Class: SGP
	Purpose: Collects and stores the frames of the animation and the overall image. 
*/
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "../SGD Wrappers/SGD_String.h"
class Frame;
class CSGD_Direct3D;
class CSGD_TextureManager;

class Animation
{
	//Private data members
	int m_nImage;
	TOSTRINGSTREAM m_szFilePath;
	float m_fTimeWaited;
	bool m_bLooping;
	int m_nCurrFrame;

	CSGD_TextureManager* pTm; 
public:
		std::vector<Frame*> Frames;
	Animation(/*Frame* frame, */int eventID, const char* szFilePath);
	~Animation();

	//Function: PlayAnim
	//Author: Gabriel Islas
	//Parameters: int posX, int posY
	//Returns: void
	//Notes: Will play the animation that is being loaded into the system. 
	void PlayAnim(int posX, int posY);

	//Function: AddFrame
	//Author: Gabriel Islas
	//Parameters: Frame* frame
	//Returns: void
	//Notes: Will add a new frame to the animation after being loaded from the XML.
	void AddFrame( Frame* frame);

	//Accessors/Mutators
	int GetImageID() { return m_nImage;}
	float GetTimeWaited() { return m_fTimeWaited;}
	bool GetLooping() { return m_bLooping; }
	Frame* GetFrameAndIndex(int index) { return Frames[index]; }


};

#endif