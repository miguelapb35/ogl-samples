///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2013 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/core/load_dds.inl
/// @date 2010-09-26 / 2013-01-28
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include "dx.hpp"
#include <fstream>
#include <cassert>

namespace gli{
namespace detail
{
	// DDS Documentation
	/*
		http://msdn.microsoft.com/en-us/library/bb943991(VS.85).aspx#File_Layout1
		http://msdn.microsoft.com/en-us/library/bb943992.aspx
	*/

	enum ddsCubemapflag
	{
		DDSCAPS2_CUBEMAP				= 0x00000200,
		DDSCAPS2_CUBEMAP_POSITIVEX		= 0x00000400,
		DDSCAPS2_CUBEMAP_NEGATIVEX		= 0x00000800,
		DDSCAPS2_CUBEMAP_POSITIVEY		= 0x00001000,
		DDSCAPS2_CUBEMAP_NEGATIVEY		= 0x00002000,
		DDSCAPS2_CUBEMAP_POSITIVEZ		= 0x00004000,
		DDSCAPS2_CUBEMAP_NEGATIVEZ		= 0x00008000,
		DDSCAPS2_VOLUME					= 0x00200000
	};

	glm::uint32 const DDSCAPS2_CUBEMAP_ALLFACES = (
		DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX |
		DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY |
		DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ);

	enum ddsFlag
	{
		DDSD_CAPS			= 0x00000001,
		DDSD_HEIGHT			= 0x00000002,
		DDSD_WIDTH			= 0x00000004,
		DDSD_PITCH			= 0x00000008,
		DDSD_PIXELFORMAT	= 0x00001000,
		DDSD_MIPMAPCOUNT	= 0x00020000,
		DDSD_LINEARSIZE		= 0x00080000,
		DDSD_DEPTH			= 0x00800000
	};

	enum ddsSurfaceflag
	{
		DDSCAPS_COMPLEX				= 0x00000008,
		DDSCAPS_MIPMAP				= 0x00400000,
		DDSCAPS_TEXTURE				= 0x00001000
	};

	struct ddsPixelFormat
	{
		glm::uint32 size; // 32
		glm::uint32 flags;
		glm::uint32 fourCC;
		glm::uint32 bpp;
		glm::uint32 redMask;
		glm::uint32 greenMask;
		glm::uint32 blueMask;
		glm::uint32 alphaMask;
	};

	struct ddsHeader
	{
		glm::uint32 size;
		glm::uint32 flags;
		glm::uint32 height;
		glm::uint32 width;
		glm::uint32 pitch;
		glm::uint32 depth;
		glm::uint32 mipMapLevels;
		glm::uint32 reserved1[11];
		ddsPixelFormat format;
		glm::uint32 surfaceFlags;
		glm::uint32 cubemapFlags;
		glm::uint32 reserved2[3];
	};

	enum D3D10_RESOURCE_DIMENSION 
	{
		D3D10_RESOURCE_DIMENSION_UNKNOWN     = 0,
		D3D10_RESOURCE_DIMENSION_BUFFER      = 1,
		D3D10_RESOURCE_DIMENSION_TEXTURE1D   = 2,
		D3D10_RESOURCE_DIMENSION_TEXTURE2D   = 3,
		D3D10_RESOURCE_DIMENSION_TEXTURE3D   = 4 
	};

	enum D3D10_RESOURCE_MISC_FLAG 
	{
		D3D10_RESOURCE_MISC_GENERATE_MIPS       = 0x1L,
		D3D10_RESOURCE_MISC_SHARED              = 0x2L,
		D3D10_RESOURCE_MISC_TEXTURECUBE         = 0x4L,
		D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX   = 0x10L,
		D3D10_RESOURCE_MISC_GDI_COMPATIBLE      = 0x20L 
	};

	struct ddsHeader10
	{
		ddsHeader10() :
			Format(dx::DXGI_FORMAT_UNKNOWN),
			resourceDimension(D3D10_RESOURCE_DIMENSION_UNKNOWN),
			miscFlag(0),
			arraySize(1),
			reserved(0)
		{}

		gli::dx::dxgiFormat			Format;
		D3D10_RESOURCE_DIMENSION	resourceDimension;
		glm::uint32					miscFlag; // D3D10_RESOURCE_MISC_GENERATE_MIPS
		glm::uint32					arraySize;
		glm::uint32					reserved;
	};

	inline gli::format format_fourcc2gli_cast(glm::uint32 const & Flags, glm::uint32 const & FourCC)
	{
		gli::dx DX;

		if(Flags & dx::DDPF_ALPHAPIXELS && FourCC == dx::D3DFMT_DXT1)
			return Flags & dx::DDPF_ALPHAPIXELS ? FORMAT_RGBA_BC1_UNORM : FORMAT_RGB_BC1_UNORM;

		for(int FormatIndex = 0; FormatIndex < FORMAT_COUNT; ++FormatIndex)
		{
			gli::dx::format const & FormatInfo = DX.translate(static_cast<format>(FormatIndex));

			if(FormatInfo.D3DFormat == FourCC)
				return static_cast<format>(FormatIndex);
		}

		return FORMAT_INVALID;
	}

