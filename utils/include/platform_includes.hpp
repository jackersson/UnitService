#ifndef NetworkIncludes_Included
#define NetworkIncludes_Included


#if defined(WIN32) || defined(UNDER_CE)
#   include <windows.h>
#   include <rpc.h>

#pragma comment( lib, "Rpcrt4.lib" )
#   if defined(UNDER_CE)
#       include <iphlpapi .h="">
#   endif
#elif defined(__APPLE__)
#   include <corefoundation corefoundation.h="">
#   include <iokit iokitlib.h="">
#   include </iokit><iokit network="" ioethernetinterface.h="">
#   include </iokit><iokit network="" ionetworkinterface.h="">
#   include </iokit><iokit network="" ioethernetcontroller.h="">
#elif defined(LINUX) || defined(linux)
#   include <string .h="">
#   include <net if.h="">
#   include <sys ioctl.h="">
#   include </sys><sys socket.h="">
#   include <arpa inet.h="">
#endif

#endif