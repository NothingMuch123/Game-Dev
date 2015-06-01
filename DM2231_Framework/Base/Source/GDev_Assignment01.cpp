#include "GDev_Assignment01.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

float GDev_Assignment01::MAX_CHANGE_WEAPON_TIMER = 0.5f;
float GDev_Assignment01::MAX_SCOPE_TIMER = 0.5f;
float GDev_Assignment01::MAX_SPAWN_TARGET_TIMER = 1.f;

GDev_Assignment01::GDev_Assignment01() : m_cMinimap(NULL), projectileList(NULL)
{
}

GDev_Assignment01::~GDev_Assignment01()
{
}

void GDev_Assignment01::Init()
{
	// Initialise SceneBase
	SceneBase::Init();

	// Load minimap texture
	m_cMinimap = new CMinimap();
	m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("Minimap", Color(1,1,1), 1.f));
	m_cMinimap->GetBackground()->textureID[0] = LoadTGA("Image//rock.tga");
	m_cMinimap->GetBackground()->textureID[1] = LoadTGA("Image//sand.tga");
	m_cMinimap->SetBorder(MeshBuilder::GenerateQuad("Minimap Border", Color(1,1,1), 1.f));
	m_cMinimap->GetBorder()->textureID[0] = LoadTGA("Image//minimap_border.tga");
	m_cMinimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("Minimap Avatar", Color(1,1,1), 1.f));

	// Create obj
	obj = new CObj(GEO_PLATFORM, Vector3(0, terrainSize.y * ReadHeightMap(m_heightMap, 0/4000, 0/4000), 0), Vector3(0, 0, 0), Vector3(80,20,80), Vector3(1,1,1));
	objList.push_back(obj);

	// Create projectile list
	for (int i = 0; i < 100; ++i)
	{
		CProjectile *p = new CProjectile;
		p->InitNULL();
		projectileList.push_back(p);
	}

	// Create target list
	for (int i = 0; i < 5; ++i)
	{
		CTarget *target = new CTarget;
		targetList.push_back(target);
	}

	// Init character
	currentChar = new CCharacter(CCharacter::CHAR_PLAYER, Vector3(0,0,0), Vector3(0,0,0), Vector3(1,1,1), Vector3(1,1,1), 100, CWeapon::W_PISTOL, "Player", camera, true);

	FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
	ChangeWeaponTimer = MAX_CHANGE_WEAPON_TIMER;
	ScopeTimer = MAX_SCOPE_TIMER;
	SpawnTargetTimer = MAX_SPAWN_TARGET_TIMER;
	ReloadCounter = 0;
	reloading = scope = false;
	rotateAngle_yaw = 0.f;
	score = 0;
}

void GDev_Assignment01::Update(double dt)
{
	// Update SceneBase
	SceneBase::Update(dt);

	rotateAngle_yaw -= Application::camera_yaw * Camera3::CAMERA_SPEED * dt;

	// Update projectile
	for (int i = 0; i < projectileList.size(); ++i)
	{
		CProjectile *p = projectileList[i];
		p->Update(dt);
	}

	FireRateCounter += dt;
	if (ScopeTimer < MAX_SCOPE_TIMER)
	{
		ScopeTimer += dt;
	}
	if (ChangeWeaponTimer < MAX_CHANGE_WEAPON_TIMER)
	{
		ChangeWeaponTimer += dt;
	}
	if (reloading)
	{
		ReloadCounter += dt;
		if (ReloadCounter > currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetReloadSpeed())
		{
			currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->Reload();
			ReloadCounter = 0;
			reloading = false;
		}
	}

	// Spawn target
	if (SpawnTargetTimer < MAX_SPAWN_TARGET_TIMER)
	{
		SpawnTargetTimer += dt;
	}
	if (SpawnTargetTimer >= MAX_SPAWN_TARGET_TIMER)
	{
		SpawnTarget();
		SpawnTargetTimer = 0;
	}

	// Update target
	for (std::vector<CTarget*>::iterator it = targetList.begin(); it != targetList.end(); ++it)
	{
		CTarget *target = (CTarget*)*it;
		if (target->GetActive())
		{
			target->Update(dt);
		}
	}

	// Check projectile with target
	for (std::vector<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		CProjectile *proj = (CProjectile*)*it;
		for (std::vector<CTarget*>::iterator it2 = targetList.begin(); it2 != targetList.end(); ++it2)
		{
			CTarget *target = (CTarget*)*it2;
			if (proj->GetStatus() && target->GetActive() && !target->GetHit())
			{
				Vector3 projMinBound = proj->GetMinBound(), projMaxBound = proj->GetMaxBound();
				Vector3 targetMinBound = target->GetMinBound(), targetMaxBound = target->GetMaxBound();
				if (projMinBound.x < targetMaxBound.x && projMaxBound.x > targetMinBound.x &&		//
					projMinBound.y < targetMaxBound.y && projMaxBound.y > targetMinBound.y &&		// Projectile hits target
					projMinBound.z < targetMaxBound.z && projMaxBound.z > targetMinBound.z)			//
				{
					target->SetHit(true);
					proj->SetStatus(false);
					++score;
				}
			}
		}
	}

	currentChar->GetCamera().Update(dt, m_heightMap, terrainSize);
	camera = currentChar->GetCamera();

	fps = (float)(1.f / dt);
}

