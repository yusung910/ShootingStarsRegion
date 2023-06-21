//#include "DataAccess.h"
//#include "PlayerVO.h"
//
//DataAccess::DataAccess()
//{
//
//}
//
//string DataAccess::SearchAccount(const string& Id, const string& Password)
//{
//	string sRet;
//
//	try
//	{
//		ResultSet* rs;
//		PreparedStatement* pstmt;
//		
//		string sql = "SELECT SEQ";
//		sql += " FROM TB_USER";
//		sql += " WHERE USER_ID = ?";
//		sql += " AND USER_PW = ?;";
//		pstmt = con->prepareStatement(sql.c_str());
//
//		pstmt->setString(1, Id.c_str());
//		pstmt->setString(2, Password.c_str());
//
//		cout << "Execute Query :" << sql.c_str() << endl;
//		rs = pstmt->executeQuery();
//		
//		while (rs->next())
//		{
//			sRet = rs->getString(1).c_str();
//		}
//
//		delete pstmt;
//	}
//	catch (SQLException& e)
//	{
//		printf_s("[ERROR] Query 실행 실패 ECode : %d, ErrorMsg : %s\n", e.getErrorCode(), e.getSQLState().c_str());
//	}
//
//	return sRet;
//}
//
//vector<map<string, string>> DataAccess::SelectCharacterList(const string& user_seq)
//{
//	vector<map<string, string>> retArry;
//
//	try
//	{
//		ResultSet* rs;
//		PreparedStatement* pstmt;
//
//		string sql = "SELECT A.USER_SEQ, A.CHAR_SEQ, A.CLASS_SEQ, A.CHAR_NM, A.CHAR_LVL, B.CLASS_NM, A.STR, A.DEX, A.VITAL, A.ENERGY, A.CUR_EXP, C.REQ_EXP";
//		sql += " FROM TB_CHAR A, TB_CLASS B, TB_REQ_EXP C";
//		sql += " WHERE A.CLASS_SEQ = B.CLASS_SEQ";
//		sql += " AND A.CHAR_LVL = C.LVL";
//		sql += " AND A.USER_SEQ = ?;";
//
//		pstmt = con->prepareStatement(sql.c_str());
//		pstmt->setString(1, user_seq.c_str());
//
//		cout << "Execute Query :" << sql.c_str() << endl;
//		rs = pstmt->executeQuery();
//
//		while (rs->next())
//		{
//			map<string, string> row;
//			row.insert(pair<string, string>("CHAR_SEQ", rs->getString("CHAR_SEQ").c_str()));
//			row.insert(pair<string, string>("USER_SEQ", rs->getString("USER_SEQ").c_str()));
//			row.insert(pair<string, string>("CLASS_SEQ", rs->getString("CLASS_SEQ").c_str()));
//			row.insert(pair<string, string>("CHAR_NM", rs->getString("CHAR_NM").c_str()));
//			row.insert(pair<string, string>("CHAR_LVL", rs->getString("CHAR_LVL").c_str()));
//			row.insert(pair<string, string>("CLASS_NM", rs->getString("CLASS_NM").c_str()));
//			row.insert(pair<string, string>("STR", rs->getString("STR").c_str()));
//			row.insert(pair<string, string>("DEX", rs->getString("DEX").c_str()));
//			row.insert(pair<string, string>("VITAL", rs->getString("VITAL").c_str()));
//			row.insert(pair<string, string>("ENERGY", rs->getString("ENERGY").c_str()));
//			row.insert(pair<string, string>("CUR_EXP", rs->getString("CUR_EXP").c_str()));
//			row.insert(pair<string, string>("REQ_EXP", rs->getString("REQ_EXP").c_str()));
//			retArry.push_back(row);
//		}
//
//		delete pstmt;
//	}
//	catch (SQLException& e)
//	{
//		printf_s("[ERROR] Query 실행 실패 ECode : %d, ErrorMsg : %s\n", e.getErrorCode(), e.getSQLState().c_str());
//	}
//
//
//	return retArry;
//}
//
//bool DataAccess::InsertCharacter(const PlayerVO& vo)
//{
//	bool bRet = false;
//
//	try
//	{
//		PreparedStatement* pstmt;
//
//		string sql = "INSERT INTO TB_CHAR(user_seq, class_seq, char_nm, char_lvl) ";
//		sql += "VALUES ( ?, ?, ?, 1)";
//
//		pstmt = con->prepareStatement(sql.c_str());
//
//		pstmt->setString(1, vo.USER_SEQ.c_str());
//		pstmt->setString(2, vo.CLASS_SEQ.c_str());
//		pstmt->setString(3, vo.CHAR_NM.c_str());
//
//		cout << "Execute Query :" << sql.c_str() << endl;
//
//		bRet = pstmt->executeUpdate();
//
//	}
//	catch (SQLException& e)
//	{
//		printf_s("[ERROR] Query 실행 실패 ECode : %d\n", e.getErrorCode());
//	}
//
//	return bRet;
//
//}
//
//bool DataAccess::DeleteCharacter(const PlayerVO& vo)
//{
//	bool bRet = false;
//	try
//	{
//		PreparedStatement* pstmt;
//
//		string sql = "DELETE FROM TB_CHAR ";
//		sql += "WHERE USER_SEQ = ? ";
//		sql += "AND CHAR_SEQ = ?;";
//
//		pstmt = con->prepareStatement(sql.c_str());
//		pstmt->setString(1, vo.USER_SEQ.c_str());
//		pstmt->setString(2, vo.CHAR_SEQ.c_str());
//
//		cout << "Execute Query :" << sql.c_str() << endl;
//
//		bRet = pstmt->executeUpdate();
//
//		delete pstmt;
//
//	}
//	catch (SQLException& e)
//	{
//		printf_s("[ERROR] Query 실행 실패 ECode : %d\n", e.getErrorCode());
//	}
//	
//	return bRet;
//}
//
//
//
//
