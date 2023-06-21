#include "stdafx.h"

void PacketProcess::CheckPacket(InMemStream& in)
{
	short type;
	in.Read(type);
	cout << "[info] 요청 패킷 : " << type << endl;
	switch (type)
	{
	case LOGIN_REQ:
		
		_DBManager.SearchAccount(in);
		break;
	default:
		cout << "[ERROR] 요청 받은 패킷 타입이 존재하지 않는 타입입니다. :" << type << endl;
		break;
	}

}
