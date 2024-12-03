#include "PointRenderer.h"
#include "../Application/Application.h"


namespace GameDev2D
{
	PointRenderer::PointRenderer() :
		m_PointVertexArray(nullptr),
		m_PointVertexBuffer(nullptr),
		m_PointShader(nullptr),
		m_PointIndexCount(0),
		m_PointVertexBufferBase(nullptr),
		m_PointVertexBufferPtr(nullptr)
	{
		m_PointVertexArray = std::make_shared<VertexArray>();

		m_PointVertexBuffer = std::make_shared<VertexBuffer>(kMaxVertices * sizeof(PointVertex));
		m_PointVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
			});

		m_PointVertexArray->AddVertexBuffer(m_PointVertexBuffer);

		m_PointVertexBufferBase = new PointVertex[kMaxVertices];
		unsigned int* pointIndices = new unsigned int[kMaxIndices];
		for (unsigned int i = 0; i < kMaxIndices; i++)
		{
			pointIndices[i] = i;
		}

		std::shared_ptr<IndexBuffer> pointIndexBuffer = std::make_shared<IndexBuffer>(pointIndices, kMaxIndices);
		m_PointVertexArray->SetIndexBuffer(pointIndexBuffer);
		delete[] pointIndices;

		m_PointShader = Application::Get().GetResourceManager().GetShaderCache().GetShared(POINT_RENDERER_SHADER_KEY);
	}

	PointRenderer::~PointRenderer()
	{
		if (m_PointVertexBufferBase != nullptr)
		{
			delete[] m_PointVertexBufferBase;
			m_PointVertexBufferBase = nullptr;
		}
	}

	RendererType PointRenderer::GetType()
	{
		return RendererType::Point;
	}

	void PointRenderer::BeginScene(Camera* camera)
	{
		m_PointShader->Bind();
		m_PointShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		m_PointShader->SetFloat2("u_Viewport", (float)camera->GetViewport().width, (float)camera->GetViewport().height);

		m_PointIndexCount = 0;
		m_PointVertexBufferPtr = m_PointVertexBufferBase;
	}

	void PointRenderer::EndScene()
	{
		unsigned int dataSize = (unsigned int)((unsigned char*)m_PointVertexBufferPtr - (unsigned char*)m_PointVertexBufferBase);
		m_PointVertexBuffer->SetData(m_PointVertexBufferBase, dataSize);

		Flush();
	}

	void PointRenderer::Flush()
	{
		if (m_PointIndexCount == 0)
			return;

		m_PointVertexArray->Bind();
		Application::Get().GetGraphics().DrawElements(RenderMode::Points, m_PointVertexArray, m_PointIndexCount);
		m_TotalDrawCalls++;
		m_FrameDrawCalls++;
	}

	void PointRenderer::FlushAndReset()
	{
		EndScene();

		m_PointIndexCount = 0;
		m_PointVertexBufferPtr = m_PointVertexBufferBase;
	}

	void PointRenderer::RenderPoint(const Vector2& position, const Color& color)
	{
		if (m_PointIndexCount >= kMaxIndices)
		{
			FlushAndReset();
		}

		m_PointVertexBufferPtr->position = position;
		m_PointVertexBufferPtr->color = color;
		m_PointVertexBufferPtr++;

		m_PointIndexCount ++;
	}
}