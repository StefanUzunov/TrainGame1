#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class CDiamond;
class CStar;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	bool WatchCollision(glm::vec3 a, glm::vec3 b);
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox* m_pSkybox;
	CCamera* m_pCamera;
	vector <CShaderProgram*>* m_pShaderPrograms;
	CPlane* m_pPlanarTerrain;
	CFreeTypeFont* m_pFtFont;
	COpenAssetImportMesh* m_pBarrelMesh;
	COpenAssetImportMesh* m_pHorseMesh;
	COpenAssetImportMesh* m_pTrainMesh;
	COpenAssetImportMesh* m_pLampMesh;
	CSphere* m_pSphere;
	vector <CDiamond*>* m_pDiamond;
	vector <CDiamond*>* m_pDiamond1;
	vector <CDiamond*>* m_pDiamond2;
	vector <CStar*>* m_pStar;
	vector <CStar*>* m_pStar1;
	vector <CStar*>* m_pStar2;
	CHighResolutionTimer* m_pHighResolutionTimer;
	CAudio* m_pAudio;
	CCatmullRom* m_pCatmullRom;

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
	int m_score;
	float m_currentDistance;
	float m_cameraSpeed;
	float forwardOffset;
	float rightOffset;
	float upOffset;
	glm::vec3 TrainPosition;
	glm::mat4 m_trainOrientation;
	glm::vec3 starPosition;
	glm::vec3 starPosition1;
	glm::vec3 starPosition2;
	glm::vec3 diamondPosition;
	glm::vec3 diamondPosition1;
	glm::vec3 diamondPosition2;
	float m_t;
	float m_shake;
	float m_shakespeed;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayHUD();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
};
