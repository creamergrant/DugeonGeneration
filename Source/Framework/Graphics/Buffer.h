#pragma once

#include "GraphicTypes.h"
#include "../Math/Vector2.h"
#include <string>
#include <vector>

namespace GameDev2D
{
	struct BufferElement
	{
		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		unsigned int GetComponentCount() const;

		std::string name;
		ShaderDataType type;
		unsigned int size;
		size_t offset;
		bool normalized;
	};

	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		unsigned int GetStride() const;
		const std::vector<BufferElement>& GetElements() const;

		std::vector<BufferElement>::iterator begin();
		std::vector<BufferElement>::iterator end();
		std::vector<BufferElement>::const_iterator begin() const;
		std::vector<BufferElement>::const_iterator end() const;

	private:
		void CalculateOffsetsAndStride();

		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size);
		VertexBuffer(float* vertices, unsigned int size);

		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, unsigned int size);

		const BufferLayout& GetLayout() const;
		void SetLayout(const BufferLayout& layout);

	private:
		unsigned int m_ID;
		BufferLayout m_Layout;
	};

	// Currently only 32-bit index buffers supportted
	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned int* indices, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount();

	private:
		unsigned int m_ID;
		unsigned int m_Count;
	};
}