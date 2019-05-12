#pragma once

enum class RevEditorActions : UINT8;
	
class RevEngineFunctions
{
public:
	static void CreateSRVRTVDescriptorHeap(
			const struct RevDescriptorInitializationData& initializationData, 
			struct RevDescriptorRuntimeData* out);

	static struct RevDescriptorHeapIncrementSizeData* FindIncrementSizes();

	static void FindWindowWidthHeight(UINT32* outX, UINT32* outY);

	static struct ID3D12Device* FindDevice();
	static struct ID3D12GraphicsCommandList* FindCommandList();
	static struct ID3D12CommandQueue* FindCommandQueue();
	static struct ID3D12CommandAllocator* FindCommandAllocator();

	static struct ID3D12Resource* FindDepthStencilBuffer();
	static struct ID3D12DescriptorHeap* FindDSVHeap();
	static class RevShaderManager* FindRevShaderManager();
	static class RevCamera* FindCamera();

	static struct RevDebugSnapshotData* FindDebugSnapshotData();
	static struct RevFrameResource* FindFrameResource(INT32 frameIndex);
	static class RevModelManager* FindModelManager();
	static void FlushCommandQueue();
	static float GetAspectRatio();

	static void RequestEditorAction(RevEditorActions editorAction);
	
	static void RequestAnimationUpdate(
		struct RevAnimationUpdateData& animationUpdateData,
		struct RevAnimationInstanceData* animationInstance);

	static void CreateNormalModelGeometry(
		struct RevNormalModelInitializationData& baseData,
		struct RevModelData* outData);

	static void CreateAnimatedModelGeometry(
		struct RevAnimatedNodelInitializationData& animatedData,
		struct RevModelData* outData);

	static void LoadTexture(const char* path, struct ID3D12Resource** resourceToEndUpAt);

	static ID3D12Resource* CreateResourceTexture(
			UINT width, 
			UINT height, 
			UINT64 format, 
			UINT64 clearValue);

};