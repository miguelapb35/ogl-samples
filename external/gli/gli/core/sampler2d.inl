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
/// @file gli/core/sampler2d.inl
/// @date 2015-09-29 / 2015-09-29
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <glm/gtc/packing.hpp>
#include <glm/gtc/color_space.hpp>

namespace gli
{
	template <typename T, glm::precision P>
	inline sampler2D<T, P>::sampler2D(texture2D const & Texture, wrap Wrap, filter Mip, filter Min, glm::tvec4<T, P> const & BorderColor)
		: sampler(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
		, Texture(Texture)
		, Convert(detail::convert<texture2D, T, P>::call(this->Texture.format()))
		, BorderColor(BorderColor)
	{
		assert(!Texture.empty());
		assert(!is_compressed(Texture.format()));
	}

	template <typename T, glm::precision P>
	inline texture2D const & sampler2D<T, P>::operator()() const
	{
		return this->Texture;
	}

	template <typename T, glm::precision P>
	inline glm::tvec4<T, P> sampler2D<T, P>::texel_fetch(dim_type const & TexelCoord, size_type const & Level) const
	{
		assert(this->Convert.Fetch);
		return this->Convert.Fetch(this->Texture, TexelCoord, Level);
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::texel_write(dim_type const & TexelCoord, size_type const & Level, tvec4<T, P> const & Texel)
	{
		assert(this->Convert.Write);
		this->Convert.Write(this->Texture, TexelCoord, Level, Texel);
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::clear(glm::tvec4<T, P> const & Color)
	{

	}

	template <typename T, glm::precision P>
	inline glm::tvec4<T, P> sampler2D<T, P>::texture_lod(samplecoord_type const & Texcoord, float Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		samplecoord_type const TexcoordWrap(this->WrapFunc(Texcoord.x), this->WrapFunc(Texcoord.y));

		size_type const minLevel = size_type(glm::floor(Level));
		size_type const maxLevel = size_type(glm::ceil(Level));

		if (this->Mip == FILTER_LINEAR)
		{
			tvec4<T, P> const minTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, minLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			tvec4<T, P> const maxTexel = this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, maxLevel) : this->texture_lod_nearest(TexcoordWrap, minLevel);
			return mix(minTexel, maxTexel, glm::fract(Level));
		} else
		{
			texture2D::size_type const level = texture2D::size_type(glm::round(Level));
			return this->Min == FILTER_LINEAR ? this->texture_lod_linear(TexcoordWrap, level) : this->texture_lod_nearest(TexcoordWrap, minLevel);
		}
	}

	template <typename T, glm::precision P>
	inline tvec4<T, P> sampler2D<T, P>::texture_lod_linear(samplecoord_type const & Texcoord, size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		dim_type const TexelDim = this->Texture.dimensions(Level);

		int const s_below = int(glm::floor(Texcoord.s * static_cast<T>(TexelDim.x - 1)));
		int const s_above = int(glm::ceil(Texcoord.s * static_cast<T>(TexelDim.x - 1)));
		int const t_below = int(glm::floor(Texcoord.t * static_cast<T>(TexelDim.y - 1)));
		int const t_above = int(glm::ceil(Texcoord.t * static_cast<T>(TexelDim.y - 1)));

		bvec4 UseBorderColor(
			s_below < 0 || s_below > static_cast<int>(TexelDim.x),
			s_above < 0 || s_above > static_cast<int>(TexelDim.x),
			t_below < 0 || t_below > static_cast<int>(TexelDim.y),
			t_above < 0 || t_above > static_cast<int>(TexelDim.y));

		T const s_below_normalized = s_below / static_cast<T>(TexelDim.x);
		T const t_below_normalized = t_below / static_cast<T>(TexelDim.y);

		tvec4<T, P> Texel00(this->BorderColor);
		tvec4<T, P> Texel10(this->BorderColor);
		tvec4<T, P> Texel11(this->BorderColor);
		tvec4<T, P> Texel01(this->BorderColor);

		if (!UseBorderColor[0] && !UseBorderColor[2])
			Texel00 = this->texel_fetch(dim_type(s_below, t_below), Level);

		if (!UseBorderColor[1] && !UseBorderColor[2])
			Texel10 = this->texel_fetch(dim_type(s_above, t_below), Level);

		if (!UseBorderColor[1] && !UseBorderColor[3])
			Texel11 = this->texel_fetch(dim_type(s_above, t_above), Level);

		if (!UseBorderColor[0] && !UseBorderColor[3])
			Texel01 = this->texel_fetch(dim_type(s_below, t_above), Level);

		T const BlendA = static_cast<T>(Texcoord.s - s_below_normalized) * static_cast<T>(TexelDim.x - 1);
		T const BlendB = static_cast<T>(Texcoord.s - s_below_normalized) * static_cast<T>(TexelDim.x - 1);
		T const BlendC = static_cast<T>(Texcoord.t - t_below_normalized) * static_cast<T>(TexelDim.y - 1);

		tvec4<T, P> const ValueA(mix(Texel00, Texel10, BlendA));
		tvec4<T, P> const ValueB(mix(Texel01, Texel11, BlendB));
		tvec4<T, P> const Texel(mix(ValueA, ValueB, BlendC));
		return Texel;
	}

	template <typename T, glm::precision P>
	inline glm::tvec4<T, P> sampler2D<T, P>::texture_lod_nearest(samplecoord_type const & Texcoord, size_type Level) const
	{
		assert(std::numeric_limits<T>::is_iec559);

		dim_type const TexelDim = this->Texture.dimensions(Level);

		int const s = int(floor(Texcoord.s * static_cast<T>(TexelDim.x - 1)));
		int const t = int(floor(Texcoord.t * static_cast<T>(TexelDim.y - 1)));

		bool const UseBorderColor = s > static_cast<int>(TexelDim.x) || s < 0 || t > static_cast<int>(TexelDim.y) || t < 0;

		return UseBorderColor ? this->BorderColor : this->texel_fetch(dim_type(s, t), Level);
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::generate_mipmaps()
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));

		this->generate_mipmaps(this->Texture.base_level(), this->Texture.max_level());
	}

