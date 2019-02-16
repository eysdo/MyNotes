#include "XVideoWidget.h"
#include <QDebug>
#include <QTimer>
extern "C"
{
#include <libavutil/frame.h>
}

// �Զ���˫����
#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

FILE *fp = NULL;

// ����shader
const char *vString = GET_STR(
	attribute vec4 vertexIn;
	attribute vec2 textureIn;
	varying vec2 textureOut;
	void main(void)
	{
		gl_Position = vertexIn;
		textureOut = textureIn;
	}
);

// ƬԪshader
const char *tString = GET_STR(
	varying vec2 textureOut;
	uniform sampler2D tex_y;
	uniform sampler2D tex_u;
	uniform sampler2D tex_v;
	void main(void)
	{
		vec3 yuv;
		vec3 rgb;
		yuv.x = texture2D(tex_y, textureOut).r;
		yuv.y = texture2D(tex_u, textureOut).r - 0.5;
		yuv.z = texture2D(tex_v, textureOut).r - 0.5;
		rgb = mat3(1.0, 1.0, 1.0,
			0.0, -0.39465, 2.03211,
			1.13983, -0.58060, 0.0) * yuv;
		gl_FragColor = vec4(rgb, 1.0);
	}
);

// ׼��yuv����
// ffmpeg -i test.mp4 -t 10 -s 240x128 -pix_fmt yuv420p out240x128.yuv
XVideoWidget::XVideoWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
}

XVideoWidget::~XVideoWidget()
{
}

void XVideoWidget::Repaint(AVFrame *frame)
{
	if (!frame) return;
	_mux.lock();
	// �ݴ�, ��֤�ߴ���ȷ
	if (!_datas[0] || _width*_height == 0 || frame->width != _width || frame->height != _height)
	{
		av_frame_free(&frame);
		_mux.unlock();
		return;
	}
	memcpy(_datas[0], frame->data[0], _width*_height);
	memcpy(_datas[1], frame->data[1], _width*_height/4);
	memcpy(_datas[2], frame->data[2], _width*_height/4);
	// �ж�������
	_mux.unlock();

	// ˢ����ʾ
	update();
}

void XVideoWidget::Init(int width, int height)
{
	_mux.lock();
	this->_width = width;
	this->_height = height;
	if (_datas[0]) delete[] _datas[0];
	if (_datas[1]) delete[] _datas[1];
	if (_datas[2]) delete[] _datas[2];

	/// ��������ڴ�ռ�
	_datas[0] = new unsigned char[_width*_height];     // Y
	_datas[1] = new unsigned char[_width*_height / 4]; // U
	_datas[2] = new unsigned char[_width*_height / 4]; // V

	if (_texs[0])
		glDeleteTextures(3, _texs);
	// ��������
	glGenTextures(3, _texs);

	// Y
	glBindTexture(GL_TEXTURE_2D, _texs[0]);
	// �Ŵ����, ���Բ�ֵ   GL_NEAREST(Ч�ʸ�, ������������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// ���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _width, _height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	// U
	glBindTexture(GL_TEXTURE_2D, _texs[1]);
	// �Ŵ����, ���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// ���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _width / 2, _height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	// V
	glBindTexture(GL_TEXTURE_2D, _texs[2]);
	// �Ŵ����, ���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// ���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _width / 2, _height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	_mux.unlock();
}

// ��ʼ��gl
void XVideoWidget::initializeGL()
{
	qDebug() << "initializeGL";
	_mux.lock();
	// ��ʼ��opengl (QOpenGLFunctions�̳�)����
	initializeOpenGLFunctions();

	// program����shader (�����ƬԪ)�ű�
	// ƬԪ(����)
	qDebug() << _program.addShaderFromSourceCode(QGLShader::Fragment, tString);
	// ����shader
	qDebug() << _program.addShaderFromSourceCode(QGLShader::Vertex, vString);

	// ���ö�������ı���
	_program.bindAttributeLocation("vertexIn", A_VER);

	// ���ò�������
	_program.bindAttributeLocation("textureIn", T_VER);

	// ����shader
	qDebug() << "_program.link() = " << _program.link();
	qDebug() << "_program.bind() = " << _program.bind();

	// ���ݶ���Ͳ�������
	// ����
	static const GLfloat ver[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f
	};

	// ����
	static const GLfloat tex[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	// ����
	glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
	glEnableVertexAttribArray(A_VER);

	// ����
	glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
	glEnableVertexAttribArray(T_VER);

	//��shader��ȡ����
	_unis[0] = _program.uniformLocation("tex_y");
	_unis[1] = _program.uniformLocation("tex_u");
	_unis[2] = _program.uniformLocation("tex_v");

	_mux.unlock();
	
	//fp = fopen("./out240x128.yuv", "rb");
	//if (!fp)
	//{
	//	qDebug() << "out240x128.yuv file open failed!";
	//}

	//// ������ʱ��
	//QTimer *ti = new QTimer(this);
	//connect(ti, SIGNAL(timeout()), this, SLOT(update()));
	//ti->start();
}

// ˢ����ʾ
void XVideoWidget::paintGL()
{
	/*if (feof(fp))
	{
		fseek(fp, 0, SEEK_SET);
	}
	fread(_datas[0], 1, _width*_height, fp);
	fread(_datas[1], 1, _width*_height / 4, fp);
	fread(_datas[2], 1, _width*_height / 4, fp);*/
	_mux.lock();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texs[0]); // 0��󶨵�Y����
											// �޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RED, GL_UNSIGNED_BYTE, _datas[0]);
	// ��shader uni��������
	glUniform1i(_unis[0], 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, _texs[1]); // 1��󶨵�U����
											// �޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width / 2, _height / 2, GL_RED, GL_UNSIGNED_BYTE, _datas[1]);
	// ��shader uni��������
	glUniform1i(_unis[1], 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, _texs[2]); // 2��󶨵�V����
											// �޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width / 2, _height / 2, GL_RED, GL_UNSIGNED_BYTE, _datas[2]);
	// ��shader uni��������
	glUniform1i(_unis[2], 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	qDebug() << "paintGL";
	_mux.unlock();
}

// ���ڳߴ�仯
void XVideoWidget::resizeGL(int width, int height)
{
	_mux.lock();
	qDebug() << "resizeGL" << width << " : " << height;
	_mux.unlock();
}