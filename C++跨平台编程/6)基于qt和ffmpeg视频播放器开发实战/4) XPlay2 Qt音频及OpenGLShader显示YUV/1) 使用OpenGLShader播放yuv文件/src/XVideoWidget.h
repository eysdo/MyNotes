#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>

class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
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
