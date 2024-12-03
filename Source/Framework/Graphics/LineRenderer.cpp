#include "LineRenderer.h"
#include "../Application/Application.h"
#include "../Math/Math.h"


namespace GameDev2D
{
	LineRenderer::LineRenderer() :
		m_LineVertexArray(nullptr),
		m_LineVertexBuffer(nullptr),
		m_LineShader(nullptr),
		m_LineIndexCount(0),
		m_LineVertexBufferBase(nullptr),
		m_LineVertexBufferPtr(nullptr)
	{
		m_LineVertexArray = std::make_shared<VertexArray>();

		m_LineVertexBuffer = std::make_shared<VertexBuffer>(kMaxVertices * sizeof(LineVertex));
		m_LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Vertices" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float, "a_LineWidth" },
		});

		m_LineVertexArray->AddVertexBuffer(m_LineVertexBuffer);

		m_LineVertexBufferBase = new LineVertex[kMaxVertices];
		unsigned int* lineIndices = new unsigned int[kMaxIndices];
		for (unsigned int i = 0; i < kMaxIndices; i++)
		{
			lineIndices[i] = i;
		}

		std::shared_ptr<IndexBuffer> quadIB = std::make_shared<IndexBuffer>(lineIndices, kMaxIndices);
		m_LineVertexArray->SetIndexBuffer(quadIB);
		delete[] lineIndices;

		m_LineShader = Application::Get().GetResourceManager().GetShaderCache().GetShared(LINE_RENDERER_SHADER_KEY);
	}

	LineRenderer::~LineRenderer()
	{
		if (m_LineVertexBufferBase != nullptr)
		{
			delete[] m_LineVertexBufferBase;
			m_LineVertexBufferBase = nullptr;
		}
	}

	RendererType LineRenderer::GetType()
	{
		return RendererType::Line;
	}

	void LineRenderer::BeginScene(Camera* camera)
	{
		m_LineShader->Bind();
		m_LineShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		m_LineShader->SetFloat2("u_Viewport", (float)camera->GetViewport().width, (float)camera->GetViewport().height);
	
		m_LineIndexCount = 0;
		m_LineVertexBufferPtr = m_LineVertexBufferBase;
	}

	void LineRenderer::EndScene()
	{
		unsigned int dataSize = (unsigned int)((unsigned char*)m_LineVertexBufferPtr - (unsigned char*)m_LineVertexBufferBase);
		m_LineVertexBuffer->SetData(m_LineVertexBufferBase, dataSize);

		Flush();
	}

	void LineRenderer::Flush()
	{
		if (m_LineIndexCount == 0)
			return;

		m_LineVertexArray->Bind();
		
		Application::Get().GetGraphics().DrawElements(RenderMode::LineStripAdjacent, m_LineVertexArray, m_LineIndexCount);
		
		m_TotalDrawCalls++;
		m_FrameDrawCalls++;
	}

	void LineRenderer::RenderLine(const Vector2& start, const Vector2& end, const Color& color, float lineWidth)
	{
		RenderLine(LineSegment(start, end), color, lineWidth);
	}

	void LineRenderer::RenderLine(const LineSegment& lineSegment, const Color& color, float lineWidth)
	{
		std::vector<Vector2> vertices;
		vertices.push_back(lineSegment.start);
		vertices.push_back(lineSegment.end);
		RenderLineStrip(vertices, color, lineWidth);
	}

	void LineRenderer::RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth)
	{
		RenderLineStrip( points, color, lineWidth, Vector2(0,0), 0 );
	}

	void LineRenderer::RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth, const Vector2& position, float radians)
	{
		if (m_LineIndexCount >= kMaxIndices)
		{
			FlushAndReset();
		}

		if (points.size() > 1)
		{
			// Rotate and offset the points.
			std::vector<Vector2> transformedPoints;
			for (unsigned int i = 0; i < points.size(); i++)
			{
				transformedPoints.push_back( position + Vector2( points[i].x * cosf(radians) - points[i].y * sinf(radians), points[i].x * sinf(radians) + points[i].y * cosf(radians) ) );
			}

			float windowWidth = static_cast<float>(Application::Get().GetWindow().GetWidth());
			float windowHeight = static_cast<float>(Application::Get().GetWindow().GetHeight());

			//First, add an adjacency vertex at the beginning
			m_LineVertexBufferPtr->vertices = transformedPoints[0] + (transformedPoints[0] - transformedPoints[1]).Normalized() * (Vector2(windowWidth, windowHeight) * 10.0f);
			m_LineVertexBufferPtr->color = color;
			m_LineVertexBufferPtr->lineWidth = lineWidth;
			m_LineVertexBufferPtr++;

			for (unsigned int i = 0; i < transformedPoints.size(); i++)
			{
				m_LineVertexBufferPtr->vertices = transformedPoints.at(i);
				m_LineVertexBufferPtr->color = color;
				m_LineVertexBufferPtr->lineWidth = lineWidth;
				m_LineVertexBufferPtr++;
			}

			//Next, add an adjacency vertex at the end
			unsigned int n = transformedPoints.size();
			m_LineVertexBufferPtr->vertices = transformedPoints[n - 1] + (transformedPoints[n - 1] - transformedPoints[n - 2]).Normalized() * ((windowWidth, windowHeight) * 10.0f);
			m_LineVertexBufferPtr->color = color;
			m_LineVertexBufferPtr->lineWidth = lineWidth;
			m_LineVertexBufferPtr++;

			m_LineIndexCount += transformedPoints.size() + 2;
		}
	}

	void LineRenderer::FlushAndReset()
	{
		EndScene();

		m_LineIndexCount = 0;
		m_LineVertexBufferPtr = m_LineVertexBufferBase;
	}
}