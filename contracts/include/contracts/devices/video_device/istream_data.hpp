#ifndef IStreamData_Included
#define IStreamData_Included


namespace contracts
{
	namespace devices
	{
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

				virtual IRawImage* try_get_data(StreamType type) const = 0;
			};
		}
	}
}

#endif
