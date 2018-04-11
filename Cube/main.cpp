#include "BoxApp.h"
#include "MutexLock.h"
#include "SoundEngine.h"

MutexLock *mutexLock;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	mutexLock = new MutexLock();
	BoxApp theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}


BoxApp::BoxApp(HINSTANCE hInstance)
	: D3DApp(hInstance),
	mVB(0),
	mWavesVB(0),
	mWavesIB(0),
	mIB(0),
	mFX(0),
	mTech(0),
	mfxWorldViewProj(0),
	mInputLayout(0),
	mTheta(1.5f*MathHelper::Pi),
	mPhi(0.25f*MathHelper::Pi),
	mRadius(80.0f),
	mWireframeRS(0),
	mCam()
{
	mMainWndCaption = L"EasyDirectX11";

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.LookAt(XMFLOAT3(10, 10, 10), XMFLOAT3(0, 0, 0), XMFLOAT3(-1, 3, -1));

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
	XMStoreFloat4x4(&mWavesWorld, I);

	XMMATRIX wavesOffset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mWavesWorld, wavesOffset);

	mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mSpotLight.Spot = 96.0f;
	mSpotLight.Range = 10000.0f;

	mWavesMat.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	mWavesMat.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	mWavesMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);
}

BoxApp::~BoxApp()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mWavesVB);
	ReleaseCOM(mWavesIB);

	ReleaseCOM(mFX);
	ReleaseCOM(mWireframeRS);
	ReleaseCOM(mInputLayout);
}

bool BoxApp::Init()
{
	if (!D3DApp::Init())
		return false;

	mWaves.Init(160, 160, 1.0f, 0.03f, 3.25f, 0.4f);


	// BuildGeometryBuffers();
	BuildWaveGeometryBuffers();

	mpObjects.push_back(new d3dObject(XMFLOAT3(0.0f, -2.0f, 0.0f)));
	mpObjects[0]->BuildVertexBuffer(md3dDevice);
	mpObjects[0]->Move(XMFLOAT3(0.0f, 2.0F, 0.0F), 0.5f);
	mWaves.Disturb(80, 79, -0.5f);

	BuildFX();
	BuildVertexLayout();


	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_SOLID;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	//HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));

	return true;
}

void BoxApp::OnResize()
{
	D3DApp::OnResize();
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void BoxApp::UpdateScene(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000)
		mCam.Walk(10.0f*dt);

	if (GetAsyncKeyState('S') & 0x8000)
		mCam.Walk(-10.0f*dt);

	if (GetAsyncKeyState('A') & 0x8000)
		mCam.Strafe(-10.0f*dt);

	if (GetAsyncKeyState('D') & 0x8000)
		mCam.Strafe(10.0f*dt);

	/*if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		mpObjects[0]->Move(XMFLOAT3(10,10,10),1.0f);*/

		/*static float t_base = 0.0f;
		if ((mTimer.TotalTime() - t_base) >= 0.25f)
		{
			t_base += 0.25f;

			DWORD i = 5 + rand() % (mWaves.RowCount() - 10);
			DWORD j = 5 + rand() % (mWaves.ColumnCount() - 10);

			float r = MathHelper::RandF(1.0f, 2.0f);

			mWaves.Disturb(i, j, r);
		}*/

	for (vector<d3dObject*>::iterator iter = mpObjects.begin(); iter != mpObjects.end(); iter++) {
		(*iter)->Update(md3dImmediateContext, dt);
	}

	mWaves.Update(dt);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(md3dImmediateContext->Map(mWavesVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);
	for (UINT i = 0; i < mWaves.VertexCount(); ++i)
	{
		v[i].Pos = mWaves[i];
		v[i].Normal = mWaves.Normal(i);
	}

	md3dImmediateContext->Unmap(mWavesVB, 0);

	mSpotLight.Position = mCam.GetPosition();
	XMStoreFloat3(&mSpotLight.Direction, XMVector3Normalize(mCam.GetLookXM()));

}

void BoxApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	md3dImmediateContext->RSSetState(mWireframeRS);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mCam.UpdateViewMatrix();
	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();

	mfxDirLight->SetRawValue(&mDirLight, 0, sizeof(mDirLight));
	mfxSpotLight->SetRawValue(&mSpotLight, 0, sizeof(mSpotLight));
	mfxEyePosW->SetRawValue(&mCam.GetPosition(), 0, sizeof(mCam.GetPosition()));

	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Matrices
		XMMATRIX world;
		XMMATRIX worldInvTranspose;
		XMMATRIX worldViewProj;

		for (vector<d3dObject*>::iterator iter = mpObjects.begin(); iter != mpObjects.end(); iter++)
		{
			(*iter)->SetBuffer(md3dImmediateContext);

			world = XMLoadFloat4x4(&(*iter)->mWorld);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world * view * proj;

			mfxWorld->SetMatrix(reinterpret_cast<float*>(&world));
			mfxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
			mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
			mfxMaterial->SetRawValue(&(*iter)->mMat, 0, sizeof((*iter)->mMat));

			mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			(*iter)->Render(md3dImmediateContext);
		}

		// Waves

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mWavesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mWavesIB, DXGI_FORMAT_R32_UINT, 0);

		world = XMLoadFloat4x4(&mWavesWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world * view * proj;

		mfxWorld->SetMatrix(reinterpret_cast<float*>(&world));
		mfxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		mfxMaterial->SetRawValue(&mWavesMat, 0, sizeof(mWavesMat));

		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(3 * mWaves.TriangleCount(), 0, 0);
	}

	HR(mSwapChain->Present(0, 0));
}

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCam.Pitch(dy);
		mCam.RotateY(dx);
	}

	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.2f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.2f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 1.0f, 500.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void BoxApp::BuildWaveGeometryBuffers()
{
	// Create the vertex buffer.  Note that we allocate space only, as
	// we will be updating the data every time step of the simulation.

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * mWaves.VertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(md3dDevice->CreateBuffer(&vbd, 0, &mWavesVB));

	std::vector<UINT> indices(3 * mWaves.TriangleCount());

	UINT m = mWaves.RowCount();
	UINT n = mWaves.ColumnCount();
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1)*n + j;

			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mWavesIB));
}

void BoxApp::BuildFX()
{
	std::ifstream fin("FX/Lighting.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, md3dDevice, &mFX));

	mTech = mFX->GetTechniqueByName("LightTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxWorld = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxWorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mfxEyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	mfxDirLight = mFX->GetVariableByName("gDirLight");
	mfxSpotLight = mFX->GetVariableByName("gSpotLight");
	mfxMaterial = mFX->GetVariableByName("gMaterial");
}

void BoxApp::BuildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout));
}