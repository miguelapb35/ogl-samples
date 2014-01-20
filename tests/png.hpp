#ifndef PNG_INCLUDED
#define PNG_INCLUDED

#include <gli/gli.hpp>

namespace glf
{
	/// Loading a PNG file
	gli::texture2D load_png(char const * Filename);

	void save_png(gli::texture2D const & Texture, char const * Filename);

}//namespace glf

#endif//PNG_INCLUDED
