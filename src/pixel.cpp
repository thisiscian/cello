#include<cello/pixel.h>

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
	return 256*256*(*(p))+256*(*(p+1))+(*(p+2));
}

bool Pixel::operator<(const Pixel& P) const {
	return sum() < P.sum();
}

bool Pixel::operator==(const Pixel& P) const {
	return sum()==P.sum();
}
