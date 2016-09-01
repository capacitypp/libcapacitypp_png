#include <iostream>
#include <fstream>

#include <Png.hpp>

using namespace std;

int main(void)
{
	string fpath("../data/sample.png");
	Png png(fpath);
	png.write("../data/output.png");
	
	return 0;
}

