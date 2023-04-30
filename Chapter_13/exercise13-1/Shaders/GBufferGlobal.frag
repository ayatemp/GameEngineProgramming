// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// 頂点シェーダーからの入力
// テクスチャ座標
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
layout(location = 0) out vec4 outColor;

// Gバッファのテクスチャ
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;

// 平行光源用に構造体を作る
struct DirectionalLight
{
	// 光の方向
	vec3 mDirection;
	// 拡散反射色
	vec3 mDiffuseColor;
	// 鏡面反射色
	vec3 mSpecColor;
};

// 照明用のuniform（第6章のPhong.fragと同様）
// Camera position (in world space)
uniform vec3 uCameraPos;
// Ambient light level
uniform vec3 uAmbientLight;
// Directional Light
uniform DirectionalLight uDirLight;
//　鏡面反射指数
uniform float uSpecPower;

void main()
{
    //アルベド / 法線 / ワールド座標をGバッファからサンプリング
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;
    //float gbufferSpecPower = texture(uSpecPower,fragTexCoord);
	// Surface normal
	vec3 N = normalize(gbufferNorm);
	// Vector from surface to light
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos - gbufferWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Phongの反射の計算をする
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * dot(N, L);
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
        //vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), gbufferSpecPower);
        Phong += Diffuse + Specular;
	}
	// Clamp light between 0-1 RGB values
	Phong = clamp(Phong, 0.0, 1.0);

	// Final color is texture color times phong light (alpha = 1)
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
