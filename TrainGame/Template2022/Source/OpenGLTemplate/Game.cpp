/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

 version 6.1a 13/02/2022 - Sorted out Release mode and a few small compiler warnings
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "Diamond.h"
#include "Star.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pTrainMesh = NULL;
	m_pLampMesh = NULL;
	m_pSphere = NULL;
	m_pDiamond = NULL;
	m_pDiamond1 = NULL;
	m_pDiamond2 = NULL;
	m_pStar = NULL;
	m_pStar1 = NULL;
	m_pStar2 = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.05f;
	forwardOffset = 20.0f;
	rightOffset = 0.0f;
	upOffset = 3.0f;
	TrainPosition = glm::vec3(20, 0, 20);
	m_trainOrientation = glm::mat4(1);
	starPosition = glm::vec3(-130, 1.2, -115);
	starPosition1 = glm::vec3(-130, 1.2, 115);
	starPosition2 = glm::vec3(130, 1.2, 120);
	diamondPosition = glm::vec3(-147, 1.2, -100);
	diamondPosition1 = glm::vec3(127, 1.2, -120);
	diamondPosition2 = glm::vec3(-127, 1.2, 125);
	m_score = 0;
	m_t = 0;
	m_shake = 0;
	m_shakespeed = 0.001;
}

// Destructor
Game::~Game()
{
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pTrainMesh;
	delete m_pLampMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pCatmullRom;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	if (m_pStar != NULL) {
		for (unsigned int i = 0; i < m_pStar->size(); i++)
			delete (*m_pStar)[i];
	}
	delete m_pStar;

	if (m_pStar1 != NULL) {
		for (unsigned int i = 0; i < m_pStar1->size(); i++)
			delete m_pStar1;
	}

	if (m_pStar2 != NULL) {
		for (unsigned int i = 0; i < m_pStar2->size(); i++)
			delete m_pStar2;
	}

	if (m_pDiamond != NULL) {
		for (unsigned int i = 0; i < m_pDiamond->size(); i++)
			delete (*m_pDiamond)[i];
	}
	delete m_pDiamond;

	if (m_pDiamond1 != NULL) {
		for (unsigned int i = 0; i < m_pDiamond1->size(); i++)
			delete m_pDiamond1;
	}

	if (m_pDiamond2 != NULL) {
		for (unsigned int i = 0; i < m_pDiamond2->size(); i++)
			delete m_pDiamond2;
	}

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise()
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram*>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pTrainMesh = new COpenAssetImportMesh;
	m_pLampMesh = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	m_pDiamond = new vector <CDiamond*>;
	m_pDiamond1 = new vector <CDiamond*>;
	m_pDiamond2 = new vector <CDiamond*>;
	m_pStar = new vector <CStar*>;
	m_pStar1 = new vector <CStar*>;
	m_pStar2 = new vector <CStar*>;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height);
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float)width / (float)height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");

	for (int i = 0; i < (int)sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int)sShaderFileNames[i].size() - 4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\" + sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram* pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram* pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);

	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pTrainMesh->Load("resources\\models\\Train\\electrictrain.obj");
	m_pLampMesh->Load("resources\\models\\Lamp\\StreetLamp.obj");

	// Creates a star pointer
	CStar* Star = new CStar;

	//Creates a diamond pointer
	CDiamond* Diamond = new CDiamond;

	//Create a diamond
	Diamond->Create("resources\\textures\\diamond.bmp");
	//Add it to the vector
	m_pDiamond->push_back(Diamond);
	m_pDiamond1->push_back(Diamond);
	m_pDiamond2->push_back(Diamond);

	//Create the star
	Star->Create("resources\\textures\\star.bmp");
	//Add it to the vector
	m_pStar->push_back(Star);
	m_pStar1->push_back(Star);
	m_pStar2->push_back(Star);

	glm::vec3 p0 = glm::vec3(-500, 10, -200);
	glm::vec3 p1 = glm::vec3(0, 10, -200);
	glm::vec3 p2 = glm::vec3(0, 10, 200);
	glm::vec3 p3 = glm::vec3(-500, 10, 200);
	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	//Setting the tracks texture
	m_pCatmullRom->CreateTrack("resources\\textures\\rails.png");
}

