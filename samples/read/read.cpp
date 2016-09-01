#include <iostream>
#include <fstream>

#include <Png.hpp>

using namespace std;

void write_ppm(const string& fpath, const Png& png)
{
	ofstream ofs(fpath.c_str());
	ofs << "P3" << endl;
	ofs << "# comment" << endl;
	ofs << png.getWidth() << " " << png.getHeight() << endl;
	ofs << "255" << endl;
	unsigned char** rowPointers = png.getRowPointers();
	for (unsigned i = 0; i < png.getHeight(); i++)
	for (unsigned j = 0; j < png.getWidth(); j++)
		ofs << (int)rowPointers[i][j * 3 + 0] << " " << (int)rowPointers[i][j * 3 + 1] << " " << (int)rowPointers[i][j * 3 + 2] << endl;
}

int main(void)
{
	string fpath("../data/sample.png");
	Png png(fpath);
	write_ppm("../data/output.ppm", png);
	
	return 0;
}

