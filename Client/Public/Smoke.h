#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_FlowableRect;
END

BEGIN(Client)

class CSmoke final : public CGameObject
{
protected:
	CSmoke(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSmoke(const CSmoke& rhs);
	virtual ~CSmoke() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_FlowableRect* m_pVIBufferCom = { nullptr };

private:
	_float m_fCloudPivot = { 0.f };

private:
	HRESULT Add_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CSmoke* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END