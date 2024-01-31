#include "stdafx.h"
#include "..\Header\Floor.h"

#include "Export_Utility.h"

CFloor::CFloor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFloor::CFloor(const CFloor& rhs)
	: Engine::CGameObject(rhs)
{

}

CFloor::~CFloor()
{
}

HRESULT CFloor::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 40.f, 40.f, 40.f };


	return S_OK;
}

Engine::_int CFloor::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CFloor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_matrix		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43);

}

void CFloor::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev ->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CFloor::Add_Component()
{
	CComponent*		pComponent = nullptr;
		
	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SkyBoxTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_SkyBoxTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
		
	return S_OK;
}

CFloor * CFloor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFloor *	pInstance = new CFloor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("SkyBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFloor::Free()
{	
	__super::Free();
}


