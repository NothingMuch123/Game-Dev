#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

float SceneText::MAX_SHOOT_TIME = 0.3f;

SceneText::SceneText()
	: m_cMinimap(NULL)
	, m_cMap(NULL)
	, projList(NULL)
	, shootTimer(MAX_SHOOT_TIME)
	, enemyList(NULL)
{
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("OBJ1", "OBJ//chair.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_OBJECT]->textureID = LoadTGA("Image//chair.tga");
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	// Load the ground mesh and texture
	meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_DARKGREEN]->textureID = LoadTGA("Image//grass_darkgreen.tga");
	meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_LIGHTGREEN]->textureID = LoadTGA("Image//grass_lightgreen.tga");

	meshList[GEO_BACKGROUND_MAIN] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1,1,1), 0.f, 0.f, 1024.f, 800.f);
	meshList[GEO_BACKGROUND_MAIN]->textureID = LoadTGA("Image//GDev_Assignment02//background_main.tga");
	meshList[GEO_BACKGROUND_MOUNTAIN] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1,1,1), 0.f, 0.f, 1024.f, 800.f);
	meshList[GEO_BACKGROUND_MOUNTAIN]->textureID = LoadTGA("Image//GDev_Assignment02//background_mountain.tga");
	meshList[GEO_BACKGROUND_STARS] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1,1,1), 0.f, 0.f, 1024.f, 800.f);
	meshList[GEO_BACKGROUND_STARS]->textureID = LoadTGA("Image//GDev_Assignment02//background_stars.tga");

	meshList[GEO_TILEGROUND] = MeshBuilder::Generate2DMesh("GEO_TILEGROUND", Color(1, 1, 1), 0.0f, 0.0f, 32.f, 32.f);
	meshList[GEO_TILEGROUND]->textureID = LoadTGA("Image//tile1_ground.tga");
	meshList[GEO_TILETREE] = MeshBuilder::Generate2DMesh("GEO_TILETREE", Color(1, 1, 1), 0.0f, 0.0f, 32.f, 32.f);
	meshList[GEO_TILETREE]->textureID = LoadTGA("Image//tile3_tree.tga");
	meshList[GEO_TILEHERO] = MeshBuilder::Generate2DMesh("GEO_TILEHERO", Color(1, 1, 1), 0.0f, 0.0f, 32.f, 32.f);
	meshList[GEO_TILEHERO]->textureID = LoadTGA("Image//tile2_hero.tga");
	meshList[GEO_TILESTRUCTURE] = MeshBuilder::Generate2DMesh("GEO_TILESTRUCTURE", Color(1, 1, 1), 0.0f, 0.0f, 32.f, 32.f);
	meshList[GEO_TILESTRUCTURE]->textureID = LoadTGA("Image//tile3_structure.tga");

	meshList[GEO_BULLET] = MeshBuilder::Generate2DMesh("Player 1 bullet", Color(1,1,1), 0.f, 0.f, 32.f * 0.125f, 32.f * 0.125f);
	meshList[GEO_BULLET]->textureID = LoadTGA("Image//GDev_Assignment02//player1//bullet.tga");

	unsigned texture = LoadTGA("Image//GDev_Assignment02//tile//tile.tga");
	meshList[GEO_TILE_DIRT] = MeshBuilder::GenerateMeshFromSheet("Tile dirt", Color(1,1,1), 1, 5, 1, 1, 32.f);
	meshList[GEO_TILE_DIRT]->textureID = texture;

	meshList[GEO_TILE_GRASS] = MeshBuilder::GenerateMeshFromSheet("Tile grass", Color(1,1,1), 1, 5, 1, 2, 32.f);
	meshList[GEO_TILE_GRASS]->textureID = texture;

	meshList[GEO_TILE_STONE] = MeshBuilder::GenerateMeshFromSheet("Tile stone", Color(1,1,1), 1, 5, 1, 3, 32.f);
	meshList[GEO_TILE_STONE]->textureID = texture;

	meshList[GEO_TILE_SLAB] = MeshBuilder::GenerateMeshFromSheet("Tile slab", Color(1,1,1), 1, 5, 1, 4, 32.f);
	meshList[GEO_TILE_SLAB]->textureID = texture;

	meshList[GEO_TILE_FLOATING] = MeshBuilder::GenerateMeshFromSheet("Tile floating", Color(1,1,1), 1, 5, 1, 5, 32.f);
	meshList[GEO_TILE_FLOATING]->textureID = texture;

	meshList[GEO_LIVE] = MeshBuilder::Generate2DMesh("Live", Color(1,1,1), 0, 0, 1, 1);
	meshList[GEO_LIVE]->textureID = LoadTGA("Image//GDev_Assignment02//live.tga");

	// Target
	meshList[GEO_TARGET_CLOSE] = MeshBuilder::GenerateSpriteAnimation("Target close", 7, 5);
	meshList[GEO_TARGET_CLOSE]->textureID = LoadTGA("Image//GDev_Assignment02//tile//point.tga");
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_TARGET_CLOSE]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 0, 1);
	}

	// Initialise and load the tile map
	CMap *newMap = new CMap();
	levelMaps.push_back(newMap);
	levelMaps[0]->Init(800, 1024, 25, 32, 800, 1024, 32);
	levelMaps[0]->LoadMap( "Image//GDev_Assignment02//MapDesign_lvl1.csv");
	for (int row = 0; row < levelMaps[0]->GetNumOfTiles_MapHeight(); ++row)
	{
		for (int col = 0; col < levelMaps[0]->GetNumOfTiles_MapWidth(); ++col)
		{
			if (levelMaps[0]->theScreenMap[row][col] == CMap::TILE_ENEMY_GROUND)
			{
				CEnemyIn2D *e = new CEnemyIn2D();
				e->Init( Vector3((col) * levelMaps[0]->GetTileSize(), levelMaps[0]->GetScreen_Height() - (row + 1) * levelMaps[0]->GetTileSize()), 1, meshList[GEO_TILE_STONE], CEnemyIn2D::ENEMY_GROUND );
				e->ChangeStrategy(new CStrategy_Patrol());
				enemyList.push_back(e);
			}
		}
	}

	//m_cMap = new CMap();
	//m_cMap->Init( 600, 800, 24, 32, 600, 1600 );
	//m_cMap->LoadMap( "Image//MapDesign.csv" );
	//m_cMap->Init( 800, 1024, 25, 32, 800, 2048, 32);
	//m_cMap->LoadMap( "Image//GDev_Assignment02//MapDesign_lvl2.csv" );

	/*// Initialise and load the rear tile map
	m_cRearMap = new CMap();
	//m_cRearMap->Init( 600, 800, 24, 32, 600, 1600 );
	//m_cRearMap->LoadMap( "Image//MapDesign_Rear.csv" );
	m_cRearMap->Init( 800, 1024, 25, 32, 800, 2048, 32);
	m_cRearMap->LoadMap( "Image//GDev_Assignment02//MapDesign_Rear_lvl2.csv", targetList_lvl2 );*/

	// Init projectile list
	InitProjList();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;

	bLightEnabled = true;
	level = 1;
	score = 0;
	lives = 3;

	// Init character
	InitCharacter();

	//levelTime = 30.f;
	//extraLives = targetList_lvl1.size();
	//lives = 3;
	//win = targetList_lvl2.size();
	//gameEnded = false;
}

