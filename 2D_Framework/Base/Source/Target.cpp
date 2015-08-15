#include "Target.h"


CTarget::CTarget(TARGET_TYPE type, Vector2 pos, Vector2 scale, float openTime, bool open, bool active) : type(type), pos(pos), scale(scale), openTime(openTime), open(open), active(active), timer(0.f), sa(NULL)
{
}

CTarget::~CTarget(void)
{
	if (sa)
	{
		delete sa;
		sa = NULL;
	}
}

void CTarget::Init(TARGET_TYPE type, Vector2 pos, Vector2 scale, float openTime, bool open, bool active)
{
	this->type = type;
	this->pos = pos;
	this->scale = scale;
	this->openTime = openTime;
	this->open = open;
	this->active = active;
	this->timer = 0.f;
	CalcBound();
	
	unsigned texture = LoadTGA("Image//GDev_Assignment02//tile//point.tga");
	if (!sa)
	{
		switch (type)
		{
		case TARGET_RED :
			{
				sa = MeshBuilder::GenerateSpriteAnimation("Target red", 7, 5);
				sa->textureID = texture;
				sa->m_anim = new Animation();
				sa->m_anim->Set(5, 9, 0, 1);
			}
			break;
		case TARGET_LIGHT_BLUE :
			{
				sa = MeshBuilder::GenerateSpriteAnimation("Target light blue", 7, 5);
				sa->textureID = texture;
				sa->m_anim = new Animation();
				sa->m_anim->Set(10, 14, 0, 1);
			}
			break;
		case TARGET_DARK_BLUE :
			{
				sa = MeshBuilder::GenerateSpriteAnimation("Target dark blue", 7, 5);
				sa->textureID = texture;
				sa->m_anim = new Animation();
				sa->m_anim->Set(15, 19, 0, 1);
			}
			break;
		case TARGET_GREEN :
			{
				sa = MeshBuilder::GenerateSpriteAnimation("Target green", 7, 5);
				sa->textureID = texture;
				sa->m_anim = new Animation();
				sa->m_anim->Set(20, 24, 0, 1);
			}
			break;
		case TARGET_YELLOW :
			{
				sa = MeshBuilder::GenerateSpriteAnimation("Target yellow", 7, 5);
				sa->textureID = texture;
				sa->m_anim = new Animation();
				sa->m_anim->Set(25, 29, 0, 1);
			}
			break;
		case TARGET_WHITE :
			{
				sa = MeshBuilder::GenerateSpriteAnimation("Target white", 7, 5);
				sa->textureID = texture;
				sa->m_anim = new Animation();
				sa->m_anim->Set(30, 34, 0, 1);
			}
			break;
		}
	}
}

void CTarget::Update(const double dt)
{
	if (type != TARGET_DESTROY)
	{
		if (openTime == -1) // Always open
		{
			open = true;
		}
		else
		{
			if (timer < openTime) // Add to timer
			{
				timer += dt;
			}
			else // Toggle open and close based on timer
			{
				open = !open;
				timer = 0.f;
			}
		}
	}
	else
	{
		open = true;
	}

	if (sa)
	{
		sa->Update(dt);
	}
}

void CTarget::SetType(TARGET_TYPE type)
{
	this->type = type;
	if (this->type == TARGET_DESTROY)
	{
		if(sa) // Remove current sa and replace with explosion
		{
			delete sa;
			sa = NULL;
		}
		sa = MeshBuilder::GenerateSpriteAnimation("Explosion", 4, 10);
		sa->textureID = LoadTGA("Image//GDev_Assignment02//explosion.tga");
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 35, 1, 3.f);
	}
}

void CTarget::SetActive(bool active)
{
	this->active = active;
}

CTarget::TARGET_TYPE CTarget::GetType()
{
	return type;
}

bool CTarget::GetActive()
{
	return active;
}

bool CTarget::GetOpen()
{
	return open;
}

float CTarget::GetOpenTime()
{
	return openTime;
}

Vector2 CTarget::GetPos()
{
	return pos;
}

Vector2 CTarget::GetScale()
{
	return scale;
}

Vector2 CTarget::GetMinBound()
{
	return minBound;
}

Vector2 CTarget::GetMaxBound()
{
	return maxBound;
}

SpriteAnimation* CTarget::GetSA()
{
	return sa;
}

void CTarget::CalcBound()
{
	minBound.Set(pos.x, pos.y);
	maxBound.Set(pos.x + 32 * scale.x, pos.y + 32 * scale.y);
}