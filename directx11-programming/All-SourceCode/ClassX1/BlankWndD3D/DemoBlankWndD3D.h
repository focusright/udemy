// DemoBlankWndD3D.h : Defines the CDemoBlankWndD3D class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _DEMOBLANKWNDD3D_H_
#define _DEMOBLANKWNDD3D_H_

#include "Dx11Base.h"


class CDemoBlankWndD3D : public CDx11Base
{
// Constructors
public:
    CDemoBlankWndD3D();
    virtual ~CDemoBlankWndD3D();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();
};


#endif // _DEMOBLANKWNDD3D_H_
