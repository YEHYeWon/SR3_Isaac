#include "stdafx.h"
#include "..\Header\Wall.h"

#include "Export_Utility.h"

#include "CubeObject.h"

CWall::CWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), 
	m_bStartScene(false),
	m_iCubeActionType(0), m_iCubeCount(0),
	m_fCubeCreateTimer(0.f)
{
}

CWall::CWall(const CWall& rhs)
	: Engine::CGameObject(rhs)
{

}

CWall::~CWall()
{
}

HRESULT CWall::Ready_GameObject()
{
	m_bIsDeleted = false;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vecCubes.reserve(VTXCNTZ * MAX_Y);

	return S_OK;
}

Engine::_int CWall::Update_GameObject(const _float& fTimeDelta)
{
	//if (!m_bIsDeleted && Get_Arrived())
	//{
	//	m_bIsDeleted = true;
	//	Free_Cubes();
	//}

	if (!m_bCreated)
	{
		m_fCubeCreateTimer += fTimeDelta;

		if (m_iCubeCount < MAX_Y)
		{
			if (m_fCubeCreateTimer > 0.15f)
			{
				m_fCubeCreateTimer = 0.f;
				Create_Cubes();
			}
		}
		else
		{
			m_bCreated = true;
		}
	}

	for (auto& iter : m_vecCubes)
		iter->Update_GameObject(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);


	return 0;
}

void CWall::LateUpdate_GameObject()
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	for (auto& iter : m_vecCubes)
		iter->LateUpdate_GameObject();

	__super::LateUpdate_GameObject();
}

void CWall::Render_GameObject()
{	
	if (!m_bIsDeleted) return;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pTextureCom->Set_Texture(0);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

// 큐브 텍스쳐 태그를 지정해주면, 그 태그에 맞는 큐브 오브젝트를 타일 개수만큼 생성시키고 포지션을 지정해준다.
HRESULT CWall::Set_Cube_Texture_Tag(const _tchar* pCubeTextureTag, int iAxis)
{
	m_wstrTexture = pCubeTextureTag;
	m_iAxis = iAxis;

	return S_OK;
}

void CWall::Set_Texture_Tag(const _tchar* pTextureTag, int iAxis)
{
	CComponent* pComponent = nullptr;

	switch (iAxis)
	{
	case WALL_LEFT:
		m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, VTXCNTZ * 0.13f, VTXCNTX * 0.5f };
		m_pTransformCom->m_vAngle.y = D3DXToRadian(-90.f);
		break;
	case WALL_RIGHT:
		m_pTransformCom->m_vInfo[INFO_POS] = { VTXCNTX, VTXCNTZ * 0.13f, VTXCNTX * 0.5f };
		m_pTransformCom->m_vAngle.y = D3DXToRadian(90.f);
		break;
	case WALL_TOP:
		m_pTransformCom->m_vInfo[INFO_POS] = { VTXCNTX * 0.5f, VTXCNTZ * 0.13f, VTXCNTX};
		//m_pTransformCom->m_vAngle.y = D3DXToRadian(90.f);
		break;
	case WALL_BOTTOM:
		m_pTransformCom->m_vInfo[INFO_POS] = { VTXCNTX * 0.5f, VTXCNTZ * 0.13f, 0.f };
		m_pTransformCom->m_vAngle.y = D3DXToRadian(180.f);
		break;
	}


}

