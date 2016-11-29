#ifndef IStreamData_Included
#define IStreamData_Included

#include <vector>
#include <memory>
namespace cv
{
	class Mat;
}
	
namespace video_device
{
	enum StreamType
	{
		  StreamTypeColor = 1
		, StreamTypeDepth = 2
	};

	class IRawImage
	{				
	public:
		virtual ~IRawImage(){}

		virtual int width()  const = 0;
		virtual int height() const = 0;
		virtual unsigned char* data() const = 0;
	};

	class IStreamData
	{
	public:
		virtual ~IStreamData() {}
		virtual const std::vector<unsigned char>& jpeg_bytes() const = 0;
		virtual const cv::Mat&  color() const = 0;
		//virtual IRawImage* try_get_data(StreamType type) const = 0;
	};

	typedef std::shared_ptr<IStreamData> IStreamDataPtr;
}

#endif
