#include "stdafx.h"
#include "PlayerBullet.h"

#include "Export_Utility.h"

CPlayerBullet::CPlayerBullet(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet& rhs)
    : CGameObject(rhs)
{
}

CPlayerBullet::~CPlayerBullet()
{
}

HRESULT CPlayerBullet::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fAccTimeDelta = 0;
    m_fCallLimit = 2;

    return S_OK;
}

_int CPlayerBullet::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    if (Check_Time(fTimeDelta))
    {
        // �ð� �� �Ǹ� ����
        m_bDead = true;
    }

    if (m_bDead == true)
    {
        return 1;
    }

    m_pTransformCom->Move_Pos(&m_vBulletDir, 15.f, fTimeDelta);

    Engine::Add_RenderGroup(RENDER_ALPHA, this);

    return 0;
}

void CPlayerBullet::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
}

void CPlayerBullet::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    m_pTextureCom->Set_Texture((_uint)0);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayerBullet::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTear"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTear", pComponent });



    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));

    //�÷��̾� ù ���� ��ġ �޾ƿͼ� �ű⼭���� ����
    _vec3   playerPos;
    dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"GameLogic", L"Player", L"Proto_Transform"))->Get_Info(INFO_POS, &playerPos);
    dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"GameLogic", L"Player", L"Proto_Transform"))->Get_Info(INFO_LOOK, &m_vBulletDir);

    m_pTransformCom->Set_Pos(playerPos);
    
    // �÷��̾ ���� �������� bullet�� y������ ȸ���ؼ� ������ �������� ���̰� ȸ��
    _vec3   BulletLook;
    m_pTransformCom->Get_Info(INFO_LOOK, &BulletLook);
    m_pTransformCom->Rotation(ROT_Y, acos(D3DXVec3Dot(&m_vBulletDir, &BulletLook)));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

//�ٵ� �̰� Ŭ���Ҷ� ���ֿ��� Ŭ���Ǹ� �ʹٴٳ��ͼ�
// �� ������ api �Ҷ�ó�� Ŭ�� ���� �� ������? �ѹ���

bool CPlayerBullet::Check_Time(const _float& fTimeDelta)
{
    m_fAccTimeDelta += fTimeDelta;

    if (m_fAccTimeDelta >= m_fCallLimit)
    {
        m_fAccTimeDelta = 0.f;
        return true;
    }

    return false;
}

CPlayerBullet* CPlayerBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayerBullet* pInstance = new CPlayerBullet(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("PlayerBullet Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CPlayerBullet::Free()
{
    __super::Free();
}