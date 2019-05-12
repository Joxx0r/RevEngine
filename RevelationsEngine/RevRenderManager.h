#pragma once

class RevRenderManager
{
public:
	void Initialize();

	void DrawFrame(
		struct RevModelFrameRender& renderEntry);
	void CopyFinalResultToBackBuffer(ID3D12Resource* outResource);

	class RevRendererScene* m_scene = nullptr;

	struct RevDescriptorRuntimeData* m_heapData = nullptr;
};