// DemoCube.h : Defines the CDemoCube class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _DEMOCUBE_H_
#define _DEMOCUBE_H_

#include "Dx11Base.h"
#include <d3dx11effect.h>


class CDemoCube : public CDx11Base
{
// Constructors
public:
    CDemoCube();
    virtual ~CDemoCube();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

// Members
protected:
    ID3DX11Effect* m_pEffect;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pIndexBuffer;
    ID3D11ShaderResourceView* m_pColorMap;
    ID3D11SamplerState* m_pColorMapSampler;
    XMMATRIX m_viewMatrix;
    XMMATRIX m_projMatrix;
};


#endif // _DEMOCUBE_H_