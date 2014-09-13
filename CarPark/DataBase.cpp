#include "stdafx.h"
#include "DataBase.h"

CDataBase::CDataBase()
{
	m_pConnection = NULL;
}

CDataBase::~CDataBase()
{
	Close();
}

int CDataBase::Open(char* CnnStr) 
{
	try
	{
		HRESULT hr;
		//hr = ::CoInitialize(NULL);//初始化OLE/COM库环境
		//if(FAILED(hr))
		//	return -1;
		hr=m_pConnection.CreateInstance(__uuidof(Connection));
		if(FAILED(hr))
		{
			_com_error e(hr);
			AfxMessageBox(e.ErrorMessage());
			return -1;
		}

		//CString connectionStr=_T("DRIVER={SQL Server};SERVER=(local);DATABASE=Angell;UID=sa;PWD=123456"); 
		hr=m_pConnection->Open(CnnStr,"","",adModeUnknown); 

		if(FAILED(hr)){
			return -1;
		}else
		{
			return 0;
		}
	} 
	catch(_com_error &e) 
	{
		MessageBox(NULL,e.Description(),NULL,NULL);
		return -1;
	} 
}

int CDataBase::Close()
{
	HRESULT hr = -1;
	if(m_pConnection->State)
		hr = m_pConnection->Close();
	//::CoUninitialize(); 
	m_pConnection = NULL;
	return hr;
}

_RecordsetPtr CDataBase::GetRecordset(_bstr_t sql)
{
	if(!m_pConnection->State)
		return NULL;

	try
	{
		_RecordsetPtr pRecordSet=NULL;
		HRESULT hr = pRecordSet.CreateInstance(_uuidof(Recordset));
		if(FAILED(hr))
			return NULL;
		hr = pRecordSet->Open(sql,_variant_t((IDispatch*)m_pConnection,true)
			,adOpenDynamic,adLockOptimistic,adCmdText);
		if(!FAILED(hr))
			return pRecordSet;
	}
	catch(_com_error &e)
	{
		MessageBox(NULL,e.Description(),NULL,NULL);
	}
	return NULL;
}

_RecordsetPtr CDataBase::ExecuteSQL(_bstr_t sql)
{
	if(!m_pConnection->State)
		return NULL;

	try{
		_CommandPtr pCommandPtr = NULL;
		HRESULT hr = pCommandPtr.CreateInstance(_uuidof(Command));
		if(FAILED(hr))
			return NULL;
		hr = pCommandPtr->put_ActiveConnection(_variant_t((IDispatch*)m_pConnection,true));
		if(FAILED(hr))
			return NULL;
		pCommandPtr->CommandText = sql;
		_RecordsetPtr pRecordSet = pCommandPtr->Execute(NULL,NULL,adCmdText);
		return pRecordSet;
	}
	catch(_com_error &e){
		MessageBox(NULL,e.Description(),NULL,NULL);
		return NULL;
	}
}