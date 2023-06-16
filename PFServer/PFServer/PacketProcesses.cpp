//
//#include <process.h>
//#include <sstream>
//#include <algorithm>
//#include <string>
//
//#include "MainIOCP.h"
//#include "PacketProcesses.h"
//
//PacketProcesses::PacketProcesses()
//{
//
//}
//
//void PacketProcesses::Login(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	string Id;
//	string Pw;
//	RecvStream >> Id;
//	RecvStream >> Pw;
//	
//	printf_s("INFO::로그인 시도 { %s }/{ %s }\n", Id.c_str(), Pw.c_str());
//	
//	string user_seq = MainIOCP::Dao.SearchAccount(Id, Pw);
//	stringstream SendStream;
//	SendStream << EPacketType::LOGIN << endl;
//		
//	if (!user_seq.empty())
//	{
//		printf_s("INFO::패킷 전송 : %d\n", MainIOCP::SessionSocket.size());
//		printf_s("INFO::사용자 SEQ : %s\n", user_seq.c_str());
//		MainIOCP::vo.USER_SEQ = user_seq;
//	}
//	else
//	{
//		user_seq = "error";
//	}
//	SendStream << user_seq << endl;
//
//	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
//	pSocket->dataBuf.buf = pSocket->messageBuffer;
//	pSocket->dataBuf.len = SendStream.str().length();
//		
//	MainIOCP::Send(pSocket);
//}
//
//void PacketProcesses::SearchCharacters(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	string user_seq;
//	RecvStream >> user_seq;
//	
//	printf_s("INFO::게임 사용자 SEQ { %s }\n", user_seq.c_str());
//	
//	//사용자 조회
//	vector<map<string, string>> charList = MainIOCP::Dao.SelectCharacterList(user_seq);
//	
//	stringstream SendStream;
//	SendStream << EPacketType::SEARCH_CHARACTER << endl;
//	PlayerVO LocalVO;
//	for (auto info : charList)
//	{
//		MainIOCP::vo = info;
//		LocalVO.SetCharacterListToStream(SendStream, MainIOCP::vo);
//
//	}
//	SendStream << endl;
//
//	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
//	pSocket->dataBuf.buf = pSocket->messageBuffer;
//	pSocket->dataBuf.len = SendStream.str().length();
//	
//	MainIOCP::Send(pSocket);
//}
//
//void PacketProcesses::Logout(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	int SessionId;
//	RecvStream >> SessionId;
//	printf_s("INFO::(%d)로그아웃 요청 수신\n", SessionId);
//	EnterCriticalSection(&MainIOCP::csPlayers);
//
//	//사망처리만 할 경우 클라이언트에 정보가 남아있게 되버림
//	//MainIOCP::cInfo.players[SessionId].PlayerCond = ECondition::IS_DEATH;
//	
//	//잔여 플레이어 목록 배열에서 해당 SessionID키값에 해당하는 데이터를 지운다
//	MainIOCP::cInfo.players.erase(SessionId);
//
//	LeaveCriticalSection(&MainIOCP::csPlayers);
//	MainIOCP::SessionSocket.erase(SessionId);
//	printf_s("INFO::클라이언트 수 : %d\n", MainIOCP::SessionSocket.size());
//	MainIOCP::WriteCharactersInfoToSocket(pSocket);
//}
//
//void PacketProcesses::CreateCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	string class_seq;
//	string char_nm;
//	RecvStream >> class_seq;
//	RecvStream >> char_nm;
//
//	PlayerVO vo;
//	vo.USER_SEQ = MainIOCP::vo.USER_SEQ;
//	vo.CLASS_SEQ = class_seq;
//	vo.CHAR_NM = char_nm;
//	printf_s("INFO::캐릭터 생성 USER_SEQ : { %s }\n", MainIOCP::vo.USER_SEQ.c_str());
//	
//	bool rslt = MainIOCP::Dao.InsertCharacter(vo);
//	
//	stringstream SendStream;
//	SendStream << EPacketType::CREATE_CHARACTER << endl;
//	SendStream << rslt << endl;
//	
//	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
//	pSocket->dataBuf.buf = pSocket->messageBuffer;
//	pSocket->dataBuf.len = SendStream.str().length();
//
//	MainIOCP::Send(pSocket);
//}
//
//void PacketProcesses::DeleteCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	string char_seq;
//	RecvStream >> char_seq;
//	PlayerVO vo;
//	vo.USER_SEQ = MainIOCP::vo.USER_SEQ;
//	vo.CHAR_SEQ = char_seq;
//
//	printf_s("INFO::캐릭터 삭제 USER_SEQ: { %s }, CHAR_SEQ: { %s }\n", MainIOCP::vo.USER_SEQ.c_str(), char_seq.c_str());
//
//	bool rslt = MainIOCP::Dao.DeleteCharacter(vo);
//
//	stringstream SendStream;
//	SendStream << EPacketType::DELETE_CHARACTER << endl;
//	SendStream << rslt << endl;
//
//	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
//	pSocket->dataBuf.buf = pSocket->messageBuffer;
//	pSocket->dataBuf.len = SendStream.str().length();
//
//	MainIOCP::Send(pSocket);
//}
//
//void PacketProcesses::EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	PlayerVO vo;
//	RecvStream >> vo;
//
//	printf_s("INFO::[%d]캐릭터 접속 - X : [%f], Y : [%f], Z : [%f], Yaw : [%f]\n",
//		vo.SessionID, vo.X, vo.Y, vo.Z, vo.Yaw);
//
//	EnterCriticalSection(&MainIOCP::csPlayers);
//
//	PlayerVO* pVO = &MainIOCP::cInfo.players[vo.SessionID];
//
//	pVO << vo;
//
//	LeaveCriticalSection(&MainIOCP::csPlayers);
//
//	MainIOCP::SessionSocket[vo.SessionID] = pSocket->socket;
//	//MainIOCP::SessionSocket.insert(pair<int, SOCKET>(vo.SessionID, pSocket->socket));
//
//	printf_s("INFO::클라이언트 수 : %d\n", MainIOCP::SessionSocket.size());
//
//	BroadcastNewPlayer(vo);
//}
//
//void PacketProcesses::SyncCharacters(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	PlayerVO vo;
//	RecvStream >> vo;
//
//	//printf_s("[INFO][%d]정보 수신 - %d\n", vo.SessionID, vo.PlayerCond);
//	EnterCriticalSection(&MainIOCP::csPlayers);
//
//	PlayerVO* pinfo = &MainIOCP::cInfo.players[vo.SessionID];
//
//	pinfo << vo;
//
//	LeaveCriticalSection(&MainIOCP::csPlayers);
//
//	MainIOCP::WriteCharactersInfoToSocket(pSocket);
//	MainIOCP::Send(pSocket);
//}
//
//void PacketProcesses::BroadcastNewPlayer(PlayerVO vo)
//{
//	stringstream SendStream;
//
//	SendStream << EPacketType::ENTER_NEW_PLAYER << endl;
//	SendStream << vo << endl;
//
//	Broadcast(SendStream);
//}
//
//void PacketProcesses::Broadcast(stringstream& SendStream)
//{
//	stSOCKETINFO* client = new stSOCKETINFO;
//	for (auto& kvp : MainIOCP::SessionSocket)
//	{
//		client->socket = kvp.second;
//		CopyMemory(client->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
//		client->dataBuf.buf = client->messageBuffer;
//		client->dataBuf.len = SendStream.str().length();
//	
//		MainIOCP::Send(client);
//	}
//}
//
//void PacketProcesses::HitCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	// 피격 처리된 세션 아이디
//	PlayerVO vo;
//	RecvStream >> vo;
//	int LocalSessionID = vo.SessionID;
//	
//	if (MainIOCP::cInfo.players.find(LocalSessionID) == MainIOCP::cInfo.players.end())
//		return;
//
//	EnterCriticalSection(&MainIOCP::csPlayers);
//
//	MainIOCP::cInfo.players[LocalSessionID].Damaged(vo.BE_DAMAGE_AMOUNT);
//
//	printf_s("INFO::Player[%d] 현재 체력: %f, 피격 데미지 : %f 받음 \n",
//			vo.SessionID, MainIOCP::cInfo.players[LocalSessionID].CUR_HP, vo.BE_DAMAGE_AMOUNT);
//	if (MainIOCP::cInfo.players[LocalSessionID].CUR_HP < 0)
//	{
//		// 캐릭터 사망처리
//		MainIOCP::cInfo.players[LocalSessionID].PlayerCond = ECondition::IS_DEATH;
//	}
//	LeaveCriticalSection(&MainIOCP::csPlayers);
//	MainIOCP::WriteCharactersInfoToSocket(pSocket);
//	MainIOCP::Send(pSocket);
//}
//
//void PacketProcesses::BroadcastChat(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	int SessionId;
//	string Temp;
//	string Chat;
//
//	RecvStream >> SessionId;
//
//	getline(RecvStream, Temp);
//	Chat += MainIOCP::cInfo.players[SessionId].CHAR_NM + "_:_";
//	while (RecvStream >> Temp)
//	{
//		Chat += Temp + "_";
//	}
//	Chat += '\0';
//
//	printf_s("[CHAT] %s\n", Chat.c_str());
//
//	stringstream SendStream;
//	SendStream << EPacketType::CHAT << endl;
//	SendStream << Chat;
//
//	Broadcast(SendStream);
//}
