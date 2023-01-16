// GxObjParser.h : Defines the CGxObjParser class.
//
// By Geelix School of Serious Games and Edutainment.
//

#ifndef _GXOBJPARSER_H_
#define _GXOBJPARSER_H_


// Vertex struct
struct VertexObj
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
    XMFLOAT3 norm;
};


// Main class
class CGxObjParser
{
// Constructors
public:
    CGxObjParser();
    virtual ~CGxObjParser();

// Methods
public:
    bool LoadFile(char* szFileName);
    void Clear();

// Attributes
public:
    VertexObj* m_pVertex;
    unsigned long m_nVertexCount;
};


#endif // _GXOBJPARSER_H_
