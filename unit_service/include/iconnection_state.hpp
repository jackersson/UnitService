#ifndef IConnectionState_Included
#define IConnectionState_Included

//TODO maybe to contracts
class IConnectionState
{
public:
	virtual ~IConnectionState() {}

	virtual bool connected() const = 0;
};

#endif
