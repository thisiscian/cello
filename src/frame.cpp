#include<cello/frame.h>
using namespace cello;

void cello::size(int squareWidth) { width=squareWidth; height=squareWidth; }
void cello::size(int w, int h) { width=w; height=h; }

Pixel::Pixel(PixCol *R, PixCol *G, PixCol *B, PixCol *A) {
	r=R; g=G; b=B; a=A;
}

void Pixel::operator=(PixCol grey) { *r=grey; *g=grey; *b=grey; *a=255; }
void Pixel::operator()(PixCol grey) { *r=grey; *g=grey; *b=grey; *a=255; }
void Pixel::operator()(PixCol grey, PixCol alpha) { *r=grey; *g=grey; *b=grey; *a=alpha; }

Pixel Frame::pixels(int x, int y) {
	int pos=y*width+x;
	return Pixel(&(r[pos]), &(g[pos]), &(b[pos]), &(a[pos]));
}

Frame::Frame() {
	width=cello::width;
	height=cello::height;
	originX=0; originY=0;
	translateX=0.0; translateY=0.0; scaleX=1.0; scaleY=1.0f; rotate=1.0f;
	r = new GifByteType[width*height]();
	g = new GifByteType[width*height]();
	b = new GifByteType[width*height]();
	a = new GifByteType[width*height]();
}
