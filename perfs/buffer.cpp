#include "buffer.hpp"
#include <memory>
#include <cassert>

namespace
{
	std::size_t const CHUNK_SIZE(65536);
	std::size_t const SLICE_SIZE(67108864);
	std::size_t const PAGE_SIZE(16384);

	GLenum make_usage(std::uint32_t Flags)
	{

	}

	class manager
	{
	public:

	private:

	};

	static std::unique_ptr<manager> Manager;


	bool const hasImmutableStorage = true;
}//namespace

namespace gl
{
	buffer::buffer(std::uint32_t Flags, std::size_t Size) :
		buffer(Flags, Size, nullptr)
	{}

	buffer::buffer(std::uint32_t Flags, std::size_t Size, void const * Data) :
		Size(Size),
		Flags(Flags)
	{
		glGenBuffers(1, &this->Handle);

		if(hasImmutableStorage)
			glNamedBufferStorageEXT(this->Handle, static_cast<GLsizeiptr>(Size), Data, Flags);
		else
			glNamedBufferDataEXT(this->Handle, static_cast<GLsizeiptr>(Size), Data, ::make_usage(Flags));
	}

	buffer::~buffer()
	{
		glDeleteBuffers(1, &this->Handle);
		this->Handle = 0;
	}

	void* buffer::map(std::uintptr_t Offset, std::size_t Size, std::uint32_t Flags)
	{

	}

	void buffer::unmap(void* memory)
	{

	}

	void buffer::flush(std::uintptr_t Offset, std::size_t Size)
	{
		assert(Size <= this->Size);

		glFlushMappedNamedBufferRangeEXT(this->Handle, static_cast<GLintptr>(Offset), static_cast<GLsizeiptr>(Size));
	}

	void buffer::barrier()
	{
		glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
	}

	void buffer::copy(buffer const & Buffer, std::uintptr_t ReadOffset, std::uintptr_t WriteOffset, std::size_t Size)
	{
		assert(Buffer.Flags & READ_BIT);

		glNamedCopyBufferSubDataEXT(
			Buffer.Handle, this->Handle, 
			static_cast<GLintptr>(ReadOffset), static_cast<GLintptr>(WriteOffset), static_cast<GLsizeiptr>(Size));
	}
}//namespace gl

