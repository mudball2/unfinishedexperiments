// GLView.h
/* 
Copyright (c) 2009, Richard S. Wright Jr.
GLTools Open Source Library
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other 
materials provided with the distribution.

Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
to endorse or promote products derived from this software without specific prior 
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __GLVIEW__
#define __GLVIEW__

#include <gltools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

class GLView
	{
	public:
		GLView();
		virtual ~GLView() {};

		void Initialize(void);							// Called after context is created
		void Shutdown(void);							// Called before context is destroyed
		void Resize(GLsizei nWidth, GLsizei nHeight);	// Called when window is resized, at least once when window is created
		void Render(void);								// Called to update OpenGL view
		
		// These methods are used by the calling framework. Set the appropriate internal
		// protected variables to tell the parent framework your desired configuration
		inline GLuint GetWidth(void) { return screenWidth; }
		inline GLuint GetHeight(void) { return screenHeight; }
		inline GLboolean GetFullScreen(void) { return bFullScreen; }
		inline GLboolean GetAnimated(void) { return bAnimated; }
		inline GLuint GetSamples(void) { return samples; }
			
	protected:
		GLsizei	 screenWidth;			// Desired window or desktop width
		GLsizei  screenHeight;			// Desired window or desktop height
		
		GLboolean bFullScreen;			// Request to run full screen
		GLboolean bAnimated;			// Request for continual updates

		GLuint    samples;

		GLShaderManager		shaderManager;			// Shader Manager
		GLMatrixStack		modelViewMatrix;		// Modelview Matrix
		GLMatrixStack		projectionMatrix;		// Projection Matrix
		GLFrustum			viewFrustum;			// View Frustum
		GLGeometryTransform	transformPipeline;		// Geometry Transform Pipeline
		GLFrame				cameraFrame;			// Camera frame

		//TO CHANGE THE CULL MODES:
		//LOOK IN THE BOOK - PG. 102 -103
		//glEnable(GL_CULL_FACE); 
		//Now for the rest of the stuff..
		GLTriangleBatch		torusBatch;				//The batch for the torus 


		void MoveCamera(void);

		GLuint					hShaderStar;
		GLint					hUniformMVP; //Uniform for the MVP
		GLint					hUniformNormal; //Uniform for the normal matrix
		GLint					hUniformMV; //Uniform for the MV
		GLint					hUniformLight; //Uniform for the position of the light.
		GLint					hUniformColorTable; //Uniform for the color table.

		GLuint					texture; //Lookup texture.
		GLuint					normTex; //Normal lookup texture. 

		//Stuff for the floor. 
		GLuint				hFloorVerts;
		GLuint				hFloorTexCoord;

		//The object loader function because this is all I have. 
		bool loadOBJ(
					const char * path,
					std::vector < glm::vec3 > & out_vertices,
					std::vector < glm::vec2 > & out_uvs,
					std::vector < glm::vec3 > & out_normals);

		void LoadBMPAsTexture(const char* szFilename,
						GLenum filter, 
						GLenum wrapMode);

		//Data for our loaded object. 
		std::vector< glm::vec3 > vertices;
		std::vector< glm::vec2 > uvs;
		std::vector< glm::vec3 > normals;

		std::vector< glm::vec3 > vertices2;
		std::vector< glm::vec2 > uvs2;
		std::vector< glm::vec3 > normals2;
		
		std::vector< glm::vec3 > vertices3;
		std::vector< glm::vec2 > uvs3;
		std::vector< glm::vec3 > normals3;

		//VBOs
		GLuint whbuffer;
		GLuint unibuffer;
		GLuint pbbuffer;

		//Textures to load.
		GLuint					texFloor;
		GLuint					texDetail;

		void DrawFloor(void);



	};
		
#endif // __GLVIEW__
