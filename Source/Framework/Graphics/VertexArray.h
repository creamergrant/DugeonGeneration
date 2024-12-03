#pragma once

#include "Buffer.h"
#include <memory>


namespace GameDev2D
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const;
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

	private:
		unsigned int m_ID;
		unsigned int m_VertexBufferIndex;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}