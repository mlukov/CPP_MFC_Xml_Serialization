// PreferencialTaxes.cpp
//
#include "stdafx.h"
#include "TaxAutomationStructs.h"
#include "PreferencialTaxes.h"
#include "TypeInfo.h"

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES

PREFERENCIAL_TAXES::PREFERENCIAL_TAXES()
{
	memset( this, 0, sizeof( *this ) );
}

PREFERENCIAL_TAXES::PREFERENCIAL_TAXES( const PREFERENCIAL_TAXES & recPrefTax )
{
	memset( this, 0, sizeof( *this ) );
	*this = recPrefTax;
}

LPCTSTR PREFERENCIAL_TAXES::GetType()
{
	if( IsIndividual() == TRUE )
		return CSMsg::GetMsgUsrLang( MSG_TYPE_PREFERENCIAL_TAX_INDIVIDUAL );
	else if( IsKlientskaPref() == TRUE ) 
		return CSMsg::GetMsgUsrLang( MSG_TYPE_PREFERENCIAL_TAX_KLIENTSKA );	
	else 
		return CSMsg::GetMsgUsrLang( MSG_TYPE_PREFERENCIAL_TAX_STANDART );
	
	return CSMsg::GetMsgUsrLang( MSG_TYPE_PREFERENCIAL_TAX_STANDART );
}

long PREFERENCIAL_TAXES::GetKeyValue() const 
{ 
	return m_lCode; 
}
	
long PREFERENCIAL_TAXES::SetKeyValue( const long lKeyValue )
{ 
	m_lCode = lKeyValue; 
	return m_lCode; 
}

CString PREFERENCIAL_TAXES::GetProcessingSystemsAsString() const
{
	CString strValues="";

	CSNomMsgIDCode oProcSystem( g_arrProcessingSystem, _countof( g_arrProcessingSystem ) );

	for( INT_PTR i = 0, nCnt = oProcSystem.GetSize(); i < nCnt; i++ )
	{
		if( ( i == nCnt - 1 ) && oProcSystem.LastIsEmpty() )
			break;

		int nCode = oProcSystem.GetCodeByIndex( i );

		if( nCode == (int)eUndefinedProcessingSystem )
			continue;

		if( CheckProcessingSystemBit( nCode )  )
		{
			if( strValues.GetLength() > 0 )
				strValues += "; ";

			strValues += oProcSystem.GetMsgUserLangByIndex( i );
		}// if
	}// for

	return strValues;
}

///////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_TAXESXmlSerializable
void PREFERENCIAL_TAXESXmlSerializable::FillTypeInfo()
{
	FDATEXmlSerilizable oFdateSer;
	AddComplexInfo( CSRECHDRXmlSerilizable().GetTypeInfo(),		"m_Hdr",				offsetof( PREFERENCIAL_TAXES, m_Hdr )	);
	AddComplexInfo( CSHISTINFOXmlSerilizable().GetTypeInfo(),	"m_histInfo",			offsetof( PREFERENCIAL_TAXES, m_histInfo )	);

	AddSimpleInfo( XMLDataTypeLong,								"m_lCode",				offsetof( PREFERENCIAL_TAXES, m_lCode ) );
	AddSimpleInfo( XMLDataTypeChar,								"m_szPreferenceName",	offsetof( PREFERENCIAL_TAXES, m_szPreferenceName ),		sizeof( PREFERENCIAL_TAXES().m_szPreferenceName )		);
	AddSimpleInfo( XMLDataTypeShort,							"m_sPreferenceStatus",	offsetof( PREFERENCIAL_TAXES, m_sPreferenceStatus ) );	
	AddSimpleInfo( XMLDataTypeInt,								"m_nDealType",			offsetof( PREFERENCIAL_TAXES, m_nDealType ) );

	AddComplexInfo( oFdateSer.GetTypeInfo(),					"m_fDateValidFrom",		offsetof( PREFERENCIAL_TAXES, m_fDateValidFrom )  );
	AddComplexInfo( oFdateSer.GetTypeInfo(),					"m_fDateValidTo",		offsetof( PREFERENCIAL_TAXES, m_fDateValidTo ) );
	AddComplexInfo( oFdateSer.GetTypeInfo(),					"m_fDateOpenFrom",		offsetof( PREFERENCIAL_TAXES, m_fDateOpenFrom ) );
	AddComplexInfo( oFdateSer.GetTypeInfo(),					"m_fDateOpenTo",		offsetof( PREFERENCIAL_TAXES, m_fDateOpenTo ) );

	AddSimpleInfo( XMLDataTypeShort,							"m_sPeriodType",		offsetof( PREFERENCIAL_TAXES, m_sPeriodType ) );
	AddSimpleInfo( XMLDataTypeInt,								"m_nNumMonths",			offsetof( PREFERENCIAL_TAXES, m_nNumMonths ) );
	AddSimpleInfo( XMLDataTypeInt,								"m_nNumDays",			offsetof( PREFERENCIAL_TAXES, m_nNumDays ) );
	AddSimpleInfo( XMLDataTypeShort,							"m_sPeriodicity",		offsetof( PREFERENCIAL_TAXES, m_sPeriodicity ) );

	AddComplexInfo( INICXmlSerilizable().GetTypeInfo(),			"m_iniDeactivate",		offsetof( PREFERENCIAL_TAXES, m_iniDeactivate ) );
	AddComplexInfo( oFdateSer.GetTypeInfo(),					"m_fDeactivateDate",	offsetof( PREFERENCIAL_TAXES, m_fDeactivateDate ) );

	AddSimpleInfo( XMLDataTypeLong,		"m_lStdPreferenceCode",	offsetof( PREFERENCIAL_TAXES, m_lStdPreferenceCode ) );
	AddSimpleInfo( XMLDataTypeSTATUS,	"m_stsProcessingSystem",offsetof( PREFERENCIAL_TAXES, m_stsProcessingSystem ),	sizeof( PREFERENCIAL_TAXES().m_stsProcessingSystem )	);	
	AddSimpleInfo( XMLDataTypeDouble,	"m_fGratisSumBgn",		offsetof( PREFERENCIAL_TAXES, m_fGratisSumBgn ) );
	AddSimpleInfo( XMLDataTypeInt,		"m_nGratisMonths",		offsetof( PREFERENCIAL_TAXES, m_nGratisMonths ) );
}


