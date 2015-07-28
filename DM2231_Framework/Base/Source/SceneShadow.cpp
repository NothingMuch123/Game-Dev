#include "SceneShadow.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

float SceneShadow::MAX_CHANGE_WEAPON_TIMER = 0.5f;
float SceneShadow::MAX_SCOPE_TIMER = 0.5f;
float SceneShadow::MAX_SPAWN_TARGET_TIMER = 1.f;
float SceneShadow::MAX_SPAWN_RAIN_TIMER = 0.0f;

SceneShadow::SceneShadow() : m_cMinimap(NULL), projectileList(NULL), targetList(NULL)
{
}

SceneShadow::~SceneShadow()
{
}

void SceneShadow::Init()
{
	// Initialise SceneBase
	SceneBase::Init();

	// Init sound
	sound = irrklang::createIrrKlangDevice();
	if (!sound)
	{
		std::cout << "Could not start sound engine" << std::endl;
	}

	// Load minimap texture
	m_cMinimap = new CMinimap();
	m_cMinimap->SetPosition(65, 45);
	m_cMinimap->SetSize(20,20);
	m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("Minimap", Color(1,1,1), 1.f));
	m_cMinimap->GetBackground()->textureID[0] = LoadTGA("Image//rock.tga");
	m_cMinimap->GetBackground()->textureID[1] = LoadTGA("Image//sand.tga");
	m_cMinimap->SetBorder(MeshBuilder::GenerateQuad("Minimap Border", Color(1,1,1), 1.f));
	m_cMinimap->GetBorder()->textureID[0] = LoadTGA("Image//minimap_border.tga");
	m_cMinimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("Minimap Avatar", Color(1,1,1), 1.f));

	// Init character
	currentChar = new CCharacter(CCharacter::CHAR_PLAYER, Vector3(0,0,0), Vector3(0,0,0), Vector3(15,Camera3::TERRAIN_OFFSET,15), Vector3(1,1,1), 100, CWeapon::W_SMG, "Player", camera, true);

	// Create obj
	obj = new CObj(GEO_PLATFORM, Vector3(0, terrainSize.y * ReadHeightMap(m_heightMap, 0/terrainSize.x, 0/terrainSize.z), 0), Vector3(0, 0, 0), Vector3(80,20,80), Vector3(1,1,1));
	objList.push_back(obj);

	// Create ammocrate
	CAmmoCrate *ammocrate = new CAmmoCrate(GEO_AMMO_CRATE, Vector3(-380,CAmmoCrate::AMMO_CRATE_OFSET + terrainSize.y * ReadHeightMap(m_heightMap, -380/terrainSize.x, 380/terrainSize.z), 380), Vector3(0,0,0), Vector3(10,10,10), Vector3(1,1,1), true);
	ammocrateList.push_back(ammocrate);

	ammocrate = new CAmmoCrate(GEO_AMMO_CRATE, Vector3(-500,CAmmoCrate::AMMO_CRATE_OFSET + terrainSize.y * ReadHeightMap(m_heightMap, -500/terrainSize.x, 500/terrainSize.z), 500), Vector3(0,0,0), Vector3(10,10,10), Vector3(1,1,1), true);
	ammocrateList.push_back(ammocrate);

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

	// Create particle list
	for (int i = 0; i < 1000; ++i)
	{
		CParticle *particle = new CParticle();
		particleList.push_back(particle);
	}

	// Create trees (billboard)
	float offset = 0;
	obj = new CObj(GEO_TREE, Vector3(450, offset + 25 + terrainSize.y * ReadHeightMap(m_heightMap, 450/terrainSize.x, 450/terrainSize.z), 450), Vector3(0,180,0), Vector3(50,50,50), Vector3(1,1,1), true);
	billboardList.push_back(obj);

	FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
	ChangeWeaponTimer = MAX_CHANGE_WEAPON_TIMER;
	ScopeTimer = MAX_SCOPE_TIMER;
	SpawnTargetTimer = MAX_SPAWN_TARGET_TIMER;
	SpawnRainTimer = MAX_SPAWN_RAIN_TIMER;
	ReloadTimer = 0;
	reloading = reloadTranslation = scope = false;
	score = 0;
	recoil = 0;
}

