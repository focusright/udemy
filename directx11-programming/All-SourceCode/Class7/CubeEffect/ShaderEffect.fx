// ShaderEffect.fx : Implements the shader.
//
// By Geelix School of Serious Games and Edutainment.
//


Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);


cbuffer cbChangesEveryFrame : register(b0)
{
    matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
    matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
    matrix projMatrix;
};


struct VS_Input
{
    float4 pos  : POSITION;
    float2 tex0 : TEXCOORD0;
};


struct PS_Input
{
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
};


////////////////////////////////////////
// Pass 0: Draws the cube

PS_Input VS0(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input)0;

    vsOut.pos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(vsOut.pos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);
    vsOut.tex0 = vertex.tex0;

    return vsOut;
}


float4 PS0(PS_Input frag) : SV_TARGET
{
    float4 col = colorMap.Sample(colorSampler, frag.tex0);
    col.a = 1.0f;
    return col;
}


////////////////////////////////////////
// Pass 1: Draws the warp trail

PS_Input VS1(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input)0;

    // Scale and move source vertexes
    vertex.pos.z *= 16.0f;
    vertex.pos += float4(0.05f,0.0f,16.8f,0.0f);

    // Standard stuff
    vsOut.pos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(vsOut.pos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);
    vsOut.tex0 = vertex.tex0;

    return vsOut;
}


float4 PS1(PS_Input frag) : SV_TARGET
{
    // Get color and adjust it
    float4 col = colorMap.Sample(colorSampler, frag.tex0);
    col.r += 0.2f;
    col.g += 0.2f;
    col.b += 0.2f;
    col.a = 0.15f;
    return col;
}


////////////////////////////////////////
// Techniques

BlendState UseAlphaBlending {
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
    SrcBlend = SRC_ALPHA;
};


technique11 Warptrail
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS0()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS0()));
    }
    pass P1
    {
        SetBlendState(UseAlphaBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, VS1()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS1()));
    }
}

technique11 WithoutWarptrail
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS0()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS0()));
    }
}
