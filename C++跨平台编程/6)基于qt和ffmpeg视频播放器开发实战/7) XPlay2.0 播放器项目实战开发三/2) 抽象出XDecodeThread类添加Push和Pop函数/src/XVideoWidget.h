#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include "IVideoCall.h"

struct AVFrame;

class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions, public IVideoCall
{
	Q_OBJECT

public:
	virtual void Init(int width, int height);

	// 不管成功与否都释放frame空间
	virtual void Repaint(AVFrame *frame);

	XVideoWidget(QWidget *parent);
	~XVideoWidget();

protected:
	// 刷新显示
	void paintGL();

	// 初始化gl
	void initializeGL();

	// 窗口尺寸变化
	void resizeGL(int width, int height);

private:
	std::mutex _mux;

	// shader程序
	QGLShaderProgram _program;

	// shader中yuv变量地址
	GLuint _unis[3] = { 0 };
	// opengl的texture地址
	GLuint _texs[3] = { 0 };

	// 材质内存空间
	unsigned char *_datas[3] = { 0 };

	int _width = 240;
	int _height = 128;
};
