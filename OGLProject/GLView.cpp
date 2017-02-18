// GLView.cpp
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

#include "GLView.h"


////////////////////////////////////////////////////////////////////////////
// Do not put any OpenGL code here. General guidence on constructors in 
// general is to not put anything that can fail here either (opening files,
// allocating memory, etc.)
GLView::GLView(void) : screenWidth(800), screenHeight(600), bFullScreen(false), bAnimated(true), samples(1)
	{
	
	
	
	}
	
	
///////////////////////////////////////////////////////////////////////////////
// OpenGL related startup code is safe to put here. Load textures, etc.
void GLView::Initialize(void)
	{
	// Make sure OpenGL entry points are set
	glewInit();

	// Initialze Shader Manager
	shaderManager.InitializeStockShaders();

	glEnable(GL_DEPTH_TEST);

	
	glClearColor(0.1f, 0.1f, 0.4f, 1.0f);


	///The one dimensional lookup table
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_1D, texture);
	GLubyte textureData[4][3] = { 32, 0, 0,
								  64, 0, 0,
								  128, 0, 0,
								  255, 0, 0};

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 4, 0, GL_RGB, 
		GL_UNSIGNED_BYTE, textureData);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	GLfloat floorHeight = -1.0f;
	//GLfloat floorSpace = 0.5f;
	GLfloat floorHalfWidth = 50.0f;

	M3DVector3f vGroundVerts[] = {
									-floorHalfWidth, floorHeight, floorHalfWidth,
									floorHalfWidth, floorHeight, floorHalfWidth,
									floorHalfWidth, floorHeight, -floorHalfWidth,
									-floorHalfWidth, floorHeight, -floorHalfWidth };
	M3DVector2f vGroundTex[] = {
									0.0f, 0.0f,
									1.0f, 0.0f, 
									1.0f, 1.0f,
									0.0f, 1.0f };

	glGenBuffers(1, &hFloorVerts); //Allocates a handle
	glBindBuffer(GL_ARRAY_BUFFER, hFloorVerts); //Select this buffer
	//Copy data into buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(M3DVector3f) * 4, vGroundVerts, GL_STATIC_DRAW);
	
	glGenBuffers(1, &hFloorTexCoord); 
	glBindBuffer(GL_ARRAY_BUFFER, hFloorTexCoord);

	//Just for an example...allocate an emtpy buffer, and then fill it in after the fact. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(M3DVector2f) * 4, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(M3DVector2f) * 4, vGroundTex);


	gltMakeTorus(torusBatch, 0.4f, 0.15f, 25, 25);

	//Call all of the stuff. 
	// Read our .obj files
	glGenBuffers(1, &whbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, whbuffer);
	bool res = loadOBJ("WH.obj", vertices, uvs, normals);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &unibuffer);
	glBindBuffer(GL_ARRAY_BUFFER, unibuffer);
	res = loadOBJ("uni.obj", vertices2, uvs2, normals2);
	glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);

	glGenBuffers(1, &pbbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pbbuffer);
	res = loadOBJ("5pb.obj", vertices3, uvs3, normals3);
	glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(glm::vec3), &vertices3[0], GL_STATIC_DRAW);

	// Preload textures
	glGenTextures(1, &texDetail);
	glBindTexture(GL_TEXTURE_2D, texDetail);
	LoadBMPAsTexture("detail.bmp", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);

	glGenTextures(1, &texFloor);
	glBindTexture(GL_TEXTURE_2D,  texFloor);
	LoadBMPAsTexture("grass.bmp", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);

	//////////////////////////////////////
	// Preload our shaders
	hShaderStar = gltLoadShaderPairWithAttributes("Toon.vp", "Toon.fp", 2, 
		GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal");

	hUniformMVP = glGetUniformLocation(hShaderStar, "mvpMatrix");
	hUniformMV = glGetUniformLocation(hShaderStar, "mvMatrix");
	hUniformNormal = glGetUniformLocation(hShaderStar, "normalMatrix");
	hUniformLight = glGetUniformLocation(hShaderStar, "vLightPosition");
	hUniformColorTable = glGetUniformLocation(hShaderStar, "colorTable");


	if(hShaderStar == NULL || hUniformMVP < 0 || hUniformNormal < 0 || hUniformLight < 0 || 
		hUniformMV < 0 || hUniformColorTable < 0 )
		MessageBox(NULL, "Problem with Shader", "Uh oh...", MB_OK);
	}


