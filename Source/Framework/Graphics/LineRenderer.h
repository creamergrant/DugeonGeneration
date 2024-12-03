#pragma once

#include "IRenderer.h"
#include "Buffer.h"
#include "Color.h"
#include "Shader.h"
#include "VertexArray.h"
#include "../Math/Vector2.h"
#include "../Math/LineSegment.h"
#include "../Math/Matrix.h"
#include <array>


namespace GameDev2D
{
	class Camera;

	class LineRenderer : public IRenderer
	{
	public:
		LineRenderer();
		~LineRenderer();

		RendererType GetType() override;

		void BeginScene(Camera* camera) override;
		void EndScene() override;

		void RenderLine(const Vector2& start, const Vector2& end, const Color& color, float lineWidth);
		void RenderLine(const LineSegment& lineSegment, const Color& color, float lineWidth);
		void RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth);
		void RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth, const Vector2& position, float radians);

	protected:
		friend class BatchRenderer;
		void Flush() override;
		void FlushAndReset() override;

	private:
		struct LineVertex
		{
			LineVertex() :
				vertices(Vector2::Zero),
				color(1.0f, 1.0f, 1.0f, 1.0f),
				lineWidth(1.0f)
			{}

			Vector2 vertices;
			Color color;
			float lineWidth;
		};


		std::shared_ptr<VertexArray> m_LineVertexArray;
		std::shared_ptr<VertexBuffer> m_LineVertexBuffer;
		std::shared_ptr<Shader> m_LineShader;
		unsigned int m_LineIndexCount;
		LineVertex* m_LineVertexBufferBase;
		LineVertex* m_LineVertexBufferPtr;

		const unsigned int kMaxLines = 20000;
		const unsigned int kMaxVertices = kMaxLines * 2;
		const unsigned int kMaxIndices = kMaxLines * 2;
	};
}