#include "stdafx.h"
#include "StructSerializer.h"


CStructSerializer::CStructSerializer()
{
}

//////////////////////////////////////////////////////////////////////////
// De-serialization
bool CStructSerializer::Deserialize( CSXMLSerializer	&	oXmlSerializer
									, BYTE				*	pStruct
									, const long			lStructSize
									, const CXmlTypeInfo&	oXmlInfo
									, const _bstr_t		*	pBstrXml /*= NULL*/
									, const bool			bDontSearchChild /*= false*/)
{
	if( !pStruct || lStructSize <= 0 )
		return false;

	try
	{
		CString strNodeName = "";
		// Ако де-сериализираме член обекти на обекта търсим неговия клон
		strNodeName.Format( "%s", oXmlInfo.m_strFieldName );

		if( pBstrXml && !oXmlSerializer.Initialize( *pBstrXml ) )
			return false;

		if( !DeserializeType( oXmlSerializer, pStruct, lStructSize, oXmlInfo, 0 ) )
		{
			CSMsg::Log( eLog,  "Error Deserialize Type: '{0}' ![{1}]"
							, &MSGKEY( "0", strNodeName )
							, &MSGKEY( "1", __FUNCTION__ ) 
						);
			return false;
		}// if

	}// try
	catch( HRESULT hr )
	{
		_com_error er( hr );
		CSMsg::Log( eLog,  er.ErrorMessage() );
		return false;
	}// catch

	return true;
}

