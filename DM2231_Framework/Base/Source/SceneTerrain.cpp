#include "SceneTerrain.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneTerrain::SceneTerrain() : m_cMinimap(NULL)
{
}

SceneTerrain::~SceneTerrain()
{
	if(m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}
}

void SceneTerrain::Init()
{
	// Initialise SceneBase
	SceneBase::Init();

	// Sprite animation
	meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("Cat", 1, 6);
	meshList[GEO_SPRITE_ANIMATION]->textureID[0] = LoadTGA("Image//cat.tga");
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 1000, 1.f);
	}

	// Load minimap texture
	m_cMinimap = new CMinimap();
	m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("Minimap", Color(1,1,1), 1.f));
	m_cMinimap->GetBackground()->textureID[0] = LoadTGA("Image//rock.tga");
	m_cMinimap->GetBackground()->textureID[1] = LoadTGA("Image//sand.tga");
	m_cMinimap->SetBorder(MeshBuilder::GenerateMinimapBorder("Minimap Border", Color(1,1,1), 1.f));
	m_cMinimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("Minimap Avatar", Color(1,1,1), 1.f));

	// Create obj
	obj = new CObj(GEO_TENT, Vector3(300, 2 + terrainSize.y * ReadHeightMap(m_heightMap, 300/terrainSize.x, 100/terrainSize.z), 100), Vector3(0,0,0), Vector3(10, 10, 10), Vector3(1,1,1));
	objList.push_back(obj);

	obj = new CObj(GEO_OBJECT, Vector3(-50, terrainSize.y * ReadHeightMap(m_heightMap, -50.f/terrainSize.x, -50.f/terrainSize.z), -50), Vector3(0,0,0), Vector3(1,1,1), Vector3(1,1,1));
	objList.push_back(obj);

	obj = new CObj(GEO_OBJECT, Vector3(100, terrainSize.y * ReadHeightMap(m_heightMap, 100.f/4000, 100.f/4000), 100), Vector3(0,0,0), Vector3(1,1,1), Vector3(1,1,1));
	objList.push_back(obj);

	for (int i = 0; i < 10; ++i)
	{
		CProjectile *p = new CProjectile;
		projectileList.push_back(p);
	}
	FireRateCounter = 0;
}

void SceneTerrain::Update(double dt)
{
	// Update SceneBase
	SceneBase::Update(dt);

	rotateAngle -= Application::camera_yaw;

	// Sprite animation update
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	if (sa)
	{
		sa->Update(dt);
	}

	for (int i = 0; i < projectileList.size(); ++i)
	{
		CProjectile *p = projectileList[i];
		p->Update(dt);
	}
	FireRateCounter += dt;
	camera.Update(dt, m_heightMap, terrainSize);

	fps = (float)(1.f / dt);
}

void SceneTerrain::Render()
{
	// Render SceneBase
	SceneBase::Render();

	//RenderSkybox();
	RenderSkyPlane();
	RenderTerrain();
	RenderObject();
	RenderTextInWorld();
	Render2D();
}

void SceneTerrain::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();
}

void SceneTerrain::Reset()
{

}

void SceneTerrain::UpdateCameraStatus(unsigned char key)
{
	camera.UpdateStatus(key);
}

void SceneTerrain::UpdateWeaponStatus(unsigned char key)
{
	if (key == WA_FIRE && FireRateCounter > 0.1)
	{
		for (int i = 0; i < projectileList.size(); ++i)
		{
			CProjectile *p = projectileList[i];
			if (!p->GetStatus())
			{
				p->Init(GEO_SPHERE, camera.target, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1, 1, 1), 500, 2, true, (camera.target - camera.position).Normalized());
				FireRateCounter = 0;
				break;
			}
		}
	}
}

void SceneTerrain::RenderSkybox()
{
	/*//left
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
	modelStack.PopMatrix();*/
}

void SceneTerrain::RenderSkyPlane()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1800, 0);
	RenderMesh(meshList[GEO_SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneTerrain::RenderTextInWorld()
{
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderText(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0));
	modelStack.PopMatrix();
}

void SceneTerrain::RenderObject()
{
	RenderMesh(meshList[GEO_AXES], false);
	
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	modelStack.PushMatrix();
	modelStack.Translate(10, 10 + terrainSize.y * ReadHeightMap(m_heightMap, 10/terrainSize.x, -10/terrainSize.z), -10);
	modelStack.Scale(10, 10, 10);
	RenderMesh(sa, false);
	modelStack.PopMatrix() ;

	// Redner objList
	for (std::vector<CObj*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		obj = (CObj*)*it;
		modelStack.PushMatrix();
		modelStack.Translate(obj->GetTranslate().x, obj->GetTranslate().y, obj->GetTranslate().z);
		modelStack.Rotate(obj->GetRotate().x, 1, 0, 0);
		modelStack.Rotate(obj->GetRotate().y, 0, 1, 0);
		modelStack.Rotate(obj->GetRotate().z, 0, 0, 1);
		modelStack.Scale(obj->GetScale().x, obj->GetScale().y, obj->GetScale().z);
		RenderMesh(meshList[obj->GetID()], false);
		modelStack.PopMatrix();
	}

	// Render projectileList
	for (std::vector<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		CProjectile *p = (CProjectile*)*it;
		if (p->GetStatus())
		{
			modelStack.PushMatrix();
			modelStack.Translate(p->GetTranslate().x, p->GetTranslate().y, p->GetTranslate().z);
			modelStack.Rotate(p->GetRotate().x, 1, 0, 0);
			modelStack.Rotate(p->GetRotate().y, 0, 1, 0);
			modelStack.Rotate(p->GetRotate().z, 0, 0, 1);
			modelStack.Scale(p->GetScale().x, p->GetScale().y, p->GetScale().z);
			RenderMesh(meshList[p->GetID()], false);
			modelStack.PopMatrix();
		}
	}
}

void SceneTerrain::Render2D()
{
	SetHUD(true);
	//On screen text
	std::ostringstream ss, y;
	ss.precision(5);
	ss << "FPS: " << fps;
	y << "Y: " << camera.position.y;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 6);
	//RenderTextOnScreen(meshList[GEO_TEXT], y.str(), Color(0, 1, 0), 3, 0, 9);
	
	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Light(" << lights[0].position.x << ", " << lights[0].position.y << ", " << lights[0].position.z << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 2, 0, 3);

	std::ostringstream sPos, sTarget;
	sPos << "Pos: " << camera.position;
	RenderTextOnScreen(meshList[GEO_TEXT], sPos.str(), Color(0, 1, 0), 2, 0, 12);
	sTarget << "Target: " << camera.target;
	RenderTextOnScreen(meshList[GEO_TEXT], sTarget.str(), Color(0, 1, 0), 2, 0, 9);

	RenderTextOnScreen(meshList[GEO_TEXT], "Hello Screen", Color(0, 1, 0), 2, 0, 0);
	
	SetHUD(false);
	
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 1, 0, 0);
	
	// Render minimap
	RenderMeshIn2D(m_cMinimap->GetAvatar(), false, 20.f, 50, -45, rotateAngle);
	RenderMeshIn2D(m_cMinimap->GetBorder(), false, 20.f, 50, -45);
	RenderMeshIn2D(m_cMinimap->GetBackground(), false, 20.f, 50, -45);
}

void SceneTerrain::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(terrainSize.x, terrainSize.y, terrainSize.z);
	RenderMesh(meshList[GEO_TERRAIN], false);
	modelStack.PopMatrix();
}

void SceneTerrain::SetHUD(const bool m_bHUDmode) // Remember to call this in pairs
{
	if(m_bHUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
	}
	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}