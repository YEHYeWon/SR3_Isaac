#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static bool Check_Intersect(CTransform* pSrcTrans, CTransform* pDstTrans);

};

END