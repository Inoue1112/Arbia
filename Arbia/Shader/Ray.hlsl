//ｸﾞﾛｰﾊﾞﾙ.

cbuffer global
{
	matrix	g_mWVP;	//WorldViewProjの変換行列.
	float4	g_vColor;//色.
};

//ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
float4 VS(float4 Pos : POSITION) : SV_Position
{
	Pos = mul(Pos, g_mWVP);
	return Pos;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS(float4 Pos : SV_Position) : SV_Target
{
	return g_vColor;
}