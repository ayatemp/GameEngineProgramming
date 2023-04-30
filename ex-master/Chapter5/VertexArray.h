// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// ���̒��_�z����A�N�e�B�u����i�`��ł���悤�ɂ���j
	void SetActive();

	unsigned int GeNtumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }

private:
	// ���_�o�b�t�@�ɂ��钸�_�̐�
	unsigned int mNumVerts;
	// �C���f�b�N�X�o�b�t�@�ɂ���C���f�b�N�X�̐�
	unsigned int mNumIndices;

	unsigned int mNumColors;

	// ���_�o�b�t�@�� OpenGL ID
	unsigned int mVertexBuffer;
	// �C���f�b�N�X�o�b�t�@�� OpenGL ID
	unsigned int mIndexBuffer;
	// ���_�z��I�u�W�F�N�g�� OpenGL ID
	unsigned int mVertexArray;
	// �J���[�o�b�t�@�� OpenGL ID
	unsigned int mColorBuffer;
};