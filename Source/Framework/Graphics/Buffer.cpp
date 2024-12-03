#include "Buffer.h"
#include "Graphics.h"
#include "../Application/Application.h"


namespace GameDev2D
{
	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized)
	{
	}

	unsigned int BufferElement::GetComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::Float:   return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 3;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Mat3:    return 3; // 3* float3
		case ShaderDataType::Mat4:    return 4; // 4* float4
		case ShaderDataType::Int:     return 1;
		case ShaderDataType::Int2:    return 2;
		case ShaderDataType::Int3:    return 3;
		case ShaderDataType::Int4:    return 4;
		case ShaderDataType::Bool:    return 1;
		}

		return 0;
	}

	BufferLayout::BufferLayout() {}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	unsigned int BufferLayout::GetStride() const { return m_Stride; }
	const std::vector<BufferElement>& BufferLayout::GetElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator BufferLayout::begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator BufferLayout::end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator BufferLayout::begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator BufferLayout::end() const { return m_Elements.end(); }

	void BufferLayout::CalculateOffsetsAndStride()
	{
		size_t offset = 0;
		m_Stride = 0;
		for (BufferElement& element : m_Elements)
		{
			element.offset = offset;
			offset += element.size;
			m_Stride += element.size;
		}
	}

	VertexBuffer::VertexBuffer(unsigned int size) :
		m_ID(0)
	{
		Application::Get().GetGraphics().CreateBuffer(&m_ID);
		Application::Get().GetGraphics().BindBuffer(BufferType::Vertex, m_ID);
		Application::Get().GetGraphics().InitBuffer(BufferType::Vertex, size, nullptr, BufferUsage::DynamicDraw);
	}

	VertexBuffer::VertexBuffer(float* vertices, unsigned int size) :
		m_ID(0)
	{
		Application::Get().GetGraphics().CreateBuffer(&m_ID);
		Application::Get().GetGraphics().BindBuffer(BufferType::Vertex, m_ID);
		Application::Get().GetGraphics().InitBuffer(BufferType::Vertex, size, vertices, BufferUsage::StaticDraw);
	}

	VertexBuffer::~VertexBuffer()
	{
		Application::Get().GetGraphics().DeleteBuffer(&m_ID);
	}

	void VertexBuffer::Bind() const
	{
		Application::Get().GetGraphics().BindBuffer(BufferType::Vertex, m_ID);
	}

	void VertexBuffer::Unbind() const
	{
		Application::Get().GetGraphics().BindBuffer(BufferType::Vertex, 0);
	}

	void VertexBuffer::SetData(const void* data, unsigned int size)
	{
		Application::Get().GetGraphics().BindBuffer(BufferType::Vertex, m_ID);
		Application::Get().GetGraphics().UpdateBuffer(BufferType::Vertex, 0, size, data);
	}

	const BufferLayout& VertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count) :
		m_ID(0),
		m_Count(count)
	{
		Application::Get().GetGraphics().CreateBuffer(&m_ID);
		Application::Get().GetGraphics().BindBuffer(BufferType::Vertex, m_ID);
		Application::Get().GetGraphics().InitBuffer(BufferType::Vertex, count * sizeof(unsigned int), indices, BufferUsage::StaticDraw);
	}

	IndexBuffer::~IndexBuffer()
	{
		Application::Get().GetGraphics().DeleteBuffer(&m_ID);
	}

	void IndexBuffer::Bind() const
	{
		Application::Get().GetGraphics().BindBuffer(BufferType::Index, m_ID);
	}

	void IndexBuffer::Unbind() const
	{
		Application::Get().GetGraphics().BindBuffer(BufferType::Index, 0);
	}

	unsigned int IndexBuffer::GetCount()
	{
		return m_Count;
	}
}