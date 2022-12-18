#pragma once

#include "SandTable/Render/Buffer/Buffer.h"
#include "SandTable/Render/Buffer/VertexBufferLayout.h"

namespace SandTable
{
	class VertexBuffer :public Buffer
	{
	public:
		virtual ~VertexBuffer() = default;
		const std::vector<float>& GetVertex()const;
		const std::shared_ptr<VertexBufferLayout>& GetVertexBufferLayout()const;

		static std::shared_ptr<Buffer> Create(const std::vector<float>& vecVertex,
			const std::shared_ptr<VertexBufferLayout>& spVertexBufferLayout);

	protected:
		VertexBuffer(const std::vector<float>& vecVertex,
			const std::shared_ptr<VertexBufferLayout>& spVertexBufferLayout);
	private:
		VertexBuffer() = delete;
		VertexBuffer(VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

	protected:
		std::vector<float> m_vecVertex;
		std::shared_ptr<VertexBufferLayout> m_spVertexBufferLayout;
	};
}


