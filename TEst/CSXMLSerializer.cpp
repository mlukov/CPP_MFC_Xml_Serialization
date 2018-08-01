#include "stdafx.h"
#include "CSXMLSerializer.h"
#include "StructSerializer.h"

#define XMLNS_XSI "xmlns:xsi"
#define XMLNS_XSD "xmlns:xsd"


CSXMLSerializer::CSXMLSerializer()
	: m_bInitialized( false )
	, m_bRootNodeXSIAttributeSet( false )
	, m_bRootNodeXSDAttributeSet( false )
	, m_bProcessingNodeAdded( false )
{
	m_strRootTagName	= "";
	m_pXMLDoc			= NULL;
	m_pRootNode			= NULL;
	m_pChildsList		= NULL;

}

bool CSXMLSerializer::Initialize( IN const _bstr_t &bstrXml ) 
{
	if( !m_bInitialized )
	{
		HRESULT hr = m_pXMLDoc.CreateInstance( __uuidof( MSXML2::DOMDocument40 ) );
		if( FAILED( hr ) )
		{
			_com_error er( hr );

			CString strMessage = "";
			strMessage.Format( "Error in CSXMLSerializer::Initialize : %s", er.ErrorMessage() );
			CSMsg::Log( eLog, strMessage );

			return false;
		}// if

		if( m_pXMLDoc->loadXML( bstrXml ) == VARIANT_FALSE )
		{
			CString strMessage = "";
			strMessage.Format( "Error creating parent element [CSXMLSerializer::Initialize]!" );
			CSMsg::Log( eLog,  strMessage );
			return false;
		}// if

		// Get the root element just created    
		m_pRootNode = m_pXMLDoc->GetdocumentElement();

		if( !m_pRootNode )
		{
			CSMsg::Log( eLog,  "Error Getting root node from document! [CSXMLSerializer::Initialize]" );
			return false;
		}// if

		m_pCurrentNode = m_pRootNode;

		m_bInitialized = true;
	}// if

	return true;
}

bool CSXMLSerializer::IsInitialized() const
{
	return m_bInitialized;
}

void CSXMLSerializer::SetRootNodeXSIAttribute( LPCTSTR szValue )
{
	if( m_bRootNodeXSIAttributeSet )
		return;

	if( m_pRootNode == NULL )
		return;

	m_pRootNode->setAttribute( XMLNS_XSI, szValue );
}

void CSXMLSerializer::SetRootNodeXSDAttribute( LPCTSTR szValue )
{
	if ( m_bRootNodeXSDAttributeSet )
		return;

	if( m_pRootNode == NULL )
		return;

	m_pRootNode->setAttribute( XMLNS_XSD, szValue );
}

bool CSXMLSerializer::AddProcessingNode()
{
	if( !m_bProcessingNodeAdded )
	{
		// Add processing info
		MSXML2::IXMLDOMProcessingInstructionPtr pXMLProcessingNode =    
			m_pXMLDoc->createProcessingInstruction( "xml", " version='1.0' encoding='UTF-16'");

		if( !pXMLProcessingNode )
		{
			CSMsg::Log( eLog,  "Error creating xml processing node! [CSXMLSerializer::AddProcessingInfoNode]" );
			return false;
		}// if

		_variant_t vtObject;
		vtObject.vt = VT_DISPATCH;
		vtObject.pdispVal = m_pRootNode;
		vtObject.pdispVal->AddRef();

		// Insert processing info before root node
		MSXML2::IXMLDOMNodePtr pProcessingNode = NULL;
		pProcessingNode = m_pXMLDoc->insertBefore( pXMLProcessingNode, vtObject );
		
		if( !pProcessingNode )
		{
			CSMsg::Log( eLog,  "Error inserting xml processing node! [CSXMLSerializer::AddProcessingInfoNode]" );
			return false;
		}// if
	}// if

	return true;
}

bool CSXMLSerializer::SerializeField( const LPCSTR szFieldName, const LPCTSTR szValue )
{
	CString strBuf = "";
	strBuf.Format( "%s", szFieldName );

	MSXML2::IXMLDOMElementPtr pNode =  m_pXMLDoc->createElement( (_bstr_t)strBuf ); 
	if( !pNode )
	{
		CString strMessage="";
		strMessage.Format( "Error creating node '%s'! [CSXMLSerializer::SerializeType]!", strBuf );
		CSMsg::Log( eLog,  strMessage );
		return false;
	}// if

	pNode->Puttext( ( _bstr_t)strBuf  );

	MSXML2::IXMLDOMElementPtr pAddedNode = m_pCurrentNode->appendChild( pNode );

	if( !pAddedNode )	
	{
		CSMsg::Log( eLog,  "Error serializing field '{0}'! [CSXMLSerializer::SerializeField]", &MSGKEY( "0", szFieldName ) );
		return false;
	}// if

	return true;
}

