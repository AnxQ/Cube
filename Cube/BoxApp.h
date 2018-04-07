#pragma once

#ifndef _BOXAPP_H_
#define _BOXAPP_H_


#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "Waves.h"
#include "Camera.h"
#include "d3dObject.h"

class BoxApp : public D3DApp
{
public:
	BoxApp(HINSTANCE hInstance);
	~BoxApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
private:
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();
	void BuildWaveGeometryBuffers();


private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11Buffer* mWavesVB;
	ID3D11Buffer* mWavesIB;

	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;
	ID3DX11EffectMatrixVariable* mfxWorld;
	ID3DX11EffectMatrixVariable* mfxWorldInvTranspose;
	ID3DX11EffectVectorVariable* mfxEyePosW;

	ID3DX11EffectVariable* mfxDirLight;
	ID3DX11EffectVariable* mfxSpotLight;
	ID3DX11EffectVariable* mfxMaterial;

	Waves mWaves;
	DirectionalLight mDirLight;
	SpotLight mSpotLight;

	Material mGeoMat;
	Material mWavesMat;

	ID3D11InputLayout* mInputLayout;

	ID3D11RasterizerState* mWireframeRS;

	Camera mCam;

	XMFLOAT4X4 mBoxWorld;
	XMFLOAT4X4 mGridWorld;
	XMFLOAT4X4 mWavesWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	UINT mBoxVertexBufferOffset;
	UINT mGridVertexBufferOffset;

	UINT mGridIndexBufferOffset;
	UINT mBoxIndexBufferOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;

	float mTheta;
	float mPhi;
	float mRadius;

	XMFLOAT3 mEyePosW;
	POINT mLastMousePos;

	std::vector<d3dObject *> mpObjects;
};

#endif // !_BOXAPP_H_
