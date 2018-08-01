#include "stdafx.h"
#include "XMLSerializable.h"
#include "StructSerializer.h"


CXMLSerializable::CXMLSerializable()
{
	m_strTypeName = GetTypeName();
	m_oXmlTypeInfo.m_strFieldName	= m_strTypeName;
	m_oXmlTypeInfo.m_eDataType		= XMLDataTypeComplex;
	m_bInfoInitialized = false;
}

bool CXMLSerializable::SerializeXML( CSXMLSerializer &oXmlSerializer, const LPCTSTR szFieldName/*= NULL*/ )
{
	try
	{
		CString strTypeTag = "";
		strTypeTag.Format( "<%s></%s>", GetTypeName(),  GetTypeName() );

		if( !oXmlSerializer.Initialize( (_bstr_t)strTypeTag ) )
			return false;

		// Ако сериализираме член променлива на обекта добавяме нов клон в дървото
		if( szFieldName != NULL && !oXmlSerializer.AddChildNode( szFieldName ) )
			return false;

		// Сериализираме 
		if( !SerializeFields( oXmlSerializer ) )
		{
			CSMsg::Log( eLog,  "Error serializing fields for '{0}' [CXMLSerializable::Serialize]!", &MSGKEY( "0", szFieldName == NULL ?  GetTypeName() : szFieldName  ) );
			return false;
		}// if

		// Ако сериализираме член променлива на обекта се връщаме един клон нагоре
		if( szFieldName != NULL && !oXmlSerializer.MoveOneNodeUp() )
			return false;

		oXmlSerializer.SetRootNodeXSIAttribute( "http://www.w3.org/2001/XMLSchema-instance" );
		oXmlSerializer.SetRootNodeXSDAttribute( "http://www.w3.org/2001/XMLSchema" );

		if( !oXmlSerializer.AddProcessingNode() )
			return false;
	}// try
	catch( HRESULT hr )
	{
		_com_error er( hr );
		CSMsg::Log( eLog,  er.ErrorMessage() );
		return false;
	}// catch
	catch(...)
	{
		CSMsg::Log( eLog,  "TRY CATCH in [CXMLSerializable::Serialize]!" );
		return false;
	}// catch

	return true;
}

bool CXMLSerializable::SerializeFields( CSXMLSerializer &oXmlSerializer )
{
	CStructSerializer oStructSerializer;
	const CXmlTypeInfo & oTypeInfo = GetTypeInfo();
	for( int i = 0; i < oTypeInfo.m_oArrInfos.GetCount(); i++ )
	{
		if( !oStructSerializer.Serialize( oXmlSerializer, (BYTE*)GetPoiner(), GetSize(), *oTypeInfo.m_oArrInfos[i] ) )
			return false;
	}// for

	return true;
}

bool CXMLSerializable::DeserializeFields( CSXMLSerializer &oXmlSerializer )
{
	CStructSerializer oStructSerializer;

	const CXmlTypeInfo & oTypeInfo = GetTypeInfo();
	for( int i = 0; i < oTypeInfo.m_oArrInfos.GetCount(); i++ )
	{
		if( !oStructSerializer.Deserialize( oXmlSerializer, (BYTE*)GetPoiner(), GetSize(), *oTypeInfo.m_oArrInfos[i] ) )
			return false;
	}// for

	return true;
}

bool CXMLSerializable::DeserializeXML( CSXMLSerializer &oXmlSerializer, IN const _bstr_t * pbstrXml /*= NULL*/, LPCSTR szFieldName/*= NULL*/ )
{
	try
	{
		if( pbstrXml && !oXmlSerializer.Initialize( *pbstrXml ) )
			return false;
		
		// Ако де-сериализираме член променлива на обекта търсим неговия клон
		if( szFieldName != NULL && !oXmlSerializer.GetChildNode( szFieldName ) )
			return false;

		// де-сериализираме член променливите
		if( !DeserializeFields( oXmlSerializer ) )
		{
			CSMsg::Log( eLog,  "Error deserializing fields for '{0}' [CXMLSerializable::Deserialize]!", &MSGKEY( "0", szFieldName == NULL ?  GetTypeName() : szFieldName  ) );
			return false;
		}// if

		// Ако де-сериализираме член променлива на обекта се връщаме един клон нагоре
		if( szFieldName != NULL && !oXmlSerializer.MoveOneNodeUp() )
			return false;
		
	}// try
	catch( HRESULT hr )
	{
		_com_error er( hr );
		CSMsg::Log( eLog,  er.ErrorMessage() );
		return false;
	}// catch
	catch(...)
	{
		CSMsg::Log( eLog,  "TRY CATCH in [CXMLSerializable::Serialize]!" );
		return false;
	}// catch


	return true;
}