bool CSXMLSerializer::SerializeStruct( IN BYTE		*	pStruct
									 , IN long			lStructSize
									 , IN LPCTSTR		szStructName
									 , IN const LPCSTR	szFieldName /*= NULL */)
{
	//return CStructSerializer().Serialize( *this, pStruct, lStructSize, szStructName, szFieldName );
	return false;
}


bool CSXMLSerializer::DeserializeField( const LPCSTR szFieldName, OUT LPCTSTR szValue, const int lSize )
{
	CString strBuf = "";
	strBuf.Format( "%s", szFieldName );

	if( !GetChildNode( szFieldName ) )
		return false;

	BSTR bstrCurNodeText;

	if( !GetCurNodeText( bstrCurNodeText) )
		return false;

	CString strValue = "";
	_bstr_t bstrBuf( bstrCurNodeText );
	strValue = (LPCTSTR)bstrBuf;

	ourstrncpy( (char*)szValue, strValue, lSize -  1 );

	if( !MoveOneNodeUp() )
		return false;

	return true;
}

bool CSXMLSerializer::DeserializeField( const LPCSTR szFieldName, OUT long lValue )
{
	CString strBuf = "";
	strBuf.Format( "%s", szFieldName );

	if( !GetChildNode( szFieldName ) )
		return false;

	BSTR bstrCurNodeText;

	if( !GetCurNodeText( bstrCurNodeText) )
		return false;

	CString strValue = "";
	_bstr_t bstrBuf( bstrCurNodeText );
	strValue = (LPCTSTR)bstrBuf;

	lValue = atol( strValue );

	if( !MoveOneNodeUp() )
		return false;

	return true;
}

bool CSXMLSerializer::DeserializeField( const LPCSTR szFieldName, OUT double dValue )
{
	CString strBuf = "";
	strBuf.Format( "%s", szFieldName );

	if( !GetChildNode( szFieldName ) )
		return false;

	BSTR bstrCurNodeText;

	if( !GetCurNodeText( bstrCurNodeText) )
		return false;

	CString strValue = "";
	_bstr_t bstrBuf( bstrCurNodeText );
	strValue = (LPCTSTR)bstrBuf;

	dValue = atof( strValue );

	if( !MoveOneNodeUp() )
		return false;

	return true;
}

bool CSXMLSerializer::DeserializeField( const LPCSTR szFieldName, OUT float fValue )
{
	CString strBuf = "";
	strBuf.Format( "%s", szFieldName );

	if( !GetChildNode( szFieldName ) )
		return false;

	BSTR bstrCurNodeText;

	if( !GetCurNodeText( bstrCurNodeText) )
		return false;

	CString strValue = "";
	_bstr_t bstrBuf( bstrCurNodeText );
	strValue = (LPCTSTR)bstrBuf;

	fValue = (float)atof( strValue );

	if( !MoveOneNodeUp() )
		return false;

	return true;
}

bool CSXMLSerializer::DeserializeStruct( IN BYTE		*	pStruct
									   , IN long			lStructSize
									   , IN LPCTSTR			szStructName
									   , IN const LPCTSTR	szFieldName /*= NULL*/
									   , IN const _bstr_t*	pBstrXml	/*= NULL*/ 
									   , IN const bool		bDontSearchChild /*= false */)
{

	//return CStructSerializer().Deserialize( *this, pStruct, lStructSize, szStructName, szFieldName, pBstrXml, bDontSearchChild );
	return false;
}

bool CSXMLSerializer::GetCurNodeText( OUT BSTR &bstr )
{
	if( !m_pCurrentNode )
	{
		CSMsg::Log( eLog,  "Error - No current node! [CSXMLSerializer::GetCurNodeText]" );
		return false;
	}// if

	HRESULT hr = m_pCurrentNode->get_text( &bstr );

	if( FAILED( hr ) )
	{
		CSMsg::Log( eLog,  "Error Get cur node text! [CSXMLSerializer::GetCurNodeText]" );
		return false;
	}// if

	return true;
}

