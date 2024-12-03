#pragma once


namespace GameDev2D
{
	enum class RendererType
	{
		Sprite = 0,
		Line,
		Point,
		Circle
	};

	class Camera;

	class IRenderer
	{
	public:
		IRenderer();
		virtual ~IRenderer() = default;

		virtual RendererType GetType() = 0;

		virtual void BeginScene(Camera* camera) = 0;
		virtual void EndScene() = 0;

		unsigned long long GetTotalDrawCalls();
		unsigned long long GetLastFrameDrawCalls();

	protected:
		virtual void Flush() = 0;
		virtual void FlushAndReset() = 0;
		void NewFrame();

		unsigned long long m_TotalDrawCalls;
		unsigned long long m_FrameDrawCalls;
		unsigned long long m_LastFrameDrawCalls;
	};
}
