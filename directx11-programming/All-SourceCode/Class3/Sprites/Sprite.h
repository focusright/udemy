// CSprite.h : Defines the CSprite class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _SPRITE_H_
#define _SPRITE_H_


class CSprite
{
// Constructors
public:
    CSprite();
    virtual ~CSprite();

// Methods
public:
    XMMATRIX GetWorldMatrix();
    void SetPosition(XMFLOAT2& position);
    void SetRotation(float rotation);
    void SetScale(XMFLOAT2& scale);

// Attributes
public:
    XMFLOAT2 m_position;
    float m_rotation;
    XMFLOAT2 m_scale;
};


#endif // _SPRITE_H_
