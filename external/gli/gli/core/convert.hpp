///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2015 G-Truc Creation (www.g-truc.net)
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
/// @file gli/core/convert.hpp
/// @date 2015-10-08 / 2015-10-08
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include "../type.hpp"
#include "../texture2d.hpp"
#include <limits>

namespace gli{
namespace detail
{
	template <typename floatType, glm::precision P, gli::format Format = FORMAT_UNDEFINED>
	struct texelFetch {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(0.0f, 0.0f, 0.0f, 1.0f);
		}
	};

	// Packed
	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_UNORM> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackUnorm3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_SNORM> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackSnorm3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_USCALED> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackU3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB10A2_SSCALED> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackI3x10_1x2(Texture.load<glm::uint32>(TexelCoord, Level)));
		}
	};

	// Half
	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_R16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackHalf1x16(Texture.load<glm::uint16>(TexelCoord, Level)), 0.0f, 0.0f, 1.0f);
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RG16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackHalf2x16(Texture.load<glm::uint16>(TexelCoord, Level)), 0.0f, 1.0f);
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGB16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			glm::u16vec3 const Packed = Texture.load<glm::u16vec3>(TexelCoord, Level);
			return glm::tvec4<floatType, P>(glm::unpackHalf1x16(Packed.x), glm::unpackHalf1x16(Packed.y), glm::unpackHalf1x16(Packed.z), 1.0f);
		}
	};

	template <typename floatType, glm::precision P>
	struct texelFetch<floatType, P, FORMAT_RGBA16_SFLOAT> {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::unpackHalf4x16(Texture.load<glm::uint64>(TexelCoord, Level)));
		}
	};

	// Normalize
	template <typename T, glm::precision P, typename valType, bool isFloat = false>
	struct texelFetchNorm1 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P, typename valType>
	struct texelFetchNorm1<T, P, valType, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::compNormalize<T>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level)).x, 0.0f, 0.0f, 1.0f);
		}
	};

	template <typename T, glm::precision P, typename valType, bool isFloat = false>
	struct texelFetchNorm2 {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(0, 0, 0, 1);
		}
	};

	template <typename T, glm::precision P, typename valType>
	struct texelFetchNorm2<T, P, valType, true> {
		static glm::tvec4<T, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<T, P>(glm::compNormalize<T>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), 0.0f, 1.0f);
		}
	};

	// Cast
	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast1 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(static_cast<floatType>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level).x), static_cast<floatType>(0), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast2 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::tvec2<floatType, P>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast3 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::tvec3<floatType, P>(Texture.load<glm::tvec3<valType, P> >(TexelCoord, Level)), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchCast4 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(Texture.load<glm::tvec4<valType, P> >(TexelCoord, Level));
		}
	};

	// Read
	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead1 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(static_cast<floatType>(Texture.load<glm::tvec1<valType, P> >(TexelCoord, Level).x), static_cast<floatType>(0), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead2 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(glm::tvec2<floatType, P>(Texture.load<glm::tvec2<valType, P> >(TexelCoord, Level)), static_cast<floatType>(0), static_cast<floatType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead3 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(Texture.load<glm::tvec3<valType, P>>(TexelCoord, Level), static_cast<valType>(1));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelFetchRead4 {
		static glm::tvec4<floatType, P> call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level)
		{
			return glm::tvec4<floatType, P>(Texture.load<glm::tvec4<valType, P>>(TexelCoord, Level));
		}
	};

	template <typename floatType, glm::precision P>
	struct texelWriteSRGB4 {
		static void call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, glm::tvec4<floatType, P> const & Texel)
		{
			Texture.store<glm::tvec4<glm::u8, P> >(TexelCoord, Level, glm::tvec4<glm::u8, P>(glm::compScale(glm::convertLinearToSRGB(Texel))));
		}
	};

	template <typename floatType, glm::precision P, typename valType>
	struct texelWriteRGBA4 {
		static void call(texture2D const & Texture, texture2D::dim_type const & TexelCoord, texture2D::size_type Level, tvec4<floatType, P> const & Texel)
		{
			Texture.store<tvec4<valType, P> >(TexelCoord, Level, tvec4<valType, P>(glm::compScale(Texel)));
		}
	};
}//namespace detail

