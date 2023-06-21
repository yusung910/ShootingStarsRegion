#pragma once
#define LOGIN_REQ 3000
#define LOGIN_RSLT 3001

class PacketProcess : public GSingleton<PacketProcess>
{
public:
	void CheckPacket(InMemStream& in);
};


#define _PacketProcess PacketProcess::Singleton()