///////////////////////////////////////////////////////////////////////////////
// Do your cleanup here. Free textures, display lists, buffer objects, etc.
void GLView::Shutdown(void)
	{


	}


///////////////////////////////////////////////////////////////////////////////
// This is called at least once and before any rendering occurs. If the screen
// is a resizeable window, then this will also get called whenever the window
// is resized.
void GLView::Resize(GLsizei nWidth, GLsizei nHeight)
	{
	screenWidth = nWidth;
        screenHeight = nHeight;
	glViewport(0, 0, nWidth, nHeight);
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);

	viewFrustum.SetPerspective(35.0f, float(nWidth)/float(nHeight), 1.0f, 100.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelViewMatrix.LoadIdentity();
	}


void GLView::MoveCamera(void)
	{
		static CStopWatch timer;
		float deltaTime = timer.GetElapsedSeconds(); //Time since last call.
		timer.Reset();
		if(GetAsyncKeyState(VK_UP))
			cameraFrame.MoveForward(deltaTime * 3.0f);

		if(GetAsyncKeyState(VK_DOWN))
			cameraFrame.MoveForward(deltaTime * -3.0f);

		if(GetAsyncKeyState(VK_LEFT))
			cameraFrame.RotateWorld(m3dDegToRad(60.0f) * deltaTime, 0.0f, 1.0f, 0.0f);

		if(GetAsyncKeyState(VK_RIGHT))
			cameraFrame.RotateWorld(m3dDegToRad(-60.0f) * deltaTime, 0.0f, 1.0f, 0.0f);
	}

void GLView::LoadBMPAsTexture(const char* szFilename,
						GLenum filter, 
						GLenum wrapMode)
	{
		GLbyte* pBits;
		int nWidth, nHeight;
		pBits = gltReadBMPBits(szFilename, &nWidth, &nHeight);
		if(pBits == NULL)
		{
			MessageBox(NULL, "Cannot load texture", szFilename, MB_OK);
			return;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //this is actually the default...

		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, nWidth, nHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pBits);
		free(pBits);

		if(filter != GL_NEAREST && filter != GL_LINEAR)
			glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapMode);
	}

///////////////////////////////////////////////////////////////////////////////
// Load an object from a file other than a header with predefined info. 
bool GLView::loadOBJ(
					const char * path,
					std::vector < glm::vec3 > & out_vertices,
					std::vector < glm::vec2 > & out_uvs,
					std::vector < glm::vec3 > & out_normals)
	{
		//Create temporary data to store from the OBJ file. 
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		FILE * file = fopen(path, "r");
		if( file == NULL )
		{
			MessageBox(NULL, "Problem with opening OBJ file", "Problem...", MB_OK);
			return false;
		}

		while( 1 )
		{
 
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.
 
			// else : parse lineHeader
			
			if ( strcmp( lineHeader, "v" ) == 0 ){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z ); 
				temp_vertices.push_back(vertex); 
			}else if ( strcmp( lineHeader, "vt" ) == 0 ){
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y );
				temp_uvs.push_back(uv);
			}else if ( strcmp( lineHeader, "vn" ) == 0 ){
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
				temp_normals.push_back(normal);
			}else if ( strcmp( lineHeader, "f" ) == 0 ){
				 std::string vertex1, vertex2, vertex3;
				 unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				 int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
				 if (matches != 9){
					 MessageBox(NULL, "File can't be read by simple parser : (Try exporting with other options", "Whoops!", MB_OK);
				     return false;
				 }
				 vertexIndices.push_back(vertexIndex[0]);
				 vertexIndices.push_back(vertexIndex[1]);
				 vertexIndices.push_back(vertexIndex[2]);
				 uvIndices    .push_back(uvIndex[0]);
				 uvIndices    .push_back(uvIndex[1]);
				 uvIndices    .push_back(uvIndex[2]);
				 normalIndices.push_back(normalIndex[0]);
				 normalIndices.push_back(normalIndex[1]);
				 normalIndices.push_back(normalIndex[2]);
			}
		}

		// For each vertex of each triangle
		for( unsigned int i=0; i<vertexIndices.size(); i++ )
		{
			unsigned int vertexIndex = vertexIndices[i];

			glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];

			out_vertices.push_back(vertex);
		}

		for( unsigned int i=0; i<uvIndices.size(); i++ )
		{
			unsigned int uvIndex = uvIndices[i];

			glm::vec2 uv = temp_uvs[ uvIndex-1 ];

			out_uvs.push_back(uv);
		}


		for( unsigned int i=0; i<normalIndices.size(); i++ )
		{
			unsigned int normalIndex = normalIndices[i];

			glm::vec3 normal = temp_vertices[ normalIndex-1 ];

			out_normals.push_back(normal);
		}

		return true; // Return true if everything worked out. 
}

