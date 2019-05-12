#pragma once

#define REV_SWAP_CHAIN_COUNT 2
#define REV_FRAME_RESOURCE_COUNT 3
#define REV_OBJECT_COUNT 2

#define REV_D3D_DRIVER_TYPE D3D_DRIVER_TYPE_HARDWARE
#define REV_BACK_BUFFER_FORMAT DXGI_FORMAT_R8G8B8A8_UNORM
#define REV_DEPTH_STENCIL_FORMAT DXGI_FORMAT_D24_UNORM_S8_UINT

#include <vector>

struct RevInitializationData
{
	HINSTANCE m_instance;
	WNDPROC m_windowHandle;

	unsigned int m_windowWidth;
	unsigned int m_windowHeight;
};


class RevEngineMain
{
public:

	static void Create(const RevInitializationData& initializationData);
	static void Destroy();
	static RevEngineMain* GetInstance();
	static void Update(float deltaTime);
	static void Draw(float deltaTime);
	static void Resize(UINT width, UINT height);
	static float GetAspectRatio();
	static XMFLOAT2 GetRenderTargetSize();
	static LRESULT ManageWindowsMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static void LoadWorld(const char* filePath);
	static void ReloadCurrentWorld();
	static void DrawMainPass( float deltaTime );
public:

	void InitializeInternal(const RevInitializationData& initializationData);
	void CreateEngineWindow(const RevInitializationData& initializationData);
	void CreateD3D(const RevInitializationData& initializationData);
	void ResizeInteral(UINT windowWidth, UINT windowHeight);

	void FlushCommandQueue();
	void DrawInternal(float deltaTime);
	void UpdateInteral(float deltaTime);
	void LoadWorldInternal(const char* path);
	void ReloadWorldInteral();

	void DrawMainPassInternal(float deltaTime);

	struct ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	static RevEngineMain* s_instance;

	UINT64 m_currentFence = 0;
	struct ID3D12CommandQueue* m_commandQueue;
	struct ID3D12CommandAllocator* m_directCmdListAlloc;
	struct ID3D12GraphicsCommandList* m_commandList;
	struct ID3D12Resource* m_swapChainBuffer[REV_SWAP_CHAIN_COUNT];
	struct ID3D12Resource* m_depthStencilBuffer;
	struct ID3D12DescriptorHeap* m_rtvHeap;
	struct ID3D12DescriptorHeap* m_dsvHeap;
	struct ID3D12DescriptorHeap* m_srvHeap;
	struct IDXGIFactory* m_dxgiFactory;
	struct IDXGISwapChain* m_swapChain;
	struct ID3D12Device* m_device;
	struct ID3D12Fence* m_fence;
	
	UINT m_currentBackBuffer;
	UINT m_rtvDescriptorSize;
	UINT m_dsvDescriptorSize;
	UINT m_cbvSrvUavDescriptorSize;

	RevEngineMain();
	~RevEngineMain() {};

public:

	HWND m_windowHandle;
	UINT32 m_currentWindowWidth;
	UINT32 m_currentWindowHeight;

	class RevShaderManager* m_shaderInstance;
	class RevModelManager* m_modelManager;
	class RevCamera* m_camera;
	class RevRenderManager* m_renderManager;
	class RevEngineEditorMain* m_editor = nullptr;
	class RevWorldLoader* m_worldLoader = nullptr;
	class RevWorld* m_activeWorld = nullptr;
	class RevUIManager* m_uiManager = nullptr;

	struct RevDebugSnapshotData* m_debugData = nullptr;

	std::vector<struct RevFrameResource*> m_frameResource;
	UINT m_currentFrameResourceIndex = 0;

	struct ID3D12DescriptorHeap* m_cbvHeap = nullptr;
};