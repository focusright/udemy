// DemoSprites.h : Defines the CDemoSprites class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _DEMOSPRITES_H_
#define _DEMOSPRITES_H_

#include "Dx11Base.h"
#include "Sprite.h"


class CDemoSprites : public CDx11Base
{
// Constructors
public:
    CDemoSprites();
    virtual ~CDemoSprites();

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
    ID3D11ShaderResourceView* m_pColorMap;
    ID3D11SamplerState* m_pColorMapSampler;
    ID3D11BlendState* m_pAlphaBlendState;
    CSprite m_Sprites[2];
    ID3D11Buffer* m_MvpCB;
    XMMATRIX m_vpMatrix;
};


#endif // _DEMOSPRITES_H_