void SceneShadow::Update(double dt)
{
	// Update SceneBase
	SceneBase::Update(dt);
	camera.Update(dt, m_heightMap, terrainSize);

	/*m_cMinimap->InactiveEntityList();

	// Bring down recoil
	if (recoil > 0)
	{
		Application::camera_pitch += 0.5 * dt;
		recoil -= 0.5 * dt;
		if (recoil < 0)
		{
			recoil = 0;
		}
	}

	currentChar->GetCamera().Update(dt, m_heightMap, terrainSize);
	currentChar->calcBound();
	camera = currentChar->GetCamera();

	// Spawn particle
	if (SpawnRainTimer < MAX_SPAWN_RAIN_TIMER)
	{
		SpawnRainTimer += dt;
	}
	if (SpawnRainTimer >= MAX_SPAWN_RAIN_TIMER)
	{
		for (int i = 0; i < 5; ++i)
		{
			SpawnRain();
		}
		SpawnRainTimer = 0;
	}

	// Update particle
	for (std::vector<CParticle*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		CParticle *particle = (CParticle*)*it;
		particle->Update(dt, terrainSize.y * ReadHeightMap(m_heightMap, particle->GetTranslate().x/terrainSize.x, particle->GetTranslate().z/terrainSize.z));
	}

	// Update projectile
	for (int i = 0; i < projectileList.size(); ++i)
	{
		CProjectile *p = projectileList[i];
		p->Update(dt);
	}

	// Update ammo crate
	static const float radius = 30.f;
	for (std::vector<CAmmoCrate*>::iterator it = ammocrateList.begin(); it != ammocrateList.end(); ++it)
	{
		CAmmoCrate *ammocrate = (CAmmoCrate*)*it;
		ammocrate->Update(dt);

		// Check collision of ammocrate with player
		Vector3 pos = ammocrate->GetTranslate();
		pos.y = currentChar->GetCamera().position.y;
		if (ammocrate->GetActive() && (pos - currentChar->GetCamera().position).Length() <= ((m_cMinimap->GetSize_x() - 5) * radius))
		{
			m_cMinimap->SetEntity(GEO_MINIMAP_AMMOCRATE, Vector2(((ammocrate->GetTranslate().x - currentChar->GetCamera().position.x) / (radius * 2)), ((currentChar->GetCamera().position.z - ammocrate->GetTranslate().z) / (radius * 2))));
		}
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
		CWeapon *weapon = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()];
		float reload_speed = weapon->GetScale().y / weapon->GetReloadSpeed() * dt;
		ReloadTimer += dt;
		if (ReloadTimer > weapon->GetReloadSpeed())	// Reload
		{
			weapon->SetTranslate(Vector3(weapon->GetTranslate().x, weapon->GetDefaultTranslateY(), 0));
			reloadTranslation = false;
			weapon->Reload();
			ReloadTimer = 0;
			reloading = false;
		}
		else	// Reload animation
		{
			if (weapon->GetTranslate().y <= weapon->GetDefaultTranslateY() - (weapon->GetScale().y/2))
			{
				reloadTranslation = true;
			}
			if (!reloadTranslation && weapon->GetTranslate().y >= weapon->GetDefaultTranslateY() - (weapon->GetScale().y/2))
			{
				weapon->SetTranslate(Vector3(weapon->GetTranslate().x, weapon->GetTranslate().y - reload_speed, weapon->GetTranslate().z));
			}
			else
			{
				weapon->SetTranslate(Vector3(weapon->GetTranslate().x, weapon->GetTranslate().y + reload_speed, weapon->GetTranslate().z));
			}
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
			Vector3 pos = target->GetTranslate();
			pos.y = currentChar->GetCamera().position.y;
			if ((pos - currentChar->GetCamera().position).Length() <= ((m_cMinimap->GetSize_x() - 5) * radius))
			{
				m_cMinimap->SetEntity(GEO_MINIMAP_TARGET, Vector2(((target->GetTranslate().x - currentChar->GetCamera().position.x) / (radius * 2)), ((currentChar->GetCamera().position.z - target->GetTranslate().z) / (radius * 2))));
			}
		}
	}

	// Check player with ammo crate
	for (std::vector<CAmmoCrate*>::iterator it = ammocrateList.begin(); it != ammocrateList.end(); ++it)
	{
		CAmmoCrate *ammocrate = (CAmmoCrate*)*it;
		if (ammocrate->GetActive())
		{
			Vector3 charMinBound = currentChar->GetMinBound(), charMaxBound = currentChar->GetMaxBound();
			Vector3 ammocrateMinBound = ammocrate->GetMinBound(), ammocrateMaxBound = ammocrate->GetMaxBound();
			if (charMinBound.x < ammocrateMaxBound.x && charMaxBound.x > ammocrateMinBound.x &&		//
				charMinBound.y < ammocrateMaxBound.y && charMaxBound.y > ammocrateMinBound.y &&		// Player picks up ammocrate
				charMinBound.z < ammocrateMaxBound.z && charMaxBound.z > ammocrateMinBound.z)		//
			{
				CWeapon *weapon = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()];
				if (weapon->GetExtraAmmo() <= weapon->GetMaxAmmo() - weapon->GetClipSize())	// Extra ammo can take in 1 clip
				{
					weapon->SetExtraAmmo(weapon->GetExtraAmmo() + weapon->GetClipSize());
					ammocrate->SetActive(false);
				}
				else if (weapon->GetExtraAmmo() < weapon->GetMaxAmmo())					// Extra ammo unable to take in 1 clip
				{
					weapon->SetExtraAmmo(weapon->GetMaxAmmo());
					ammocrate->SetActive(false);
				}
			}
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
	}*/

	fps = (float)(1.f / dt);
}

