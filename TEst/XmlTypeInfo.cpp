#include "stdafx.h"
#include "XmlTypeInfo.h"
#include "XMLSerializable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// class CXMLTypeInfoArray
INT_PTR CXMLTypeInfoArray::AddSimpleInfo(	const XMLDataType	eDataType,  
											const LPCTSTR		szFieldName,
											const int			nOffset, 
											const int			nSize /*= 0*/, 
											const LPCTSTR		szSubTypeName /*= NULL*/ )
{
	CanAddOffset( nOffset, szFieldName );

	return Add( new CXmlTypeInfo( eDataType, szFieldName, nOffset, nSize, szSubTypeName ) );
}

INT_PTR CXMLTypeInfoArray::AddComplexInfo( const CXmlTypeInfo &	oTypeInfo,
										   const LPCTSTR		szFieldName,
										   const int			nOffset		)
{
	CanAddOffset( nOffset, szFieldName );

	CXmlTypeInfo *pInfo = CreateNew();

	pInfo->m_eDataType		= XMLDataTypeComplex;
	pInfo->m_strFieldName	= szFieldName;
	pInfo->m_nOffset		= nOffset;
	pInfo->m_nSize			= 0;

	pInfo->m_oArrInfos.Clone( oTypeInfo.m_oArrInfos );

	return (INT_PTR)pInfo;
}

INT_PTR CXMLTypeInfoArray::AddCPtrArrayInfo(const LPCTSTR		szFieldName,
											const int			nOffset,
											const LPCTSTR		szSubTypeFieldName,
											const XMLDataType	eSubTypeDataType,  
											const CXmlTypeInfo &oSubTypeInfo,
											const int			nSubTypeSize
											 )
{
	CanAddOffset( nOffset, szFieldName );

	CXmlTypeInfo *pInfo = CreateNew();

	pInfo->m_eDataType		= XMLDataTypeCPtrArray;
	pInfo->m_strFieldName	= szFieldName;
	pInfo->m_nOffset		= nOffset;
	pInfo->m_nSize			= 0;
	pInfo->m_strSubTypeName = szSubTypeFieldName;

	CXmlTypeInfo *pSubTypeInfo	= new CXmlTypeInfo();

	pSubTypeInfo->m_strFieldName	= szSubTypeFieldName;
	pSubTypeInfo->m_eDataType		= eSubTypeDataType;
	pSubTypeInfo->m_nOffset			= 0;
	pSubTypeInfo->m_nSize			= nSubTypeSize;

	pInfo->m_oArrInfos.Add( pSubTypeInfo );

	pSubTypeInfo->m_oArrInfos.Clone( oSubTypeInfo.m_oArrInfos );

	return (INT_PTR)pInfo;
}

bool CXMLTypeInfoArray::CanAddOffset( const int nOffset, const LPCTSTR szFieldName )
{
	bool bResult = true;
	
	for( int i = 0; i< GetCount(); i++ )
	{
		if( GetAt(i)->m_nOffset == nOffset )
		{
			CSMsg::Log( eScreenLog, "Duplicated offset '{0}' for fields '{1}' and '{2}' in same type detected while adding type info!", 
				&MSGKEY( "0", nOffset ),
				&MSGKEY( "1", szFieldName ),
				&MSGKEY( "2",  GetAt( i )->m_strFieldName ) );
			bResult = false;
		}// if
	}// for

	return bResult;
}


//////////////////////////////////////////////////////////////////////////
// class CXmlTypeInfo
CXmlTypeInfo::CXmlTypeInfo(	const XMLDataType	eDataType,  
						 const LPCTSTR	szFieldName,
						 const int		nOffset, 
						 const int		nSize /*= 0*/, 
						 const LPCTSTR	szSubTypeName /*= NULL*/)
		 : m_nSize( nSize ) 
		 , m_nOffset( nOffset )
{
	if( szSubTypeName )
		m_strSubTypeName = szSubTypeName;

	m_strFieldName	= szFieldName;
	m_eDataType		= eDataType;
}

CXmlTypeInfo::CXmlTypeInfo( const CXmlTypeInfo & oInfo )
			: m_nSize( oInfo.m_nSize )
			, m_nOffset( oInfo.m_nOffset )
{
	m_strSubTypeName	= oInfo.m_strSubTypeName;
	m_eDataType			= oInfo.m_eDataType;
	m_strFieldName		= oInfo.m_strFieldName;

	for( int i = 0; i < oInfo.m_oArrInfos.GetCount(); i++ )
		m_oArrInfos.Add( new CXmlTypeInfo( *oInfo.m_oArrInfos.GetAt( i ) ) );
}

CXmlTypeInfo & CXmlTypeInfo::operator = ( const CXmlTypeInfo & oInfo )
{
	m_nSize				= oInfo.m_nSize;
	m_nOffset			= oInfo.m_nOffset;
	m_strSubTypeName	= oInfo.m_strSubTypeName;
	m_eDataType			= oInfo.m_eDataType;
	m_strFieldName		= oInfo.m_strFieldName;

	for( int i = 0; i < oInfo.m_oArrInfos.GetCount(); i++ )
		m_oArrInfos.Add( new CXmlTypeInfo( *oInfo.m_oArrInfos.GetAt( i ) ) );

	return *this;
}



const CXmlTypeInfo * CXmlTypeInfo::GetTypeInfo( const LPCTSTR szFieldName ) const
{
	if( m_strFieldName == szFieldName )
		return this;

	for( int i = 0; i < m_oArrInfos.GetCount(); i++ )
	{
		const CXmlTypeInfo * pTypeInfo = m_oArrInfos[i]->GetTypeInfo( szFieldName );

		if( pTypeInfo != NULL )
			return pTypeInfo;
	}// for

	return NULL;
}

bool  CXmlTypeInfo::IsSimpleType() const
{
	if( m_eDataType == XMLDataTypeNONE
		|| m_eDataType == XMLDataTypeComplex
		|| m_eDataType == XMLDataTypeCPtrArray )
		return false;

	return true;
}

bool  CXmlTypeInfo::IsCPtrArray() const
{
	return m_eDataType == XMLDataTypeCPtrArray;
}

bool  CXmlTypeInfo::IsComplexType() const
{
	return m_eDataType == XMLDataTypeComplex;
}


int CXmlTypeInfo::GetTypeSize() const
{
	int nSize = 0;
	switch( m_eDataType )
	{
		case XMLDataTypeNONE:
			nSize = 0; 
			break; 
		case XMLDataTypeInt:
			nSize = sizeof( int ); 
			break; 
		case XMLDataTypeLong:
			nSize = sizeof( long ); 
			break; 
		case XMLDataTypeShort:
			nSize = sizeof( short ); 
			break; 
		case XMLDataTypeWORD:
			nSize = sizeof( WORD ); 
			break; 
		case XMLDataTypeDWORD:
			nSize = sizeof( DWORD ); 
			break; 
		case XMLDataTypeDouble:
			nSize = sizeof( double ); 
			break; 
		case XMLDataTypeFloat:
			nSize = sizeof( float ); 
			break; 
		case XMLDataTypeChar:
			nSize = sizeof( char ); 
			break; 
		case XMLDataTypeSTATUS:
			nSize = sizeof( STATUS ); 
			break; 
		case XMLDataTypeCPtrArray:
			nSize = sizeof( CPtrArray ); 
			break; 
		case XMLDataTypeComplex:
			nSize = 0;
			break;
		default:
			nSize = 0;
	}// switch

	return nSize;
}

////////////////////////////////////////////////////////////////////////