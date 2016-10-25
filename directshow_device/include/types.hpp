#ifndef Types_Included
#define Types_Included


#include <contracts/devices/video_device/istream_data.hpp>
#include <contracts/devices/device_observer.hpp>
typedef
contracts::devices::IDeviceObserver<contracts::devices::video_device::IStreamData>
IVideoDeviceObserver;

#endif