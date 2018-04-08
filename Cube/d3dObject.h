#pragma once

// �ݶ�Ϊ�������ͣ��������޸���״����

#ifndef _D3DOBJECT_H_
#define _D3DOBJECT_H_

#include "d3dUtil.h"
#include "GeometryGenerator.h"

enum AnimationMethod {
	LINEAR,
	DIRECT,
	SMOOTH
};

enum AnimationType {
	NONE,
	MOVING,
	SCALING,
	ROTATING
};

class d3dObject
{
public:

	// Object data

	GeometryGenerator mGeoGen;
	GeometryGenerator::MeshData mMeshData;
	Material mMat;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mVertexBuffer;

	// Animating

	float mStartTick;
	float mCurrentTick;
	float mEndTick;
	bool mTransforming;

	AnimationMethod mCurrentMethod;
	AnimationType mCurrentType;

	//	�任����
	XMMATRIX mCurrentWorldMatrix;
	XMMATRIX mCurrentTransformMatrix;

	//	�ƶ�����
	XMVECTOR mTOriginVector;
	XMVECTOR mTCurrentVector;
	//	��ת����
	XMVECTOR mROriginVector;
	XMVECTOR mRCurrentVector;
	//	��������
	XMVECTOR mSOriginVector;
	XMVECTOR mSCurrentVector;

	//	Position&Transform
	XMFLOAT4X4 mWorld;
	XMFLOAT3 mPosition;


public:
	bool BuildVertexBuffer(ID3D11Device*);
	bool Update(ID3D11DeviceContext*, float);
	bool Render(ID3D11DeviceContext*);
	bool SetBuffer(ID3D11DeviceContext*);

	void UpdateWorldMatrix();

	bool Move(XMFLOAT3);
	bool Move(XMFLOAT3, float duration);
	bool Move(XMFLOAT3, float duration, float start, float end);

	bool Rotate(XMFLOAT3);

	d3dObject();
	~d3dObject();
};

#endif  //!_D3DOBJECT_H_
