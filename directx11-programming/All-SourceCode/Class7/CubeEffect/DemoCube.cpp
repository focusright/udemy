// DemoCube.cpp : Implements the CDemoCube class.
//
// By Geelix School of Serious Games and Edutainment.
//

#include "DemoCube.h"

// Vertex struct
struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};


//////////////////////////////////////////////////////////////////////
// Constructors

CDemoCube::CDemoCube()
{
    m_pEffect = NULL;
    m_pInputLayout = NULL;
    m_pVertexBuffer = NULL;
    m_pIndexBuffer = NULL;
    m_pColorMap = NULL;
    m_pColorMapSampler = NULL;
}

CDemoCube::~CDemoCube()
{
}


//////////////////////////////////////////////////////////////////////
// Overrides

bool CDemoCube::LoadContent()
{
    // Compile effect file
    ID3DBlob* pBuffer = NULL;
    bool res = CompileShader(L"ShaderEffect.fx", NULL, "fx_5_0", &pBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load effect file", L"ERROR", MB_OK);
        return false;
    }

    // Create effect
    HRESULT hr;
    hr = ::D3DX11CreateEffectFromMemory(
        pBuffer->GetBufferPointer(),
        pBuffer->GetBufferSize(),
        0, m_pD3DDevice, &m_pEffect);
    if (FAILED(hr)) {
        if (pBuffer)
            pBuffer->Release();
        return false;
    }

    // Define input layout
    D3D11_INPUT_ELEMENT_DESC shaderInputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numLayoutElements = ARRAYSIZE(shaderInputLayout);

    // Get technique and pass
    ID3DX11EffectTechnique* warpTechnique = 
        m_pEffect->GetTechniqueByName("Warptrail");
    ID3DX11EffectPass* pass0 = 
        warpTechnique->GetPassByIndex(0);

    // Get pass and effect shader descriptions
    D3DX11_PASS_SHADER_DESC passShaderDesc;
    pass0->GetVertexShaderDesc(&passShaderDesc);
    D3DX11_EFFECT_SHADER_DESC effectShaderDesc;
    passShaderDesc.pShaderVariable->GetShaderDesc(passShaderDesc.ShaderIndex, &effectShaderDesc);

    // Create input layout
    hr = m_pD3DDevice->CreateInputLayout(
        shaderInputLayout, numLayoutElements,
        effectShaderDesc.pBytecode, effectShaderDesc.BytecodeLength,
        &m_pInputLayout);
    if (FAILED(hr)) {
        return false;
    }

    // Release buffer
    pBuffer->Release();
    pBuffer = NULL;

    // Define triangles
    Vertex vertices[] =
    {
        { XMFLOAT3( -1.0f,  1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f,  1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f,  1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3(  1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3(  1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f,  1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f,  1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
    };

    // Vertex description
    D3D11_BUFFER_DESC vertexDesc;
    ::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(Vertex) * 24;

    // Resource data
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(resourceData));
    resourceData.pSysMem = vertices;

    // Create vertex buffer
    hr = m_pD3DDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Index buffer data
    WORD indices[] =
    {
        3,   1,  0,  2,  1,  3,
        6,   4,  5,  7,  4,  6,
        11,  9,  8, 10,  9, 11,
        14, 12, 13, 15, 12, 14,
        19, 17, 16, 18, 17, 19,
        22, 20, 21, 23, 20, 22
    };

    // Create index buffer
    D3D11_BUFFER_DESC indexBufferDesc;
    ::ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sizeof( WORD ) * 36;
    indexBufferDesc.CPUAccessFlags = 0;
    resourceData.pSysMem = indices;
    hr = m_pD3DDevice->CreateBuffer(&indexBufferDesc, &resourceData, &m_pIndexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Load texture
    hr = ::D3DX11CreateShaderResourceViewFromFile(
        m_pD3DDevice, L"borg.dds", 0, 0, &m_pColorMap, 0);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Unable to load texture", L"ERROR", MB_OK);
        return false;
    }

    // Texture sampler
    D3D11_SAMPLER_DESC textureDesc;
    ::ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    textureDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    textureDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = m_pD3DDevice->CreateSamplerState(&textureDesc, &m_pColorMapSampler);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Unable to create texture sampler state", L"ERROR", MB_OK);
        return false;
    }

    // Initialize matrixes
    // Note: We do not transpose the matrixes
    m_viewMatrix = XMMatrixIdentity();
    m_projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, 640.0f / 480.0f, 0.01f, 100.0f);

    return true;
}

void CDemoCube::UnloadContent()
{
    // Cleanup
    if (m_pColorMap)
        m_pColorMap->Release();
    m_pColorMap = NULL;
    if (m_pColorMapSampler)
        m_pColorMapSampler->Release();
    m_pColorMapSampler = NULL;
    if (m_pEffect)
        m_pEffect->Release();
    m_pEffect = NULL;
    if (m_pInputLayout)
        m_pInputLayout->Release();
    m_pInputLayout = NULL;
    if (m_pVertexBuffer)
        m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
    if (m_pIndexBuffer)
        m_pIndexBuffer->Release();
    m_pIndexBuffer = NULL;
}

void CDemoCube::Update()
{
}

void CDemoCube::Render()
{
    // Check if D3D is ready
    if (m_pD3DContext == NULL)
        return;

    // Clear back buffer
    float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);
    m_pD3DContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


    // Stride and offset
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // Set vertex buffer
    m_pD3DContext->IASetInputLayout(m_pInputLayout);
    m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pD3DContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Position cube in the world
    XMMATRIX r = XMMatrixRotationRollPitchYaw(0.0f, 0.4f, 0.0f);
    XMMATRIX t = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
    XMMATRIX w = r * t;

    // Set effect color map
    ID3DX11EffectShaderResourceVariable* colorMap;
    colorMap = m_pEffect->GetVariableByName("colorMap")->AsShaderResource();
    colorMap->SetResource(m_pColorMap);

    // Set effect color map sampler
    ID3DX11EffectSamplerVariable* colorMapSampler;
    colorMapSampler = m_pEffect->GetVariableByName("colorSampler")->AsSampler();
    colorMapSampler->SetSampler(0, m_pColorMapSampler);

    // Set effect world matrix
    ID3DX11EffectMatrixVariable* worldMatrix;
    worldMatrix = m_pEffect->GetVariableByName("worldMatrix")->AsMatrix();
    worldMatrix->SetMatrix((float*)&w);

    // Set effect view matrix
    ID3DX11EffectMatrixVariable* viewMatrix;
    viewMatrix = m_pEffect->GetVariableByName( "viewMatrix" )->AsMatrix();
    viewMatrix->SetMatrix((float*)&m_viewMatrix);

    // Set effect projection matrix
    ID3DX11EffectMatrixVariable* projMatrix;
    projMatrix = m_pEffect->GetVariableByName( "projMatrix" )->AsMatrix();
    projMatrix->SetMatrix((float*)&m_projMatrix);

    // Get effect technique (warptrail)
    ID3DX11EffectTechnique* warpTechnique;
    warpTechnique = m_pEffect->GetTechniqueByName("Warptrail");

    // Set effect technique description
    D3DX11_TECHNIQUE_DESC techDesc;
    warpTechnique->GetDesc(&techDesc);

    // Draw object for each pass in technique
    for (unsigned int p = 0; p < techDesc.Passes; p++)
    {
        ID3DX11EffectPass* pass = warpTechnique->GetPassByIndex(p);

        if (pass != 0)
        {
            pass->Apply(0, m_pD3DContext);
            m_pD3DContext->DrawIndexed(36, 0, 0);
        }
    }


    // Present back buffer to display
    m_pSwapChain->Present(0, 0);
}
