#include<cello/pixel.h>
using std::set;

Pixel::Pixel(GifByteType *R, GifByteType *G, GifByteType *B) {
	r=R; g=G; b=B;
}

void Pixel::operator=(GifByteType grey) { *r=grey; *g=grey; *b=grey;}
void Pixel::operator()(GifByteType grey) { *r=grey; *g=grey; *b=grey;}

bool PixelCompare::operator()(const Pixel& lhs, const Pixel& rhs) {
	if(*(lhs.r) < *(rhs.r)) { return true; }
	else if(*(lhs.r) > *(rhs.r)) { return false; }

	if(*(lhs.g) < *(rhs.g)) { return true; }
	else if(*(lhs.g) > *(rhs.g)) { return false; }

	if(*(lhs.b) < *(rhs.b)) { return true; }
	else if(*(lhs.b) > *(rhs.b)) { return false; }

	return false;
}
