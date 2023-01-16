// CSprite.h : Defines the CSprite class.
//
// By Geelix School of Serious Games and Edutainment.
//

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "sprite.h"


CSprite::CSprite()
{
    m_position.x = m_position.y = 0;
    m_rotation = 0;
    m_scale.x = m_scale.y = 1.0f;
}


CSprite::~CSprite()
{
}


// Gets the world transformation matrix
XMMATRIX CSprite::GetWorldMatrix()
{
    XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, 0.0f );
    XMMATRIX rotationZ = XMMatrixRotationZ(m_rotation);
    XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, 1.0f);
    return translation * rotationZ * scale;
}


// Sets the sprite position
void CSprite::SetPosition(XMFLOAT2& position)
{
    m_position = position;
}


// Sets the sprite rotation
void CSprite::SetRotation(float rotation)
{
    m_rotation = rotation;
}


// Sets the sprite scale
void CSprite::SetScale(XMFLOAT2& scale)
{
    m_scale = scale;
}
