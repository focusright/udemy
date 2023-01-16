// ShaderTexture.fx : Implements the shader.
//
// By Geelix School of Serious Games and Edutainment.
//


Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);


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


PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input)0;

    vsOut.pos = vertex.pos;
    vsOut.tex0 = vertex.tex0;

    return vsOut;
}


float4 PS_Main(PS_Input frag) : SV_TARGET
{
    // Set variables
    float4 cbk1 = float4(0,0,0.5f,1); // new back color
    float dx = 1.0f / 640.0f; // font texture adjustment
    float dy = 1.0f / 224.0f; // font texture adjustment

    // Get color samples
    float4 cc = colorMap.Sample(colorSampler, frag.tex0 );
    float4 cl = colorMap.Sample(colorSampler, frag.tex0 + float2(-dx, 0) );
    float4 cr = colorMap.Sample(colorSampler, frag.tex0 + float2(+dx, 0) );
    float4 ct = colorMap.Sample(colorSampler, frag.tex0 + float2(0, -dx) );
    float4 cb = colorMap.Sample(colorSampler, frag.tex0 + float2(0, +dx) );

    // Change bacground color
    if (cc.r == 0) cc = cbk1;
    if (cl.r == 0) cl = cbk1;
    if (cr.r == 0) cr = cbk1;
    if (ct.r == 0) ct = cbk1;
    if (cb.r == 0) cb = cbk1;

    // Calculate color
    float4 col = (cc * 0.20f) + (cl * 0.20f) + (cr * 0.20f) + (ct * 0.20f) + (cb * 0.20f);

    // Without (test)
    //col = colorMap.Sample(colorSampler, frag.tex0);

    // Return the color
    return col;
}
