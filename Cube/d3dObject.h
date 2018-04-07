#pragma once

// 暂定为刚体类型，不允许修改形状数据

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
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};
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

	AnimationMethod mCurrentMethod;
	AnimationType mCurrentType;
	XMVECTOR mOriginTransforVector;
	XMVECTOR mCurrentTransformVector;

	XMFLOAT4X4 mWorld;

public:
	bool BuildVertexBuffer(ID3D11Device*);
	bool Update(ID3D11DeviceContext*);
	bool Render(ID3D11DeviceContext*);
	bool Move(XMVECTOR);
	bool Rotate(XMVECTOR);
	bool Scale(XMVECTOR);
	d3dObject();
	~d3dObject();
};

#endif  //!_D3DOBJECT_H_