	inline gli::format format_dds2gli_cast(dx::dxgiFormat const & Format)
	{
		gli::dx DX;

		for(int FormatIndex = 0; FormatIndex < FORMAT_COUNT; ++FormatIndex)
		{
			gli::dx::format const & FormatInfo = DX.translate(static_cast<format>(FormatIndex));

			if(FormatInfo.DXGIFormat == Format)
				return static_cast<format>(FormatIndex);
		}

		return FORMAT_INVALID;
	}
}//namespace detail


/* Taken from http://stackoverflow.com/a/13059195 */
struct membuf : std::streambuf 
{
	membuf(char const* base, size_t size)
	{
		char* p(const_cast<char*>(base));
		this->setg(p, p, p + size);
	}
};

struct imemstream : virtual membuf, std::istream
{
	imemstream(char const* base, size_t size) : 
		membuf(base, size),
		std::istream(static_cast<std::streambuf*>(this))
	{

	}
};


inline storage load_dds(const char* pBuffer, size_t size)
{
	assert(pBuffer != nullptr && size > 0);

	imemstream memstream(pBuffer, size);

	return load_dds(memstream);
}


inline storage load_dds(char const * Filename)
{
	std::ifstream FileIn(Filename, std::ios::in | std::ios::binary);
	assert(!FileIn.fail());

	if(FileIn.fail())
		return storage();

	return load_dds(FileIn);
}


inline storage load_dds(std::istream& FileIn)
{
	detail::ddsHeader HeaderDesc;
	detail::ddsHeader10 HeaderDesc10;
	char Magic[4]; 

	//* Read magic number and check if valid .dds file 
	FileIn.read((char*)&Magic, sizeof(Magic));

	assert(strncmp(Magic, "DDS ", 4) == 0);

	// Get the surface descriptor 
	FileIn.read((char*)&HeaderDesc, sizeof(HeaderDesc));
	if(HeaderDesc.format.flags & dx::DDPF_FOURCC && HeaderDesc.format.fourCC == dx::D3DFMT_DX10)
		FileIn.read((char*)&HeaderDesc10, sizeof(HeaderDesc10));

	gli::format Format(gli::FORMAT_INVALID);
	if(HeaderDesc.format.fourCC == dx::D3DFMT_DX10 && HeaderDesc10.Format != dx::DXGI_FORMAT_UNKNOWN)
		Format = detail::format_dds2gli_cast(HeaderDesc10.Format);
	
	if(HeaderDesc.format.flags & dx::DDPF_FOURCC && Format == FORMAT_INVALID)
		Format = detail::format_fourcc2gli_cast(HeaderDesc.format.flags, HeaderDesc.format.fourCC);
	
	if(HeaderDesc.format.flags & dx::DDPF_RGB && Format == FORMAT_INVALID)
	{
		switch(HeaderDesc.format.bpp)
		{
		case 8:
			Format = FORMAT_R8_UNORM;
			break;
		case 16:
			Format = FORMAT_RG8_UNORM;
			break;
		case 24:
			Format = FORMAT_RGB8_UNORM;
			break;
		case 32:
			Format = FORMAT_RGBA8_UNORM;
			break;
		}
	}

	std::streamoff Curr = FileIn.tellg();
	FileIn.seekg(0, std::ios_base::end);
	std::streamoff End = FileIn.tellg();
	FileIn.seekg(Curr, std::ios_base::beg);

	storage::size_type const MipMapCount = (HeaderDesc.flags & detail::DDSD_MIPMAPCOUNT) ? 
		HeaderDesc.mipMapLevels : 1;

	storage::size_type FaceCount(1);
	if(HeaderDesc.cubemapFlags & detail::DDSCAPS2_CUBEMAP)
		FaceCount = int(glm::bitCount(HeaderDesc.cubemapFlags & detail::DDSCAPS2_CUBEMAP_ALLFACES));

	storage::size_type DepthCount = 1;
	if(HeaderDesc.cubemapFlags & detail::DDSCAPS2_VOLUME)
			DepthCount = HeaderDesc.depth;

	storage Storage(
		HeaderDesc10.arraySize,
		FaceCount,
		MipMapCount,
		Format,
		storage::dim_type(HeaderDesc.width, HeaderDesc.height, DepthCount));

	FileIn.read((char*)Storage.data(), std::size_t(End - Curr));

	detail::formatInfo const & Info = detail::getInfo(Storage.format());

	if(HeaderDesc.format.fourCC != dx::D3DFMT_DX10 && !(Info.Flags & FORMAT_COMPRESSED_BIT) && Info.Component >= 3)
	{
		switch(Info.Component)
		{
		default:
			assert(0);
			break;
		case 3:
			for(std::size_t Offset = 0; Offset < Storage.size() / Info.Component; ++Offset)
			{
				glm::u8vec3 Src = *(reinterpret_cast<glm::u8vec3 const *>(Storage.data()) + Offset);
				*(reinterpret_cast<glm::u8vec3*>(Storage.data()) + Offset) = glm::u8vec3(Src.z, Src.y, Src.x);
			}
			break;
		case 4:
			for(std::size_t Offset = 0; Offset < Storage.size() / Info.Component; ++Offset)
			{
				glm::u8vec4 Src = *(reinterpret_cast<glm::u8vec4 const *>(Storage.data()) + Offset);
				*(reinterpret_cast<glm::u8vec4*>(Storage.data()) + Offset) = glm::u8vec4(Src.z, Src.y, Src.x, Src.w);
			}
			break;
		}
	}

	return Storage;
}

}//namespace gli
