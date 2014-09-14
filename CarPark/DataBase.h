#pragma once
//#import "C:\Program Files\Common Files\System\ado\msado15.dll"  rename("EOF","adoEOF"), rename("BOF","adoBOF")  
#import "\\192.168.10.5\Toolkits\ado-XP\msado15.dll"  rename("EOF","adoEOF"), rename("BOF","adoBOF")         
using   namespace   ADODB;

class CDataBase
{
public:
	CDataBase();
	~CDataBase();
	int Open(char* CnnStr);
	int Close();
	_RecordsetPtr GetRecordset(_bstr_t sql);
	_RecordsetPtr ExecuteSQL(_bstr_t sql);
private:
	char m_sConnection[256];
	_ConnectionPtr m_pConnection;
};