/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_SPR
PREFERENCIAL_TAXES_SPR::PREFERENCIAL_TAXES_SPR()
{
	memset( this, 0, sizeof( *this ) );
}

PREFERENCIAL_TAXES_SPR::PREFERENCIAL_TAXES_SPR( const PREFERENCIAL_TAXES &recPrefTax )
{
	memset( this, 0, sizeof( *this ) );

	PREFERENCIAL_TAXES * pRec = (PREFERENCIAL_TAXES *)this;

	if( !pRec )
		return;

	*pRec = recPrefTax;

	m_bDependsOnDateOpen	= recPrefTax.m_Hdr.CheckBit( STS_DEPENDS_ON_DATE_OPEN ) ? true : false;
}

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_DEAL_SPR
PREFERENCIAL_TAXES_TO_DEAL_SPR::PREFERENCIAL_TAXES_TO_DEAL_SPR()
{
	memset( this, 0, sizeof( *this ) );
}

PREFERENCIAL_TAXES_TO_DEAL_SPR::PREFERENCIAL_TAXES_TO_DEAL_SPR( const PREFERENCIAL_TAXES &recPrefTax )
{
	memset( this, 0, sizeof( *this ) );

	PREFERENCIAL_TAXES * pRec = (PREFERENCIAL_TAXES *)this;

	if( !pRec )
		return;

	*pRec = recPrefTax;
}

/////////////////////////////////////////////////////////////////////////////
// class CPreferencialTaxFullRec
CPreferencialTaxFullRec::CPreferencialTaxFullRec()
{
}

CPreferencialTaxFullRec::CPreferencialTaxFullRec( const CPreferencialTaxFullRec & oPrefTaxFullRec )
{
	Copy( oPrefTaxFullRec );
}

void CPreferencialTaxFullRec::Copy( const CPreferencialTaxFullRec & oPrefTaxFullRec )
{
	m_recPrefTax		= oPrefTaxFullRec.m_recPrefTax;

	m_recPrefToDeal		=oPrefTaxFullRec.m_recPrefToDeal;

	m_arrPlan.Release();
	m_arrPlan.Copy( oPrefTaxFullRec.m_arrPlan );

	m_arrEventsTaxes.Release();
	m_arrEventsTaxes.Copy( oPrefTaxFullRec.m_arrEventsTaxes );

	m_arrTaxesToStdDeals.Release();
	m_arrTaxesToStdDeals.Copy( oPrefTaxFullRec.m_arrTaxesToStdDeals );
}

BOOL CPreferencialTaxFullRec::Serialize( ISerializer &serializer ) const 
{
	serializer.Write( (LPVOID)&m_recPrefTax, sizeof( m_recPrefTax ) );

	serializer.Write( (LPVOID)&m_recPrefToDeal, sizeof( m_recPrefToDeal ) );

	if( !m_arrPlan.Serialize( serializer ) )
	{
		CSMsg::Log( eScreenLog, MSG_ERR_SERIALIZING_ITEM_IN_FUNCTION, 
				&MSGKEY( "0", "CPreferencePlanArray" ), &MSGKEY("1", "CPreferencialTaxFullRec::Serialize 1" ) );
		return FALSE;
	}// if

	if( !m_arrEventsTaxes.Serialize( serializer ) )		
	{
		CSMsg::Log( eScreenLog, MSG_ERR_SERIALIZING_ITEM_IN_FUNCTION, 
				&MSGKEY( "0", "CPreferenceEventsTaxesArray" ), &MSGKEY("1", "CPreferencialTaxFullRec::Serialize 2" ) );
		return FALSE;
	}// if

	if( !m_arrTaxesToStdDeals.Serialize( serializer ) )
	{
		CSMsg::Log( eScreenLog, MSG_ERR_SERIALIZING_ITEM_IN_FUNCTION, 
			&MSGKEY( "0", "CPreferenceTaxesToStdDealsArray" ), &MSGKEY("1", "CPreferencialTaxFullRec::Serialize 3" ) );
		return FALSE;
	}
	
	return TRUE;
}

