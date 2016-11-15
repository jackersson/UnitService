#ifndef DirectShowDeviceInfo_Included
#define DirectShowDeviceInfo_Included

#include <string>
#include <vector>

namespace directshow_device
{
	class Capability
	{
	public:
		Capability(): width_(0), height_(0)
		{	}


		Capability(double widht, double height) 
			: width_(static_cast<int>(widht)), height_(static_cast<int>(height))
		{	}

		Capability(int widht, int height) : width_(widht), height_(height)
		{	}

		int width() const {
			return width_;
		}

		int height() const {
			return height_;
		}

		int size() const {
			return width_ * height_;
		}
		
	private:
		int width_ ;
		int height_;
	};

	//TODO use device id instead so many copy paste
	class DirectShowDeviceInfo 
	{	

	public:
		DirectShowDeviceInfo();
		DirectShowDeviceInfo(const std::string& device_name, int device_id);

		void set_capabilities(const std::vector<Capability>& caps);

		int   id() const;	

		const std::string& name() const;

		const std::vector<Capability>& capabilities() const;

		Capability best_capability() const;

		bool DirectShowDeviceInfo::operator==(const DirectShowDeviceInfo &other) const;

	private:
		std::string device_name_;
		int         device_id_  ;
		std::vector<Capability> capabilities_;	
	};
}

#endif
