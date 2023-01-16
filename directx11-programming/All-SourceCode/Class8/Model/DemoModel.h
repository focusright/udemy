// DemoModel.h : Defines the CDemoModel class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _DEMOMODEL_H_
#define _DEMOMODEL_H_

#include "Dx11Base.h"
#include "ObjParser.h"


class CDemoModel : public CDx11Base
{
// Constructors
public:
    CDemoModel();
    virtual ~CDemoModel();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

// Implementation
protected:
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
    CObjParser m_ObjParser;
    XMFLOAT3 m_SphericalCamera;
    XMFLOAT3 m_SphericalCameraPos;
};


#endif // _DEMOMODEL_H_
