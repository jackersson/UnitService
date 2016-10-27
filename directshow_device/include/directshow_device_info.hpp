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
		int width_;
		int height_;
	};

	class DirectShowDeviceInfo 
	{	

	public:
		DirectShowDeviceInfo(): device_id_(0)
		{}

		DirectShowDeviceInfo(const std::string& device_name, int device_id)
		                  	: device_name_(device_name)
			                  , device_id_(device_id)
		{}

		void set_capabilities(std::vector<Capability>& caps)	{
			capabilities_= caps;		
		}

		int id() const	{
			return device_id_;
		}
		
		const std::string& name() const	{
			return device_name_;
		}

		const std::vector<Capability>& capabilities() const	{
			return capabilities_;
		}

		Capability best_capability() const
		{
			Capability best;
			for (auto cp : capabilities_)
			{
				if (best.size() < cp.size())
					best = cp;
			}
			return best;
		}

		bool DirectShowDeviceInfo::operator==(const DirectShowDeviceInfo &other) const {
		
			return device_name_ == other.device_name_
				  && device_id_ == other.device_id_;
		}

	private:
		std::string device_name_;
		int device_id_;
		std::vector<Capability> capabilities_;
	
	};
}

#endif