void SceneShadow::Render()
{
	// Render SceneBase
	//SceneBase::Render();
	
	RenderPassGPass();
	RenderPassMain();
	//RenderSkybox();
	/*RenderSkyPlane();
	RenderTerrain();
	RenderObject();
	RenderTextInWorld();
	Render2D();*/
}

void SceneShadow::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();

	// Delete sound
	sound->drop();

	// Delete minimap
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

	for (std::vector<CTarget*>::iterator it = targetList.begin(); it != targetList.end(); ++it)
	{
		CTarget *target = (CTarget*)*it;
		if (target)
		{
			delete target;
		}
	}
	targetList.clear();

	for (std::vector<CParticle*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		CParticle *p = (CParticle*)*it;
		if (p)
		{
			delete p;
		}
	}
	particleList.clear();

	for (std::vector<CObj*>::iterator it = billboardList.begin(); it != billboardList.end(); ++it)
	{
		obj = (CObj*)*it;
		if (obj)
		{
			delete obj;
		}
	}
	billboardList.clear();

	if (currentChar)
	{
		delete currentChar;
		currentChar = NULL;
	}
}

void SceneShadow::Reset()
{
	SceneBase::Reset();
	currentChar->Reset();
	reloading = scope = false;
	FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
	ChangeWeaponTimer = MAX_CHANGE_WEAPON_TIMER;
	ScopeTimer = MAX_SCOPE_TIMER;
	SpawnTargetTimer = MAX_SPAWN_TARGET_TIMER;
	ReloadTimer = 0.f;
	score = recoil = 0;
}

void SceneShadow::UpdateCameraStatus(unsigned char key)
{
	//currentChar->GetCamera().UpdateStatus(key);
	camera.UpdateStatus(key);
	if (key == 't')
	{
		Reset();
	}
}

