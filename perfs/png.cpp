#include "png.hpp"
#include <FreeImage.h>
#include <cstdlib>

namespace glf
{
	static void FreeImageFree()
	{
		FreeImage_DeInitialise();
	}


	static void FreeImageInit()
	{
		static bool Init = false;

		if(!Init)
		{
			Init = true;
			FreeImage_Initialise(false);
			atexit(FreeImageFree);
		}
	}

	/// Loading a PNG file
	gli::texture2D load_png(char const * Filename)
	{
		FreeImageInit();

		FIBITMAP * Bitmap = FreeImage_Load(FIF_PNG, Filename, 0);
		glm::uint BPP = FreeImage_GetBPP(Bitmap);
		glm::uint Width = FreeImage_GetWidth(Bitmap);
		glm::uint Height = FreeImage_GetHeight(Bitmap);

		gli::texture2D Texture(1, BPP == 24 ? gli::RGB8_UNORM : gli::RGBA8_UNORM, gli::texture2D::dimensions_type(Width, Height));
		memcpy(Texture.data(), FreeImage_GetBits(Bitmap), Texture.size());

		FreeImage_Unload(Bitmap);

		return Texture;
	}

	void save_png(gli::texture2D const & Texture, char const * Filename)
	{
		FreeImageInit();

		FIBITMAP* Bitmap = FreeImage_ConvertFromRawBits(
			const_cast<BYTE*>(Texture.data<BYTE>()),
			Texture.dimensions().x, Texture.dimensions().y,
			Texture.dimensions().x * gli::component_count(Texture.format()),
			24, 0x0000FF, 0xFF0000, 0x00FF00, false);

		FreeImage_Save(FIF_PNG, Bitmap, Filename, 0);
		FreeImage_Unload(Bitmap);
	}

}//namespace glf

