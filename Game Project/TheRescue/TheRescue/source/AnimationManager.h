/*
	File: AnimationManger.h
	Author: Gabriel Islas
	Class: SGP
	Purpose: Serves as the manager of the several animations at once by updating, setting and loading them.
*/
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <vector>
#include <Windows.h>

#include "Animation.h"
class Frame;
class AnimInfo;

////////////////////////////////////////////////////
// SGD Wrappers
//	- forward declarations reduce header dependencies
//	- can ONLY make pointers & references to the type
class CSGD_Direct3D;
class CSGD_TextureManager;

class AnimationManager
{
	//Private data members
	std::vector<Animation*> Animations;

	Frame* fFrame;
	Animation* anime;
	int m_nFrameCount;

	//Declare an instance of this class. 
	static AnimationManager* s_Instance;

	//Struct to temporarily hold values for the rest of the engine.
	struct tInfo
	{
		char filePath[64];
		int aX, aY;
		int pX, pY, w, h;
		int cX, cY, cW, cH;
		double time;
		int nEvent;
	};

	//Texture manager pointer. 
	CSGD_TextureManager* pTm;
public:
	//SINGLETON
	static AnimationManager* GetInstance();

	//Function: DeleteInstance
	//Author: Gabriel Islas
	//Parameters: none
	//Return: STATIC void
	//Notes: Used to delete an instance of this singleton class. 
	static void DeleteInstance();

	//Constructor 
	AnimationManager();

	//Destructor
	~AnimationManager();

	//Function: GetFinalCollision
	//Author: Gabriel Islas
	//Parameters: none
	//Return: RECT
	//Notes: Returns the final collision box for the game. 
	RECT GetFinalCollision(); 

	//Function: UpdateAnimation
	//Author: Gabriel Islas
	//Parameters: int AnimIndex, int posX, int posY
	//Return: void
	//Notes: Used to update all of the animation info collected.
	void UpdateAnimInfo( int AnimIndex, int posX, int posY );

	//Function: DrawAnim
	//Author: Gabriel Islas
	//Parameters: POINT position, AnimInfo info, float fRotation, unsigned char alpha
	//Return: void
	//Notes: Used to display the frame at it's curFrame of curAnim
	void DrawAnim(POINT position, AnimInfo* info, float fRotation, unsigned char alpha);
};

#endif