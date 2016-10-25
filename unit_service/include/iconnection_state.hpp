#ifndef IConnectionState_Included
#define IConnectionState_Included

class IConnectionState
{
public:
	virtual ~IConnectionState() {}

	virtual bool connected() const = 0;
};

#endif
