#pragma once

#include "IRenderer.h"
#include "Buffer.h"
#include "Color.h"
#include "Shader.h"
#include "VertexArray.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"
#include <array>


namespace GameDev2D
{
	class Camera;

	class PointRenderer : public IRenderer
	{
	public:
		PointRenderer();
		~PointRenderer();

		RendererType GetType() override;

		void BeginScene(Camera* camera) override;
		void EndScene() override;

		void RenderPoint(const Vector2& position, const Color& color);

	protected:
		friend class BatchRenderer;
		void Flush() override;
		void FlushAndReset() override;

	private:
		struct PointVertex
		{
			Vector2 position;
			Color color;
		};

		std::shared_ptr<VertexArray> m_PointVertexArray;
		std::shared_ptr<VertexBuffer> m_PointVertexBuffer;
		std::shared_ptr<Shader> m_PointShader;
		unsigned int m_PointIndexCount;
		PointVertex* m_PointVertexBufferBase;
		PointVertex* m_PointVertexBufferPtr;

		const unsigned int kMaxPoints = 10000;
		const unsigned int kMaxVertices = kMaxPoints;
		const unsigned int kMaxIndices = kMaxPoints;
	};
}