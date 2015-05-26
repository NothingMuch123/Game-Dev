#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <vector>
#include "Mesh.h"

using std::vector;

struct Animation
{
	Animation()
	{
		startFrame = endFrame = repeatCount = animTime = 0;
		ended = false;
	}

	void Set(int startFrame, int endFrame, int repeat, float time)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
	}

	int startFrame, endFrame, repeatCount;
	float animTime;
	bool ended;
};

class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation(void);

	void Update(double dt);
	virtual void Render();

	int m_row, m_col;

	float m_currentTime;
	int m_currentFrame, m_playCount;
	Animation *m_anim;
};

#endif