HRESULT CWall::Create_Cubes()
{
	CCubeObject* pCube = nullptr;

	//액션 제거
	if (m_bStartScene)
	{
		CCubeObject* pCube = nullptr;

		for (int i = 0; i < MAX_Y; ++i)
		{
			for (int j = 0; j < VTXCNTX; ++j)
			{
				int iIdx = i * VTXCNTX + j;

				pCube = CCubeObject::Create(m_pGraphicDev, m_bStartScene);
		
				NULL_CHECK_RETURN(pCube, E_FAIL);
				pCube->Set_Cute_Texture(m_wstrTexture.c_str());
				CTransform* pTemp = dynamic_cast<CTransform*>(pCube->Get_Component(ID_DYNAMIC, L"Proto_Transform"));

				_vec3 vScale(pTemp->m_vScale.x * 2, pTemp->m_vScale.y * 2, pTemp->m_vScale.z * 2);

				//어디 세울 벽인지에 따른 큐브의 목적지 포지션 세팅
				switch (m_iAxis)
				{
				case WALL_LEFT:
					pCube->Set_Dst_Pos({ 0, vScale .y * i, (_float)(j * vScale.z) });
					break;
				case WALL_RIGHT:
					pCube->Set_Dst_Pos({ vScale.x * VTXCNTX, vScale.y * i, (_float)(j * vScale.z) });
					break;
				case WALL_TOP:
					pCube->Set_Dst_Pos({ (_float)(j * vScale.x) , vScale.y * i, vScale.z * VTXCNTX });
					break;
				case WALL_BOTTOM:
					pCube->Set_Dst_Pos({ (_float)(j * vScale.x), vScale.y * i, 0 });
					break;
				}

				pCube->Set_Cube_Action_Type(m_iCubeActionType);
				m_vecCubes.push_back(pCube);
			}

		}

		m_bCreated = true;
	}
	else
	{
		for (int i = 0; i < VTXCNTX; i++)
		{
			_float x = i;
			_float z = m_iCubeCount;

			pCube = CCubeObject::Create(m_pGraphicDev, m_bStartScene);
			NULL_CHECK_RETURN(pCube, E_FAIL);
			pCube->Set_Cute_Texture(m_wstrTexture.c_str());
			CTransform* pTemp = dynamic_cast<CTransform*>(pCube->Get_Component(ID_DYNAMIC, L"Proto_Transform"));

			_vec3 vScale(pTemp->m_vScale.x * 2, pTemp->m_vScale.y * 2, pTemp->m_vScale.z * 2);

			//어디 세울 벽인지에 따른 큐브의 목적지 포지션 세팅
			switch (m_iAxis)
			{
			case WALL_LEFT:
				pCube->Set_Dst_Pos({ 0, vScale.y * z, (_float)(x * vScale.z) });
				break;
			case WALL_RIGHT:
				pCube->Set_Dst_Pos({ vScale.x * VTXCNTX, vScale.y * z, (_float)(x * vScale.z) });
				break;
			case WALL_TOP:
				pCube->Set_Dst_Pos({ (_float)(x * vScale.x) , vScale.y * z, vScale.z * VTXCNTX });
				break;
			case WALL_BOTTOM:
				pCube->Set_Dst_Pos({ (_float)(x * vScale.x), vScale.y * z, 0 });
				break;
			}

			pCube->Set_Cube_Action_Type(m_iCubeActionType);
			m_vecCubes.push_back(pCube);
		}
	}

	

	m_iCubeCount++;
}

bool CWall::Get_Arrived()
{
	// 사이즈가 1미만이라면 한개도 생성되지 않은 시점이기 때문에 검사를 하지않고 거짓을 반환
	if (1 > m_vecCubes.size()) return false;

	//하나라도 도착하지 않았다면 false를 return 한다
	for (auto& iter : m_vecCubes)
	{
		if (!iter->Get_Arrived())
			return false;
	}

	return true;
}

HRESULT CWall::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransformCom->m_vScale = { 21.f, 6.f, 10.f };

	return S_OK;
}

void CWall::Free_Cubes()
{
	for_each(m_vecCubes.begin(), m_vecCubes.end(), CDeleteObj());
	m_vecCubes.clear();
}

CWall * CWall::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
	_int iCubeActionType, 
	bool bStartScene)
{
	CWall *	pInstance = new CWall(pGraphicDev);
	pInstance->m_bStartScene = bStartScene;
	pInstance->m_iCubeActionType = iCubeActionType;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Wall Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWall::Free()
{	
	Free_Cubes();
	__super::Free();
}


