// DemoText.cpp : Implements the CDemoText class.
//
// By Geelix School of Serious Games and Edutainment.
//

#include "DemoText.h"

// Vertex struct
struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};

// Maximum number of characters that can be drawn by DrawText()
// Feel free to increase this number if necessary
#define DRAWTEXT_MAXCHARS 1024

// Specifies the shader file
#define SHADERFILE L"ShaderTexture.fx"


//////////////////////////////////////////////////////////////////////
// Constructors

CDemoText::CDemoText()
{
    m_pVS = NULL;
    m_pPS = NULL;
    m_pInputLayout = NULL;
    m_pVertexBuffer = NULL;
    m_pColorMap = NULL;
    m_pColorMapSampler = NULL;
}

CDemoText::~CDemoText()
{
}


//////////////////////////////////////////////////////////////////////
// Overrides

bool CDemoText::LoadContent()
{
    // Compile vertex shader
    ID3DBlob* pVSBuffer = NULL;
    bool res = CompileShader(SHADERFILE, "VS_Main", "vs_4_0", &pVSBuffer);
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
    res = CompileShader(SHADERFILE, "PS_Main", "ps_4_0", &pPSBuffer);
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

    // Vertex description
    D3D11_BUFFER_DESC vertexDesc;
    ::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(Vertex) * 6 * DRAWTEXT_MAXCHARS;

    // Create vertex buffer
    hr = m_pD3DDevice->CreateBuffer(&vertexDesc, NULL, &m_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Load texture
    hr = ::D3DX11CreateShaderResourceViewFromFile(
        m_pD3DDevice, L"arial.dds", 0, 0, &m_pColorMap, 0);
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

    return true;
}

void CDemoText::UnloadContent()
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
}

void CDemoText::Update()
{
}

void CDemoText::Render()
{
    // Check if D3D is ready
    if (m_pD3DContext == NULL)
        return;

    // Clear back buffer
    float color[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
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

    // Draw text
    DrawText("Hello World!", -0.9f, 0.0f);


    // Present back buffer to display
    m_pSwapChain->Present(0, 0);
}


//////////////////////////////////////////////////////////////////////
// Implementation

// This function draws text by creating multiple sprites in the vertex
// buffer m_pVertexBuffer and maps letters from the arial.dds texture.
// If you wish, you can extend this function by having multiple vertex
// buffers that you can draw to. You can also make varying width chars
// by e.g. storing the char widths in an array and adjust the width of
// each sprite (and texture coordinates) accordingly.
// Usage:
// - text can contain any string of characters, including newline "\n"
//   and special characters (e.g. "\x8a" is the copyright symbol).
// - x and y specify the starting x and y position of the text area.
// - scale adjusts the size of the text (e.g. 0.5f is half size).
bool CDemoText::DrawText(LPCSTR text, float x, float y, float scale)
{
    // Get the length of the text
    int nTextLen = strlen(text);

    // Variables
    int nSpriteCount = 0; // sprite count
    float cx = x; // current x-position
    float cy = y; // current y-position
    float cw = scale * 32.0f / 320.0f; // char width
    float ch = scale * 32.0f / 240.0f; // char height

    // Map vertex buffer to system memory
    // (so that we can write to it)
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_pD3DContext->Map(
        m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Unable to map resource", L"ERROR", MB_OK);
        return false;
    }

    // Get a pointer to our vertexes
    Vertex* vptr = (Vertex*)mappedResource.pData;

    // Process text
    for (int i = 0; i < nTextLen; i++)
    {
        // Get character
        UCHAR character = (UCHAR)text[i];
        
        // Chack range
        if (((character < 32) || (character >= 172)) && (character != '\n'))
            continue; // ignore illegal characters

        // Newline
        if (character == '\n') {
            cy -= ch;
            cx = x;
            continue;
        }

        // Calculate end x/y position
        float ex = cx + cw;
        float ey = cy + ch;

        // Make sprite (i.e. two triangles)
        vptr[0].pos = XMFLOAT3( ex,  ey,  1.0f );
        vptr[1].pos = XMFLOAT3( ex,  cy,  1.0f );
        vptr[2].pos = XMFLOAT3( cx,  cy,  1.0f );
        vptr[3].pos = XMFLOAT3( cx,  cy,  1.0f );
        vptr[4].pos = XMFLOAT3( cx,  ey,  1.0f );
        vptr[5].pos = XMFLOAT3( ex,  ey,  1.0f );

        // Calculate texture positions
        int textureX = ((character - 32) % 20) * 32;
        int textureY = ((character - 32) / 20) * 32;
        float tcx = ((float)textureX) / 640.0f;
        float tcy = ((float)textureY) / 224.0f;
        float tex = tcx + (32.0f / 640.0f);
        float tey = tcy + (32.0f / 224.0f);

        // Map texture character to sprite
        vptr[0].tex0 = XMFLOAT2( tex, tcy );
        vptr[1].tex0 = XMFLOAT2( tex, tey );
        vptr[2].tex0 = XMFLOAT2( tcx, tey );
        vptr[3].tex0 = XMFLOAT2( tcx, tey );
        vptr[4].tex0 = XMFLOAT2( tcx, tcy );
        vptr[5].tex0 = XMFLOAT2( tex, tcy );

        // Next
        cx += cw;
        vptr += 6;
        nSpriteCount++;
        if (nSpriteCount >= DRAWTEXT_MAXCHARS)
            break; // no more vertexes
    }

    // Unmap the vertex buffer
    // (uploads it to the GPU)
    m_pD3DContext->Unmap(m_pVertexBuffer, 0);

    // Draw triangles
    m_pD3DContext->Draw(6 * nSpriteCount, 0);

    return true;
}
