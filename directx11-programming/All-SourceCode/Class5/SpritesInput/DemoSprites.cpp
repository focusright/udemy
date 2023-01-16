// DemoSprites.cpp : Implements the CDemoSprites class.
//
// By Geelix School of Serious Games and Edutainment.
//

#include "DemoSprites.h"

// Vertex struct
struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};


//////////////////////////////////////////////////////////////////////
// Constructors

CDemoSprites::CDemoSprites()
{
    m_pVS = NULL;
    m_pPS = NULL;
    m_pInputLayout = NULL;
    m_pVertexBuffer = NULL;
    m_pColorMap = NULL;
    m_pColorMapSampler = NULL;
    m_pAlphaBlendState = NULL;
    m_MvpCB = NULL;

    m_pDirectInput = NULL;
    m_pKeyboardDevice = NULL;
    m_pMouseDevice = NULL;
}

CDemoSprites::~CDemoSprites()
{
}


//////////////////////////////////////////////////////////////////////
// Overrides

bool CDemoSprites::LoadContent()
{
    // Compile vertex shader
    ID3DBlob* pVSBuffer = NULL;
    bool res = CompileShader(L"ShaderTexture.fx", "VS_Main", "vs_4_0", &pVSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load vertex shader", L"ERROR", MB_OK);
        return false;
    }

    // Create vertex shader
    HRESULT hr;
    hr = m_pD3DDevice->CreateVertexShader(
        pVSBuffer->GetBufferPointer(),
        pVSBuffer->GetBufferSize(),
        0, &m_pVS);
    if (FAILED(hr)) {
        if (pVSBuffer)
            pVSBuffer->Release();
        return false;
    }

    // Define input layout
    D3D11_INPUT_ELEMENT_DESC shaderInputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numLayoutElements = ARRAYSIZE(shaderInputLayout);

    // Create input layout
    hr = m_pD3DDevice->CreateInputLayout(
        shaderInputLayout, numLayoutElements,
        pVSBuffer->GetBufferPointer(), 
        pVSBuffer->GetBufferSize(), 
        &m_pInputLayout);
    if (FAILED(hr)) {
        return false;
    }

    // Release VS buffer
    pVSBuffer->Release();
    pVSBuffer = NULL;

    // Compile pixel shader
    ID3DBlob* pPSBuffer = NULL;
    res = CompileShader(L"ShaderTexture.fx", "PS_Main", "ps_4_0", &pPSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load pixel shader", L"ERROR", MB_OK);
        return false;
    }

    // Create pixel shader
    hr = m_pD3DDevice->CreatePixelShader(
        pPSBuffer->GetBufferPointer(),
        pPSBuffer->GetBufferSize(), 
        0, &m_pPS);
    if (FAILED(hr)) {
        return false;
    }

    // Cleanup PS buffer
    pPSBuffer->Release();
    pPSBuffer = NULL;

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

    // Get texture size
    ID3D11Resource* pColorMapRes;
    m_pColorMap->GetResource(&pColorMapRes);
    D3D11_TEXTURE2D_DESC colorMapDesc;
    ((ID3D11Texture2D*)pColorMapRes)->GetDesc(&colorMapDesc);
    pColorMapRes->Release();
    float halfWidth = (float)colorMapDesc.Width / 2.0f;
    float halfHeight = (float)colorMapDesc.Height / 2.0f;

    // Define triangle
    Vertex vertices[] =
    {
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -halfWidth, -halfHeight, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  halfWidth,  halfHeight, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
    };

    // Vertex description
    D3D11_BUFFER_DESC vertexDesc;
    ::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(Vertex) * 6;

    // Resource data
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(resourceData));
    resourceData.pSysMem = vertices;

    // Create vertex buffer
    hr = m_pD3DDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Create world matrix buffer
    D3D11_BUFFER_DESC constDesc;
    ::ZeroMemory(&constDesc, sizeof(constDesc));
    constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constDesc.ByteWidth = sizeof(XMMATRIX);
    constDesc.Usage = D3D11_USAGE_DEFAULT;
    hr = m_pD3DDevice->CreateBuffer(&constDesc, 0, &m_MvpCB);
    if (FAILED(hr)) {
        return false;
    }

    // Position and scale sprites
    m_Sprites[0].SetPosition(XMFLOAT2(400.0f, 600.0f));
    m_Sprites[1].SetPosition(XMFLOAT2(1200.0f, 600.0f));
    m_Sprites[0].SetScale(XMFLOAT2(0.5, 0.5));
    m_Sprites[1].SetScale(XMFLOAT2(0.5, 0.5));

    // Make world matrix
    XMMATRIX view = XMMatrixIdentity();
    XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
        0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    m_vpMatrix = XMMatrixMultiply(view, projection);

    // Set blend description
    D3D11_BLEND_DESC blendDesc;
    ::ZeroMemory(&blendDesc, sizeof(blendDesc));
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Set blend state
    m_pD3DDevice->CreateBlendState(&blendDesc, &m_pAlphaBlendState);
    m_pD3DContext->OMSetBlendState(m_pAlphaBlendState, blendFactor, 0xFFFFFFFF);


    // Create DirectInput8 device
    hr = ::DirectInput8Create(
        m_hInst, DIRECTINPUT_VERSION, 
        IID_IDirectInput8, (void**)&m_pDirectInput, 0);
    if (FAILED(hr))
        return false;

    // Create keyboard device
    hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, 0);
    if (FAILED(hr))
        return false;
    hr = m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr))
        return false;
    hr = m_pKeyboardDevice->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
        return false;
    hr = m_pKeyboardDevice->Acquire();
    if (FAILED(hr))
        return false;

    // Create mouse device
    hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, 0);
    if (FAILED(hr))
        return false;
    hr = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hr))
        return false;
    hr = m_pMouseDevice->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
        return false;
    hr = m_pMouseDevice->Acquire();
    if (FAILED(hr))
        return false;

    return true;
}