void SceneText::InitProjList()
{
	for (int i = 0; i < 50; ++i)
	{
		CProjectile *p = new CProjectile();
		projList.push_back(p);
	}
}

CProjectile* SceneText::FetchProj()
{
	for (std::vector<CProjectile*>::iterator it = projList.begin(); it != projList.end(); ++it)
	{
		CProjectile *p = (CProjectile*)*it;
		if (!p->GetActive())
		{
			return p;
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		CProjectile *p = new CProjectile();
		projList.push_back(p);
	}
	return projList.back();
}

void SceneText::InitCharacter()
{
	// Create player
	CCharacter *c = new CCharacter(Vector2(100,750), Vector2(2,2));
	c->CalcBound(levelMaps[level - 1]);

	SpriteAnimation *sa = MeshBuilder::GenerateSpriteAnimation("ANIM_FALL_LEFT", 7, 8);
	sa->textureID = LoadTGA("Image//GDev_Assignment02//player1//player1_animations.tga");
	c->SetSprite(sa);

	Animation *a;
	// ANIM_FALL_LEFT
	a = new Animation();
	a->Set(0, 0, 0, 1);
	c->SetAnimation(CCharacter::ANIM_FALL_LEFT, a);

	// ANIM_FALL_RIGHT
	a = new Animation();
	a->Set(1, 1, 0, 1);
	c->SetAnimation(CCharacter::ANIM_FALL_RIGHT, a);

	// ANIM_LIE_LEFT
	a = new Animation();
	a->Set(2,4,0,1);
	c->SetAnimation(CCharacter::ANIM_LIE_LEFT, a);

	// ANIM_LIE_RIGHT
	a = new Animation();
	a->Set(5,7,0,1);
	c->SetAnimation(CCharacter::ANIM_LIE_RIGHT, a);

	// ANIM_IDLE_LEFT
	a = new Animation();
	a->Set(8,11,0,1);
	c->SetAnimation(CCharacter::ANIM_IDLE_LEFT, a);

	// ANIM_IDLE_RIGHT
	a = new Animation();
	a->Set(12,15,0,1);
	c->SetAnimation(CCharacter::ANIM_IDLE_RIGHT, a);

	// ANIM_JUMP_LEFT
	a = new Animation();
	a->Set(16,23,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_JUMP_LEFT, a);

	// ANIM_JUMP_RIGHT
	a = new Animation();
	a->Set(24,31,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_JUMP_RIGHT, a);

	// ANIM_LOOK_DOWN_LEFT
	sa = MeshBuilder::GenerateSpriteAnimation("ANIM_LOOK_DOWN_LEFT", 7, 8);
	a = new Animation();
	a->Set(32,35,1,0.05f);
	c->SetAnimation(CCharacter::ANIM_LOOK_DOWN_LEFT, a);

	// ANIM_LOOK_DOWN_RIGHT
	a = new Animation();
	a->Set(36,39,1,0.05f);
	c->SetAnimation(CCharacter::ANIM_LOOK_DOWN_RIGHT, a);

	// ANIM_LOOK_UP_LEFT
	a = new Animation();
	a->Set(40,43,1,0.05f);
	c->SetAnimation(CCharacter::ANIM_LOOK_UP_LEFT, a);

	// ANIM_LOOK_UP_RIGHT
	a = new Animation();
	a->Set(44,47,1,0.05f);
	c->SetAnimation(CCharacter::ANIM_LOOK_UP_RIGHT, a);

	// ANIM_MOVE_LEFT_SHOOT
	a = new Animation();
	a->Set(48,51,0,0.2f);
	c->SetAnimation(CCharacter::ANIM_MOVE_LEFT_SHOOT, a);

	// ANIM_MOVE_RIGHT_SHOOT
	a = new Animation();
	a->Set(52,55,0,0.2f);
	c->SetAnimation(CCharacter::ANIM_MOVE_RIGHT_SHOOT, a);

	// ANIM_RUN_LEFT
	a = new Animation();
	a->Set(0,10,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_RUN_LEFT, a);

	// ANIM_RUN_RIGHT
	a = new Animation();
	a->Set(0,10,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_RUN_RIGHT, a);

	//ANIM_MOVE_LEFT_SHOOT_TOP
	a = new Animation();
	a->Set(0,19,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_MOVE_LEFT_SHOOT_TOP, a);
	
	//ANIM_MOVE_LEFT_SHOOT_DOWN
	a = new Animation();
	a->Set(0,10,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_MOVE_LEFT_SHOOT_DOWN, a);

	//ANIM_MOVE_RIGHT_SHOOT_TOP
	a = new Animation();
	a->Set(0,19,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_MOVE_RIGHT_SHOOT_TOP, a);
	
	//ANIM_MOVE_RIGHT_SHOOT_DOWN
	a = new Animation();
	a->Set(0,10,0,0.5f);
	c->SetAnimation(CCharacter::ANIM_MOVE_RIGHT_SHOOT_DOWN, a);

	characterList.push_back(c);
}

void SceneText::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if(Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if(Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if(Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if(Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if(Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	/*if (lives <= 0)
	{
		gameEnded = true;
	}
	if (!gameEnded)
	{
		if (level == 1)
		{
			if (extraLives == 0)
			{
				level = 2;
				characterList[0]->SetPos(Vector2(100, 750));
				lives += extraLives;
				for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
				{
					CEnemy *e = (CEnemy*)*it;
					e->Reset();
				}
				for (std::vector<CProjectile*>::iterator it = projList.begin(); it != projList.end(); ++it)
				{
					CProjectile *p = (CProjectile*)*it;
					p->Reset();
				}
			}
			if (levelTime > 0.f)
			{
				levelTime -= dt;
			}
			else // Level 1 ended
			{
				level = 2;
				characterList[0]->SetPos(Vector2(100, 750));
				lives += extraLives;
				for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
				{
					CEnemy *e = (CEnemy*)*it;
					e->Reset();
				}
				for (std::vector<CProjectile*>::iterator it = projList.begin(); it != projList.end(); ++it)
				{
					CProjectile *p = (CProjectile*)*it;
					p->Reset();
				}
			}
		}
		else if (level == 2)
		{
			if (win == 0)
			{
				gameEnded = true;
			}
		}*/

		/*SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_TARGET_CLOSE]);
		if (sa)
		{
			sa->Update(dt);
		}*/

		if (shootTimer < MAX_SHOOT_TIME)
		{
			shootTimer += dt;
		}

		characterList[0]->Update(dt, levelMaps[level - 1]);
		/*for (std::vector<CTarget*>::iterator it = targetList_lvl1.begin(); it != targetList_lvl1.end(); ++it)
			{
				CTarget *t = (CTarget*)*it;
				if (t->GetActive()) // Update target
				{
					t->Update(dt);
				}
				if (t->GetActive() && t->GetType() == CTarget::TARGET_DESTROY && t->GetSA()->m_anim->ended) // Delete target if disappear
				{
					t->SetActive(false);
				}

				// Target-Enemy collision check
				for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
				{
					CEnemy *e = (CEnemy*)*it;

					// Collision check
					if (t->GetActive() && t->GetType() != CTarget::TARGET_DESTROY && e->GetActive() &&
						t->GetMinBound().x < e->GetMaxBound().x && t->GetMaxBound().x > e->GetMinBound().x &&
						t->GetMinBound().y < e->GetMaxBound().y && t->GetMaxBound().y > e->GetMinBound().y) // Collide
					{
						t->SetType(CTarget::TARGET_DESTROY);
						//--extraLives;
						e->Reset();
					}
				}
			}*/

			/*for (std::vector<CEnemySpawner*>::iterator it = enemySpawnerList_lvl1.begin(); it != enemySpawnerList_lvl1.end(); ++it)
			{
				CEnemySpawner *spawner = (CEnemySpawner*)*it;
				if (spawner->CheckSpawn(dt)) // Spawn enemy
				{
					CEnemy *e = FetchEnemy();
					e->Init(spawner->GetPos(), Vector2(2,2));
				}
			}*/
		/*else if (level == 2)
		{
			characterList[0]->Update(dt, m_cMap);
			for (std::vector<CTarget*>::iterator it = targetList_lvl2.begin(); it != targetList_lvl2.end(); ++it)
			{
				CTarget *t = (CTarget*)*it;
				if (t->GetActive())
				{
					t->Update(dt);
				}
				if (t->GetActive() && t->GetType() == CTarget::TARGET_DESTROY && t->GetSA()->m_anim->ended)
				{
					t->SetActive(false);
				}

				// Target-Enemy collision check
				for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
				{
					CEnemy *e = (CEnemy*)*it;

					// Collision check
					if (t->GetActive() && t->GetType() != CTarget::TARGET_DESTROY && e->GetActive() &&
						t->GetMinBound().x < e->GetMaxBound().x && t->GetMaxBound().x > e->GetMinBound().x &&
						t->GetMinBound().y < e->GetMaxBound().y && t->GetMaxBound().y > e->GetMinBound().y) // Collide
					{
						t->SetType(CTarget::TARGET_DESTROY);
						e->Reset();
					}
				}
			}

			for (std::vector<CEnemySpawner*>::iterator it = enemySpawnerList_lvl2.begin(); it != enemySpawnerList_lvl2.end(); ++it)
			{
				CEnemySpawner *spawner = (CEnemySpawner*)*it;
				if (spawner->CheckSpawn(dt)) // Spawn enemy
				{
					CEnemy *e = FetchEnemy();
					e->Init(spawner->GetPos(), Vector2(2,2));
				}
			}
		}*/

		// Update enemy list
		/*for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			CEnemy *e = (CEnemy*)*it;
			if (e->GetActive())
			{
				if (level == 1)
				{
					e->Update(dt, levelMaps);
				}
				else if (level == 2)
				{
					e->Update(dt, m_cMap);
				}
			}

			// Enemy-Player collision check
			for (std::vector<CCharacter*>::iterator it = characterList.begin(); it != characterList.end(); ++it)
			{
				CCharacter *c = (CCharacter*)*it;

				if (level == 1)
				{
					if (e->GetActive() &&
						levelMaps->GetMapOffset().x + c->GetMinBound().x < e->GetMaxBound().x && levelMaps->GetMapOffset().x + c->GetMaxBound().x > e->GetMinBound().x &&
						levelMaps->GetMapOffset().y + c->GetMinBound().y < e->GetMaxBound().y && levelMaps->GetMapOffset().y + c->GetMaxBound().y > e->GetMinBound().y) // Collide
					{
						c->SetPos(Vector2(100,750));
						//--lives;
						e->Reset();
					}
				}
				else if (level == 2)
				{
					if (e->GetActive() &&
						m_cMap->GetMapOffset().x + c->GetMinBound().x < e->GetMaxBound().x && m_cMap->GetMapOffset().x + c->GetMaxBound().x > e->GetMinBound().x &&
						m_cMap->GetMapOffset().y + c->GetMinBound().y < e->GetMaxBound().y && m_cMap->GetMapOffset().y + c->GetMaxBound().y > e->GetMinBound().y) // Collide
					{
						c->SetPos(Vector2(100,750));
						--lives;
						e->Reset();
					}
				}
			}
		}*/

		for (std::vector<CProjectile*>::iterator it = projList.begin(); it != projList.end(); ++it)
		{
			CProjectile *p = (CProjectile*)*it;
			if (p->GetActive()) // Update projectile
			{
				p->Update(dt, m_cMap);
			}

			// Projectile-Target collision check
			/*if (level == 1) // Using level 1 targetList
			{
				for (std::vector<CTarget*>::iterator it = targetList_lvl1.begin(); it != targetList_lvl1.end(); ++it)
				{
					CTarget *t = (CTarget*)*it;

					// Collision check
					if (t->GetActive() && t->GetType() != CTarget::TARGET_DESTROY && t->GetOpen() && p->GetActive() &&
						t->GetMinBound().x < levelMaps->GetMapOffset().x + p->GetMaxBound().x && t->GetMaxBound().x > levelMaps->GetMapOffset().x + p->GetMinBound().x &&
						t->GetMinBound().y < levelMaps->GetMapOffset().y + p->GetMaxBound().y && t->GetMaxBound().y > levelMaps->GetMapOffset().y + p->GetMinBound().y) // Collide
					{
						t->SetType(CTarget::TARGET_DESTROY);
						//--extraLives;
						p->Reset();
					}
				}
			}
			else if (level == 2) // Using level 2 targetList
			{
				for (std::vector<CTarget*>::iterator it = targetList_lvl2.begin(); it != targetList_lvl2.end(); ++it)
				{
					CTarget *t = (CTarget*)*it;

					// Collision check
					if (t->GetActive() && t->GetType() != CTarget::TARGET_DESTROY && t->GetOpen() && p->GetActive() &&
						t->GetMinBound().x < m_cMap->GetMapOffset().x + p->GetMaxBound().x && t->GetMaxBound().x > m_cMap->GetMapOffset().x + p->GetMinBound().x &&
						t->GetMinBound().y < m_cMap->GetMapOffset().y + p->GetMaxBound().y && t->GetMaxBound().y > m_cMap->GetMapOffset().y + p->GetMinBound().y) // Collide
					{
						// Adding score
						switch (t->GetType())
						{
						case CTarget::TARGET_RED :
							{
								score += 1;
							}
							break;
						case CTarget::TARGET_LIGHT_BLUE :
							{
								score += 2;
							}
							break;
						case CTarget::TARGET_DARK_BLUE :
							{
								score += 3;
							}
							break;
						case CTarget::TARGET_GREEN :
							{
								score += 4;
							}
							break;
						case CTarget::TARGET_YELLOW :
							{
								score += 5;
							}
							break;
						case CTarget::TARGET_WHITE :
							{
								score += 6;
							}
							break;
						}
						
						t->SetType(CTarget::TARGET_DESTROY);
						--win;
						p->Reset();
					}
				}
			}*/

			// Projectile-Enemy collision check
			/*for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
			{
				CEnemy *e = (CEnemy*)*it;
				if (level == 1)
				{
					if (e->GetActive() && p->GetActive() &&
						levelMaps->GetMapOffset().x + p->GetMinBound().x < e->GetMaxBound().x && levelMaps->GetMapOffset().x + p->GetMaxBound().x > e->GetMinBound().x &&
						levelMaps->GetMapOffset().y + p->GetMinBound().y < e->GetMaxBound().y && levelMaps->GetMapOffset().y + p->GetMaxBound().y > e->GetMinBound().y) // Collide
					{
						e->Reset();
						p->Reset();
					}
				}
				else if (level == 2)
				{
					if (e->GetActive() && p->GetActive() &&
						m_cMap->GetMapOffset().x + p->GetMinBound().x < e->GetMaxBound().x && m_cMap->GetMapOffset().x + p->GetMaxBound().x > e->GetMinBound().x &&
						m_cMap->GetMapOffset().y + p->GetMinBound().y < e->GetMaxBound().y && m_cMap->GetMapOffset().y + p->GetMaxBound().y > e->GetMinBound().y) // Collide
					{
						e->Reset();
						p->Reset();
					}
				}
			}*/
		}

		// Update enemy list based on level
		for (std::vector<CEnemyIn2D*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			CEnemyIn2D *e = (CEnemyIn2D*)*it;
			e->Update(levelMaps[level - 1], dt);
		}

		camera.Update(dt);

		fps = (float)(1.f / dt);
}

/********************************************************************************
 Update Camera position
 ********************************************************************************/
void SceneText::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
 Update Character status
 ********************************************************************************/
void SceneText::UpdateCharacterStatus(const CCharacter::CHARACTER_ACTION action, const bool status)
{
	if (action == CCharacter::CA_SHOOT) // Shoot
	{
		if (shootTimer >= MAX_SHOOT_TIME)
		{
			CCharacter *c = characterList[0];
			switch (c->GetCurrentAnim()) // Shoot while prone
			{
			case CCharacter::ANIM_LIE_LEFT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.15f * c->GetScale().x), Vector2(-1 * CProjectile::BULLET_SPEED, 0 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_LIE_RIGHT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.15f * c->GetScale().x), Vector2(1 * CProjectile::BULLET_SPEED, 0 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_IDLE_LEFT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.4f * c->GetScale().x), Vector2(-1 * CProjectile::BULLET_SPEED, 0 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_IDLE_RIGHT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.3f * c->GetScale().x), Vector2(1 * CProjectile::BULLET_SPEED, 0 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_MOVE_LEFT_SHOOT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.4f * c->GetScale().x), Vector2(-1 * CProjectile::BULLET_SPEED, 0 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_MOVE_RIGHT_SHOOT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.3f * c->GetScale().x), Vector2(1 * CProjectile::BULLET_SPEED, 0 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_LOOK_DOWN_LEFT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.4f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(0 * CProjectile::BULLET_SPEED, -1 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_LOOK_DOWN_RIGHT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.4f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(0 * CProjectile::BULLET_SPEED, -1 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_LOOK_UP_LEFT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.6f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(0 * CProjectile::BULLET_SPEED, 1 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_LOOK_UP_RIGHT :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.6f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(0 * CProjectile::BULLET_SPEED, 1 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_MOVE_LEFT_SHOOT_DOWN :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(-0.5 * CProjectile::BULLET_SPEED, -0.5 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_MOVE_LEFT_SHOOT_TOP :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(-0.5 * CProjectile::BULLET_SPEED, 0.5 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_MOVE_RIGHT_SHOOT_DOWN :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(0.5 * CProjectile::BULLET_SPEED, -0.5 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			case CCharacter::ANIM_MOVE_RIGHT_SHOOT_TOP :
				{
					CProjectile *p = FetchProj();
					p->Init(CProjectile::PROJ_BULLET, Vector2(c->GetPos().x + m_cMap->GetTileSize() * 0.5f * c->GetScale().x, c->GetPos().y + m_cMap->GetTileSize() * 0.5f * c->GetScale().x), Vector2(0.5 * CProjectile::BULLET_SPEED, 0.5 * CProjectile::BULLET_SPEED), m_cMap, true);
				}
				break;
			}
			shootTimer = 0.f;
		}
	}
	else // Other actions
	{
		characterList[0]->SetActions(action, status);
	}
}

static const float SKYBOXSIZE = 1000.f;

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void SceneText::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, float rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, levelMaps[level - 1]->GetScreen_Width(), 0, levelMaps[level - 1]->GetScreen_Height(), -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				if (rotate)
					modelStack.Rotate(rotate, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void SceneText::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneText::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x=0; x<10; x++)
	{
		for (int z=0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x-5.0f, z-5.0f, 0.0f);
			if ( ((x*9+z) % 2) == 0)
				RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void SceneText::RenderSkybox()
{
	//left
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

void SceneText::RenderBackGround_Mountain()
{
	Render2DMesh(meshList[GEO_BACKGROUND_MOUNTAIN], false, 1.f, -levelMaps[level - 1]->GetMapOffset().x * 0.2f, levelMaps[level - 1]->GetMapOffset().y);
	Render2DMesh(meshList[GEO_BACKGROUND_MOUNTAIN], false, 1.f, (-levelMaps[level - 1]->GetMapOffset().x * 0.2f) + levelMaps[level - 1]->GetScreen_Width(), levelMaps[level - 1]->GetMapOffset().y);
}

void SceneText::RenderBackGround_Stars()
{
	Render2DMesh(meshList[GEO_BACKGROUND_STARS], false, 1.f, -levelMaps[level - 1]->GetMapOffset().x * 0.5f, levelMaps[level - 1]->GetMapOffset().y);
	Render2DMesh(meshList[GEO_BACKGROUND_STARS], false, 1.f, (-levelMaps[level - 1]->GetMapOffset().x * 0.5f) + levelMaps[level - 1]->GetScreen_Width(), levelMaps[level - 1]->GetMapOffset().y);
}

void SceneText::Render()
{
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	// Render the background image
	//RenderBackground();
	RenderBackGround_Stars();
	RenderBackGround_Mountain();

	// Render the rear tile map

	// Render tile map
	if (level == 1)
	{
		RenderTileMap(levelMaps[level - 1]);
		//RenderTargetList(targetList_lvl1, levelMaps);
		RenderEnemyList();
	}

	// Render character
	RenderCharacter();

	// Render projectile
	RenderProjList();

	// Render lives
	Render2DMesh(meshList[GEO_LIVE], false, 64.f, 0.f, 736.f);
	std::ostringstream sLives;
	sLives << "x" << lives;
	RenderTextOnScreen(meshList[GEO_TEXT], sLives.str(), Color(1, 1, 1), 3, 7, 56);

	//On screen text
	/*if (gameEnded && win != 0)
	{
		std::ostringstream gameover;
		gameover << "Game Over";
		RenderTextOnScreen(meshList[GEO_TEXT], gameover.str(), Color(1, 1, 1), 5, 17.5, 25);
	}
	else if (gameEnded && win == 0)
	{
		std::ostringstream youWin;
		youWin << "You Win";
		RenderTextOnScreen(meshList[GEO_TEXT], youWin.str(), Color(1, 1, 1), 5, 22.5, 25);
	}*/

	/*if (level == 1)
	{
		std::ostringstream sLevelTime;
		int time = ceil(levelTime);
		sLevelTime << "Time: " << time;
		RenderTextOnScreen(meshList[GEO_TEXT], sLevelTime.str(), Color(1, 1, 1), 4, 0, 2);
	}*/

	std::ostringstream sFPS;
	sFPS.precision(5);
	sFPS << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], sFPS.str(), Color(1, 1, 1), 2, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderEnemyList()
{
	/*for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy *e = (CEnemy*)*it;
		if (e->GetActive())
		{
			Render2DMesh(e->GetCurrentAnimation(), false, e->GetScale().x * map->GetTileSize(), e->GetPos().x - map->GetMapOffset().x, e->GetPos().y - map->GetMapOffset().y);
		}
	}*/
	for (std::vector<CEnemyIn2D*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemyIn2D *e = (CEnemyIn2D*)*it;
		if (e->GetActive())
		{
			Render2DMesh(e->GetMesh(), false, 1.f, e->GetPos().x, e->GetPos().y);
		}
	}
}

void SceneText::RenderTargetList(std::vector<CTarget*> targetList, CMap *map)
{
	for (std::vector<CTarget*>::iterator it = targetList.begin(); it != targetList.end(); ++it)
	{
		CTarget *t = (CTarget*)*it;
		if (t->GetActive())
		{
			if (t->GetOpen())
			{
				Render2DMesh(t->GetSA(), false, map->GetTileSize(), t->GetPos().x - map->GetMapOffset().x, t->GetPos().y - map->GetMapOffset().y);
			}
			else
			{
				Render2DMesh(meshList[GEO_TARGET_CLOSE], false, map->GetTileSize(), t->GetPos().x - map->GetMapOffset().x, t->GetPos().y - map->GetMapOffset().y);
			}
		}
	}
}

void SceneText::RenderProjList()
{
	for (std::vector<CProjectile*>::iterator it = projList.begin(); it != projList.end(); ++it)
	{
		CProjectile *p = (CProjectile*)*it;
		if (p->GetActive())
		{
			switch (p->GetType())
			{
			case CProjectile::PROJ_BULLET :
				{
					Render2DMesh(meshList[GEO_BULLET], false, 1.f, p->GetPos().x, p->GetPos().y, Math::RadianToDegree(atan2(p->GetVel().Normalized().y, p->GetVel().Normalized().x)));
				}
				break;
			}
		}
	}
}

void SceneText::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	if (m_cMap)
	{
		delete m_cMap;
	}

	/*if (m_cRearMap)
	{
		delete m_cRearMap;
	}*/

	if (m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}

	for (std::vector<CProjectile*>::iterator it = projList.begin(); it != projList.end(); ++it)
	{
		CProjectile *p = (CProjectile*)*it;
		if (p)
		{
			delete p;
			p = NULL;
		}
	}

	/*for (std::vector<CTarget*>::iterator it = targetList_lvl1.begin(); it != targetList_lvl1.end(); ++it)
	{
		CTarget *t = (CTarget*)*it;
		if (t)
		{
			delete t;
			t = NULL;
		}
	}

	for (std::vector<CTarget*>::iterator it = targetList_lvl2.begin(); it != targetList_lvl2.end(); ++it)
	{
		CTarget *t = (CTarget*)*it;
		if (t)
		{
			delete t;
			t = NULL;
		}
	}*/

	/*for (std::vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy *e = (CEnemy*)*it;
		if (e)
		{
			delete e;
			e = NULL;
		}
	}

	for (std::vector<CEnemySpawner*>::iterator it = enemySpawnerList_lvl1.begin(); it != enemySpawnerList_lvl1.end(); ++it)
	{
		CEnemySpawner *spawner = (CEnemySpawner*)*it;
		if (spawner)
		{
			delete spawner;
			spawner = NULL;
		}
	}

	for (std::vector<CEnemySpawner*>::iterator it = enemySpawnerList_lvl2.begin(); it != enemySpawnerList_lvl2.end(); ++it)
	{
		CEnemySpawner *spawner = (CEnemySpawner*)*it;
		if (spawner)
		{
			delete spawner;
			spawner = NULL;
		}
	}*/

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

/********************************************************************************
 Render the tile map. This is a private function for use in this class only
 ********************************************************************************/
void SceneText::RenderTileMap(CMap *map)
{
	int m = 0;
	//m_cMap->GetMapFineOffset().x = ((int)m_cMap->GetMapOffset().x) % m_cMap->GetTileSize();
	map->SetMapFineOffset( Vector2(((int)map->GetMapOffset().x) % map->GetTileSize() , map->GetMapFineOffset().y) );
	for(int i = 0; i < map->GetNumOfTiles_Height(); i ++)
	{
		for(int k = 0; k < map->GetNumOfTiles_Width()+1; k ++)
		{
			m = map->GetTileOffset().x + k;

			// If we have reached the right side of the map, then do not display the extra column of tiles
			if ((map->GetTileOffset().x + k) >= map->GetNumOfTiles_MapWidth())
			{
				break;
			}

			switch (map->theScreenMap[i][m])
			{
			case CMap::TILE_DIRT: 
				{
					Render2DMesh(meshList[GEO_TILE_DIRT], false, 1.0f, k*map->GetTileSize() - map->GetMapFineOffset().x, map->GetScreen_Height() - (i+1)*map->GetTileSize());
				}
				break;
			case CMap::TILE_GRASS: 
				{
					Render2DMesh(meshList[GEO_TILE_GRASS], false, 1.0f, k*map->GetTileSize() - map->GetMapFineOffset().x, map->GetScreen_Height() - (i+1)*map->GetTileSize());
				}
				break;
			case CMap::TILE_STONE:
				{
					Render2DMesh(meshList[GEO_TILE_STONE], false, 1.0f, k*map->GetTileSize() - map->GetMapFineOffset().x, map->GetScreen_Height() - (i+1)*map->GetTileSize());
				}
				break;
			case CMap::TILE_SLAB:
				{
					Render2DMesh(meshList[GEO_TILE_SLAB], false, 1.0f, k*map->GetTileSize() - map->GetMapFineOffset().x, map->GetScreen_Height() - (i+1)*map->GetTileSize());
				}
				break;
			case CMap::TILE_FLOATING:
				{
					Render2DMesh(meshList[GEO_TILE_FLOATING], false, 1.0f, k*map->GetTileSize() - map->GetMapFineOffset().x, map->GetScreen_Height() - (i+1)*map->GetTileSize());
				}
				break;
			}
		}
	}
}

/*void SceneText::RenderRearTileMap()
{
	//m_cRearMap->GetMapOffset().x = (int)(m_cMap->GetMapOffset().x * 0.5);
	m_cRearMap->SetMapOffset( Vector2((int)(m_cMap->GetMapOffset().x * 0.5) , m_cRearMap->GetMapFineOffset().y) );
	//m_cRearMap->GetMapOffset().y = 0;
	m_cRearMap->SetMapOffset( Vector2(m_cRearMap->GetMapOffset().x , 0) );
	//m_cRearMap->GetTileOffset().y = 0;
	m_cRearMap->SetTileOffset( Vector2(m_cRearMap->GetTileOffset().x , 0) );
	//m_cRearMap->GetTileOffset().x = (int)(m_cRearMap->GetMapOffset().x / m_cRearMap->GetTileSize());
	m_cRearMap->SetTileOffset( Vector2((int)(m_cRearMap->GetMapOffset().x / m_cRearMap->GetTileSize()) , m_cRearMap->GetTileOffset().y) );
	if (m_cRearMap->GetTileOffset().x + m_cRearMap->GetNumOfTiles_Width() > m_cRearMap->GetNumOfTiles_MapWidth())
	{
		//m_cRearMap->GetTileOffset().x = m_cRearMap->GetNumOfTiles_MapWidth() - m_cRearMap->GetNumOfTiles_Width();
		m_cRearMap->SetTileOffset( Vector2(m_cRearMap->GetNumOfTiles_MapWidth() - m_cRearMap->GetNumOfTiles_Width() , m_cRearMap->GetTileOffset().y) );
	}
	//m_cRearMap->GetMapFineOffset().x = ((int)m_cRearMap->GetMapOffset().x) % m_cRearMap->GetTileSize();
	m_cRearMap->SetMapFineOffset( Vector2(((int)m_cRearMap->GetMapOffset().x) % m_cRearMap->GetTileSize() , m_cRearMap->GetMapFineOffset().y) );

	int m = 0;
	for (int i = 0; i < m_cRearMap->GetNumOfTiles_Height(); ++i)
	{
		for (int k = 0; k < m_cRearMap->GetNumOfTiles_Width() + i; ++k)
		{
			m = m_cRearMap->GetTileOffset().x + k;
			// If we have reached the right side of the map, then do not display the extra column of tiles
			if ((m_cRearMap->GetTileOffset().x + k) >= m_cRearMap->GetNumOfTiles_MapWidth())
			{
				break;
			}
			if (m_cRearMap->theScreenMap[i][m] == 3)
			{
				Render2DMesh(meshList[GEO_TILESTRUCTURE], false, 1.f, k * m_cRearMap->GetTileSize() - m_cRearMap->GetMapFineOffset().x, m_cMap->GetScreen_Height() - (i+1) * m_cRearMap->GetTileSize());
			}
		}
	}
}*/

void SceneText::RenderCharacter()
{
	// Hero animation
	CCharacter *c = characterList[0];
	Render2DMesh(c->GetSprite(), false, levelMaps[level - 1]->GetTileSize() * c->GetScale().x, c->GetPos().x, c->GetPos().y);
}