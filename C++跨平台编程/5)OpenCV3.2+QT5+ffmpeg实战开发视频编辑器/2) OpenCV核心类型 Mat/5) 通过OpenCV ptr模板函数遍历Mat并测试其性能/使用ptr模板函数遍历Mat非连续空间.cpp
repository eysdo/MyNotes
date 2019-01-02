#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world320.lib")

void PrintMs(const char *text = "");

int main(int argc, char** argv)
{
	Mat mat(3000, 4000, CV_8UC3);
	//mat.create(3000, 4000, CV_8UC3);

	printf("mat.rows:%d\nmat.cols:%d\nmat.elemSize:%d\n", mat.rows, mat.cols, mat.elemSize());
	int element_size = mat.elemSize();
	int size = mat.rows * mat.cols * element_size;

	// 遍历Mat连续空间
	PrintMs();
	for (int i = 0; i < size; i += element_size)
	{
		mat.data[i] = 255;   // B
		mat.data[i + 1] = 0; // G
		mat.data[i + 2] = 0; // R
	}
	PrintMs("遍历Mat连续空间执行时间");

	// 遍历Mat非连续空间
	PrintMs();
	for (int row = 0; row < mat.rows; row++)
	{
		for (int col = 0; col < mat.cols; col++)
		{
			(&mat.data[row * mat.step])[col * element_size] = 0;       // B
			(&mat.data[row * mat.step])[col * element_size + 1] = 0;   // G
			(&mat.data[row * mat.step])[col * element_size + 2] = 255; // R
		}
	}
	PrintMs("遍历Mat非连续空间执行时间");

	// 使用ptr遍历Mat非连续空间
	PrintMs();
	for (int row = 0; row < mat.rows; row++)
	{
		for (int col = 0; col < mat.cols; col++)
		{
			Vec3b *c = mat.ptr<Vec3b>(row, col);
			c->val[0] = 0;   // B
			c->val[1] = 255; // G
			c->val[2] = 0;   // R
		}
	}
	PrintMs("使用ptr遍历Mat非连续空间执行时间");


	namedWindow("img");
	imshow("img", mat);
	waitKey(0);

	getchar();
	return 0;
}

void PrintMs(const char *text)
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
		printf("%s: %d ms\n", text, ms);
	}
	last = 0;
}