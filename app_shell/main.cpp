#include <string>
#include <iostream>
#include "launcher.hpp"

void init()
{
	Launcher launcher;
	launcher.init();
	std::cin.get();
}


/*
std::cout << fs.rdbuf() << std::endl;
while (!fs.eof())
{
char i;
fs >> i;
buff.push_back(i);
}
*/


int main()
{
	/*
	//TODO move to decode, encode tests
	DataTypes::FrameBytes bytes;
	int params[3] = { 0 };
	params[0] = CV_IMWRITE_JPEG_QUALITY;
	params[1] = 80;
	std::vector<unsigned char> buff_t;
	auto file = cv::imread("D:\\3.jpg");
	cv::imencode(".jpg", file, buff_t, std::vector<int>(params, params + 2));
	std::cout << buff_t.size() << std::endl;
	bytes.set_frame_data(std::string(buff_t.begin(), buff_t.end()));
	std::ofstream fso("D:\\2.dat", std::ios::binary);
	fso << bytes.frame_data();
	//for (auto uc : buff_t)
	//  fso << uc;
	fso.close();
	
	//return 0;
	
	std::ifstream fs("D:\\2.dat", std::ios::binary);
	std::string contents((std::istreambuf_iterator<char>(fs)),
		std::istreambuf_iterator<char>());

	std::vector<unsigned char> buff (contents.begin(), contents.end());
	
	fs.close();
	std::cout << buff.size() << std::endl;
	cv::Mat mat;
	cv::imdecode(buff, CV_LOAD_IMAGE_COLOR , &mat);

	//cv::imencode(mat, 0, &dest);

	cv::imwrite("D:\\134.jpg", mat);
	std::cin.get();
	return 0; */
	init();

	std::cout << "Unit service exit. Good-bye)" << std::endl;
	return 0;
}