// Render method runs repeatedly in a loop
void Game::Render()
{
	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram* pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	pMainProgram->SetUniform("CubeMapTex", 1);

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pMainProgram->SetUniform("t", m_t);
	pMainProgram->SetUniform("m_shake", m_shake);

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light

	glm::vec4 lightPosition2 = glm::vec4(0, 10, 0, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light2.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light2.La", glm::vec3(1.0f, 1.0f, 0.0f)); //Ambient colour of light
	pMainProgram->SetUniform("light2.Ld", glm::vec3(1.0f, 1.0f, 0.0f));// Diffuse colour of light
	pMainProgram->SetUniform("light2.Ls", glm::vec3(1.0f, 1.0f, 0.0f)); // Specular colour of light
	pMainProgram->SetUniform("light2.direction", glm::normalize(viewNormalMatrix * glm::vec3(0, -1, 0)));
	pMainProgram->SetUniform("light2.exponent", 39.0f);
	pMainProgram->SetUniform("light2.cutoff", 25.0f);
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.2f, 0.2f, 0.2f));
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f, 0.0f, 1.0f));
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f, 1.0f, 1.0f));
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property

	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("renderSkybox", true);
	// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
	glm::vec3 vEye = m_pCamera->GetPosition();
	modelViewMatrixStack.Translate(vEye);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSkybox->Render();
	pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();

	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	// Render the train
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(TrainPosition);
	modelViewMatrixStack *= m_trainOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(90.0f));
	modelViewMatrixStack.Scale(0.4f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pTrainMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the lamp
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-100.0f, 0.0f, -100.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(90.0f));
	modelViewMatrixStack.Scale(2.f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pLampMesh->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
		m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//m_pCatmullRom->RenderCentreline();
	//m_pCatmullRom->RenderOffsetCurves();
	m_pCatmullRom->RenderTrack();
	modelViewMatrixStack.Pop();

	// Rendering the CentreLine
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 3.0f, 0.0f));
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderCentreline();
	modelViewMatrixStack.Pop();

	//Render diamond 0
	if (m_pDiamond->size() > 0)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(diamondPosition);
		modelViewMatrixStack.Scale(0.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		(*m_pDiamond)[0]->Render();
		modelViewMatrixStack.Pop();
	}

	//Render diamond 1
	if (m_pDiamond1->size() > 0)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(diamondPosition1);
		modelViewMatrixStack.Scale(0.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		(*m_pDiamond1)[0]->Render();
		modelViewMatrixStack.Pop();
	}

	//Render diamond 2
	if (m_pDiamond2->size() > 0)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(diamondPosition2);
		modelViewMatrixStack.Scale(0.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		(*m_pDiamond2)[0]->Render();
		modelViewMatrixStack.Pop();
	}

	//Render the star0
	if (m_pStar->size() > 0)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(starPosition);
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -45.0f);
		modelViewMatrixStack.Scale(0.1f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("checkdiscard", true);
		(*m_pStar)[0]->Render();
		modelViewMatrixStack.Pop();
	}

	//Render star1
	if (m_pStar1->size() > 0)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(starPosition1);
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -45.0f);
		modelViewMatrixStack.Scale(0.1f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("checkdiscard", true);
		(*m_pStar1)[0]->Render();
		modelViewMatrixStack.Pop();
	}

	//Render the star2
	if (m_pStar2->size() > 0)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(starPosition2);
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -45.0f);
		modelViewMatrixStack.Scale(0.1f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("checkdiscard", true);
		(*m_pStar2)[0]->Render();
		modelViewMatrixStack.Pop();
	}
	pMainProgram->SetUniform("checkdiscard", false);

	// Draw the 2D graphics after the 3D graphics
	DisplayHUD();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());
}


