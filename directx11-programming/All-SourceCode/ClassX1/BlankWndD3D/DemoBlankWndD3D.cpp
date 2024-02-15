// DemoBlankWndD3D.cpp : Implements the CDemoBlankWndD3D class.
//
// By Geelix School of Serious Games and Edutainment.
//

#include "DemoBlankWndD3D.h"


CDemoBlankWndD3D::CDemoBlankWndD3D()
{
}

CDemoBlankWndD3D::~CDemoBlankWndD3D()
{
}


//////////////////////////////////////////////////////////////////////
// Overrides


bool CDemoBlankWndD3D::LoadContent()
{
    return true;
}

void CDemoBlankWndD3D::UnloadContent()
{
}

void CDemoBlankWndD3D::Update()
{
}

void CDemoBlankWndD3D::Render()
{
    // Check if D3D is ready
    if (m_pD3DContext == NULL)
        return;

    // Clear back buffer
    float color[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);

    // Present back buffer to display
    m_pSwapChain->Present(0, 0);
}