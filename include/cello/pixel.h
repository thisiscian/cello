#ifndef __cello_pixel__
#define __cello_pixel__
	#include <cstdlib>
	class Pixel {
		public:
		Pixel(unsigned char* P);
		Pixel();
		Pixel(char* P);
		unsigned char operator[](int x);
		const unsigned char operator[](int x) const;
		bool operator<(const Pixel& P) const;
		bool operator==(const Pixel& P) const;
		void set(unsigned char* P);
		void set(char* P);
		void colour(unsigned char r, unsigned char b, unsigned char g);
		private:
		unsigned char* p;
		const int sum() const;
	};
#endif
