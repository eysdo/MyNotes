#include "XFilter.h"
#include <QMutex>
#include "XImagePro.h"
class CXFliter :public XFilter 
{
public:
	std::vector<XTask> tasks;
	QMutex mutex;
	cv::Mat Filter(cv::Mat mat1, cv::Mat mat2)
	{
		mutex.lock();
		XImagePro p;
		p.Set(mat1, mat2);
		for (int i = 0; i < tasks.size(); i++)
		{
			switch (tasks[i].type)
			{
				case XTASK_GAIN: 
				{
					// ���ȶԱȶȵ���
					p.Gain(tasks[i].para[0], tasks[i].para[1]);
					break;
				}
				case XTASK_ROTATE90:
				{
					p.Rotate90();
					break;
				}
				case XTASK_ROTATE180:
				{
					p.Rotate180();
					break;
				}
				case XTASK_ROTATE270:
				{
					p.Rotate270();
					break;
				}
				case XTASK_FLIPX: 
				{
					p.FlipX();
					break;
				}
				case XTASK_FLIPY: 
				{
					p.FlipY();
					break;
				}
				case XTASK_FLIPXY:
				{
					p.FilpXY();
					break;
				}
				case XTASK_RESIZE:
				{
					p.Resize(tasks[i].para[0], tasks[i].para[1]);
					break;
				}
				case XTASK_PYDOWN:
				{
					p.PyDown(tasks[i].para[0]);
					break;
				}
				case XTASK_PYUP:
				{
					p.PyUp(tasks[i].para[0]);
					break;
				}
				case XTASK_CLIP:
				{
					p.Clip(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2], tasks[i].para[3]);
					break;
				}
				case XTASK_GRAY:
				{
					p.Gray();
					break;
				}
				case XTASK_MARK:
				{
					p.Mark(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2]);
					break;
				}
				default:break;
			}
		}
		cv::Mat re = p.Get();
		mutex.unlock();
		return re;
	}
	void Add(XTask task)
	{
		mutex.lock();
		tasks.push_back(task);
		mutex.unlock();
	}
	void Clear()
	{
		mutex.lock();
		tasks.clear();
		mutex.unlock();
	}
};

XFilter* XFilter::Get()
{
	static CXFliter cx;
	return &cx;
}

XFilter::XFilter()
{
}


XFilter::~XFilter()
{
}
