#pragma once

// �ݶ�Ϊ�������ͣ��������޸���״����

#ifndef _D3DOBJECT_H_
#define _D3DOBJECT_H_

#include "d3dUtil.h"
#include "GeometryGenerator.h"
#include "MutexLock.h"

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
	float mLastTick;
	float mEndTick;
	bool mTransforming;

	AnimationMethod mCurrentMethod;
	AnimationType mCurrentType;

	//	�ƶ�����
	XMFLOAT3 mTOriginVector;
	//	��ת����
	XMFLOAT3 mROriginVector;
	//	��������
	XMFLOAT3 mSOriginVector;
	//	�ݴ�ԭʼ�������
	XMFLOAT4X4 mCurrentWorldMatrix;

	//	Position & Transform
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

	bool Scale(XMFLOAT3);

	d3dObject();
	d3dObject(XMFLOAT3);
	d3dObject(XMFLOAT3,const GeometryGenerator::MeshData&);
	~d3dObject();
};

#endif  //!_D3DOBJECT_H_
