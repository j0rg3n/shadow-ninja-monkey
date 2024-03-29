Shader "Ninja black" {
Properties {
	_MainTex ("Base (RGB)", RECT) = "white" {}
	_RampTex ("Base (RGB)", 2D) = "grayscaleRamp" {}
}

SubShader {
	Pass {
		ZTest Always Cull Off ZWrite Off
		Fog { Mode off }
				
CGPROGRAM
#pragma vertex vert_img
#pragma fragment frag
#pragma fragmentoption ARB_precision_hint_fastest 
#include "UnityCG.cginc"

uniform samplerRECT _MainTex;
uniform sampler2D _RampTex;
uniform float _RampOffset;

float4 frag (v2f_img i) : COLOR
{
	float4 original = texRECT(_MainTex, i.uv);
	float grayscale = Luminance(original.rgb) *3;
	grayscale = grayscale > 1 ? 1 : grayscale;
//	float2 remap = float2 (grayscale + _RampOffset, .5);
//	float4 output = tex2D(_RampTex, remap);
//	float4 output = float4(grayscale,grayscale,grayscale,grayscale);
	float4 output = original * 2 ;
	output.a = original.a;
	return output;
}
ENDCG

	}
}

Fallback off

}