BOOL CPreferencialTaxFullRec::Deserialize( ISerializer &serializer )
{
	try
	{
		long lSize = 0;

		m_recPrefTax = *(PREFERENCIAL_TAXES*)serializer.Read( lSize ) ;

		if( lSize != sizeof( m_recPrefTax ) )
		{
			CSMsg::Log( eScreenLog, MSG_ERR_DESERIALIZE_ITEM_IN_FUNC, 
						&MSGKEY("0", "PREFERENCIAL_TAXES"), 
						&MSGKEY("1", "CPreferencialTaxFullRec::Deserialize") );
			return FALSE;
		}// if

		m_recPrefToDeal = *(PREFERENCIAL_TAXES_TO_DEALS*)serializer.Read( lSize ) ;

		if( lSize != sizeof( m_recPrefToDeal ) )
		{
			CSMsg::Log( eScreenLog, MSG_ERR_DESERIALIZE_ITEM_IN_FUNC, 
				&MSGKEY("0", "PREFERENCIAL_TAXES_TO_DEALS"), 
				&MSGKEY("1", "CPreferencialTaxFullRec::Deserialize") );
			return FALSE;
		}// if

		if( !m_arrPlan.Deserialize( serializer ) )
		{
			CSMsg::Log( eScreenLog, MSG_ERR_DESERIALIZE_ITEM_IN_FUNC, 
				&MSGKEY("0", "CPreferencePlanArray"), 
				&MSGKEY("1", "CPreferencialTaxFullRec::Deserialize") );

			return FALSE;	
		}// if

		if( !m_arrEventsTaxes.Deserialize( serializer ) )
		{	
			CSMsg::Log( eScreenLog, MSG_ERR_DESERIALIZE_ITEM_IN_FUNC, 
				&MSGKEY("0", "CPreferenceEventsTaxesArray"), 
				&MSGKEY("1", "CPreferencialTaxFullRec::Deserialize") );
	
			return FALSE;
		}// if

		if( !m_arrTaxesToStdDeals.Deserialize( serializer ) )
		{
			CSMsg::Log( eScreenLog, MSG_ERR_DESERIALIZE_ITEM_IN_FUNC, 
				&MSGKEY("0", "CPreferenceTaxesToStdDealsArray"), 
				&MSGKEY("1", "CPreferencialTaxFullRec::Deserialize") );

			return FALSE;
		}// if
		
	}//try
	catch(...)
	{
		return FALSE;
	}// catch

	return TRUE;
}

LPCTSTR CPreferencialTaxFullRec::GetTypeName() const 
{ 
	return "CPreferencialTaxFullRec"; 
}

