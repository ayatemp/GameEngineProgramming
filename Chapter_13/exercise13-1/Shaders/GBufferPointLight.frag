// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
layout(location = 0) out vec4 outColor;

// Different textures from G-buffer
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

// Create a struct for the point light
struct PointLight
{
	// Position of light
	vec3 mWorldPos;
	// Diffuse color
	vec3 mDiffuseColor;
	// Radius of the light
	float mInnerRadius;
	float mOuterRadius;
    
    //課題１３−１
    float mSpecPower;
    vec3 mSpecColor;
};

uniform PointLight uPointLight;
// Stores width/height of screen
uniform vec2 uScreenDimensions;

void main()
{
	//　Gバッファをサンプリングする座標を計算
    vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;
	
	// Gバッファからサンプリング
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;
	
	// 法線および，表面から高原までのベクトルを計算
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(uPointLight.mWorldPos - gbufferWorldPos);

	// Phong拡散反射成分を計算
	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		// 光源とワールド座標位置との間の距離を求める
		float dist = distance(uPointLight.mWorldPos, gbufferWorldPos);
		// smoothstepで，内側と外側の半径の間の
		// 範囲[0,1]の輝度に依存する
		float intensity = smoothstep(uPointLight.mInnerRadius,
									 uPointLight.mOuterRadius, dist);
		// 光の拡散反射は輝度に依存する
		vec3 DiffuseColor = mix(uPointLight.mDiffuseColor,
								vec3(0.0, 0.0, 0.0), intensity);
        vec3 Specular = mix(uPointLight.mSpecColor,vec3(0.0, 0.0, 0.0), intensity);
        Phong = (DiffuseColor +Specular)* NdotL;
        //Phong = DiffuseColor * NdotL;
	}

	// テクスチャの色にPhongの拡散反射をかけて最終的な色にする
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
