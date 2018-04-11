#pragma once

// 暂定为刚体类型，不允许修改形状数据

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

	//	移动向量
	XMFLOAT3 mTOriginVector;
	//	旋转向量
	XMFLOAT3 mROriginVector;
	//	缩放向量
	XMFLOAT3 mSOriginVector;
	//	暂存原始世界矩阵
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