void SceneShadow::UpdateWeaponStatus(unsigned char key, double dt)
{
	if (key == WA_FIRE && FireRateCounter > currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate() && !reloading)
	{
		CProjectile *p = FetchProj();
		if(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->Fire(p, &(currentChar->GetCamera()), dt, recoil, sound))
		{
			FireRateCounter = 0;
		}
		else if (currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetExtraAmmo() > 0 && !reloading)
		{
			reloading = true;
			reloadTranslation = false;
			switch(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetID())
			{
			case CWeapon::W_PISTOL: sound->play2D("Sound//pistol_reload.wav");
				break;
			case CWeapon::W_ROCKET_LAUNCHER : sound->play2D("Sound//rocket_launcher_reload.wav");
				break;
			case CWeapon::W_SNIPER : sound->play2D("Sound//sniper_reload.wav");
				break;
			case CWeapon::W_SMG : sound->play2D("Sound//smg_reload.wav");
			}
			recoil = 0;
		}
	}
	else if (key == WA_RELOAD)
	{
		if (!reloading && currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetExtraAmmo() > 0 && currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetActiveAmmo() < currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetClipSize())
		{
			reloading = true;
			reloadTranslation = false;
			switch(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetID())
			{
			case CWeapon::W_PISTOL: sound->play2D("Sound//pistol_reload.wav");
				break;
			case CWeapon::W_ROCKET_LAUNCHER : sound->play2D("Sound//rocket_launcher_reload.wav");
				break;
			case CWeapon::W_SNIPER : sound->play2D("Sound//sniper_reload.wav");
				break;
			case CWeapon::W_SMG : sound->play2D("Sound//smg_reload.wav");
			}
			recoil = 0;
		}
	}
	else if (key == WA_CHANGEWEAPON_NEXT && ChangeWeaponTimer >= MAX_CHANGE_WEAPON_TIMER)
	{
		reloading = reloadTranslation = scope = false;
		fov = 45.f;
		CWeapon *weapon = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()];
		weapon->SetTranslate(Vector3(weapon->GetTranslate().x, weapon->GetDefaultTranslateY(), weapon->GetTranslate().z));
		currentChar->SetCurrentWeapon(currentChar->GetCurrentWeapon() + 1);
		if (currentChar->GetCurrentWeapon() == CWeapon::W_TOTAL)
		{
			currentChar->SetCurrentWeapon(0);
		}
		FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
		ReloadTimer = ChangeWeaponTimer = recoil = 0;
	}
	else if (key == WA_CHANGEWEAPON_PREV && ChangeWeaponTimer >= MAX_CHANGE_WEAPON_TIMER)
	{
		reloading = reloadTranslation = scope = false;
		fov = 45.f;
		CWeapon *weapon = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()];
		weapon->SetTranslate(Vector3(weapon->GetTranslate().x, weapon->GetDefaultTranslateY(), weapon->GetTranslate().z));
		currentChar->SetCurrentWeapon(currentChar->GetCurrentWeapon() - 1);
		if (currentChar->GetCurrentWeapon() == -1)
		{
			currentChar->SetCurrentWeapon(CWeapon::W_TOTAL - 1);
		}
		FireRateCounter = currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetFirerate();
		ReloadTimer = ChangeWeaponTimer = recoil = 0;
	}
	else if (key == WA_SCOPE && ScopeTimer >= MAX_SCOPE_TIMER && currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]->GetID() == CWeapon::W_SNIPER)
	{
		scope = !scope;
		if (scope)
		{
			fov = 15.f;
		}
		else
		{
			fov = 45.f;
		}
		ScopeTimer = 0;
	}
}