namespace detail
{
	enum convertMode
	{
		CONVERT_MODE_DEFAULT,
		CONVERT_MODE_CAST,
		CONVERT_MODE_NORM,
		CONVERT_MODE_SRGB,
		CONVERT_MODE_RGB9E5,
		CONVERT_MODE_RG11B10F,
		CONVERT_MODE_RGB10A2UNORM,
		CONVERT_MODE_RGB10A2SNORM,
		CONVERT_MODE_RGB10A2UINT,
		CONVERT_MODE_RGB10A2SINT,
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType, convertMode mode = CONVERT_MODE_CAST, bool isSamplerFloat = false>
	struct convertFunc
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			return make_vec4<retType, P>(vecType<retType, P>(Texture.template load<vecType<T, P> >(TexelCoord, Level)));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			Texture.template store<vecType<T, P> >(TexelCoord, Level, vecType<T, P>(Texel));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType, bool isSamplerFloat>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_DEFAULT, isSamplerFloat>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			return tvec4<retType, P>(0, 0, 0, 1);
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_NORM, true>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_NORM requires a float sampler");
			return make_vec4<retType, P>(compNormalize<retType>(Texture.template load<vecType<T, P> >(TexelCoord, Level)));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_NORM requires a float sampler");
			Texture.template store<vecType<T, P> >(TexelCoord, Level, compScale<T>(vecType<retType, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P, template <typename, precision> class vecType>
	struct convertFunc<textureType, retType, T, P, vecType, CONVERT_MODE_SRGB, true>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_SRGB requires a float sampler");
			return make_vec4<retType, P>(convertSRGBToLinear(compNormalize<retType>(Texture.template load<vecType<T, P> >(TexelCoord, Level))));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_SRGB requires a float sampler");
			Texture.template store<vecType<T, P> >(TexelCoord, Level, gli::compScale<T>(convertLinearToSRGB(vecType<retType, P>(Texel))));
		}
	};

	template <typename textureType, typename retType, typename T, precision P>
	struct convertFunc<textureType, retType, T, P, tvec3, CONVERT_MODE_RGB9E5, true>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB9E5 requires a float sampler");
			return tvec4<retType, P>(unpackF3x9_E1x5(Texture.template load<uint32>(TexelCoord, Level)), static_cast<retType>(1));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB9E5 requires a float sampler");
			Texture.template store<uint32>(TexelCoord, Level, packF3x9_E1x5(tvec3<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P>
	struct convertFunc<textureType, retType, T, P, tvec3, CONVERT_MODE_RG11B10F, true>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RG11B10F requires a float sampler");
			return tvec4<retType, P>(unpackF2x11_1x10(Texture.template load<uint32>(TexelCoord, Level)), static_cast<retType>(1));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RG11B10F requires a float sampler");
			Texture.template store<uint32>(TexelCoord, Level, packF2x11_1x10(tvec3<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P>
	struct convertFunc<textureType, retType, T, P, tvec4, CONVERT_MODE_RGB10A2UNORM, true>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2UNORM requires a float sampler");
			return tvec4<retType, P>(unpackUnorm3x10_1x2(Texture.template load<uint32>(TexelCoord, Level)));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2UNORM requires a float sampler");
			Texture.template store<uint32>(TexelCoord, Level, packUnorm3x10_1x2(tvec4<float, P>(Texel)));
		}
	};

	template <typename textureType, typename retType, typename T, precision P>
	struct convertFunc<textureType, retType, T, P, tvec4, CONVERT_MODE_RGB10A2SNORM, true>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2SNORM requires a float sampler");
			return tvec4<retType, P>(unpackSnorm3x10_1x2(Texture.template load<uint32>(TexelCoord, Level)));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_iec559, "CONVERT_MODE_RGB10A2SNORM requires a float sampler");
			Texture.template store<uint32>(TexelCoord, Level, packSnorm3x10_1x2(Texel));
		}
	};

