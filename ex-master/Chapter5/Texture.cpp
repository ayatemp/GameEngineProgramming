// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Texture.h"
#include <SOIL.h>
#include <GL/glew.h>
#include <SDL.h>

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	// �e�N�X�`����ǂݍ���
	// ���A�����A�`���l�������L�^�����
	unsigned char* image = SOIL_load_image(fileName.c_str(), &mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);

	// �ǂݍ��߂Ȃ�������G���[��f��
	if (image == nullptr)
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	// �ǂݍ��񂾃e�N�X�`����RGB��RGBA���𔻒�
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// OpenG+�e�N�X�`���I�u�W�F�N�g���쐬
	glGenTextures(1, &mTextureID);
	// �e�N�X�`�����A�N�e�B�u��
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// �e�N�X�`���� OpenGL ���Ǘ�����e�N�X�`���������Ɋ��蓖�Ă�
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
		GL_UNSIGNED_BYTE, image);

	// �e�N�X�`���������Ɋ��蓖�Ă��̂ŁA����������������
	SOIL_free_image_data(image);

	// �o�C���j�A�t�B���^��L���ɂ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