void GDev_Assignment01::Render()
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

void GDev_Assignment01::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();

	if(m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}

	for (std::vector<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		CProjectile *p = (CProjectile*)*it;
		if (p)
		{
			delete p;
		}
	}
	projectileList.clear();

	if (currentChar)
	{
		delete currentChar;
		currentChar = NULL;
	}
}

void GDev_Assignment01::Reset()
{
	SceneBase::Reset();
	currentChar->Reset();
	reloading = scope = false;
	FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
	ChangeWeaponTimer = MAX_CHANGE_WEAPON_TIMER;
	ScopeTimer = MAX_SCOPE_TIMER;
	ReloadCounter = rotateAngle_yaw = 0.f;
}

void GDev_Assignment01::UpdateCameraStatus(unsigned char key)
{
	currentChar->GetCamera().UpdateStatus(key);
	if (key == 't')
	{
		Reset();
	}
}

void GDev_Assignment01::UpdateWeaponStatus(unsigned char key)
{
	if (key == WA_FIRE && FireRateCounter > currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate() && !reloading)
	{
		CProjectile *p = FetchProj();
		if(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->Fire(p, &(currentChar->GetCamera())))
		{
			FireRateCounter = 0;
		}
		else if (currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetExtraAmmo() > 0)
		{
			reloading = true;
		}
	}
	else if (key == WA_RELOAD)
	{
		if (currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetExtraAmmo() > 0 && currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetActiveAmmo() < currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetClipSize())
		{
			reloading = true;
		}
	}
	else if (key == WA_CHANGEWEAPON_NEXT && ChangeWeaponTimer >= MAX_CHANGE_WEAPON_TIMER)
	{
		reloading = scope = false;
		currentChar->SetCurrentWeapon(currentChar->GetCurrentWeapon() + 1);
		if (currentChar->GetCurrentWeapon() == CWeapon::W_TOTAL)
		{
			currentChar->SetCurrentWeapon(0);
		}
		FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
		ReloadCounter = ChangeWeaponTimer = 0;
	}
	else if (key == WA_CHANGEWEAPON_PREV && ChangeWeaponTimer >= MAX_CHANGE_WEAPON_TIMER)
	{
		reloading = scope = false;
		currentChar->SetCurrentWeapon(currentChar->GetCurrentWeapon() - 1);
		if (currentChar->GetCurrentWeapon() == -1)
		{
			currentChar->SetCurrentWeapon(CWeapon::W_TOTAL - 1);
		}
		FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
		ReloadCounter = ChangeWeaponTimer = 0;
	}
	else if (key == WA_SCOPE && ScopeTimer >= MAX_SCOPE_TIMER && currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetID() == CWeapon::W_SNIPER)
	{
		scope = !scope;
		/*static const float ScopeDist = 200.f;
		if (scope)
		{
			Vector3 dir = (currentChar->GetCamera().target - currentChar->GetCamera().position).Normalized();
			currentChar->GetCamera().target += (dir * ScopeDist);
			currentChar->GetCamera().position += (dir * ScopeDist);
		}
		else
		{
			Vector3 dir = (currentChar->GetCamera().target - currentChar->GetCamera().position).Normalized();
			currentChar->GetCamera().target -= (dir * ScopeDist);
			currentChar->GetCamera().position -= (dir * ScopeDist);
		}*/
		ScopeTimer = 0;
	}
}

void GDev_Assignment01::RenderSkybox()
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

void GDev_Assignment01::RenderSkyPlane()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1800, 0);
	RenderMesh(meshList[GEO_SKYPLANE], false);
	modelStack.PopMatrix();
}

void GDev_Assignment01::RenderTextInWorld()
{
}

void GDev_Assignment01::RenderObject()
{
	RenderMesh(meshList[GEO_AXES], false);
	
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	// Render objList
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

	// Render target list
	for (std::vector<CTarget*>::iterator it = targetList.begin(); it != targetList.end(); ++it)
	{
		CTarget *target = (CTarget*)*it;
		if (target->GetActive())
		{
			modelStack.PushMatrix();
			modelStack.Translate(target->GetTranslate().x, target->GetTranslate().y, target->GetTranslate().z);
			modelStack.Rotate(target->GetRotate().x, 1, 0, 0);
			modelStack.Rotate(target->GetRotate().y, 0, 1, 0);
			modelStack.Rotate(target->GetRotate().z, 0, 0, 1);
			modelStack.Scale(target->GetScale().x, target->GetScale().y, target->GetScale().z);
			RenderMesh(meshList[GEO_TARGET], false);
			modelStack.PopMatrix();
		}
	}

	// Render projectileList
	for (std::vector<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		CProjectile *p = (CProjectile*)*it;
		if (p->GetStatus())
		{
			RenderProjectile(p);
		}
	}
}

