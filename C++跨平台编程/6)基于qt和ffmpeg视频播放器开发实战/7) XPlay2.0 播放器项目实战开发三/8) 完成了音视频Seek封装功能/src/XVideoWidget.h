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

	// ���ܳɹ�����ͷ�frame�ռ�
	virtual void Repaint(AVFrame *frame);

	XVideoWidget(QWidget *parent);
	~XVideoWidget();

protected:
	// ˢ����ʾ
	void paintGL();

	// ��ʼ��gl
	void initializeGL();

	// ���ڳߴ�仯
	void resizeGL(int width, int height);

private:
	std::mutex _mux;

	// shader����
	QGLShaderProgram _program;

	// shader��yuv������ַ
	GLuint _unis[3] = { 0 };
	// opengl��texture��ַ
	GLuint _texs[3] = { 0 };

	// �����ڴ�ռ�
	unsigned char *_datas[3] = { 0 };

	int _width = 240;
	int _height = 128;
};
