#include "d3dObject.h"

d3dObject::d3dObject(XMFLOAT3 initPos, const GeometryGenerator::MeshData& initMesh) 
	:d3dObject(initPos) {
	mMeshData = initMesh;
}

d3dObject::d3dObject(XMFLOAT3 initPos)
	:d3dObject(){
	XMStoreFloat4x4(&mCurrentWorldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&mWorld, XMMatrixTranslation(initPos.x, initPos.y, initPos.z));
}

d3dObject::d3dObject()
	:mStartTick(0),
	mCurrentTick(0),
	mEndTick(0),
	mCurrentType(AnimationType::NONE),
	mCurrentMethod(AnimationMethod::SMOOTH),
	mTransforming(false)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mCurrentWorldMatrix, I);
	XMStoreFloat4x4(&mWorld, I);

	mMat.Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mMat.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	mGeoGen.CreateBox(3.0f, 3.0f, 3.0f, mMeshData);
}

d3dObject::~d3dObject()
{
	ReleaseCOM(mIndexBuffer);
	ReleaseCOM(mVertexBuffer);
}

bool d3dObject::BuildVertexBuffer(ID3D11Device* d3dContext) {
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(GeometryGenerator::Vertex) * mMeshData.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
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

	return true;
}

bool d3dObject::Render(ID3D11DeviceContext* d3dContext) {
	d3dContext->DrawIndexed(mMeshData.Indices.size(), 0, 0);
	return true;
}

bool d3dObject::SetBuffer(ID3D11DeviceContext* d3dContext) {
	UINT stride = sizeof(GeometryGenerator::Vertex);
	UINT offset = 0;
	d3dContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	d3dContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	return true;
}

bool d3dObject::Update(ID3D11DeviceContext* d3dContext, float dt) {
	if (!mTransforming)
		return true;
	mCurrentTick += dt;
	if (mCurrentTick >= mEndTick) {
		mCurrentType = AnimationType::NONE;
		mTransforming = false;
		UpdateWorldMatrix();
		mTOriginVector = {0,0,0};
		return true;
	}
	float trans_delta = 0.0f;
	switch (mCurrentType) {
	case AnimationType::MOVING:
		trans_delta = (mCurrentTick - mStartTick) / (mEndTick - mStartTick);
		XMStoreFloat4x4(&mWorld, 
			XMMatrixTranslationFromVector(
				trans_delta*XMLoadFloat3(&mTOriginVector)) 
			* XMLoadFloat4x4(&mCurrentWorldMatrix));
		break;
	default:
		break;
	}
	return true;
}

bool d3dObject::Move(XMFLOAT3 delta) {
	mTOriginVector = delta;
	XMStoreFloat3(&mPosition,XMLoadFloat3(&delta)+XMLoadFloat3(&mPosition));
	UpdateWorldMatrix();
	return true;
}

bool d3dObject::Move(XMFLOAT3 delta, float duration) {
	if (mTransforming)
		return false;
	mCurrentWorldMatrix = mWorld;
	mEndTick = duration;
	mCurrentTick = 0;
	mTOriginVector = delta;
	mCurrentType = AnimationType::MOVING;
	mTransforming = true;
	return true;
}

void d3dObject::UpdateWorldMatrix() {
	XMStoreFloat4x4(&mWorld, XMMatrixTranslationFromVector(XMLoadFloat3(&mTOriginVector)) * XMLoadFloat4x4(&mCurrentWorldMatrix));
}