	template <typename T, glm::precision P>
	inline void sampler2D<T, P>::generate_mipmaps(typename sampler2D<T, P>::size_type BaseLevel, typename sampler2D<T, P>::size_type MaxLevel)
	{
		assert(!this->Texture.empty());
		assert(!is_compressed(this->Texture.format()));
		assert(this->Texture.max_level() >= MaxLevel);
		assert(BaseLevel <= MaxLevel);

		for(size_type Level = BaseLevel; Level < MaxLevel; ++Level)
		{
			dim_type const DimDst = this->Texture.dimensions(Level + 1);
			samplecoord_type const SampleDimDst(DimDst);

			for(size_t j = 0; j < DimDst.y; ++j)
			for(size_t i = 0; i < DimDst.x; ++i)
			{
				size_t const x = (i << 1);
				size_t const y = (j << 1);

				tvec4<T, P> Texel00 = this->texture_lod(samplecoord_type(x + 0, y + 0) / SampleDimDst, static_cast<float>(Level));
				tvec4<T, P> Texel01 = this->texture_lod(samplecoord_type(x + 0, y + 1) / SampleDimDst, static_cast<float>(Level));
				tvec4<T, P> Texel11 = this->texture_lod(samplecoord_type(x + 1, y + 1) / SampleDimDst, static_cast<float>(Level));
				tvec4<T, P> Texel10 = this->texture_lod(samplecoord_type(x + 1, y + 0) / SampleDimDst, static_cast<float>(Level));

				tvec4<T, P> const Texel = (Texel00 + Texel01 + Texel11 + Texel10) * static_cast<float>(0.25);
				this->texel_write(dim_type(i, j), Level + 1, Texel);
			}
		}
	}
}//namespace gli