void CPreferencialTaxFullRec::FillTypeInfo()
{ 
	AddComplexInfo( PREFERENCIAL_TAXESXmlSerializable().GetTypeInfo(),			"m_recPrefTax",		offsetof( CPreferencialTaxFullRec, m_recPrefTax )		);
	AddComplexInfo( PREFERENCIAL_TAXES_TO_DEALSXmlSerializable().GetTypeInfo(),	"m_recPrefToDeal",	offsetof( CPreferencialTaxFullRec, m_recPrefToDeal )	);
	
	AddCPtrArrayInfo(	"m_arrPlan",			
						offsetof( CPreferencialTaxFullRec, m_arrPlan ) ,
						"PREFERENCIAL_PLANS",
						XMLDataTypeComplex,
						PREFERENCIAL_PLANSXmlSerializable().GetTypeInfo(),
						sizeof( PREFERENCIAL_PLANS ) 
					);
						
	AddCPtrArrayInfo(	"m_arrEventsTaxes",	
						offsetof( CPreferencialTaxFullRec, m_arrEventsTaxes ),
						"PREFERENCIAL_EVENTS_TAXES",
						XMLDataTypeComplex,
						PREFERENCIAL_EVENTS_TAXESXmlSerializable().GetTypeInfo(), 
						sizeof( PREFERENCIAL_EVENTS_TAXES ) 
					);		
						
	AddCPtrArrayInfo(	"m_arrTaxesToStdDeals",
						offsetof( CPreferencialTaxFullRec, m_arrTaxesToStdDeals ),
						"PREFERENCIAL_TAXES_TO_STD_DEALS",
						XMLDataTypeComplex,
						PREFERENCIAL_TAXES_TO_STD_DEALSXmlSerializable().GetTypeInfo(),
						sizeof( PREFERENCIAL_TAXES_TO_STD_DEALS ) 
					);	
}
//
//bool CPreferencialTaxFullRec::SerializeFields( CSXMLSerializer &oXmlSerializer )
//{
//	if( !oXmlSerializer.SerializeStruct( (BYTE*)&m_recPrefTax, sizeof( m_recPrefTax )
//										, "PREFERENCIAL_TAXES", "m_recPrefTax" ) )
//		return false;
//
//	if( !oXmlSerializer.SerializeStruct( (BYTE*)&m_recPrefToDeal, sizeof( m_recPrefToDeal )
//										, "PREFERENCIAL_TAXES_TO_DEALS", "m_recPrefToDeal" ) )
//		return false;
//
//	// CPreferencePlanArray
//	if( !oXmlSerializer.AddChildNode( "m_arrPlan" ) )
//		return false;
//	
//	for( int i = 0; i< m_arrPlan.GetCount(); i++ )
//	{
//		if( !oXmlSerializer.SerializeStruct( (BYTE*)m_arrPlan[i], sizeof( PREFERENCIAL_PLANS )
//											, "PREFERENCIAL_PLANS" ) )
//		return false;
//	}// for
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	//CPreferenceEventsTaxesArray
//	if( !oXmlSerializer.AddChildNode( "m_arrEventsTaxes" ) )
//		return false;
//
//	for( int i = 0; i< m_arrEventsTaxes.GetCount(); i++ )
//	{
//		if( !oXmlSerializer.SerializeStruct( (BYTE*)m_arrEventsTaxes[i], sizeof( PREFERENCIAL_EVENTS_TAXES )
//											, "PREFERENCIAL_EVENTS_TAXES" ) )
//			return false;
//	}// for
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	//CPreferenceTaxesToStdDealsArray
//	if( !oXmlSerializer.AddChildNode( "m_arrTaxesToStdDeals" ) )
//		return false;
//
//	for( int i = 0; i< m_arrTaxesToStdDeals.GetCount(); i++ )
//	{
//		if( !oXmlSerializer.SerializeStruct( (BYTE*)m_arrTaxesToStdDeals[i], sizeof( PREFERENCIAL_TAXES_TO_STD_DEALS )
//											, "PREFERENCIAL_TAXES_TO_STD_DEALS" ) )
//			return false;
//	}// for
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//	
//	return true;
//}
//
//bool CPreferencialTaxFullRec::DeserializeFields( CSXMLSerializer &oXmlSerializer )
//{
//
//	if( !oXmlSerializer.DeserializeStruct( (BYTE*)&m_recPrefTax, sizeof( m_recPrefTax )
//		, "PREFERENCIAL_TAXES", "m_recPrefTax" ) )
//		return false;
//
//	if( !oXmlSerializer.DeserializeStruct( (BYTE*)&m_recPrefToDeal, sizeof( m_recPrefToDeal )
//		, "PREFERENCIAL_TAXES_TO_DEALS", "m_recPrefToDeal" ) )
//		return false;
//
//	// CPreferencePlanArray
//	if( !oXmlSerializer.GetChildNode( "m_arrPlan" ) )
//		return false;
//	
//	long lPlanItemsCount = oXmlSerializer.GetChildNodes();
//
//	PREFERENCIAL_PLANS recPlanBuf;
//	m_arrPlan.Release();
//
//	for( int i = 0; i< lPlanItemsCount; i++ )
//	{
//		if( !oXmlSerializer.GetChildNodeAt( i ) )
//			return false;
//
//		if( !oXmlSerializer.DeserializeStruct( (BYTE*)&recPlanBuf, sizeof( PREFERENCIAL_PLANS )
//												, "PREFERENCIAL_PLANS", NULL, NULL, true ) )
//			return false;
//
//		PREFERENCIAL_PLANS *pPlan = new PREFERENCIAL_PLANS();
//		*pPlan = recPlanBuf;
//		m_arrPlan.Add( pPlan );
//	}// for
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	// CPreferenceEventsTaxesArray
//	if( !oXmlSerializer.GetChildNode( "m_arrEventsTaxes" ) )
//		return false;
//
//	long lEvtTaxesCount = oXmlSerializer.GetChildNodes();
//	m_arrEventsTaxes.Release();
//	PREFERENCIAL_EVENTS_TAXES recPrefEvtTaxes;
//
//	for( int i = 0; i< lEvtTaxesCount; i++ )
//	{
//		if( !oXmlSerializer.GetChildNodeAt( i ) )
//			return false;
//
//		if( !oXmlSerializer.DeserializeStruct( (BYTE*)&recPrefEvtTaxes, sizeof( PREFERENCIAL_EVENTS_TAXES )
//												, "PREFERENCIAL_EVENTS_TAXES", NULL, NULL, true ) )
//			return false;
//
//		PREFERENCIAL_EVENTS_TAXES *pRecPrefEvtTaxes = new PREFERENCIAL_EVENTS_TAXES();
//		m_arrEventsTaxes.Add( pRecPrefEvtTaxes );
//	}// for
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	// CPreferenceTaxesToStdDealsArray
//	if( !oXmlSerializer.GetChildNode( "m_arrTaxesToStdDeals" ) )
//		return false;
//
//	long lTaxesToStdCount = oXmlSerializer.GetChildNodes();
//	PREFERENCIAL_TAXES_TO_STD_DEALS recTaxStdDeal;
//	m_arrTaxesToStdDeals.Release();
//
//	for( int i = 0; i < lTaxesToStdCount; i++ )
//	{
//		if( !oXmlSerializer.GetChildNodeAt( i ) )
//			return false;
//
//		if( !oXmlSerializer.DeserializeStruct( (BYTE*)&recTaxStdDeal, sizeof( PREFERENCIAL_TAXES_TO_STD_DEALS )
//												, "PREFERENCIAL_TAXES_TO_STD_DEALS", NULL, NULL, true ) )
//			return false;
//
//		PREFERENCIAL_TAXES_TO_STD_DEALS *pRecTaxStd = new PREFERENCIAL_TAXES_TO_STD_DEALS();
//		*pRecTaxStd = recTaxStdDeal;
//		m_arrTaxesToStdDeals.Add( pRecTaxStd );
//	}// for
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	if( !oXmlSerializer.MoveOneNodeUp() )
//		return false;
//
//	return true;
//}