void SceneShadow::RenderSkyPlane()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1800, 0);
	RenderMesh(meshList[GEO_SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneShadow::RenderTextInWorld()
{
}

void SceneShadow::RenderObject()
{
	RenderMesh(meshList[GEO_AXES], false);
	
	/*modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();*/

	RenderObjList();
	RenderTargetList();
	RenderProjectileList();
	RenderAmmocrateList();
	RenderParticleList();
	RenderBillboardList();
}

void SceneShadow::RenderObjList()
{
	// Render obj list
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
}

void SceneShadow::RenderTargetList()
{
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
}

void SceneShadow::RenderProjectileList()
{
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

void SceneShadow::RenderAmmocrateList()
{
	// Render ammocrate list
	for (std::vector<CAmmoCrate*>::iterator it = ammocrateList.begin(); it != ammocrateList.end(); ++it)
	{
		CAmmoCrate *ammocrate = (CAmmoCrate*)*it;
		if (ammocrate->GetActive())
		{
			modelStack.PushMatrix();
			modelStack.Translate(ammocrate->GetTranslate().x, ammocrate->GetTranslate().y, ammocrate->GetTranslate().z);
			modelStack.Rotate(ammocrate->GetRotate().x, 1, 0, 0);
			modelStack.Rotate(ammocrate->GetRotate().y, 0, 1, 0);
			modelStack.Rotate(ammocrate->GetRotate().z, 0, 0, 1);
			modelStack.Scale(ammocrate->GetScale().x, ammocrate->GetScale().y, ammocrate->GetScale().z);
			RenderMesh(meshList[ammocrate->GetID()], false);
			modelStack.PopMatrix();
		}
	}
}

void SceneShadow::RenderParticleList()
{
	// Render particle list
	for (std::vector<CParticle*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		CParticle *particle = (CParticle*)*it;
		if (particle->GetActive() && particle->GetRender())
		{
			modelStack.PushMatrix();
			modelStack.Translate(particle->GetTranslate().x, particle->GetTranslate().y, particle->GetTranslate().z);
			modelStack.Rotate(particle->GetRotate().x, 1, 0, 0);
			modelStack.Rotate(particle->GetRotate().y, 0, 1, 0);
			modelStack.Rotate(particle->GetRotate().z, 0, 0, 1);
			modelStack.Scale(particle->GetScale().x, particle->GetScale().y, particle->GetScale().z);
			RenderMesh(meshList[particle->GetID()], false);
			modelStack.PopMatrix();
		}
	}
}

void SceneShadow::RenderBillboardList()
{
	bool fogToggled = false;
	if (fogEnabled)
	{
		fogToggled = true;
		glUniform1f(m_parameters[U_FOG_ENABLE], !fogEnabled);
	}
	for (std::vector<CObj*>::iterator it = billboardList.begin(); it != billboardList.end(); ++it)
	{
		obj = (CObj*)*it;

		Vector3 dir = (obj->GetTranslate() - currentChar->GetCamera().position).Normalized();

		modelStack.PushMatrix();
		modelStack.Translate(obj->GetTranslate().x, obj->GetTranslate().y, obj->GetTranslate().z);
		modelStack.Rotate(obj->GetRotate().x, 1, 0, 0);
		modelStack.Rotate(obj->GetRotate().y + Math::RadianToDegree(atan2(dir.x, dir.z)), 0, 1, 0);
		modelStack.Rotate(obj->GetRotate().z, 0, 0, 1);
		modelStack.Scale(obj->GetScale().x, obj->GetScale().y, obj->GetScale().z);
		RenderMesh(meshList[obj->GetID()], false);
		modelStack.PopMatrix();
	}
	if (fogToggled)
	{
		glUniform1f(m_parameters[U_FOG_ENABLE], !fogEnabled);
	}
}

void SceneShadow::Render2D()
{
	SetHUD(true);

	// Render minimap
	float minimap_x = m_cMinimap->GetPosition_x(), minimap_y = m_cMinimap->GetPosition_y();
	RenderMeshIn2D(m_cMinimap->GetBackground(), false, m_cMinimap->GetSize_x(), minimap_x, minimap_y, currentChar->GetCamera().total_yaw);
	RenderMeshIn2D(m_cMinimap->GetBorder(), false, m_cMinimap->GetSize_x(), minimap_x, minimap_y, currentChar->GetCamera().total_yaw);
	RenderMeshIn2D(m_cMinimap->GetAvatar(), false, 10.f, minimap_x, minimap_y);

	for (std::vector<CEntity*>::iterator it = m_cMinimap->GetEntityList().begin(); it != m_cMinimap->GetEntityList().end(); ++it)
	{
		CEntity *entity = (CEntity*)*it;
		if (entity->GetActive())
		{
			RenderEntity(meshList[entity->GetID()], false, Vector2(minimap_x, minimap_y), entity->GetPosition(), currentChar->GetCamera().total_yaw);
		}
	}

	if (!scope)
	{
		RenderWeapon(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]);
	}
	else
	{
		RenderMeshIn2D(meshList[GEO_SCOPE], false, 160, 0, 0);
	}

	if (!scope && !reloading)
	{
		RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 5, 0, 0);
	}

	RenderMeshIn2D(meshList[GEO_UI_BORDER], false, 80, -60, -55);

	RenderAmmo(currentChar->GetWeaponList()[currentChar->GetCurrentWeapon()]);
	RenderMeshIn2D(meshList[GEO_HEALTH], false, 10, -70, -35);

	std::ostringstream ssHealth;
	ssHealth << currentChar->GetHealth();
	RenderTextOnScreen(meshList[GEO_TEXT], ssHealth.str(), Color(1,1,1), 4, 10, 10.5);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 1, 58);

	std::ostringstream ssScore;
	ssScore << "Score: " << score;
	RenderTextOnScreen(meshList[GEO_TEXT], ssScore.str(), Color(1,1,1), 3, 2, 18);

	if (reloading)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Reloading...", Color(1, 0, 0), 3, 25, 10);
	}
	
	SetHUD(false);
}

