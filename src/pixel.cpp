#include<cello/pixel.h>

Pixel::Pixel() {
	p=NULL;
}

Pixel::Pixel(unsigned char* P) {
	p=P;
}

Pixel::Pixel(char* P) {
	p=(unsigned char*) P;
}

unsigned char Pixel::operator[](int x) {
	return *(p+x);
}

const unsigned char Pixel::operator[](int x) const {
	return *(p+x);
}

const int Pixel::sum() const {
	return 256*256*(p[0])+256*(p[1])+(p[2]);
}

bool Pixel::operator<(const Pixel& P) const {
	return sum() < P.sum();
}

bool Pixel::operator==(const Pixel& P) const {
	return sum()==P.sum();
}

void Pixel::set(char* P) {
	p=(unsigned char*) P;
}

void Pixel::set(unsigned char* P) {
	p=P;
}

void Pixel::colour(const Pixel &P) {
	*p=P[0];
	*(p+1)=P[1];
	*(p+2)=P[2];
}

void Pixel::colour(unsigned char r, unsigned char g, unsigned char b) {
	*p=r;
	*(p+1)=g;
	*(p+2)=b;
}