// Update method runs repeatedly with the Render method
void Game::Update()
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);

	//Creating the TNB frame and defining location of the train and orientation
	m_currentDistance += m_dt * m_cameraSpeed;
	glm::vec3 p;
	glm::vec3 pNext;
	m_pCatmullRom->Sample(m_currentDistance, p);
	m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext);
	glm::vec3 T = glm::normalize(pNext - p);
	glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
	glm::vec3 B = glm::normalize(glm::cross(N, T));
	TrainPosition = (p);
	m_trainOrientation = glm::mat4(glm::mat3(T, B, N));

	//Setting the camera to follow the train
	m_pCamera->Set(p + forwardOffset * T + N * rightOffset + B * upOffset, (pNext + T * forwardOffset + N * rightOffset + B * 3.0f), B);

	//Watch for Collisions and delete when it occurs
	if (WatchCollision(m_pCamera->GetPosition(), starPosition) && m_pStar->size() > 0) {
		m_pStar->erase(m_pStar->begin());
		m_score -= 4;
	}
	if (WatchCollision(m_pCamera->GetPosition(), starPosition1) && m_pStar1->size() > 0) {
		m_pStar1->erase(m_pStar1->begin());
		m_score -= 4;
	}
	if (WatchCollision(m_pCamera->GetPosition(), starPosition2) && m_pStar2->size() > 0) {
		m_pStar2->erase(m_pStar2->begin());
		m_score -= 4;
	}
	if (WatchCollision(m_pCamera->GetPosition(), diamondPosition) && m_pDiamond->size() > 0)
	{
		m_pDiamond->erase(m_pDiamond->begin());
		m_score++;
	}
	if (WatchCollision(m_pCamera->GetPosition(), diamondPosition1) && m_pDiamond1->size() > 0)
	{
		m_pDiamond1->erase(m_pDiamond1->begin());
		m_score++;
	}
	if (WatchCollision(m_pCamera->GetPosition(), diamondPosition2) && m_pDiamond2->size() > 0)
	{
		m_pDiamond2->erase(m_pDiamond2->begin());
		m_score++;
	}

	//Loss condition
	if (m_score <= -1)
	{
		upOffset = 80;
	}

	//Win condition
	if (m_score >= 3) {
		m_currentDistance = 0;
		upOffset = 50;
		m_shake = 0;
	}

	//Creating a shake effect
	m_t += 0.001 * (float)m_dt;
	m_shake += m_shakespeed * (float)m_dt;
	if (m_shake > 1)
		m_shakespeed = -0.003;
	else if (m_shake < 0)
		m_shakespeed = 0.003;
}

bool Game::WatchCollision(glm::vec3 a, glm::vec3 b)
{
	float distance = glm::length(b - a);
	return distance < 5;
}

void Game::DisplayHUD()
{
	CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
	{
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
	}

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
	}
	//Setting the train speed on the HUD
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(20, height - 40, 20, "Train Speed: %s", std::to_string(m_cameraSpeed).c_str());

	//Setting up the score
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(20, height - 60, 20, "Score: %s", std::to_string(m_score).c_str());

	//Loss condition
	if (m_score <= -1) {
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(350, height - 260, 20, "You lost! ", std::to_string(m_score).c_str());
	}
	//Win condition
	if (m_score >= 3) {
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(350, height - 260, 20, "You win! ", std::to_string(m_score).c_str());
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
}

WPARAM Game::Execute()
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if (!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	MSG msg;

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (m_appActive) {
			GameLoop();
		}
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;

	switch (message) {

	case WM_ACTIVATE:
	{
		switch (LOWORD(w_param))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			m_appActive = true;
			m_pHighResolutionTimer->Start();
			break;
		case WA_INACTIVE:
			m_appActive = false;
			break;
		}
		break;
	}

	case WM_SIZE:
		RECT dimensions;
		GetClientRect(window, &dimensions);
		m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch (w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			forwardOffset = 20.0f;
			rightOffset = 0.0f;
			upOffset = 3.0f;
			break;
		case '2':
			forwardOffset = 0.0f;
			rightOffset = 0.0f;
			upOffset = 50.0f;
			break;
		case 'W':
			m_cameraSpeed += 0.1;
			break;
		case 'S':
			m_cameraSpeed -= 0.1;
			break;
		case 'A':
			rightOffset += -3.0f;
			break;
		case 'D':
			rightOffset += 3.0f;
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance()
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance)
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int)
{
	Game& game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
