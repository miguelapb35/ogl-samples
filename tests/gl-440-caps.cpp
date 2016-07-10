///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Samples Pack (ogl-samples.g-truc.net)
///
/// Copyright (c) 2004 - 2014 G-Truc Creation (www.g-truc.net)
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
///////////////////////////////////////////////////////////////////////////////////

#include "test.hpp"

class gl_440_caps : public test
{
public:
	gl_440_caps(int argc, char* argv[]) :
		test(argc, argv, "gl-440-caps", test::CORE, 4, 4)
	{}

private:
	bool checkCaps()
	{
		caps Caps(caps::CORE);

		GLint NumCompressedTextureFormats = 0;
		glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &NumCompressedTextureFormats);
		std::vector<GLint> CompressedTextureFormats(NumCompressedTextureFormats);
		glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, &CompressedTextureFormats[0]);

		if(this->checkExtension("GL_EXT_texture_compression_s3tc"))
		{
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGB_S3TC_DXT1_EXT) == CompressedTextureFormats.end())
				return false;
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) == CompressedTextureFormats.end())
				return false;
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) == CompressedTextureFormats.end())
				return false;
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) == CompressedTextureFormats.end())
				return false;
		}

		if(this->checkExtension("GL_EXT_texture_sRGB") && this->checkExtension("GL_EXT_texture_compression_s3tc"))
		{
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB_S3TC_DXT1_EXT) == CompressedTextureFormats.end())
				return false;
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT) == CompressedTextureFormats.end())
				return false;
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT) == CompressedTextureFormats.end())
				return false;
			if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT) == CompressedTextureFormats.end())
				return false;
		}

		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RED_RGTC1) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SIGNED_RED_RGTC1) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RG_RGTC2) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SIGNED_RG_RGTC2) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGBA_BPTC_UNORM) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGB8_ETC2) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB8_ETC2) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RGBA8_ETC2_EAC) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_R11_EAC) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SIGNED_R11_EAC) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_RG11_EAC) == CompressedTextureFormats.end())
			return false;
		if(std::find(CompressedTextureFormats.begin(), CompressedTextureFormats.end(), GL_COMPRESSED_SIGNED_RG11_EAC) == CompressedTextureFormats.end())
			return false;

		return true;
	}

	bool begin()
	{
		return checkCaps();
	}

	bool end()
	{
		return true;
	}

	bool render()
	{
		glm::uvec2 WindowSize = this->getWindowSize();

		glViewport(0, 0, WindowSize.x, WindowSize.y);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_440_caps Test(argc, argv);
	Error += Test();

	return Error;
}