void GDev_Assignment01::Render2D()
{
	SetHUD(true);

	// Render minimap
	static const float minimap_x = 65.f, minimap_y = 45.f;
	RenderMeshIn2D(m_cMinimap->GetBackground(), false, 20.f, minimap_x, minimap_y);
	RenderMeshIn2D(m_cMinimap->GetBorder(), false, 20.f, minimap_x, minimap_y);
	RenderMeshIn2D(m_cMinimap->GetAvatar(), false, 20.f, minimap_x, minimap_y, rotateAngle_yaw);

	if (!scope)
	{
		RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 5, 0, 0);
		RenderWeapon(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]);
	}
	else
	{
		RenderMeshIn2D(meshList[GEO_SCOPE], false, 160, 0, 0);
	}
	RenderAmmo(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 1, 58);

	if (reloading)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Reloading...", Color(1, 0, 0), 3, 25, 10);
	}
	
	SetHUD(false);
}

void GDev_Assignment01::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(terrainSize.x, terrainSize.y, terrainSize.z);
	RenderMesh(meshList[GEO_TERRAIN], false);
	modelStack.PopMatrix();
}

void GDev_Assignment01::SetHUD(const bool m_bHUDmode) // Remember to call this in pairs
{
	if(m_bHUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		glUniform1f(m_parameters[U_FOG_ENABLE], 0);
	}
	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
		glUniform1f(m_parameters[U_FOG_ENABLE], fogEnabled);
	}
}

CProjectile *GDev_Assignment01::FetchProj()
{
	for (std::vector<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		CProjectile *p = (CProjectile*)*it;
		if (!p->GetStatus())
		{
			p->SetStatus(true);
			return p;
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		CProjectile *p = new CProjectile;
		p->InitNULL();
		p->SetStatus(true);
		projectileList.push_back(p);
	}
	CProjectile *p = projectileList.back();
	return p;
}

void GDev_Assignment01::RenderProjectile(CProjectile *p)
{
	modelStack.PushMatrix();
	modelStack.Translate(p->GetTranslate().x, p->GetTranslate().y, p->GetTranslate().z);
	modelStack.Rotate(p->GetRotate().x, 1, 0, 0);
	modelStack.Rotate(p->GetRotate().y, 0, 1, 0);
	modelStack.Rotate(p->GetRotate().z, 0, 0, 1);
	modelStack.Scale(p->GetScale().x, p->GetScale().y, p->GetScale().z);

	switch(p->GetID())
	{
	case CProjectile::PROJ_BULLET:
		modelStack.PushMatrix();
		RenderMesh(meshList[GEO_SPHERE], false);
		modelStack.PopMatrix();
		break;
	case CProjectile::PROJ_ROCKET:
		modelStack.PushMatrix();
		modelStack.Scale(4, 4, 4);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}

	modelStack.PopMatrix();
}

void GDev_Assignment01::RenderWeapon(CWeapon *weapon)
{
	switch(weapon->GetID())
	{
	case CWeapon::W_PISTOL:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_PISTOL], false, 80, 40, -40);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_ROCKET_LAUNCHER:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_ROCKET_LAUNCHER], false, 80, 40, -20);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_SNIPER:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_SNIPER], false, 80, 55, -30);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_MELEE:
		modelStack.PushMatrix();
		modelStack.PopMatrix();
		break;
	}
}

void GDev_Assignment01::RenderAmmo(CWeapon *weapon)
{
	std::ostringstream ammo;
	ammo << weapon->GetActiveAmmo() << "/" << weapon->GetExtraAmmo();
	switch(weapon->GetID())
	{
	case CWeapon::W_PISTOL:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_PISTOL_BULLET], false, 10, -70, -50);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_ROCKET_LAUNCHER:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_ROCKET_BULLET], false, 10, -70, -50);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_SNIPER:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_SNIPER_BULLET], false, 10, -70, -50);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_MELEE:
		modelStack.PushMatrix();
		modelStack.PopMatrix();
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ammo.str(), Color(0, 1, 0), 4, 8, 3);
}

void GDev_Assignment01::SpawnTarget()
{
	for (int i = 0; i < targetList.size(); ++i)
	{
		CTarget *target = targetList[i];
		if (!target->GetActive())
		{
			int x = (rand() % 800) - 400;
			int z = 100 - (i * 100);
			target->Init((rand() % (CTarget::NUM_TARGET - 1)) + 1, Vector3(x, 5 + terrainSize.y * ReadHeightMap(m_heightMap, x/terrainSize.x, z/terrainSize.z), z), Vector3(-90, 0, 0), Vector3(8,8,8), Vector3(2.6,5,2.6), true, (rand() % 7) + 3);
			break;
		}
	}
}