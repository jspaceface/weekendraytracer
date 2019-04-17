#include <fstream>
#include <iostream>

int main()
{
	// open ppm file
	std::ofstream fs("generated.ppm");
	if (!fs)
	{
		std::cerr << "Cannot open the output file." << std::endl;
		return 1;
	}
	
	int nx = 20;
	int ny = 10;

	fs << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2;
			int ir = int(255.99 * r);
			int ig = int(255.99 * g);
			int ib = int(255.99 * b);

			fs << ir << " " << ig << " " << ib << "\n";
		}
	}

	fs.close();

	// todo remove for release
	std::cin.get();
	return 0;
}