void SceneShadow::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(terrainSize.x, terrainSize.y, terrainSize.z);
	RenderMesh(meshList[GEO_TERRAIN], false);
	modelStack.PopMatrix();
}

void SceneShadow::SetHUD(const bool m_bHUDmode) // Remember to call this in pairs
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

CProjectile *SceneShadow::FetchProj()
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

CParticle *SceneShadow::FetchParticle()
{
	for (std::vector<CParticle*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		CParticle *p = (CParticle*)*it;
		if (!p->GetActive())
		{
			return p;
		}
	}
	/*for (int i = 0; i < 10; ++i)
	{
		CParticle *p = new CParticle;
		particleList.push_back(p);
	}
	CParticle *p = particleList.back();
	return p;*/
	return NULL;
}

void SceneShadow::RenderProjectile(CProjectile *p)
{
	modelStack.PushMatrix();
	modelStack.Translate(p->GetTranslate().x, p->GetTranslate().y, p->GetTranslate().z);
	modelStack.Rotate(p->GetRotate().y, 0, 1, 0);
	modelStack.Rotate(p->GetRotate().x, 1, 0, 0);
	modelStack.Rotate(p->GetRotate().z, 0, 0, 1);
	modelStack.Scale(p->GetScale().x, p->GetScale().y, p->GetScale().z);

	switch(p->GetID())
	{
	case CProjectile::PROJ_BULLET:
		RenderMesh(meshList[GEO_SPHERE], false);
		break;
	case CProjectile::PROJ_ROCKET:
		RenderMesh(meshList[GEO_CUBE], false);
		break;
	}
	modelStack.PopMatrix();
}

