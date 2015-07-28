#include "Enemy.h"


CEnemy::CEnemy(CStrategy *strategy) : strategy(strategy)
{
}


CEnemy::~CEnemy(void)
{
}

void CEnemy::Init(void)
{

}

void CEnemy::Update(double dt, CMap* m_cMap)
{

}

void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete=true)
{

}