/// <summary>Задаване на текста на текущият клон на дървото</summary>
/// <returns>Функцията връща true при успех и false при грешка</returns>
bool CSXMLSerializer::SetCurNodeText( const LPCTSTR szNodeText )
{
	CString strBuf = "";
	strBuf.Format( "%s", szNodeText );

	if( !m_pCurrentNode )
	{
		CSMsg::Log( eLog,  "Error - No current node! [CSXMLSerializer::SetCurNodeText]" );
		return false;
	}// if

	m_pCurrentNode->Puttext( ( _bstr_t)strBuf );

	return true;
}

bool CSXMLSerializer::AddChildNode( const LPCTSTR szFieldName )
{
	MSXML2::IXMLDOMElementPtr pNode = m_pXMLDoc->createElement( (_bstr_t)szFieldName ); 

	if( !pNode )
	{
		CSMsg::Log( eLog,  "Error creating node for field '{0}'! [CSXMLSerializer::AddChildNode]", &MSGKEY( "0", szFieldName ) );
		return false;
	}// if

	pNode = m_pCurrentNode->appendChild( pNode );
	m_pCurrentNode = pNode;

	if( !m_pCurrentNode )
	{
		CSMsg::Log( eLog,  "Error appending node for field '{0}'! [CSXMLSerializer::AddChildNode]", &MSGKEY( "0",  szFieldName ) );
		return false;
	}// if

	return true;
}

long CSXMLSerializer::GetChildNodes()
{
	if( !m_pCurrentNode )
	{
		CSMsg::Log( eLog,  "Error - No current node! [CSXMLSerializer::GetChildNodes]" );
		return FALSE;	
	}// if

	MSXML2::IXMLDOMNodeListPtr pChildsList = m_pCurrentNode->GetchildNodes();

	if( !pChildsList )
		return 0;

	m_pChildsList = pChildsList;

	return m_pChildsList->Getlength();
}

bool CSXMLSerializer::GetChildNodeAt( const long lIndex )
{
	MSXML2::IXMLDOMNodePtr pChild;

	if( !m_pChildsList )
	{
		CSMsg::Log( eLog,  "Error - No current child node list! [CSXMLSerializer::GetChildNodeAt]" );
		return false;	
	}// if

	if( lIndex >= m_pChildsList->Getlength() )
		return false;

	HRESULT hr = S_OK;
	hr = m_pChildsList->get_item( lIndex, &pChild );

	if( FAILED( hr ) || !pChild )
		return false;

	m_pCurrentNode = pChild;

	return true;
}

bool CSXMLSerializer::GetChildNode( const LPCTSTR szFieldName )
{
	MSXML2::IXMLDOMElementPtr pNode;
	BSTR bstrTypeName;
	CString strTypeName ="";
	HRESULT hr = S_OK;

	MSXML2::IXMLDOMNodeListPtr pChildsList = m_pCurrentNode->GetchildNodes();
	MSXML2::IXMLDOMNodePtr pChild;

	for( long l = 0; l < pChildsList->Getlength(); l++ )
	{
		hr = pChildsList->get_item( l, &pChild );

		if( FAILED( hr ) || !pChild )
			continue;

		hr = pChild->get_baseName( &bstrTypeName );
		_bstr_t bstrBuf( bstrTypeName );

		strTypeName = (LPCTSTR)bstrBuf ;

		if( !FAILED( hr ) && strncmp( strTypeName, szFieldName, strTypeName.GetLength()  ) == 0 )
		{
			pNode = pChild;
			break;
		}// if
	}// for

	if( !pNode )
	{
		CSMsg::Log( eLog,  "Error finding child node '{0}'! [CSXMLSerializer::GetChildNode]", &MSGKEY( "0", szFieldName ) );
		return false;
	}// if

	m_pCurrentNode = pNode;
	
	return true;
}

bool CSXMLSerializer::MoveOneNodeUp()
{
	MSXML2::IXMLDOMElementPtr pParentNode = NULL;

	pParentNode = m_pCurrentNode->GetparentNode();
	if( !pParentNode )
	{
		CSMsg::Log( eLog,  "Error getting parent node for node '{0}'! [CSXMLSerializer::AddChildNode]", &MSGKEY( "0", m_pCurrentNode->GetnodeName() ) );
		return false;
	}// if

	m_pCurrentNode = pParentNode;
	
	return true;
}

bool CSXMLSerializer::GetXML( OUT _bstr_t &bstrXml )
{
	try
	{
		bstrXml = m_pXMLDoc->Getxml();
	}
	catch( HRESULT hr )
	{
		_com_error er( hr );
		CSMsg::Log( eLog,  er.ErrorMessage() );
		return false;
	}// catch

	return true;
}