void CXMLSerializable::SetDataType( const XMLDataType	eDataType )
{
	m_oXmlTypeInfo.m_eDataType = eDataType;
}

INT_PTR CXMLSerializable::AddSimpleInfo(const XMLDataType	eDataType,  
										const LPCTSTR	szFieldName,
										const int		nOffset, 
										const int		nSize /*= 0*/, 
										const LPCTSTR	szSubTypeName /*= NULL*/ )
{
	return  m_oXmlTypeInfo.m_oArrInfos.AddSimpleInfo( eDataType, szFieldName, nOffset, nSize, szSubTypeName );
}

INT_PTR CXMLSerializable::AddComplexInfo( const CXmlTypeInfo &	oTypeInfo,
										  const LPCTSTR	szFieldName,
										  const int		nOffset )
{
	return m_oXmlTypeInfo.m_oArrInfos.AddComplexInfo( oTypeInfo, szFieldName, nOffset );
}

INT_PTR CXMLSerializable::AddCPtrArrayInfo(const LPCTSTR		szFieldName,
										   const int			nOffset,
										   const LPCTSTR		szSubTypeFieldName,
										   const XMLDataType	eSubTypeDataType,  
										   const CXmlTypeInfo &oSubTypeInfo,
										   const int			nSubTypeSize )
{
	return m_oXmlTypeInfo.m_oArrInfos.AddCPtrArrayInfo( szFieldName, nOffset, szSubTypeFieldName, eSubTypeDataType, oSubTypeInfo, nSubTypeSize );
}


const CXmlTypeInfo & CXMLSerializable::GetTypeInfo()
{
	if( !m_bInfoInitialized )
	{
		FillTypeInfo();
		m_bInfoInitialized = true;
	}// if

	return m_oXmlTypeInfo;
}

LPCTSTR CXMLSerializable::GetTypeName() const
{
	return NULL;
}

const long CXMLSerializable::GetSize() const
{
	return -1;
}

void * CXMLSerializable::GetPoiner() const
{
	return NULL;
}

void CXMLSerializable::FillTypeInfo()
{
}


//////////////////////////////////////////////////////////////////////////
//class INICXmlSerilizable
void INICXmlSerilizable::FillTypeInfo()
{
	AddSimpleInfo( XMLDataTypeChar ,"inic", offsetof( INIC, inic ), INIC_SIZE );
}

//////////////////////////////////////////////////////////////////////////
//class FDATEXmlSerilizable 

void FDATEXmlSerilizable::FillTypeInfo()
{
	AddSimpleInfo( XMLDataTypeDouble ,"m_Date", offsetof( FDATE, m_Date ) );
}

//////////////////////////////////////////////////////////////////////////
//class  CSRECHDRXmlSerilizable 
void CSRECHDRXmlSerilizable::FillTypeInfo()
{
	AddSimpleInfo( XMLDataTypeShort,	"ChekSum",	offsetof( CSRECHDR, ChekSum	)	);
	AddSimpleInfo( XMLDataTypeSTATUS,	"sts",		offsetof( CSRECHDR, sts		), 4 );
	AddSimpleInfo( XMLDataTypeLong,		"trkod",	offsetof( CSRECHDR, trkod	)	);
}

//////////////////////////////////////////////////////////////////////////
//class  CSHISTINFOXmlSerilizable  
void CSHISTINFOXmlSerilizable::FillTypeInfo()
{
	AddComplexInfo( INICXmlSerilizable().GetTypeInfo(),   "m_ini",		offsetof( CSHISTINFO, m_ini ) );
	AddComplexInfo( FDATEXmlSerilizable().GetTypeInfo(),  "m_fdate",	offsetof( CSHISTINFO, m_fdate ) );
}