void GLView::DrawFloor(void)
	{
		M3DVector4f vFloorColor = { 1.0f, 1.0f, 1.0f, 0.75f };
		shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE,
			transformPipeline.GetModelViewProjectionMatrix(),
			vFloorColor, 0);
		//glUseProgram(hShaderFloor);
		//glUniform4fv(hUniformColor, 1, vFloorColor);
		//glUniformMatrix4fv(hUniformMVP, 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix()); 


		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texDetail);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texFloor);

		//floorBatch.Draw();

		//Turn on two attribute arrays
		glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
		glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);

		//Set up the vertex positions
		glBindBuffer(GL_ARRAY_BUFFER, hFloorVerts);
		glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, hFloorTexCoord);
		glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//Draw the floor---
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
		glDisableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);

	//	glDisable(GL_BLEND);
	}

///////////////////////////////////////////////////////////////////////////////
// Render a frame. The owning framework is responsible for buffer swaps,
// flushes, etc.
void GLView::Render(void)
	{
		MoveCamera();
	static CStopWatch timer;
	float yRot = timer.GetElapsedSeconds() * 60.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelViewMatrix.PushMatrix();	
		M3DMatrix44f mCamera;
		cameraFrame.GetCameraMatrix(mCamera);
		modelViewMatrix.MultMatrix(mCamera);

		// Draw stuff relative to the camera

		DrawFloor();

		M3DVector4f vLightPosWorld = { 0.0f, 3.0f, 2.0f, 1.0f };
		M3DVector4f vLightPosEye;

		// vLightPosEye = mCamera * vLightPosWorld;
		modelViewMatrix.GetMatrix(mCamera); //Conatains camera + inversion
		//cameraFrame.GetCameraMatrix(mCamera);
		m3dTransformVector4(vLightPosEye, vLightPosWorld, mCamera);
		glEnable(GL_CULL_FACE);

		/////////////////////////////////////////////////////////////////
		// Drawing the torus
		modelViewMatrix.PushMatrix();
		glCullFace(GL_FRONT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		M3DVector4f vColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		modelViewMatrix.Scale(1.75f, 1.75f, 1.75f);
		modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
		glLineWidth(10.0f);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
			transformPipeline.GetModelViewMatrix(), 
			transformPipeline.GetProjectionMatrix(), 
			vLightPosEye,vColor);

		torusBatch.Draw();


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		M3DVector4f vColored = { 1.0f, 0.0f, 0.0f, 1.0f };
		//shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
		//	transformPipeline.GetModelViewMatrix(), 
		//	transformPipeline.GetProjectionMatrix(), 
		//	vLightPosEye,vColored);
		glBindTexture(GL_TEXTURE_1D, texture);
		glUseProgram(hShaderStar);
		glUniformMatrix4fv(hUniformMVP, 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix()); 
		glUniformMatrix4fv(hUniformMV, 1, GL_FALSE, transformPipeline.GetModelViewMatrix());
		glUniformMatrix3fv(hUniformNormal, 1, GL_FALSE, transformPipeline.GetNormalMatrix());
		glUniform3fv(hUniformLight, 1, vLightPosWorld);
		//glUniform1fv(hUniformColorTable, 1, vColored);


		glCullFace(GL_BACK);

		torusBatch.Draw();
		modelViewMatrix.PopMatrix();
		//End torus
		///////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////
		//Draw loaded wh model
		modelViewMatrix.PushMatrix();
		glCullFace(GL_FRONT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		modelViewMatrix.Translate(0.0f, 0.0f, -4.5f);
		modelViewMatrix.Scale(1.75f, 1.75f, 1.75f);
		modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
		glLineWidth(5.0f);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
			transformPipeline.GetModelViewMatrix(), 
			transformPipeline.GetProjectionMatrix(), 
			vLightPosEye,vColor);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, whbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,      // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDisableVertexAttribArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
		//	transformPipeline.GetModelViewMatrix(), 
		//	transformPipeline.GetProjectionMatrix(), 
		//	vLightPosEye,vColored);
		glBindTexture(GL_TEXTURE_1D, texture);
		glUseProgram(hShaderStar);
		glUniformMatrix4fv(hUniformMVP, 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix()); 
		glUniformMatrix4fv(hUniformMV, 1, GL_FALSE, transformPipeline.GetModelViewMatrix());
		glUniformMatrix3fv(hUniformNormal, 1, GL_FALSE, transformPipeline.GetNormalMatrix());
		glUniform3fv(hUniformLight,1 , vLightPosWorld);
		//glUniform1fv(hUniformColorTable, 1, vColored);


		glCullFace(GL_BACK);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, whbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDisableVertexAttribArray(0);

		modelViewMatrix.PopMatrix();
		//End wh model
		///////////////////////////////////////////////////////////////////
		
		///////////////////////////////////////////////////////////////////
		//Draw loaded uni model
		modelViewMatrix.PushMatrix();
		glCullFace(GL_FRONT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		modelViewMatrix.Translate(-5.0f, 0.0f, -4.5f);
		modelViewMatrix.Scale(1.75f, 1.75f, 1.75f);
		modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
		glLineWidth(5.0f);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
			transformPipeline.GetModelViewMatrix(), 
			transformPipeline.GetProjectionMatrix(), 
			vLightPosEye,vColor);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, unibuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,      // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices2.size());
		glDisableVertexAttribArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
		//	transformPipeline.GetModelViewMatrix(), 
		//	transformPipeline.GetProjectionMatrix(), 
		//	vLightPosEye,vColored);
		glBindTexture(GL_TEXTURE_1D, texture);
		glUseProgram(hShaderStar);
		glUniformMatrix4fv(hUniformMVP, 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix()); 
		glUniformMatrix4fv(hUniformMV, 1, GL_FALSE, transformPipeline.GetModelViewMatrix());
		glUniformMatrix3fv(hUniformNormal, 1, GL_FALSE, transformPipeline.GetNormalMatrix());
		glUniform3fv(hUniformLight,1 , vLightPosWorld);
		//glUniform1fv(hUniformColorTable, 1, vColored);


		glCullFace(GL_BACK);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, unibuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices2.size());
		glDisableVertexAttribArray(0);

		modelViewMatrix.PopMatrix();
		//End uni model
		///////////////////////////////////////////////////////////////////
				
		///////////////////////////////////////////////////////////////////
		//Draw loaded 5pb model
		modelViewMatrix.PushMatrix();
		glCullFace(GL_FRONT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		modelViewMatrix.Translate(5.0f, 0.0f, -4.5f);
		modelViewMatrix.Scale(1.75f, 1.75f, 1.75f);
		modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
		glLineWidth(5.0f);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
			transformPipeline.GetModelViewMatrix(), 
			transformPipeline.GetProjectionMatrix(), 
			vLightPosEye,vColor);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, pbbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,      // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices3.size());
		glDisableVertexAttribArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
		//	transformPipeline.GetModelViewMatrix(), 
		//	transformPipeline.GetProjectionMatrix(), 
		//	vLightPosEye,vColored);
		glBindTexture(GL_TEXTURE_1D, texture);
		glUseProgram(hShaderStar);
		glUniformMatrix4fv(hUniformMVP, 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix()); 
		glUniformMatrix4fv(hUniformMV, 1, GL_FALSE, transformPipeline.GetModelViewMatrix());
		glUniformMatrix3fv(hUniformNormal, 1, GL_FALSE, transformPipeline.GetNormalMatrix());
		glUniform3fv(hUniformLight,1 , vLightPosWorld);
		//glUniform1fv(hUniformColorTable, 1, vColored);


		glCullFace(GL_BACK);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, pbbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices3.size());
		glDisableVertexAttribArray(0);

		modelViewMatrix.PopMatrix();
		//End 5pb model
		///////////////////////////////////////////////////////////////////

		glDisable(GL_CULL_FACE);



	modelViewMatrix.PopMatrix();
	}
