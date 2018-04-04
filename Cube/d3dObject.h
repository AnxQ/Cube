#pragma once

#ifndef _D3DOBJECT_H_
#define _D3DOBJECT_H_

#include "d3dUtil.h"
#include "GeometryGenerator.h"

enum AnimationMethod {
	LINEAR,
	DIRECT
};

enum AnimationType {
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
private:

	// Object data

	std::vector<Vertex> mVertexContainer;
	GeometryGenerator mGeoGen;
	GeometryGenerator::MeshData mMeshData;
	Material mMat;

	// Animating

	float mStartTick;
	float mCurrentTick;
	float mEndTick;
	AnimationMethod mCurrentMethod;
	AnimationType mCurrentType;
	XMVECTOR mOriginTransforVector;
	XMVECTOR mCurrentTransformVector;
public:
	bool BuildVertexBuffer(ID3D11DeviceContext *);
	bool Update(ID3D11DeviceContext*);
	bool Render(ID3D11DeviceContext*);
	bool Move(XMVECTOR);
	bool Rotate(XMVECTOR);
	bool Scale(XMVECTOR);
	virtual bool UpdateVertex(ID3D11DeviceContext*);
	d3dObject();
	~d3dObject();
};

#endif 