void SceneShadow::RenderWeapon(CWeapon *weapon)
{
	switch(weapon->GetID())
	{
	case CWeapon::W_PISTOL:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_PISTOL], false, weapon->GetScale().x, weapon->GetTranslate().x, weapon->GetTranslate().y, weapon->GetRotate().z);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_ROCKET_LAUNCHER:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_ROCKET_LAUNCHER], false, weapon->GetScale().x, weapon->GetTranslate().x, weapon->GetTranslate().y, weapon->GetRotate().z);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_SNIPER:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_SNIPER], false, weapon->GetScale().x, weapon->GetTranslate().x, weapon->GetTranslate().y, weapon->GetRotate().z);
		modelStack.PopMatrix();
		break;
	case CWeapon::W_SMG:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_SMG], false, weapon->GetScale().x, weapon->GetTranslate().x, weapon->GetTranslate().y, weapon->GetRotate().z);
		modelStack.PopMatrix();
		break;
	}
}

void SceneShadow::RenderAmmo(CWeapon *weapon)
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
	case CWeapon::W_SMG:
		modelStack.PushMatrix();
		RenderMeshIn2D(meshList[GEO_SMG_BULLET], false, 10, -70, -50);
		modelStack.PopMatrix();
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ammo.str(), Color(1, 1, 1), 4, 8, 3);
}

void SceneShadow::SpawnTarget()
{
	for (int i = 0; i < targetList.size(); ++i)
	{
		CTarget *target = targetList[i];
		if (!target->GetActive())
		{
			int x = (rand() % (800 - (int)(8 * 2.6 * 2))) - (400 - (int)(8 * 2.6));
			int z = 100 - (i * 100);
			target->Init((rand() % (CTarget::NUM_TARGET - 1)) + 1, Vector3(x, 5 + terrainSize.y * ReadHeightMap(m_heightMap, x/terrainSize.x, z/terrainSize.z), z), Vector3(-90, 0, 0), Vector3(8,8,8), Vector3(2.6,5,1.5), true, (rand() % 7) + 3);
			break;
		}
	}
}

void SceneShadow::SpawnRain()
{
	static const float distOffset = 300.f;
	CParticle *p = FetchParticle();
	if (p != NULL)
	{
		p->Init(GEO_RAIN_PARTICLE, Vector3(Math::RandFloatMinMax(currentChar->GetCamera().position.x - distOffset, currentChar->GetCamera().position.x + distOffset), 300, Math::RandFloatMinMax(currentChar->GetCamera().position.z - distOffset, currentChar->GetCamera().position.z + distOffset)), Vector3(0,0,0), Vector3(1,1,1), Vector3(1,1,1), Vector3(0, Math::RandFloatMinMax(-100, -200), 0), true, true);
	}
}

void SceneShadow::RenderEntity(Mesh *mesh, bool enableLight, Vector2 minimapPos, Vector2 objectPos, float rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(minimapPos.x, minimapPos.y, 0);
				modelStack.Rotate(-rotate, 0, 0, 1);
				modelStack.Translate(objectPos.x, objectPos.y, 0);
				modelStack.Rotate(rotate, 0, 0, 1);

				Mtx44 MVP, modelView, modelView_inverse_transpose;

				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				
				for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
				{
					if(mesh->textureID[i] > 0)
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + (i * 2)], 1);
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
						glUniform1i(m_parameters[U_COLOR_TEXTURE + (i * 2)], i);
					}
					else
					{
						glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + (i * 2)], 0);
					}
				}
				mesh->Render();
				for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
				{
					if(mesh->textureID[i] > 0)
					{
						glBindTexture(GL_TEXTURE_2D, 0);
					}
				}

			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneShadow::RenderWorld()
{
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 1);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0,2);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix();
}

void SceneShadow::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	
	m_lightDepthFBO.BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_gPassShaderID);

	// These matrices should change when light position or direction changes
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		m_lightDepthProj.SetToOrtho(-10, 10, -10, 10, -10, 20);
	}
	else
	{
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0,0,0,0,1,0);

	RenderWorld();
}

void SceneShadow::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::m_window_width, Application::m_window_height);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	// Pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);
	//glActiveTexture(GL_TEXTURE0);

	SceneBase::Render();

	RenderWorld();

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -10);
	modelStack.Scale(20,20,20);
	RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false);
	modelStack.PopMatrix();
}