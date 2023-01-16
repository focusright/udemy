// DemoAudioXACT3.h : Defines the CDemoAudioXACT3 class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _DEMOAUDIOXACT3_H_
#define _DEMOAUDIOXACT3_H_

#include "Dx11Base.h"
#include "XACT3Util.h"


class CDemoAudioXACT3 : public CDx11Base
{
// Constructors
public:
    CDemoAudioXACT3();
    virtual ~CDemoAudioXACT3();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

// Members
protected:
    ID3D11VertexShader* m_pVS;
    ID3D11PixelShader* m_pPS;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pIndexBuffer;
    ID3D11ShaderResourceView* m_pColorMap;
    ID3D11SamplerState* m_pColorMapSampler;
    ID3D11Buffer* m_pViewCB;
    ID3D11Buffer* m_pProjCB;
    ID3D11Buffer* m_pWorldCB;
    XMMATRIX m_viewMatrix;
    XMMATRIX m_projMatrix;
    float m_fRotation;
    CXACT3Util m_XACT3;
};


#endif // _DEMOAUDIOXACT3_H_
