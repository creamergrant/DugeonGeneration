#include "VertexArray.h"
#include "Graphics.h"
#include "../Application/Application.h"
#include <assert.h>


namespace GameDev2D
{
	VertexArray::VertexArray() :
		m_ID(0),
		m_VertexBufferIndex(0)
	{
		Application::Get().GetGraphics().CreateVertexArray(&m_ID);
	}

	VertexArray::~VertexArray()
	{
		Application::Get().GetGraphics().DeleteVertexArray(&m_ID);
	}

	void VertexArray::Bind()
	{
		Application::Get().GetGraphics().BindVertexArray(m_ID);
	}

	void VertexArray::Unbind()
	{
		Application::Get().GetGraphics().BindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		//Vertex Buffer has no layout!
		assert(vertexBuffer->GetLayout().GetElements().size());

		Application::Get().GetGraphics().BindVertexArray(m_ID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				Application::Get().GetGraphics().EnableVertexAttributeArray(m_VertexBufferIndex);
				Application::Get().GetGraphics().SetVertexAttributePointer(m_VertexBufferIndex, element, layout.GetStride(), (const void*)element.offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				unsigned char count = element.GetComponentCount();
				for (unsigned char i = 0; i < count; i++)
				{
					Application::Get().GetGraphics().EnableVertexAttributeArray(m_VertexBufferIndex);
					Application::Get().GetGraphics().SetVertexAttributePointer(m_VertexBufferIndex, element, layout.GetStride(), (const void*)(element.offset + sizeof(float) * count * i));
					Application::Get().GetGraphics().SetVertexAttributeDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
		Application::Get().GetGraphics().BindVertexArray(m_ID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& VertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& VertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}