void CDemoSprites::UnloadContent()
{
    // Cleanup
    if (m_pColorMap)
        m_pColorMap->Release();
    m_pColorMap = NULL;
    if (m_pColorMapSampler)
        m_pColorMapSampler->Release();
    m_pColorMapSampler = NULL;
    if (m_pVS)
        m_pVS->Release();
    m_pVS = NULL;
    if (m_pPS)
        m_pPS->Release();
    m_pPS = NULL;
    if (m_pInputLayout)
        m_pInputLayout->Release();
    m_pInputLayout = NULL;
    if (m_pVertexBuffer)
        m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
    if (m_MvpCB)
        m_MvpCB->Release();
    m_MvpCB = NULL;
    if (m_pAlphaBlendState)
        m_pAlphaBlendState->Release();
    m_pAlphaBlendState = NULL;
    if (m_pKeyboardDevice) {
        m_pKeyboardDevice->Unacquire();
        m_pKeyboardDevice->Release();
    }
    m_pKeyboardDevice = NULL;
    if (m_pMouseDevice) {
        m_pMouseDevice->Unacquire();
        m_pMouseDevice->Release();
    }
    m_pMouseDevice = NULL;
    if (m_pDirectInput)
        m_pDirectInput->Release();
    m_pDirectInput = NULL;
}


void CDemoSprites::Update()
{
    HandleKeyboard();
    HandleMouse();
    HandleXInput();
}

void CDemoSprites::HandleKeyboard()
{
    // Get keyboard state data
    char keyboardData[256];
    m_pKeyboardDevice->GetDeviceState(sizeof(keyboardData), (void*)&keyboardData);

    // Key: Down arrow
    if (keyboardData[DIK_DOWN] & 0x80) { 
        m_Sprites[0].m_position.y -= 0.1f;
    }

    // Key: Up arrow
    if (keyboardData[DIK_UP] & 0x80) { 
        m_Sprites[0].m_position.y += 0.1f;
    }

    // Key: Left arrow
    if (keyboardData[DIK_LEFT] & 0x80) { 
        m_Sprites[0].m_position.x -= 0.1f;
    }

    // Key: Right arrow
    if (keyboardData[DIK_RIGHT] & 0x80) { 
        m_Sprites[0].m_position.x += 0.1f;
    }
    
    // Key: Escape
    if (keyboardData[DIK_ESCAPE] & 0x80) { 
        ::PostQuitMessage(0);
    }
}

void CDemoSprites::HandleMouse()
{
    // Get mouse state data
    DIMOUSESTATE mouseData;
    m_pMouseDevice->GetDeviceState(sizeof(mouseData), (void*)&mouseData);
    
    // Button: Left mouse button
    if (mouseData.rgbButtons[0] & 0x80) {
        m_Sprites[1].m_rotation += 0.00001f;
    }
    
    // Button: Right mouse button
    if (mouseData.rgbButtons[1] & 0x80) {
        m_Sprites[1].m_rotation -= 0.00001f;
    }

    // Mouse move
    m_Sprites[1].m_position.x += (mouseData.lX / 10.0f);
    m_Sprites[1].m_position.y -= (mouseData.lY / 10.0f);
}

void CDemoSprites::HandleXInput()
{
    // Get XInput state data
    XINPUT_STATE xinputData;
    HRESULT hr = ::XInputGetState(0, &xinputData);

    // Button: Back
    if (xinputData.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) { 
        ::PostQuitMessage(0);
    }

    // Button: B
    if (xinputData.Gamepad.wButtons & XINPUT_GAMEPAD_B) { 
        m_Sprites[1].m_position.x += 0.1f;
    }

    // Button: A
    if (xinputData.Gamepad.wButtons & XINPUT_GAMEPAD_A) { 
        m_Sprites[1].m_position.x -= 0.1f;
    }
}

void CDemoSprites::Render()
{
    // Check if D3D is ready
    if (m_pD3DContext == NULL)
        return;

    // Clear back buffer
    float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);


    // Stride and offset
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // Set vertex buffer
    m_pD3DContext->IASetInputLayout(m_pInputLayout);
    m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set shaders
    m_pD3DContext->VSSetShader(m_pVS, 0, 0);
    m_pD3DContext->PSSetShader(m_pPS, 0, 0);
    m_pD3DContext->PSSetShaderResources(0, 1, &m_pColorMap);
    m_pD3DContext->PSSetSamplers(0, 1, &m_pColorMapSampler);

    // Draw sprites
    for (int i = 0; i < 2; i++)
    {
        XMMATRIX world = m_Sprites[i].GetWorldMatrix( );
        XMMATRIX mvp = XMMatrixMultiply(world, m_vpMatrix);
        mvp = XMMatrixTranspose(mvp);

        m_pD3DContext->UpdateSubresource( m_MvpCB, 0, 0, &mvp, 0, 0 );
        m_pD3DContext->VSSetConstantBuffers( 0, 1, &m_MvpCB );

        m_pD3DContext->Draw(6, 0);
    }


    // Present back buffer to display
    m_pSwapChain->Present(0, 0);
}