	template <typename textureType, typename retType, typename T, precision P>
	struct convertFunc<textureType, retType, T, P, tvec4, CONVERT_MODE_RGB10A2UINT, false>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2UINT requires an integer sampler");
			return tvec4<retType, P>(unpackU3x10_1x2(Texture.template load<uint32>(TexelCoord, Level)));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2UINT requires an integer sampler");
			Texture.template store<uint32>(TexelCoord, Level, packU3x10_1x2(Texel));
		}
	};

	template <typename textureType, typename retType, typename T, precision P>
	struct convertFunc<textureType, retType, T, P, tvec4, CONVERT_MODE_RGB10A2SINT, false>
	{
		static tvec4<retType, P> fetch(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2SINT requires an integer sampler");
			return tvec4<retType, P>(unpackI3x10_1x2(Texture.template load<uint32>(TexelCoord, Level)));
		}

		static void write(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, tvec4<retType, P> const & Texel)
		{
			static_assert(std::numeric_limits<retType>::is_integer, "CONVERT_MODE_RGB10A2SINT requires an integer sampler");
			Texture.template store<uint32>(TexelCoord, Level, packI3x10_1x2(Texel));
		}
	};

	template <typename textureType, typename samplerValType, precision P>
	struct convert
	{
		typedef glm::tvec4<samplerValType, P>(*convertFetchFunc)(textureType const & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level);
		typedef void(*convertWriteFunc)(textureType & Texture, typename textureType::dim_type const & TexelCoord, typename textureType::size_type Level, glm::tvec4<samplerValType, P> const & Texel);

		struct func
		{
			convertFetchFunc Fetch;
			convertWriteFunc Write;
		};

		static func call(format Format)
		{
			static func Table[] =
			{
				{nullptr, nullptr},			// FORMAT_RG4_UNORM
				{nullptr, nullptr},			// FORMAT_RG4_USCALED
				{nullptr, nullptr},			// FORMAT_RGBA4_UNORM
				{nullptr, nullptr},			// FORMAT_RGBA4_USCALED
				{nullptr, nullptr},			// FORMAT_R5G6B5_UNORM
				{nullptr, nullptr},			// FORMAT_R5G6B5_USCALED
				{nullptr, nullptr},			// FORMAT_RGB5A1_UNORM
				{nullptr, nullptr},			// FORMAT_RGB5A1_USCALED

				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R8_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R8_SNORM
				{detail::texelFetchCast1<samplerValType, P, glm::u8>::call, nullptr},					// FORMAT_R8_USCALED
				{detail::texelFetchCast1<samplerValType, P, glm::i8>::call, nullptr},					// FORMAT_R8_SSCALED
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R8_UINT
				{convertFunc<textureType, samplerValType, i8, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R8_SINT
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R8_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG8_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG8_SNORM
				{detail::texelFetchCast2<samplerValType, P, glm::u8>::call, nullptr},					// FORMAT_RG8_USCALED
				{detail::texelFetchCast2<samplerValType, P, glm::i8>::call, nullptr},					// FORMAT_RG8_SSCALED
				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG8_UINT
				{convertFunc<textureType, samplerValType, i8, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG8_SINT
				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG8_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB8_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB8_SNORM
				{detail::texelFetchCast3<samplerValType, P, glm::u8>::call, nullptr},					// FORMAT_RGB8_USCALED
				{detail::texelFetchCast3<samplerValType, P, glm::i8>::call, nullptr},					// FORMAT_RGB8_SSCALED
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB8_UINT
				{convertFunc<textureType, samplerValType, i8, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB8_SINT
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB8_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA8_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA8_SNORM
				{detail::texelFetchCast4<samplerValType, P, glm::u8>::call, nullptr},										// FORMAT_RGBA8_USCALED
				{detail::texelFetchCast4<samplerValType, P, glm::i8>::call, nullptr},										// FORMAT_RGBA8_SSCALED
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA8_UINT
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA8_SINT
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA8_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UNORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UNORM, std::numeric_limits<samplerValType>::is_iec559>::write},	// FORMAT_RGB10A2_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SNORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SNORM, std::numeric_limits<samplerValType>::is_iec559>::write},	// FORMAT_RGB10A2_SNORM
				{detail::texelFetch<samplerValType, P, FORMAT_RGB10A2_USCALED>::call, nullptr},			// FORMAT_RGB10A2_USCALED
				{detail::texelFetch<samplerValType, P, FORMAT_RGB10A2_SSCALED>::call, nullptr},			// FORMAT_RGB10A2_SSCALED
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UINT, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UINT, std::numeric_limits<samplerValType>::is_iec559>::write},		// FORMAT_RGB10A2_UINT
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SINT, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SINT, std::numeric_limits<samplerValType>::is_iec559>::write},		// FORMAT_RGB10A2_SINT

				{convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R16_UNORM
				{convertFunc<textureType, samplerValType, i16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R16_SNORM
				{detail::texelFetchCast1<samplerValType, P, glm::u16>::call, nullptr},					// FORMAT_R16_USCALED
				{detail::texelFetchCast1<samplerValType, P, glm::i16>::call, nullptr},					// FORMAT_R16_SSCALED
				{convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R16_UINT
				{convertFunc<textureType, samplerValType, i16, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec1, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_R16_SINT
				{detail::texelFetch<samplerValType, P, FORMAT_R16_SFLOAT>::call, nullptr},				// FORMAT_R16_SFLOAT

				{convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG16_UNORM
				{convertFunc<textureType, samplerValType, i16, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG16_SNORM
				{detail::texelFetchCast2<samplerValType, P, glm::u16>::call, nullptr},					// FORMAT_RG16_USCALED
				{detail::texelFetchCast2<samplerValType, P, glm::i16>::call, nullptr},					// FORMAT_RG16_SSCALED
				{convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG16_UINT
				{convertFunc<textureType, samplerValType, i16, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec2, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RG16_SINT
				{detail::texelFetch<samplerValType, P, FORMAT_RG16_SFLOAT>::call, nullptr},				// FORMAT_RG16_SFLOAT

				{convertFunc<textureType, samplerValType, u16, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB16_UNORM
				{convertFunc<textureType, samplerValType, i16, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB16_SNORM
				{detail::texelFetchCast3<samplerValType, P, glm::u16>::call, nullptr},					// FORMAT_RGB16_USCALED
				{detail::texelFetchCast3<samplerValType, P, glm::i16>::call, nullptr},					// FORMAT_RGB16_SSCALED
				{convertFunc<textureType, samplerValType, u16, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB16_UINT
				{convertFunc<textureType, samplerValType, i16, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec3, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGB16_SINT
				{detail::texelFetch<samplerValType, P, FORMAT_RGB16_SFLOAT>::call, nullptr},			// FORMAT_RGB16_SFLOAT

				{convertFunc<textureType, samplerValType, u16, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA16_UNORM
				{convertFunc<textureType, samplerValType, i16, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA16_SNORM
				{detail::texelFetchCast4<samplerValType, P, glm::u16>::call, nullptr},					// FORMAT_RGBA16_USCALED
				{detail::texelFetchCast4<samplerValType, P, glm::i16>::call, nullptr},					// FORMAT_RGBA16_SSCALED
				{convertFunc<textureType, samplerValType, u16, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA16_UINT
				{convertFunc<textureType, samplerValType, i16, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i16, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_RGBA16_SINT
				{detail::texelFetch<samplerValType, P, FORMAT_RGBA16_SFLOAT>::call, nullptr},			// FORMAT_RGBA16_SFLOAT

				{detail::texelFetchRead1<samplerValType, P, glm::u32>::call, nullptr},					// FORMAT_R32_UINT
				{detail::texelFetchRead1<samplerValType, P, glm::i32>::call, nullptr},					// FORMAT_R32_SINT
				{detail::texelFetchRead1<samplerValType, P, glm::f32>::call, nullptr},					// FORMAT_R32_SFLOAT
				{detail::texelFetchRead2<samplerValType, P, glm::u32>::call, nullptr},					// FORMAT_RG32_UINT
				{detail::texelFetchRead2<samplerValType, P, glm::i32>::call, nullptr},					// FORMAT_RG32_SINT
				{detail::texelFetchRead2<samplerValType, P, glm::f32>::call, nullptr},					// FORMAT_RG32_SFLOAT
				{detail::texelFetchRead3<samplerValType, P, glm::u32>::call, nullptr},					// FORMAT_RGB32_UINT
				{detail::texelFetchRead3<samplerValType, P, glm::i32>::call, nullptr},					// FORMAT_RGB32_SINT
				{detail::texelFetchRead3<samplerValType, P, glm::f32>::call, nullptr},					// FORMAT_RGB32_SFLOAT
				{detail::texelFetchRead4<samplerValType, P, glm::u32>::call, nullptr},					// FORMAT_RGBA32_UINT
				{detail::texelFetchRead4<samplerValType, P, glm::i32>::call, nullptr},					// FORMAT_RGBA32_SINT
				{detail::texelFetchRead4<samplerValType, P, glm::f32>::call, nullptr},					// FORMAT_RGBA32_SFLOAT

				{detail::texelFetchRead1<samplerValType, P, glm::f64>::call, nullptr},					// FORMAT_R64_SFLOAT
				{detail::texelFetchRead2<samplerValType, P, glm::f64>::call, nullptr},					// FORMAT_RG64_SFLOAT
				{detail::texelFetchRead3<samplerValType, P, glm::f64>::call, nullptr},					// FORMAT_RGB64_SFLOAT
				{detail::texelFetchRead4<samplerValType, P, glm::f64>::call, nullptr},					// FORMAT_RGBA64_SFLOAT

				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_RG11B10F, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_RG11B10F, std::numeric_limits<samplerValType>::is_iec559>::write},			// FORMAT_RG11B10_UFLOAT
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_RGB9E5, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_RGB9E5, std::numeric_limits<samplerValType>::is_iec559>::write},				// FORMAT_RGB9E5_UFLOAT

				{convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_DEFAULT>::write},						// FORMAT_D16_UNORM
				{convertFunc<textureType, samplerValType, u32, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u32, P, tvec1, CONVERT_MODE_DEFAULT>::write},						// FORMAT_D24_UNORM
				{convertFunc<textureType, samplerValType, float, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, float, P, tvec1, CONVERT_MODE_DEFAULT>::write},					// FORMAT_D32_SFLOAT
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::write},							// FORMAT_S8_UINT
				{convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_DEFAULT>::write},						// FORMAT_D16_UNORM_S8_UINT
				{convertFunc<textureType, samplerValType, u32, P, tvec2, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u32, P, tvec2, CONVERT_MODE_DEFAULT>::write},						// FORMAT_D24_UNORM_S8_UINT
				{convertFunc<textureType, samplerValType, u32, P, tvec2, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u32, P, tvec2, CONVERT_MODE_DEFAULT>::write},						// FORMAT_D32_SFLOAT_S8_UINT

				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_DXT1_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_DXT1_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_DXT1_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_DXT1_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_DXT3_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_DXT3_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_DXT5_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_DXT5_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::write},							// FORMAT_R_ATI1N_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::write},							// FORMAT_R_ATI1N_SNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RG_ATI2N_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RG_ATI2N_SNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_BP_UFLOAT
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_BP_SFLOAT
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_BP_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_BP_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_ETC2_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_ETC2_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_ETC2_A1_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_ETC2_A1_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_ETC2_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_ETC2_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::write},							// FORMAT_R_EAC_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_DEFAULT>::write},							// FORMAT_R_EAC_SNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RG_EAC_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RG_EAC_SNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_4x4_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_4x4_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_5x4_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_5x4_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_5x5_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_5x5_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_6x5_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_6x5_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_6x6_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_6x6_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_8x5_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_8x5_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_8x6_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_8x6_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_8x8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_8x8_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x5_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x5_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x6_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x6_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x8_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x10_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_10x10_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_12x10_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_12x10_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_12x12_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_ASTC_12x12_SRGB

				{nullptr, nullptr},							// FORMAT_BGRA4_UNORM
				{nullptr, nullptr},							// FORMAT_BGRA4_USCALED
				{nullptr, nullptr},							// FORMAT_B5G6R5_UNORM
				{nullptr, nullptr},							// FORMAT_B5G6R5_USCALED
				{nullptr, nullptr},							// FORMAT_BGR5A1_UNORM
				{nullptr, nullptr},							// FORMAT_BGR5A1_USCALED

				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGR8_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGR8_SNORM
				{detail::texelFetchCast3<samplerValType, P, glm::u8>::call, nullptr},											// FORMAT_BGR8_USCALED
				{detail::texelFetchCast3<samplerValType, P, glm::i8>::call, nullptr},											// FORMAT_BGR8_SSCALED
				{nullptr, nullptr},													// FORMAT_BGR8_UINT
				{nullptr, nullptr},													// FORMAT_BGR8_SINT
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGR8_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGRA8_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGRA8_SNORM
				{detail::texelFetchCast4<samplerValType, P, glm::u8>::call, nullptr},											// FORMAT_BGRA8_USCALED
				{detail::texelFetchCast4<samplerValType, P, glm::i8>::call, nullptr},											// FORMAT_BGRA8_SSCALED
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGRA8_UINT
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_CAST, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGRA8_SINT
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGRA8_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UNORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UNORM, std::numeric_limits<samplerValType>::is_iec559>::write},	// FORMAT_BGR10A2_UNORM
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SNORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SNORM, std::numeric_limits<samplerValType>::is_iec559>::write},	// FORMAT_BGR10A2_SNORM
				{detail::texelFetch<samplerValType, P, FORMAT_BGR10A2_USCALED>::call, nullptr},			// FORMAT_BGR10A2_USCALED
				{detail::texelFetch<samplerValType, P, FORMAT_BGR10A2_SSCALED>::call, nullptr},			// FORMAT_BGR10A2_SSCALED
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UINT, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_RGB10A2UINT, std::numeric_limits<samplerValType>::is_iec559>::write},		// FORMAT_BGR10A2_UINT
				{convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SINT, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, i8, P, tvec4, CONVERT_MODE_RGB10A2SINT, std::numeric_limits<samplerValType>::is_iec559>::write},		// FORMAT_BGR10A2_SINT

				{nullptr, nullptr},							// FORMAT_RG3B2_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGRX8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_SRGB, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_BGRX8_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_L8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_A8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_LA8_UNORM
				{convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_L16_UNORM
				{convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec1, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_A16_UNORM
				{convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::fetch, convertFunc<textureType, samplerValType, u16, P, tvec2, CONVERT_MODE_NORM, std::numeric_limits<samplerValType>::is_iec559>::write},					// FORMAT_LA16_UNORM

				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_PVRTC1_8X8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_PVRTC1_8X8_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_PVRTC1_16X8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_PVRTC1_16X8_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC1_8X8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC1_8X8_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC1_16X8_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC1_16X8_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC2_4X4_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC2_4X4_SRGB
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC2_8X4_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_PVRTC2_8X4_SRGB

				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_ETC_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec3, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGB_ATC_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write},							// FORMAT_RGBA_ATC_EXPLICIT_UNORM
				{convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::fetch, convertFunc<textureType, samplerValType, u8, P, tvec4, CONVERT_MODE_DEFAULT>::write}							// FORMAT_RGBA_ATC_INTERPOLATED_UNORM
			};
			static_assert(sizeof(Table) / sizeof(Table[0]) == FORMAT_COUNT, "Texel functions need to be updated");

			return Table[Format - FORMAT_FIRST];
		}
	};
}//namespace detail
}//namespace gli