bool CStructSerializer::DeserializeType( IN CSXMLSerializer		&	oXmlSerializer,
											BYTE				*	pStruct,
											const long				lStructSize,
											const CXmlTypeInfo	&	oXmlInfo,
											const long				nOffset, 
											const bool				bDontSearchChild /*= false*/)
{
	if( !bDontSearchChild && !oXmlSerializer.GetChildNode( oXmlInfo.m_strFieldName ) )
		return false;

	if( oXmlInfo.IsSimpleType() ) // IS SIMPLE
	{
		if( !SetValueFromSerializer( oXmlSerializer, pStruct, lStructSize, oXmlInfo, nOffset ) )
		{
			CSMsg::Log( eLog, "Error get value for Type: '{0}', field: '{1}' field type: '{2}'  [{3}]!" 
							, &MSGKEY( "0", oXmlInfo.m_strSubTypeName )
							, &MSGKEY( "1", oXmlInfo.m_strFieldName )
							, &MSGKEY( "2", oXmlInfo.m_eDataType )
							, &MSGKEY( "3", __FUNCTION__ ) 
					);
			return false;
		}// if

	}// if
	else if( oXmlInfo.IsCPtrArray() )
	{
		if( oXmlInfo.m_oArrInfos.GetCount() <= 0 )
		{
			CSMsg::Log( eLog, "No elements in m_oArrInfos array for CPtrArray with subtype '{3}' type '{0}' data type '{1}' in [{2}]!"
				, &MSGKEY( "0", oXmlInfo.m_strFieldName )
				, &MSGKEY( "1", oXmlInfo.m_eDataType )
				, &MSGKEY( "2", __FUNCTION__ ) 
				, &MSGKEY( "3", oXmlInfo.m_strSubTypeName ) 
				);
			return false;
		}// if

		const CXmlTypeInfo *pSubType = oXmlInfo.m_oArrInfos.GetAt( 0 );
		if( !pSubType )
		{
			CSMsg::Log( eLog, "Null element in m_oArrInfos array for CPtrArray field '{0}' data type '{1}' in [{2}]!"
				, &MSGKEY( "0", oXmlInfo.m_strFieldName )
				, &MSGKEY( "1", oXmlInfo.m_eDataType )
				, &MSGKEY( "2", __FUNCTION__ ) 
				);
			return false;
		}// if

		int nCalcOffset = nOffset + oXmlInfo.m_nOffset;

		if( nCalcOffset + oXmlInfo.GetTypeSize() > lStructSize  )
		{
			CSMsg::Log( eLog, "Offset plus field size exceeds the size of struct! struct size {ss},  calc.size {cs} .Data type: '{0}', field: '{1}' field type: '{2}'  [{3}]!"
				, &MSGKEY( "ss", lStructSize )
				, &MSGKEY( "cs", nCalcOffset + oXmlInfo.GetTypeSize() )
				, &MSGKEY( "1", oXmlInfo.m_eDataType )
				, &MSGKEY( "1", oXmlInfo.m_strFieldName )
				, &MSGKEY( "2", oXmlInfo.m_eDataType )
				, &MSGKEY( "3", __FUNCTION__ ) 
				);
			return false;
		}// if

		CPtrArray * pArray = (CPtrArray *)(pStruct + nCalcOffset);
		if( !pArray )
		{
			CSMsg::Log( eLog, "Null poiner after casting  poniter + offset:{cs} ! Data type: '{0}', field: '{1}' field type: '{2}'  [{3}]!"
				, &MSGKEY( "cs", nCalcOffset )
				, &MSGKEY( "1", oXmlInfo.m_eDataType )
				, &MSGKEY( "1", oXmlInfo.m_strFieldName )
				, &MSGKEY( "2", oXmlInfo.m_eDataType )
				, &MSGKEY( "3", __FUNCTION__ ) 
				);
			return false;
		}// if

		// Взимаме подклоните на текущия(на масива)
		int nChildNodesCount = oXmlSerializer.GetChildNodes();
		for( int i=0; i < nChildNodesCount; i++ )
		{
			if( !oXmlSerializer.GetChildNodeAt( i ) )
				return false;

			BYTE * pNewElement = new BYTE[ pSubType->m_nSize ];

			if( !pNewElement )
				return false;

			pArray->Add( pNewElement );

			if( !DeserializeType( oXmlSerializer, pNewElement, pSubType->m_nSize, *pSubType, 0, true ) )
			{
				CSMsg::Log( eLog, "Error de-serializing field '{0}' of type: '{1}'  [{2}]!" 
					, &MSGKEY( "0", oXmlInfo.m_strFieldName )
					, &MSGKEY( "1", oXmlInfo.m_eDataType )
					, &MSGKEY( "2", __FUNCTION__ ) 
					);
				return false;
			}// if
		}// for

		// Връщаме се един клон нагоре
		if( !oXmlSerializer.MoveOneNodeUp() )
			return false;

	}// else if
	else if( oXmlInfo.IsComplexType() )
	{
		for( int i = 0; i < oXmlInfo.m_oArrInfos.GetCount(); i++ )
		{
			const CXmlTypeInfo *pSubType = oXmlInfo.m_oArrInfos.GetAt( i );
			if( !pSubType )
			{
				CSMsg::Log( eLog, "Null element in m_oArrInfos array for type '{0}' data type '{1}' in [{2}]!"
								, &MSGKEY( "0", oXmlInfo.m_strFieldName )
								, &MSGKEY( "1", oXmlInfo.m_eDataType )
								, &MSGKEY( "2", __FUNCTION__ ) 
							);
				return false;
			}// if

			if( !DeserializeType( oXmlSerializer,  pStruct, lStructSize, *pSubType, nOffset + oXmlInfo.m_nOffset  ) )
			{
				CSMsg::Log( eLog, "Error de-serializing field '{0}' of type: '{1}'  [{2}]!" 
								, &MSGKEY( "0", oXmlInfo.m_strFieldName )
								, &MSGKEY( "1", oXmlInfo.m_eDataType )
								, &MSGKEY( "2", __FUNCTION__ ) 
							);
				return false;
			}// if

		}// for
	}// else if

	if( !bDontSearchChild && !oXmlSerializer.MoveOneNodeUp() )
		return false;

	return true;
}


