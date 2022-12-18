#pragma once
#include "SandTable/Render/Buffer/Buffer.h"

namespace SandTable
{
	class IndexBuffer :public Buffer
	{
	public:
		~IndexBuffer() = default;
		static std::shared_ptr<Buffer> Create(const std::vector<int>& vecIndex);
		const std::vector<int> GetIndex()const;

	private:
		IndexBuffer() = delete;
		IndexBuffer(IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

	protected:
		IndexBuffer(const std::vector<int>& vecIndex);

	protected:
		std::vector<int> m_vecIndex;
	};
}