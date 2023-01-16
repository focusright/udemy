// GxDemoApp.h : Defines the CGxDemoApp class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _GXDEMOAPP_H_
#define _GXDEMOAPP_H_

#include "GxDx11Base.h"
#include "GxObjParser.h"
#include "XACT3Util.h"


class CGxDemoApp : public CGxDx11Base
{
// Constructors
public:
    CGxDemoApp();
    virtual ~CGxDemoApp();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

// Implementation
public:
    int WndProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
    void CalcSphericalCamera();

// Members
protected:
    ID3D11VertexShader* m_pVS;
    ID3D11PixelShader* m_pPS;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11ShaderResourceView* m_pColorMap;
    ID3D11SamplerState* m_pColorMapSampler;
    ID3D11Buffer* m_pViewCB;
    ID3D11Buffer* m_pProjCB;
    ID3D11Buffer* m_pWorldCB;
    ID3D11Buffer* m_pCameraPosCB;
    XMMATRIX m_viewMatrix;
    XMMATRIX m_projMatrix;
    CGxObjParser m_ObjParser;
    XMFLOAT3 m_SphericalCamera;
    XMFLOAT3 m_SphericalCameraPos;
    CXACT3Util m_XACT3;
};


#endif // _GXDEMOAPP_H_
