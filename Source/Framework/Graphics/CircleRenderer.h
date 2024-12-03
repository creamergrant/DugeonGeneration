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

	class CircleRenderer : public IRenderer
	{
	public:
		CircleRenderer();
		~CircleRenderer();

		RendererType GetType() override;

		void BeginScene(Camera* camera) override;
		void EndScene() override;

		void RenderCircle(const Vector2& position, float radius, const Color& fillColor, const Color& outlineColor, float outlineSize);
	
	protected:
		friend class BatchRenderer;

		void Flush() override;
		void FlushAndReset() override;

	private:
		struct CircleVertex
		{
			Vector2 vertices;
			Vector2 position;
			float radius;
			float outlineSize;
			Color fillColor;
			Color outlineColor;
		};

		std::shared_ptr<VertexArray> m_CircleVertexArray;
		std::shared_ptr<VertexBuffer> m_CircleVertexBuffer;
		std::shared_ptr<Shader> m_CircleShader;
		unsigned int m_CircleIndexCount;
		CircleVertex* m_CircleVertexBufferBase;
		CircleVertex* m_CircleVertexBufferPtr;
		Vector2 m_CircleVertexPositions[4];

		const unsigned int kMaxCircles = 20000;
		const unsigned int kCircleVertexCount = 4;
		const unsigned int kMaxVertices = kMaxCircles * kCircleVertexCount;
		const unsigned int kCircleIndexCount = 6;
		const unsigned int kMaxIndices = kMaxCircles * kCircleIndexCount;
	};
}