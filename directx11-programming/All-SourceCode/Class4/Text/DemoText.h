// DemoText.h : Defines the CDemoText class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _DEMOTEXT_H_
#define _DEMOTEXT_H_

#include "Dx11Base.h"


class CDemoText : public CDx11Base
{
// Constructors
public:
    CDemoText();
    virtual ~CDemoText();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

// Implementation
protected:
    bool DrawText(LPCSTR text, float x, float y, float scale = 1.0f);

// Members
protected:
    ID3D11VertexShader* m_pVS;
    ID3D11PixelShader* m_pPS;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11ShaderResourceView* m_pColorMap;
    ID3D11SamplerState* m_pColorMapSampler;
};


#endif // _DEMOTEXT_H_
