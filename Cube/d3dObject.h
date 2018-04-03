#pragma once

#ifndef _D3DOBJECT_H_
#define _D3DOBJECT_H_

#include "d3dUtil.h"
#include "GeometryGenerator.h"

class d3dObject
{
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT3 normal;
	};
private:
	std::vector<Vertex> mVertexContainer;
	GeometryGenerator mGeoGen;
	Material mMat;
	float mStartTick;
	float mCurrentTick;
	float mEndTick;
	float 
public:
	bool BuildVertexBuffer(ID3D11DeviceContext *);
	bool Update(ID3D11DeviceContext*);
	bool Render(ID3D11DeviceContext*);
	virtual bool UpdateVertex(ID3D11DeviceContext*);
	d3dObject();
	~d3dObject();
};

#endif // !1
