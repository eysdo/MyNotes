#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world320.lib")

void PrintMs(const char *text = "")
{
	static long long last = 0;
	long long cur = getTickCount();
	if (last == 0)
	{
		last = cur;
		return;
	}
	long long ms = 0;
	ms = ((double)(cur - last) / getTickFrequency()) * 1000;
	if (*text != 0)
	{
		printf("%s: %d ms", text, ms);
	}
	cur = getTickCount();
}

int main(int argc, char** argv)
{
	Mat mat(3000, 4000, CV_8UC3);
	//mat.create(3000, 4000, CV_8UC3);

	printf("mat.rows:%d\nmat.cols:%d\nmat.elemSize:%d\n", mat.rows, mat.cols, mat.elemSize());
	int element_size = mat.elemSize();
	int size = mat.rows * mat.cols * element_size;
	PrintMs();
	for (int i = 0; i < size; i += element_size)
	{
		mat.data[i] = 255;   // B
		mat.data[i + 1] = 0; // G
		mat.data[i + 2] = 0; // R
	}
	PrintMs("执行使用时间");
	namedWindow("img");
	imshow("img", mat);
	waitKey(0);

	getchar();
	return 0;
}