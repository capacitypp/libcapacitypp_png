#include <iostream>
#include <fstream>

#include <Png.hpp>

using namespace std;

int main(void)
{
	Png png(2, 2);
	png[0][1] = 0x00;
	png[0][2] = 0x00;
	png[0][4] = 0x00;
	png[0][6] = 0x00;
	png[1][0] = 0x00;
	png[1][1] = 0x00;
	png.write("../data/output.png");
	
	return 0;
}

