/*
	File: AnimationManager.cpp
	Author: Gabriel Islas
	Class: SGP
	Purpose: Serves as the manager of the several animations at once by updating, setting and loading them.
*/
#include "AnimationManager.h"
#include "Frame.h"
#include "AnimInfo.h"
//#include "stdafx.h"

////////////////////////////////////////////////////
// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../TinyXML/tinyxml.h"

//GLOBAL instance of the AnimationManager class. 
AnimationManager* AnimationManager::s_Instance = nullptr;

AnimationManager::AnimationManager() 
{

	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load the file
	doc.LoadFile( "Resources/Animations/Animations.xml" );

	// Access the root element 
	TiXmlElement* pRoot = doc.RootElement();

	// Iterate through the list of nodes
	TiXmlElement* pAnimation = pRoot->FirstChildElement( "Animation" );

	while( pAnimation != nullptr )
	{
		tInfo info;
		
		// Attempt to get the name from the node
		const char* name = pAnimation->Attribute("FilePath");
		std::string sName(name);
		std::string tempName;
		for( unsigned int i = 0; i <= sName.size()-1; i++)
		{
			tempName.push_back(sName[i]);
		}

		strcpy_s(info.filePath, tempName.c_str());

		pAnimation->FirstChild("Frame");

		TiXmlElement* pFrame = pAnimation->FirstChildElement("Frame");

		
		// Push the Frame onto the Animation vector, making sure to send the address of the frame object
		/*Animation* */anime = new Animation(info.nEvent, info.filePath );

		while (pFrame != nullptr)
		{
			// Attempt the attributes
			pFrame->Attribute( "AnchorX", &info.aX );
			pFrame->Attribute( "AnchorY", &info.aY );
			pFrame->Attribute( "PosX", &info.pX );
			pFrame->Attribute( "PosY", &info.pY );
			pFrame->Attribute( "Width", &info.w );
			pFrame->Attribute( "Height", &info.h );
			pFrame->Attribute( "CollisionX", &info.cX );
			pFrame->Attribute( "CollisionY", &info.cY );
			pFrame->Attribute( "CollisionWidth", &info.cW );
			pFrame->Attribute( "CollisionHeight", &info.cH );
			pFrame->Attribute( "Time", &info.time);
			pFrame->Attribute( "EventID", &info.nEvent);
			
			// Move to the next frame node
			pFrame = pFrame->NextSiblingElement( "Frame" );

			// Transfer the info from the tInfo struct into a Frame object
		/*Frame**/ fFrame = new Frame(info.aX, info.aY, info.pX, info.pY, info.w, 
					info.h, info.cX, info.cY, info.cW, info.cH, info.time );
			anime->AddFrame(fFrame);
		}

		// Store the animation info into the vector
		Animations.push_back( anime ); //あにめ！

		// Move to the next frame node 
		pAnimation = pAnimation->NextSiblingElement( "Animation" );

	}

	//Initialize instance of the texture manager. 
	pTm = CSGD_TextureManager::GetInstance();
}

AnimationManager::~AnimationManager()
{
	//Delete dynamic memory
	for(unsigned int i = 0; i <= Animations.size()-1; i++)
		delete Animations[i];
	Animations.clear();
}

//SINGLETON
/*static*/ AnimationManager* AnimationManager::GetInstance()
{
	if (nullptr == s_Instance) s_Instance = new AnimationManager;
	return s_Instance;
}

//Function: DeleteInstance
//Author: Gabriel Islas
//Parameters: none
//Return: void
//Notes: Used to delete an instance of this singleton class. 
void AnimationManager::DeleteInstance()
{
	if (nullptr != s_Instance) delete s_Instance;
	s_Instance = nullptr;
}

//Function: UpdateAnimation
//Author: Gabriel Islas
//Parameters: int AnimIndex, int posX, int posY
//Return: void
//Notes: Used to update all of the animation info collected, also plays the animation.
void AnimationManager::UpdateAnimInfo( int AnimIndex, int posX, int posY )
{
	//Call play animation.
	Animations[AnimIndex]->PlayAnim(posX, posY);
}

//Function: GetFinalCollision
//Author: Gabriel Islas
//Parameters: none
//Return: RECT
//Notes: Returns the final collision box for the game. 
RECT AnimationManager::GetFinalCollision()
{
	return fFrame->GetCollisionRect();
}

//Function: DrawAnim
//Author: Gabriel Islas
//Parameters: POINT position, AnimInfo* info, float fRotation, unsigned char Alpha
//Return: void
//Notes: Used to display the frame at it's curFrame of curAnim
void AnimationManager::DrawAnim(POINT position, AnimInfo* info, float fRotation, unsigned char alpha)
{
	//This is the algorithm of code used for Anchor point animation as demonstrated by Doug in SGD. 
	//Use this as a reference point to get the animation running correctly through anchor points with
	//respect to time.

	//Update the animation timer
	//Need to add time on frame. 
	//info->AddTimeOnFrame(Core().DeltaTime());

	pTm->Draw(Animations[info->GetCurAnim()]->GetImageID(), //Get the image ID.
		position.x-(Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetAnchor().x-Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetImageRect().left),// Calculate the anchor point offset.
		position.y-(Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetAnchor().y-Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetImageRect().top), // Calculate the anchor point offset. 
		1.0f, 1.0f, &Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetImageRect(), //Get the current frames' image rect.
		static_cast<float>(Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetAnchor().x-Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetImageRect().left), //Base the rotation point off the anchor point, upcast to float.
		static_cast<float>(Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetAnchor().y-Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetImageRect().top),  //Base the rotation point off the anchor point, upcast to float. 
		fRotation,D3DCOLOR_ARGB(alpha, 255,255,255)); //Rotate image at desired radian value. 
	
	if( info->GetTimeOnFrame() >= Animations[info->GetCurAnim()]->Frames[info->GetCurFrame()]->GetDuration())
	{

		info->SetTimeOnFrame(0);
		info->UpdateCurrentFrame(); //Increment frame value

		//The end of animation?
		if( info->GetCurFrame() == Animations[info->GetCurAnim()]->Frames.size() )
		{
			//Loop?
			if(Animations[info->GetCurAnim()]->GetLooping() == true)
			{
				info->SetCurFrame(0);
			}
			else
			{
				//Stop on the last frame
				//m_nCurrFrame = Frames.size()-1;
				info->SetCurFrame(Animations[info->GetCurAnim()]->Frames.size()-1);
			}
		}

	}
}