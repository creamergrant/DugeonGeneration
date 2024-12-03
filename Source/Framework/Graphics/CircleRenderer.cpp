#include "CircleRenderer.h"
#include "../Application/Application.h"


namespace GameDev2D
{
	CircleRenderer::CircleRenderer() :
		m_CircleVertexArray(nullptr),
		m_CircleVertexBuffer(nullptr),
		m_CircleShader(nullptr),
		m_CircleIndexCount(0),
		m_CircleVertexBufferBase(nullptr),
		m_CircleVertexBufferPtr(nullptr)
	{
		m_CircleVertexArray = std::make_shared<VertexArray>();

		m_CircleVertexBuffer = std::make_shared<VertexBuffer>(kMaxVertices * sizeof(CircleVertex));
		m_CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Vertices" },
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float, "a_Radius" },
			{ ShaderDataType::Float, "a_OutlineSize" },
			{ ShaderDataType::Float4, "a_FillColor" },
			{ ShaderDataType::Float4, "a_OutlineColor" }
		});

		m_CircleVertexArray->AddVertexBuffer(m_CircleVertexBuffer);

		m_CircleVertexBufferBase = new CircleVertex[kMaxVertices];
		unsigned int* circleIndices = new unsigned int[kMaxIndices];

		unsigned int offset = 0;
		for (unsigned int i = 0; i < kMaxIndices; i += kCircleIndexCount)
		{
			circleIndices[i + 0] = offset + 0;
			circleIndices[i + 1] = offset + 1;
			circleIndices[i + 2] = offset + 2;

			circleIndices[i + 3] = offset + 2;
			circleIndices[i + 4] = offset + 3;
			circleIndices[i + 5] = offset + 0;

			offset += kCircleVertexCount;
		}

		std::shared_ptr<IndexBuffer> circleIndexBuffer = std::make_shared<IndexBuffer>(circleIndices, kMaxIndices);
		m_CircleVertexArray->SetIndexBuffer(circleIndexBuffer);
		delete[] circleIndices;

		m_CircleShader = Application::Get().GetResourceManager().GetShaderCache().GetShared(CIRCLE_RENDERER_SHADER_KEY);
	
		m_CircleVertexPositions[0] = { 0.0f, 0.0f };
		m_CircleVertexPositions[1] = { 1.0f, 0.0f };
		m_CircleVertexPositions[2] = { 1.0f, 1.0f };
		m_CircleVertexPositions[3] = { 0.0f, 1.0f };
	}

	CircleRenderer::~CircleRenderer()
	{
		if (m_CircleVertexBufferBase != nullptr)
		{
			delete[] m_CircleVertexBufferBase;
			m_CircleVertexBufferBase = nullptr;
		}
	}

	RendererType CircleRenderer::GetType()
	{
		return RendererType::Circle;
	}

	void CircleRenderer::BeginScene(Camera* camera)
	{
		m_CircleShader->Bind();
		m_CircleShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());

		m_CircleIndexCount = 0;
		m_CircleVertexBufferPtr = m_CircleVertexBufferBase;
	}

	void CircleRenderer::EndScene()
	{
		unsigned int dataSize = (unsigned int)((unsigned char*)m_CircleVertexBufferPtr - (unsigned char*)m_CircleVertexBufferBase);
		m_CircleVertexBuffer->SetData(m_CircleVertexBufferBase, dataSize);

		Flush();
	}

	void CircleRenderer::Flush()
	{
		if (m_CircleIndexCount == 0)
			return;

		m_CircleVertexArray->Bind();

		Application::Get().GetGraphics().DrawElements(RenderMode::Triangles, m_CircleVertexArray, m_CircleIndexCount);

		m_TotalDrawCalls++;
		m_FrameDrawCalls++;
	}

	void CircleRenderer::FlushAndReset()
	{
		EndScene();

		m_CircleIndexCount = 0;
		m_CircleVertexBufferPtr = m_CircleVertexBufferBase;
	}

	void CircleRenderer::RenderCircle(const Vector2& position, float radius, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		if (m_CircleIndexCount + kCircleIndexCount >= kMaxIndices)
		{
			FlushAndReset();
		}

		Matrix transform = Matrix::MakeTranslation(position);
		Vector2 size((radius + outlineSize) * 2.0f, (radius + outlineSize) * 2.0f);
		Vector2 offset(size.x * 0.5f, size.y * 0.5f);

		for (unsigned int i = 0; i < kCircleVertexCount; i++)
		{
			m_CircleVertexBufferPtr->vertices = transform * ((m_CircleVertexPositions[i] * size) - offset);
			m_CircleVertexBufferPtr->position = position;
			m_CircleVertexBufferPtr->radius = radius;
			m_CircleVertexBufferPtr->outlineSize = outlineSize;
			m_CircleVertexBufferPtr->fillColor = fillColor;
			m_CircleVertexBufferPtr->outlineColor = outlineColor;
			m_CircleVertexBufferPtr++;
		}

		m_CircleIndexCount += kCircleIndexCount;
	}
}