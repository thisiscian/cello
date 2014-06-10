#include<cello/pixel.h>
using std::set;

Pixel::Pixel(GifByteType *R, GifByteType *G, GifByteType *B) {
	r=R; g=G; b=B;
}

void Pixel::operator=(GifByteType grey) { *r=grey; *g=grey; *b=grey;}
void Pixel::operator()(GifByteType grey) { *r=grey; *g=grey; *b=grey;}

bool PixelCompare::operator()(const unsigned char* lhs, const unsigned char* rhs) {
	int l=lhs[2]+lhs[1]*256+lhs[0]*256*256;
	int r=rhs[2]+rhs[1]*256+rhs[0]*256*256;
	if(l < r) { return true; }
	return false;
}

bool PixelCompare::operator()(const Pixel& lhs, const Pixel& rhs) {
	if(*(lhs.r) < *(rhs.r)) { return true; }
	else if(*(lhs.r) > *(rhs.r)) { return false; }

	if(*(lhs.g) < *(rhs.g)) { return true; }
	else if(*(lhs.g) > *(rhs.g)) { return false; }

	if(*(lhs.b) < *(rhs.b)) { return true; }
	else if(*(lhs.b) > *(rhs.b)) { return false; }

	return false;
}
