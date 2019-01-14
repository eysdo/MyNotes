#include "XImagePro.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
// ����ԭͼ, ����������
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty()) return;
	this->src1 = mat1;
	this->src2 = mat2;
	this->src1.copyTo(des);
}

// �������ȺͶԱȶ�
///@para bright   double  ����   0~100
///@para contrast double  �Աȶ� 1.0~3.0
void XImagePro::Gain(double bright, double contrast)
{
	if (des.empty()) return;
	des.convertTo(des, -1, contrast, bright);
}

void XImagePro::Rotate90()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_90_CLOCKWISE);
}
void XImagePro::Rotate180()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_180);
}
void XImagePro::Rotate270()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_90_COUNTERCLOCKWISE);
}

// ���Ҿ���
void XImagePro::FlipX()
{
	if (des.empty()) return;
	flip(des, des, 0);
}

// ���¾���
void XImagePro::FlipY()
{
	if (des.empty()) return;
	flip(des, des, 1);
}

void XImagePro::FilpXY()
{
	if (des.empty()) return;
	flip(des, des, -1);
}

// ͼ��ߴ�
void XImagePro::Resize(int width, int height)
{
	if (des.empty()) return;
	resize(des, des, Size(width, height));
}

XImagePro::XImagePro()
{
}


XImagePro::~XImagePro()
{
}