bool CStructSerializer::SetValueFromSerializer( CSXMLSerializer		&	oSerializer
											   , BYTE				*	pStruct
											   , const long				lStructSize
											   , const CXmlTypeInfo&	oXmlInfo
											   , const long				nOffset ) 
{
	try
	{
		BSTR bstr;
		oSerializer.GetCurNodeText( bstr );

		CString strValue = "";
		_bstr_t bstrBuf( bstr );
		strValue = (LPCTSTR)bstrBuf;

		int nTypeSize = oXmlInfo.GetTypeSize();

		if( nTypeSize <= 0 )
		{
			CSMsg::Log( eLog, "Error get size for simple type: '{0}', field '{1}' in type '{2}' ! [{3}]"
				, &MSGKEY( "0", oXmlInfo.m_eDataType )
				, &MSGKEY( "1", oXmlInfo.m_strFieldName )
				, &MSGKEY( "2", oXmlInfo.m_strSubTypeName )
				, &MSGKEY( "3", __FUNCTION__ ) );

			return false;
		}// if

		if( nOffset + oXmlInfo.m_nOffset + nTypeSize > lStructSize )
		{
			CSMsg::Log( eLog, "Size plus offset bigger than struct! Field '{0}' Type: '{1}',  in type '{2}' ! [{3}]",
						&MSGKEY( "1", oXmlInfo.m_strFieldName ),
						&MSGKEY( "0", oXmlInfo.m_eDataType ),
						&MSGKEY( "2", oXmlInfo.m_strSubTypeName ),
						&MSGKEY( "3", __FUNCTION__ ) );
			return false;
		}// if

		BYTE * pBuf = pStruct + nOffset + oXmlInfo.m_nOffset;

		if( !strValue.IsEmpty() )
		{
			if( oXmlInfo.m_eDataType == XMLDataTypeInt )
			{
				int *pNVal = (int*)( pBuf  );
				*pNVal = atoi( strValue );
			}// if
			else if( oXmlInfo.m_eDataType == XMLDataTypeLong )
			{
				long *pLVal = (long*)( pBuf );
				*pLVal = atol( strValue );
			}// if
			else if( oXmlInfo.m_eDataType ==XMLDataTypeShort )
			{
				short *pSVal = (short*)(pBuf );
				*pSVal = atoi( strValue );
			}// if
			else if( oXmlInfo.m_eDataType == XMLDataTypeWORD )
			{
				WORD *pWVal = (WORD*)( pBuf );
				*pWVal = (WORD)atol( strValue );
			}// if
			else if( oXmlInfo.m_eDataType ==XMLDataTypeDWORD )
			{
				DWORD *pDWVal = (DWORD*)( pBuf );
				*pDWVal = (DWORD)atol( strValue );
			}// if
			else if( oXmlInfo.m_eDataType == XMLDataTypeDouble )
			{
				double *pDVal = (double*)( pBuf );
				*pDVal = atof( strValue );
			}// if
			else if( oXmlInfo.m_eDataType == XMLDataTypeFloat )
			{
				float *pFVal = (float*)( pBuf );
				*pFVal = (float)atof( strValue );
			}// if
			else if( oXmlInfo.m_eDataType == XMLDataTypeChar )
			{
				ourstrncpy( (char*)( pBuf ), strValue, oXmlInfo.m_nSize -  1  );
			}// if
			else if( oXmlInfo.m_eDataType == XMLDataTypeSTATUS )
			{
				CSBitVector oBitVector( STATUSSIZE, (char*)( pBuf ) );
				oBitVector.InitFromString( strValue, strValue.GetLength() );
			}// else
		}// if
	}// try
	catch( ... )
	{
		CSMsg::Log( eLog,  "TRY-CATCH for field '{0}' of type '{1}' in type '{2}' in [{3}]!"
						, &MSGKEY( "0", oXmlInfo.m_strFieldName )
						, &MSGKEY( "1", oXmlInfo.m_eDataType )
						, &MSGKEY( "2", oXmlInfo.m_strSubTypeName ) 
						, &MSGKEY( "3", __FUNCTION__ ) 
					);
		return false;
	}// catch

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Serialization
bool CStructSerializer::Serialize( IN	CSXMLSerializer		&	oXmlSerializer,
								  const BYTE			*	pStruct,
								  const long				lStructSize,
								  const CXmlTypeInfo	&	oXmlInfo,
								  const LPCSTR				szFieldName /*= NULL */)
{

	CString strTypeTag = "", strTypeName = "";
	strTypeName = szFieldName ? szFieldName : oXmlInfo.m_strFieldName;
	strTypeTag.Format( "<%s></%s>"	, strTypeName, strTypeName );

	if( !oXmlSerializer.IsInitialized() && !oXmlSerializer.Initialize( (_bstr_t)strTypeTag ) )
	{
		return false;
	}// if
	
	if( !SerializeType( oXmlSerializer, pStruct, lStructSize, oXmlInfo, 0 ) )
	{
		CSMsg::Log( eLog, "Error Serializing type: '{0}' ![{1}]"
						, &MSGKEY( "0", strTypeName )
						, &MSGKEY( "1", __FUNCTION__ ) 
					);
		return false;
	}// if


	return true;
}

bool CStructSerializer::SerializeType(IN	CSXMLSerializer	&	oXmlSerializer,
									  const BYTE			*	pStruct,
									  const long				lStructSize,
									  const CXmlTypeInfo	&	oXmlInfo,
									  const long				nOffset ) 
{

	CString strBuf = "";

	// create branch for array
	if( !oXmlSerializer.AddChildNode( oXmlInfo.m_strFieldName ) )
		return false;

	if( oXmlInfo.IsSimpleType() ) // IS SIMPLE
	{
		if( !GetValue( pStruct, lStructSize, strBuf, oXmlInfo, nOffset ) )
		{
			CSMsg::Log( eLog, "Error get value for field: '{1}' field type: '{2}'  [{3}]!"
							, &MSGKEY( "1", oXmlInfo.m_strFieldName )
							, &MSGKEY( "2", oXmlInfo.m_eDataType )
							, &MSGKEY( "3", __FUNCTION__ ) 
						);

			return false;
		}// if

		// Add Value to node
		if( !oXmlSerializer.SetCurNodeText( strBuf ) )
		{
			CSMsg::Log( eLog, "Error set current node text for field: '{1}' field type: '{2}'  [{3}]!"
							, &MSGKEY( "1", oXmlInfo.m_strFieldName )
							, &MSGKEY( "2", oXmlInfo.m_eDataType )
							, &MSGKEY( "3", __FUNCTION__ ) 
						);
			return false;
		}// if


	}// if
	else if( oXmlInfo.IsCPtrArray() )
	{
		if( oXmlInfo.m_oArrInfos.GetCount() <= 0 )
		{
			CSMsg::Log( eLog, "No elements in m_oArrInfos array for CPtrArray with subtype '{3}' type '{0}' data type '{1}' in [{2}]!"
							, &MSGKEY( "0", oXmlInfo.m_strFieldName )
							, &MSGKEY( "1", oXmlInfo.m_eDataType )
							, &MSGKEY( "2", __FUNCTION__ ) 
							, &MSGKEY( "3", oXmlInfo.m_strSubTypeName ) 
						);
			return false;
		}// if

		const CXmlTypeInfo *pSubType = oXmlInfo.m_oArrInfos.GetAt( 0 );
		if( !pSubType )
		{
			CSMsg::Log( eLog, "Null element in m_oArrInfos array for CPtrArray field '{0}' data type '{1}' in [{2}]!"
				, &MSGKEY( "0", oXmlInfo.m_strFieldName )
				, &MSGKEY( "1", oXmlInfo.m_eDataType )
				, &MSGKEY( "2", __FUNCTION__ ) 
				);
			return false;
		}// if

		int nCalcOffset = nOffset + oXmlInfo.m_nOffset;

		if( nCalcOffset + oXmlInfo.GetTypeSize() > lStructSize  )
		{
			CSMsg::Log( eLog, "Offset plus field size exceeds the size of struct! struct size {ss},  calc.size {cs} .Data type: '{0}', field: '{1}' field type: '{2}'  [{3}]!"
			, &MSGKEY( "ss", lStructSize )
			, &MSGKEY( "cs", nCalcOffset + oXmlInfo.GetTypeSize() )
			, &MSGKEY( "1", oXmlInfo.m_eDataType )
			, &MSGKEY( "1", oXmlInfo.m_strFieldName )
			, &MSGKEY( "2", oXmlInfo.m_eDataType )
			, &MSGKEY( "3", __FUNCTION__ ) 
			);
			return false;
		}// if

		const CPtrArray *pArray = (CPtrArray *)(pStruct + nCalcOffset);
		if( !pArray )
		{
			CSMsg::Log( eLog, "Null poiner after casting  poniter + offset:{cs} ! Data type: '{0}', field: '{1}' field type: '{2}'  [{3}]!"
				, &MSGKEY( "cs", nCalcOffset )
				, &MSGKEY( "1", oXmlInfo.m_eDataType )
				, &MSGKEY( "1", oXmlInfo.m_strFieldName )
				, &MSGKEY( "2", oXmlInfo.m_eDataType )
				, &MSGKEY( "3", __FUNCTION__ ) 
				);
			return false;
		}// if

		const CPtrArray &oArray = *pArray;
		
		for( int i=0; i < oArray.GetCount(); i++ )
		{
			if( !SerializeType( oXmlSerializer, (BYTE*)oArray.GetAt( i ), pSubType->m_nSize,  *pSubType, 0 ) )
			{
				CSMsg::Log( eLog, "Error serializing field: '{1}' field type: '{2}'  [{3}]!"
					, &MSGKEY( "1", pSubType->m_strFieldName )
					, &MSGKEY( "2", pSubType->m_eDataType )
					, &MSGKEY( "3", __FUNCTION__ ) 
					);
				return false;
			}// if
		}// for

	}// else if
	else if( oXmlInfo.IsComplexType() )
	{
		for( int i=0; i < oXmlInfo.m_oArrInfos.GetCount(); i++ )
		{
			const CXmlTypeInfo *pSubType = oXmlInfo.m_oArrInfos[i];
			if( !pSubType )
			{
				CSMsg::Log( eLog, "Null element in m_oArrInfos array for field '{0}' data type '{1}' in [{2}]!"
					, &MSGKEY( "0", oXmlInfo.m_strFieldName )
					, &MSGKEY( "1", oXmlInfo.m_eDataType )
					, &MSGKEY( "2", __FUNCTION__ ) 
					);
				return false;
			}// if

			if( !SerializeType( oXmlSerializer, pStruct, lStructSize, *pSubType, nOffset + oXmlInfo.m_nOffset ) )
			{
				CSMsg::Log( eLog, "Error serializing field: '{1}' field type: '{2}'  [{3}]!"
								, &MSGKEY( "1", pSubType->m_strFieldName )
								, &MSGKEY( "2", pSubType->m_eDataType )
								, &MSGKEY( "3", __FUNCTION__ ) 
							);
				return false;
			}// if
		}// for
	}// else

	if( !oXmlSerializer.MoveOneNodeUp() )
		return false;
	
	return true;
}


bool CStructSerializer::GetValue(  const BYTE * pStruct
								 , const long lStructSize
								 , CString &strValue
								 , const CXmlTypeInfo &oXmlInfo
								 , const int nOffset ) 
{
	strValue = "";

	int nTypeSize = oXmlInfo.GetTypeSize();

	if( nTypeSize <= 0 )
	{
		CSMsg::Log( eLog, "Error get size for simple type '{0}', field: '{1}' field type: '{2}'  [{3}]!"
						, &MSGKEY( "1", oXmlInfo.m_eDataType )
						, &MSGKEY( "1", oXmlInfo.m_strFieldName )
						, &MSGKEY( "2", oXmlInfo.m_eDataType )
						, &MSGKEY( "3", __FUNCTION__ )
					);
		return false;
	}// if

	try
	{
		int nCalcOffset = nOffset + oXmlInfo.m_nOffset ;

		if( nCalcOffset + nTypeSize >  lStructSize ) 
		{
			CSMsg::Log( eLog, "Offset plus field size exceeds the size of struct! struct size {ss},  calc.size {cs} .Data type: '{0}', field: '{1}' field type: '{2}'  [{3}]!"
							, &MSGKEY( "ss", lStructSize )
							, &MSGKEY( "cs", nCalcOffset + nTypeSize )
							, &MSGKEY( "1", oXmlInfo.m_eDataType )
							, &MSGKEY( "1", oXmlInfo.m_strFieldName )
							, &MSGKEY( "2", oXmlInfo.m_eDataType )
							, &MSGKEY( "3", __FUNCTION__ ) 
						);

			return false;
		}// if

		const BYTE * pBuf = pStruct + nCalcOffset;

		if( oXmlInfo.m_eDataType == XMLDataTypeInt )
		{
			int nVal = 0;
			nVal =  (*(int*)( pBuf ));
			strValue.Format( "%d", nVal );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeLong )
		{
			long lVal = 0;
			lVal =  (*(long*)( pBuf ));
			strValue.Format( "%d", lVal );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeShort )
		{
			short sVal = 0;
			sVal =  (*(short*)( pBuf ));
			strValue.Format( "%d", sVal );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeWORD )
		{
			WORD wVal = 0;
			wVal =  (*(WORD*)( pBuf ));
			strValue.Format( "%d", wVal );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeDWORD )
		{
			DWORD dwVal = 0;
			dwVal =  (*(DWORD*)( pBuf ));
			strValue.Format( "%d", dwVal );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeDouble )
		{
			double dVal = 0;
			dVal =  (*(double*)( pBuf ));
			strValue.Format( "%f", dVal );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeFloat )
		{
			float fVal = 0;
			fVal =  (*(float*)( pBuf ));
			strValue.Format( "%f", fVal );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeChar )
		{
			strValue.Format( "%s", ((char*)( pBuf ) ) );
		}// if
		else if( oXmlInfo.m_eDataType == XMLDataTypeSTATUS ) 
		{
			CSBitVector oBitVector( STATUSSIZE , (char*)( pBuf ) );
			strValue.Format( "%s", oBitVector.ToString() );
		}// else
	}// try
	catch( ... )
	{
		CSMsg::Log( eLog,  "TRY-CATCH for field '{0}' of type '{1}' in type '{2}' in [{3}]!"
						, &MSGKEY( "0", oXmlInfo.m_strFieldName )
						, &MSGKEY( "1", oXmlInfo.m_eDataType )
						, &MSGKEY( "2", oXmlInfo.m_strSubTypeName ) 
						, &MSGKEY( "3", __FUNCTION__ ) 
				);
		return false;
	}// catch

	return true;
}