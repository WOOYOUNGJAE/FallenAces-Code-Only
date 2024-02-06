#include "VIBuffer_FlowableRect.h"

CVIBuffer_FlowableRect::CVIBuffer_FlowableRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer_Rect(pGraphic_Device), m_hBaseTexHandle{}, m_hBaseTexDesc{}, m_hAmount{}
{
}

CVIBuffer_FlowableRect::CVIBuffer_FlowableRect(const CVIBuffer_FlowableRect& rhs)
	: CVIBuffer_Rect(rhs), m_hBaseTexHandle{ rhs.m_hBaseTexHandle }, m_hBaseTexDesc{ rhs.m_hBaseTexDesc }
	, m_hAmount(rhs.m_hAmount)
{
}

HRESULT CVIBuffer_FlowableRect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_FlowableRect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Create_VertexShader();
	Create_PixelShader();

	Initialize_ConstantTable();

	return S_OK;
}

HRESULT CVIBuffer_FlowableRect::Render()
{
	m_pVertexConstantTable->SetMatrix(
		m_pGraphic_Device,
		m_hTransformWorldHandle,
		&m_WorldMatrix);

	m_pVertexConstantTable->SetMatrix(
		m_pGraphic_Device,
		m_hTransformViewHandle,
		&m_ViewMatrix);

	m_pVertexConstantTable->SetMatrix(
		m_pGraphic_Device,
		m_hTransformProjectionHandle,
		&m_ProjectionMatrix);

	m_pVertexConstantTable->SetFloat(
		m_pGraphic_Device,
		m_hAmount,
		m_fAmount
	);

	m_pGraphic_Device->SetTexture(m_hBaseTexDesc.RegisterIndex, m_pTexture);

	// Vertex
	m_pGraphic_Device->SetVertexShader(m_pVertexShader);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

	// Pixel
	m_pGraphic_Device->SetPixelShader(m_pPixelShader);

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	m_pGraphic_Device->SetVertexShader(nullptr);
	m_pGraphic_Device->SetPixelShader(nullptr);

	return S_OK;
}

HRESULT CVIBuffer_FlowableRect::Create_VertexShader()
{
	ID3DXBuffer* pVertexShader = nullptr;
	ID3DXBuffer* errorBuffer = nullptr;

	HRESULT hr = D3DXCompileShaderFromFile(
		L"../Shader/FlowVertexShader.hlsl",
		0,
		0,
		"main",
		"vs_3_0",
		D3DXSHADER_DEBUG,
		&pVertexShader,
		&errorBuffer,
		&m_pVertexConstantTable);

	// output any error messages
	if (errorBuffer)
	{
		::MessageBox(0, (wchar_t*)errorBuffer->GetBufferPointer(), 0, 0);
		errorBuffer->Release();
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return E_FAIL;
	}

	hr = m_pGraphic_Device->CreateVertexShader(
		(DWORD*)pVertexShader->GetBufferPointer(),
		&m_pVertexShader);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return E_FAIL;
	}

	pVertexShader->Release();

	return E_NOTIMPL;
}

HRESULT CVIBuffer_FlowableRect::Create_PixelShader()
{
	ID3DXBuffer* pPixelShader = nullptr;
	ID3DXBuffer* errorBuffer = nullptr;

	HRESULT hr = D3DXCompileShaderFromFile(
		L"../Shader/DefaultPixelShader.hlsl",
		0,
		0,
		"main",
		"ps_3_0",
		D3DXSHADER_DEBUG,
		&pPixelShader,
		&errorBuffer,
		&m_pPixelConstantTable);

	// output any error messages
	if (errorBuffer)
	{
		::MessageBox(0, (wchar_t*)errorBuffer->GetBufferPointer(), 0, 0);
		errorBuffer->Release();
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return E_FAIL;
	}

	hr = m_pGraphic_Device->CreatePixelShader(
		(DWORD*)pPixelShader->GetBufferPointer(),
		&m_pPixelShader);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreatePixelShader - FAILED", 0, 0);
		return E_FAIL;
	}

	pPixelShader->Release();

	return E_NOTIMPL;
}

void CVIBuffer_FlowableRect::Initialize_ConstantTable()
{
	m_hTransformWorldHandle = m_pVertexConstantTable->GetConstantByName(0, "worldMatrix");
	m_hTransformViewHandle = m_pVertexConstantTable->GetConstantByName(0, "viewMatrix");
	m_hTransformProjectionHandle = m_pVertexConstantTable->GetConstantByName(0, "projectionMatrix");

	m_hAmount = m_pVertexConstantTable->GetConstantByName(0, "g_Amount");

	m_hBaseTexHandle = m_pPixelConstantTable->GetConstantByName(0, "g_Sampler");

	UINT count;
	m_pPixelConstantTable->GetConstantDesc(m_hBaseTexHandle, &m_hBaseTexDesc, &count);

	m_pVertexConstantTable->SetDefaults(m_pGraphic_Device);
	m_pPixelConstantTable->SetDefaults(m_pGraphic_Device);
}

CVIBuffer_FlowableRect* CVIBuffer_FlowableRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_FlowableRect* pInstance = new CVIBuffer_FlowableRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_FlowableRect");
	}

	return pInstance;
}

CComponent* CVIBuffer_FlowableRect::Clone(void* pArg)
{
	CVIBuffer_FlowableRect* pInstance = new CVIBuffer_FlowableRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_FlowableRect");
	}

	return pInstance;
}

void CVIBuffer_FlowableRect::Free()
{
	__super::Free();

	Safe_Release(m_pVertexShader);
	Safe_Release(m_pPixelShader);

	Safe_Release(m_pVertexConstantTable);
	Safe_Release(m_pPixelConstantTable);
}
