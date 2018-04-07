#include "d3dObject.h"

d3dObject::d3dObject()
	:mStartTick(0),
	mCurrentTick(0),
	mEndTick(0),
	mCurrentType(AnimationType::NONE),
	mCurrentMethod(AnimationMethod::SMOOTH)
{
}

d3dObject::~d3dObject()
{
	ReleaseCOM(mIndexBuffer);
	ReleaseCOM(mVertexBuffer);

}

bool d3dObject::BuildVertexBuffer(ID3D11Device* d3dContext) {
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mMeshData.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mMeshData.Vertices[0];
	HR(d3dContext->CreateBuffer(&vbd, &vinitData, &mVertexBuffer));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mMeshData.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mMeshData.Indices[0];
	HR(d3dContext->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));
}

bool d3dObject::Render(ID3D11DeviceContext* d3dContext) {

}