bool CPreferencialTaxFullRec::operator == ( const CPreferencialTaxFullRec & src ) const
{
	// PREFERENCIAL_TAXES
	const PREFERENCIAL_TAXES* pPrefTax = &this->m_recPrefTax;
	const PREFERENCIAL_TAXES* pPrefTaxRight = &src.m_recPrefTax;

	if( !pPrefTax || !pPrefTaxRight )
		return false;

	if( memcmp( pPrefTax, pPrefTaxRight, sizeof( PREFERENCIAL_TAXES ) ) )
		return false;

	// PREFERENCIAL_TAXES_TO_DEALS
	const PREFERENCIAL_TAXES_TO_DEALS* pPrefToDeal = &this->m_recPrefToDeal;
	const PREFERENCIAL_TAXES_TO_DEALS* pPrefToDealRight = &src.m_recPrefToDeal;

	if( !pPrefToDeal || !pPrefToDealRight )
		return false;

	if( memcmp( pPrefToDeal, pPrefToDealRight, sizeof( PREFERENCIAL_TAXES_TO_DEALS ) ) )
		return false;

	// CPreferencePlanArray
	const CPreferencePlanArray& oArrPlanNew = this->m_arrPlan;
	const CPreferencePlanArray& oArrPlanOld = src.m_arrPlan;

	CSCmnRecsArrayComparable< PREFERENCIAL_PLANS >::ResultArrayType ÓArrPlanResult;
	oArrPlanNew.Compare( oArrPlanOld, ÓArrPlanResult );

	for( INT_PTR i = 0 ; i < ÓArrPlanResult.GetCount() ; i++ )
	{
		STRUCT_STATE_EXTENDER_T< PREFERENCIAL_PLANS > *pRec = ÓArrPlanResult.GetAt( i );
		if( !pRec )
			continue;

		switch( pRec->GetState() )
		{
		case STRUCT_STATE_EXTENDER_T< PREFERENCIAL_PLANS >::eRecordAdded:
			{
				return false;
			} // case
		case STRUCT_STATE_EXTENDER_T< PREFERENCIAL_PLANS >::eRecordDeleted:
			{	
				return false;
			} // case
		default:
			break;
		} // switch
	} // for

	// CPreferenceEventsTaxesArray
	const CPreferenceEventsTaxesArray& oArrEventsTaxesNew = this->m_arrEventsTaxes;
	const CPreferenceEventsTaxesArray& oArrEventsTaxesOld = src.m_arrEventsTaxes;

	CSCmnRecsArrayComparable< PREFERENCIAL_EVENTS_TAXES >::ResultArrayType ÓArrEventsTaxesResult;
	oArrEventsTaxesNew.Compare( oArrEventsTaxesOld, ÓArrEventsTaxesResult );

	for( INT_PTR i = 0 ; i < ÓArrEventsTaxesResult.GetCount() ; i++ )
	{
		STRUCT_STATE_EXTENDER_T< PREFERENCIAL_EVENTS_TAXES > *pRec = ÓArrEventsTaxesResult.GetAt( i );
		if( !pRec )
			continue;

		switch( pRec->GetState() )
		{
		case STRUCT_STATE_EXTENDER_T< PREFERENCIAL_EVENTS_TAXES >::eRecordAdded:
			{
				return false;
			} // case
		case STRUCT_STATE_EXTENDER_T< PREFERENCIAL_EVENTS_TAXES >::eRecordDeleted:
			{	
				return false;
			} // case
		default:
			break;
		} // switch
	} // for

	// CPreferenceTaxesToStdDealsArray
	const CPreferenceTaxesToStdDealsArray& oArrTaxesToStdDealsNew = this->m_arrTaxesToStdDeals;
	const CPreferenceTaxesToStdDealsArray& oArrTaxesToStdDealsOld = src.m_arrTaxesToStdDeals;

	CSCmnRecsArrayComparable< PREFERENCIAL_TAXES_TO_STD_DEALS >::ResultArrayType ÓArrTaxesToStdDealsResult;
	oArrTaxesToStdDealsNew.Compare( oArrTaxesToStdDealsOld, ÓArrTaxesToStdDealsResult );

	for( INT_PTR i = 0 ; i < ÓArrTaxesToStdDealsResult.GetCount() ; i++ )
	{
		STRUCT_STATE_EXTENDER_T< PREFERENCIAL_TAXES_TO_STD_DEALS > *pRec = ÓArrTaxesToStdDealsResult.GetAt( i );
		if( !pRec )
			continue;

		switch( pRec->GetState() )
		{
		case STRUCT_STATE_EXTENDER_T< PREFERENCIAL_TAXES_TO_STD_DEALS >::eRecordAdded:
			{
				return false;
			} // case
		case STRUCT_STATE_EXTENDER_T< PREFERENCIAL_TAXES_TO_STD_DEALS >::eRecordDeleted:
			{	
				return false;
			} // case
		default:
			break;
		} // switch
	} // for

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_PLANS
PREFERENCIAL_PLANS::PREFERENCIAL_PLANS()
{
	memset( this, 0, sizeof( *this ) );
}
PREFERENCIAL_PLANS::PREFERENCIAL_PLANS( const PREFERENCIAL_PLANS & recPlan )
{
	memset( this, 0, sizeof( *this ) );
	*this = recPlan;
}

long PREFERENCIAL_PLANS::GetKeyValue() const 
{
	return m_lCode;
}

long PREFERENCIAL_PLANS::SetKeyValue( const long lKeyValue )
{
	m_lCode = lKeyValue;
	return m_lCode;
}

int PREFERENCIAL_PLANS::CompareStructs( const PREFERENCIAL_PLANS &oRec )
{
	 return GetKeyValue() - oRec.GetKeyValue();
}

int  PREFERENCIAL_PLANS::compareByEventCountSumOver( const void* p1, const void* p2 )
{
	PREFERENCIAL_PLANS* pPrefToDeal1 = *(PREFERENCIAL_PLANS**)p1;
	PREFERENCIAL_PLANS* pPrefToDeal2 = *(PREFERENCIAL_PLANS**)p2;

	if( pPrefToDeal1->m_fSumOver < pPrefToDeal2->m_fSumOver )
		return -1;

	if( pPrefToDeal1->m_fSumOver > pPrefToDeal2->m_fSumOver )
		return 1;

	if( pPrefToDeal1->m_nEventCountFrom < pPrefToDeal2->m_nEventCountFrom )
		return -1;

	if( pPrefToDeal1->m_nEventCountFrom > pPrefToDeal2->m_nEventCountFrom )
		return 1;

	return 0;
}

///////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_PLANSXmlSerializable
void PREFERENCIAL_PLANSXmlSerializable::FillTypeInfo()
{
	AddComplexInfo( CSRECHDRXmlSerilizable().GetTypeInfo(),	"m_Hdr",					offsetof( PREFERENCIAL_PLANS, m_Hdr ) );
	AddComplexInfo( CSHISTINFOXmlSerilizable().GetTypeInfo(),"m_histInfo",				offsetof( PREFERENCIAL_PLANS, m_histInfo ) );
	AddSimpleInfo( XMLDataTypeLong,							"m_lCode",					offsetof( PREFERENCIAL_PLANS, m_lCode )	);
	AddSimpleInfo( XMLDataTypeLong,							"m_lPreferenceCode",		offsetof( PREFERENCIAL_PLANS, m_lPreferenceCode ) );
	AddSimpleInfo( XMLDataTypeShort,						"m_sPreferenceType",		offsetof( PREFERENCIAL_PLANS, m_sPreferenceType ) );

	AddSimpleInfo( XMLDataTypeDouble,						"m_fPercent",				offsetof( PREFERENCIAL_PLANS, m_fPercent )	);
	AddSimpleInfo( XMLDataTypeDouble,						"m_fSumSolid",				offsetof( PREFERENCIAL_PLANS, m_fSumSolid )	);
	AddSimpleInfo( XMLDataTypeShort,						"m_sDependType",			offsetof( PREFERENCIAL_PLANS, m_sDependType )	);
	AddSimpleInfo( XMLDataTypeShort,						"m_sBalanceTurnoverType",	offsetof( PREFERENCIAL_PLANS, m_sBalanceTurnoverType ) );
	AddSimpleInfo( XMLDataTypeDouble,						"m_fSumOver",				offsetof( PREFERENCIAL_PLANS, m_fSumOver )	);

	AddSimpleInfo( XMLDataTypeInt,							"m_nEventCountFrom",		offsetof( PREFERENCIAL_PLANS, m_nEventCountFrom ) );
	AddSimpleInfo( XMLDataTypeInt,							"m_nEventCountTo",			offsetof( PREFERENCIAL_PLANS, m_nEventCountTo )	);
	AddSimpleInfo( XMLDataTypeInt,							"m_nPrefDealTypeCode",		offsetof( PREFERENCIAL_PLANS, m_nPrefDealTypeCode )	);
}

/////////////////////////////////////////////////////////////////////////////
// class CPreferenceListArray

int CPreferencePlanArray::GetMaxEventCountTo()
{
	int nMaxEventCountTo = 0;

	for( INT_PTR i =  0 ; i < GetCount() ; i++ )
		if( GetAt( i )->m_nEventCountTo > nMaxEventCountTo )
			nMaxEventCountTo = GetAt( i )->m_nEventCountTo;

	return nMaxEventCountTo;
}

/* m_sBalanceTurnover - eBalanceTurnoverType */ 
SUMA CPreferencePlanArray::GetMaxSumOver( const short m_sBalanceTurnover )
{
	SUMA fMaxSumOver = 0;

	for( INT_PTR i =  0 ; i < GetCount() ; i++ )
		if(  GetAt( i )->m_sBalanceTurnoverType == m_sBalanceTurnover && GetAt( i )->m_fSumOver > fMaxSumOver )
			fMaxSumOver = GetAt( i )->m_fSumOver;

	return fMaxSumOver;
}
/////////////////////////////////////////////////////////////////////////////
// class CPreferenceEventsTaxesArray

bool CPreferenceEventsTaxesArray::EventHasTaxes( const long lEventType )
{
	bool bResult = false;

	for( INT_PTR i =  0 ; i < GetCount() ; i++ )
		if(  GetAt( i )->m_lEventCode == lEventType && GetAt( i )->m_lTaxCode> 0 )
		{
			bResult = true;
			break;
		}// if

	return bResult;
}

bool CPreferenceEventsTaxesArray::HasDoubledTax( const long lTaxCode )
{
	if( lTaxCode <= 0 )
		return false;
	
	int nCounter =0;
	for( INT_PTR i = 0; i < GetCount() ; i++ )
		if( GetAt( i )->m_lTaxCode  == lTaxCode )
		{
			nCounter++;
			if( nCounter > 1 )
				return true;
		}// if

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_EVENTS_TAXES
PREFERENCIAL_EVENTS_TAXES::PREFERENCIAL_EVENTS_TAXES()
{
	memset( this, 0, sizeof( *this ) );
}
PREFERENCIAL_EVENTS_TAXES::PREFERENCIAL_EVENTS_TAXES( const PREFERENCIAL_EVENTS_TAXES & recPlan )
{
	memset( this, 0, sizeof( *this ) );
	*this = recPlan;
}

long PREFERENCIAL_EVENTS_TAXES::GetKeyValue() const 
{
	return m_lCode;
}

long PREFERENCIAL_EVENTS_TAXES::SetKeyValue( const long lKeyValue )
{
	m_lCode = lKeyValue;
	return m_lCode;
}

///////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_EVENTS_TAXESXmlSerializable
void PREFERENCIAL_EVENTS_TAXESXmlSerializable::FillTypeInfo()
{
	AddComplexInfo( CSRECHDRXmlSerilizable().GetTypeInfo(),		"m_Hdr",			offsetof( PREFERENCIAL_EVENTS_TAXES, m_Hdr ) );
	AddComplexInfo( CSHISTINFOXmlSerilizable().GetTypeInfo(),	"m_histInfo",		offsetof( PREFERENCIAL_EVENTS_TAXES, m_histInfo ) );
	AddSimpleInfo( XMLDataTypeLong,								"m_lCode",			offsetof( PREFERENCIAL_EVENTS_TAXES, m_lCode ) );
	AddSimpleInfo( XMLDataTypeLong,								"m_lPreferenceCode",offsetof( PREFERENCIAL_EVENTS_TAXES, m_lPreferenceCode ) );
	AddSimpleInfo( XMLDataTypeLong,								"m_lEventCode",		offsetof( PREFERENCIAL_EVENTS_TAXES, m_lEventCode ) );
	AddSimpleInfo( XMLDataTypeLong,								"m_lTaxCode",		offsetof( PREFERENCIAL_EVENTS_TAXES, m_lTaxCode ) );
}

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_STD_DEALS
PREFERENCIAL_TAXES_TO_STD_DEALS::PREFERENCIAL_TAXES_TO_STD_DEALS()
{
	memset( this, 0, sizeof( *this ) );
}
PREFERENCIAL_TAXES_TO_STD_DEALS::PREFERENCIAL_TAXES_TO_STD_DEALS( const PREFERENCIAL_TAXES_TO_STD_DEALS & recTaxStdDeal )
{
	memset( this, 0, sizeof( *this ) );
	*this = recTaxStdDeal;
}

long PREFERENCIAL_TAXES_TO_STD_DEALS::GetKeyValue() const 
{
	return m_lCode;
}

long PREFERENCIAL_TAXES_TO_STD_DEALS::SetKeyValue( const long lKeyValue )
{
	m_lCode = lKeyValue;
	return m_lCode;
}

//////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_TAXES_TO_STD_DEALSXmlSerializable
void PREFERENCIAL_TAXES_TO_STD_DEALSXmlSerializable::FillTypeInfo()
{
	AddComplexInfo( CSRECHDRXmlSerilizable().GetTypeInfo(),		"m_Hdr",					offsetof( PREFERENCIAL_TAXES_TO_STD_DEALS, m_Hdr )	);
	AddComplexInfo( CSHISTINFOXmlSerilizable().GetTypeInfo(),	"m_histInfo",				offsetof( PREFERENCIAL_TAXES_TO_STD_DEALS, m_histInfo )	);
	AddSimpleInfo( XMLDataTypeLong,								"m_lCode",					offsetof( PREFERENCIAL_TAXES_TO_STD_DEALS, m_lCode )	);
	AddSimpleInfo( XMLDataTypeLong,								"m_lDealType",				offsetof( PREFERENCIAL_TAXES_TO_STD_DEALS, m_lDealType )	);
	AddSimpleInfo( XMLDataTypeLong,								"m_lStdDogCode",			offsetof( PREFERENCIAL_TAXES_TO_STD_DEALS, m_lStdDogCode )	);
	AddSimpleInfo( XMLDataTypeLong,								"m_lPreferencialTaxCode",	offsetof( PREFERENCIAL_TAXES_TO_STD_DEALS, m_lPreferencialTaxCode )	);
}

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_STD_DEALS_EXT
PREFERENCIAL_TAXES_TO_STD_DEALS_EXT::PREFERENCIAL_TAXES_TO_STD_DEALS_EXT()
{
	memset( this, 0, sizeof( *this ) );
}

PREFERENCIAL_TAXES_TO_STD_DEALS_EXT::PREFERENCIAL_TAXES_TO_STD_DEALS_EXT( const PREFERENCIAL_TAXES_TO_STD_DEALS & recPrefTaxToStd )
{
	memset( this, 0, sizeof( *this ) );
	
	*(PREFERENCIAL_TAXES_TO_STD_DEALS*)this = recPrefTaxToStd;
}

PREFERENCIAL_TAXES_TO_STD_DEALS_EXT::PREFERENCIAL_TAXES_TO_STD_DEALS_EXT( const PREFERENCIAL_TAXES_TO_STD_DEALS_EXT & recPrefTaxToStdExt )
{
	memset( this, 0, sizeof( *this ) );
	*this = recPrefTaxToStdExt;
}

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_STD_DEALS
PREFERENCIAL_TAXES_TO_DEALS::PREFERENCIAL_TAXES_TO_DEALS()
{
	memset( this, 0, sizeof( *this ) );
}

PREFERENCIAL_TAXES_TO_DEALS::PREFERENCIAL_TAXES_TO_DEALS( const PREFERENCIAL_TAXES_TO_DEALS & recPrefToDeal )
{
	memset( this, 0, sizeof( *this ) );
	*this = recPrefToDeal;
}

long PREFERENCIAL_TAXES_TO_DEALS::GetKeyValue() const 
{
	return m_lCode;
}

long PREFERENCIAL_TAXES_TO_DEALS::SetKeyValue( const long lKeyValue )
{
	m_lCode = lKeyValue;
	return m_lCode;
}

//////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_TAXES_TO_DEALSXmlSerializable
void PREFERENCIAL_TAXES_TO_DEALSXmlSerializable::FillTypeInfo()
{
	AddComplexInfo( CSRECHDRXmlSerilizable().GetTypeInfo(),		"m_Hdr",					offsetof( PREFERENCIAL_TAXES_TO_DEALS, m_Hdr )					);
	AddComplexInfo( CSHISTINFOXmlSerilizable().GetTypeInfo(),	"m_histInfo",				offsetof( PREFERENCIAL_TAXES_TO_DEALS, m_histInfo )				);
	AddSimpleInfo( XMLDataTypeLong,								"m_lCode",					offsetof( PREFERENCIAL_TAXES_TO_DEALS, m_lCode )				);
	AddSimpleInfo( XMLDataTypeLong,								"m_lDealType",				offsetof( PREFERENCIAL_TAXES_TO_DEALS, m_lDealType )			);
	AddSimpleInfo( XMLDataTypeLong,								"m_lDealNum",				offsetof( PREFERENCIAL_TAXES_TO_DEALS, m_lDealNum )				);
	AddSimpleInfo( XMLDataTypeLong,								"m_lPreferencialTaxCode",	offsetof( PREFERENCIAL_TAXES_TO_DEALS, m_lPreferencialTaxCode )	);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// TAXES_ON_DEAL_PREFERENCE_COUNTERS

TAXES_ON_DEAL_PREFERENCE_COUNTERS::TAXES_ON_DEAL_PREFERENCE_COUNTERS()
{
	memset( this, 0, sizeof( *this ) );
}

TAXES_ON_DEAL_PREFERENCE_COUNTERS::TAXES_ON_DEAL_PREFERENCE_COUNTERS( const TAXES_ON_DEAL_PREFERENCE_COUNTERS & recEvtOnDealCount )
{
	memset( this, 0, sizeof( *this ) );
	*this = recEvtOnDealCount;
}
long TAXES_ON_DEAL_PREFERENCE_COUNTERS::GetKeyValue() const 
{
	return m_lCode;
}
long TAXES_ON_DEAL_PREFERENCE_COUNTERS::SetKeyValue( const long lKeyValue )
{
	m_lCode = lKeyValue;
	return m_lCode;
}
