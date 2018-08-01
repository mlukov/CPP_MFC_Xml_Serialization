// PreferencialTaxesDlg.cpp
//
#include "stdafx.h"
#include "bfrofC.h"
#include "CSCmnClientTemplates.h"
#include "FormViewDlg.h"
#include "PreferencialTaxes.h"
#include "PreferencialTaxDlg.h"
#include "RpcBfrof.h"
#include "BlkSumAccStr.h"
#include "ExtListCtrl.h"
#include "CodeNameChooseDlg.h"
#include "TaxAutomationStructs.h"
#include "TopDetStr.h"
#include "TaxAutomationStructs.h"
#include "PreferencialDealTypeC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MIN_DATE 18900101
#define MAX_PERCENT 100

#define ID_ADD_EVENT	1
#define ID_DELETE_EVENT	2
#define ID_ADD_TAX		3
#define ID_DELETE_TAX	4

#define ID_WM_DEAL_TYPE_CHANGED WM_USER + 1001

ACCSYScEXP int StoreSelDetValueFromComboX( CComboBox& cb, long& value );
ACCSYScEXP void LoadDetInComboX( WORD wCode, BYTE byDetOrNom, long curvalue, CComboBox& cb, BOOL fNamesOnly/*=FALSE*/ );


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPreferencialTaxSheet::DoXmlSerialization()
{
	_bstr_t bstrBuf;
	CSXMLSerializer oXmlSerializer;
	CPreferencialTaxFullRec oFullRecBuffer;
	oFullRecBuffer.Copy( *m_pBuf ); 

	if( !oFullRecBuffer.SerializeXML( oXmlSerializer ) )
		return;

	if( !oXmlSerializer.GetXML( bstrBuf ) )
		return;

	CXmlEditDlg oDlgXmlEditor;
	oDlgXmlEditor.m_bstr = bstrBuf;

	if( oDlgXmlEditor.DoModal() != IDOK )
		return;

	bstrBuf = oDlgXmlEditor.m_bstr;

	CPreferencialTaxFullRec oFullRecBuff2;

	CSXMLSerializer oXmlDeserializer;

	if( !oFullRecBuff2.DeserializeXML( oXmlDeserializer, &bstrBuf, NULL ) )
		return;

	m_pBuf->Copy( oFullRecBuff2 );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region MainPage
/////////////////////////////////////////////////////////////////////////////
// CPreferencialTaxMainPage
BEGIN_LANG_WND( CPreferencialTaxMainPage )
WND(	WND_TITLE,							MSG_BASE_DATA							),
WND(	IDC_RDB_MONTHS,						MSG_NUMBER_MONTHS						),
WND(	IDC_RDB_DAYS,						MSG_NUMBER_OF_DAYS						),
WND(	IDC_CHK_DEPENDS_ON_DATE_OPEN,		MSG_DEPENDS_ON_DEAL_OPEN_DATE			),
WND(	IDC_RDB_DEPENDS_ON,					MSG_DEPENDS_ON							),
WND(	IDC_RDB_EVENT_COUNT,				MSG_NUMBER_EVENTS						),
WND(	IDC_RDB_PERCENT,					MSG_PERCENT								),
WND(	IDC_RDB_SOLID_SUM,					MSG_FIXED_SUM							),
WND(	IDC_BTN_ADD,						MSG_ACCSYSC_1797						),
WND(	IDC_BTN_DEL,						MSG_ACCSYSC_1798						),
WND(    IDC_GRB_PREF_CONDITIONS,			MSG_PREFERENCE_CONDITIONS				),
WND(    IDC_STT_CODE,						MSG_ACCSYSC_1509						),
WND(    IDC_STT_PREF_TAX_NAME,				MSG_ACCSYSC_2141						),
WND(    IDC_GRB_PREF_MAIN_DATA,				MSG_PREFERENCE_MAIN_DATA				),
WND(    IDC_STT_PREF_STATUS,				MSG_ACCSYSC_2140						),
WND(    IDC_STT_TERM_OF_ACTION,				MSG_TERM_OF_ACTIVITY					),
WND(	IDC_STT_PERIODICITY,				MSG_CYCLE								),
WND(    IDC_STT_TERM_FROM,					MSG_FROM_SHORT							),
WND(    IDC_STT_TERM_TO,					MSG_UNTIL								),
WND(	IDC_STT_DATE_OPEN_FROM,				MSG_FROM_ACC							),
WND(    IDC_STT_DATE_OPEN_TO,				MSG_UNTIL								),
WND(	IDC_STT_SUM_OVER,					MSG_SUM_OVER							),
WND(    IDC_STT_EVENT_COUNT_FROM,			MSG_FROM_ACC							),
WND(	IDC_STT_EVENT_COUNT_TO,				MSG_UNTIL								),
WND(    IDC_GRB_PREF_PLAN_BY_EVENTS,		MSG_EVENT_PREFERENCE_PLAN				),
WND(    IDC_STT_DEAL_TYPE,					MSG_DEAL_TYPE_2							),
WND(	IDC_STT_PREF_SYSTEM,				MSG_SERVICE_SYSTEM						),
WND(	IDC_RDB_NO_DEPENDANCY,				MSG_NO_ADDITIONAL_PREFERENCE_CONDITIONS	),
WND(	IDC_GRB_GRATIS_SUM_PERIOD,			MSG_GRATIS_SUM_AND_PERIOD				),
WND(	IDC_CHK_GRATIS_SUM_VALIDITY,		MSG_GRATIS_SUM_VALIDITY_MONTHS			),
WND(	IDC_STT_GRATIS_SUM,					MSG_GRATIS_SUM_AMMOUNT_IN_LV			),
WND(	IDC_RDB_DEAL_SERVICE_COMB,			MSG_DEAL_SERVICE_COMBINATION			),
END_LANG_WND()

IMPLEMENT_DYNAMIC( CPreferencialTaxMainPage, CBasePropertyPage )

CPreferencialTaxMainPage::CPreferencialTaxMainPage( CPreferencialTaxFullRec & recPrefTax
	, const short sDlgMode,  CWnd* pParent )
: CBasePropertyPage( CPreferencialTaxMainPage::IDD, GetHISTANCEBfrofCDLL() )
, m_eDlgMode( (ePrefTaxDlgMode)sDlgMode )
{
	m_pBuf = &recPrefTax;
	m_nCode					= 0;
	m_nRdbDaysMonths		= -1;
	m_nRdbBalanceNumEvents	= -1;
	m_nRdbPercentSolidSum	= -1;
	m_nGratisMonths = 0;

	m_nNumMonths = 0;
	m_nNumDays = 0;
	m_nNumEventsFrom = 0;
	m_nNumEventsTo =0;
}

CPreferencialTaxMainPage::~CPreferencialTaxMainPage()
{
}

BEGIN_MESSAGE_MAP( CPreferencialTaxMainPage, CBasePropertyPage )
	ON_EN_KILLFOCUS(	IDC_EDB_TERM_FROM,				&CPreferencialTaxMainPage::OnEnKillfocusEdbTermFrom )
	ON_EN_KILLFOCUS(	IDC_EDB_MONTHS,					&CPreferencialTaxMainPage::OnEnKillfocusEdbMonths )
	ON_EN_KILLFOCUS(	IDC_EDB_DAYS,					&CPreferencialTaxMainPage::OnEnKillfocusEdbDays )
	ON_BN_CLICKED(		IDC_RDB_MONTHS,					&CPreferencialTaxMainPage::OnBnClickedRdbMonths )
	ON_BN_CLICKED(		IDC_RDB_DAYS,					&CPreferencialTaxMainPage::OnBnClickedRdbMonths )
	ON_BN_CLICKED(		IDC_CHK_DEPENDS_ON_DATE_OPEN,	&CPreferencialTaxMainPage::OnBnClickedChkDependsOnDateOpen )
	ON_EN_KILLFOCUS(	IDC_EDB_DATE_OPEN_TO,			&CPreferencialTaxMainPage::OnEnKillfocusEdbDateOpenTo )
	ON_BN_CLICKED(		IDC_RDB_DEPENDS_ON,				&CPreferencialTaxMainPage::OnBnClickedRdbDependsOn )
	ON_BN_CLICKED(		IDC_RDB_EVENT_COUNT,			&CPreferencialTaxMainPage::OnBnClickedRdbDependsOn )
	ON_BN_CLICKED(		IDC_RDB_NO_DEPENDANCY,			&CPreferencialTaxMainPage::OnBnClickedRdbDependsOn )
	ON_BN_CLICKED(		IDC_RDB_DEAL_SERVICE_COMB,		&CPreferencialTaxMainPage::OnBnClickedRdbDependsOnDealTypeService )
	ON_BN_CLICKED(		IDC_RDB_PERCENT,				&CPreferencialTaxMainPage::OnBnClickedRdbPercent )
	ON_BN_CLICKED(		IDC_RDB_SOLID_SUM,				&CPreferencialTaxMainPage::OnBnClickedRdbPercent )
	ON_BN_CLICKED(		IDC_BTN_ADD,					&CPreferencialTaxMainPage::OnBnClickedAdd )
	ON_BN_CLICKED(		IDC_BTN_DEL,					&CPreferencialTaxMainPage::OnBnClickedDel )
	ON_CBN_SELCHANGE(	IDC_CMB_DEAL_TYPE,				&CPreferencialTaxMainPage::OnCbnDealTypeSelChange )
	ON_LBN_SELCHANGE(	IDC_LSB_PREF_SYSTEM,			&CPreferencialTaxMainPage::OnLbsProcSystemSelChange)
	ON_BN_CLICKED(		IDC_CHK_GRATIS_SUM_VALIDITY,	&CPreferencialTaxMainPage::OnBnClickedChkGratisSumValidity )
END_MESSAGE_MAP()


bool CPreferencialTaxMainPage::Control()
{
	if( !UpdateData() )
		return false;

	if( !m_pBuf )
		return false;

	LDATE lMaxDate = MAX_DATE;

	m_edbName.GetWindowText( m_pBuf->m_recPrefTax.m_szPreferenceName, sizeof( m_pBuf->m_recPrefTax.m_szPreferenceName )-1 )	;

	if( strlen( m_pBuf->m_recPrefTax.m_szPreferenceName ) <= 0 )
	{
		CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_ACCSYSC_2141 ) ) );
		return false;
	}// if

	long lSel = -1;
	lSel = m_cmbDealType.GetCurSel();
	if( lSel < 0 )
	{
		CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_DEAL_TYPE_3 ) ) );
		return false;
	}// if

	m_pBuf->m_recPrefTax.m_nDealType = m_cmbDealType.GetItemData( lSel );

	if( m_pBuf->m_recPrefTax.m_nDealType < 0 )
	{
		CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_DEAL_TYPE_3 ) ) );
		return false;
	}// if

	// Обслужваща система
	BOOL bHasChoosenSystem = FALSE;

	CSNomMsgIDCode oProcSystem( g_arrProcessingSystem, _countof( g_arrProcessingSystem ) );
	CSCmnRecsArray<long> arrProcSystems;

	for( INT_PTR i = 0, nCnt = oProcSystem.GetSize(); i < nCnt; i++ )
	{
		if( ( i == nCnt - 1 ) && oProcSystem.LastIsEmpty() )
			break;

		int nCode = oProcSystem.GetCodeByIndex( i );

		if( nCode == (int)eUndefinedProcessingSystem )
			continue;

		if( m_pBuf->m_recPrefTax.CheckProcessingSystemBit( nCode ) )
		{
			bHasChoosenSystem = TRUE;
			break;
		}// if
	}// for

	if( !bHasChoosenSystem )
	{
		CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_SERVICE_SYSTEM ) ) );
		return false;
	}// if

	m_pBuf->m_recPrefTax.m_fDateValidFrom = (LDATE)m_edbDateActiveFrom.GetDate();

	if(  ( m_eDlgMode == eAdd ||  m_eDlgMode == eAddNoStdDog ) &&
		 ( (LDATE)m_pBuf->m_recPrefTax.m_fDateValidFrom < CUR_DATE
		 || (LDATE)m_pBuf->m_recPrefTax.m_fDateValidFrom > lMaxDate ) )
	{
		CSMsg::Log( eScreen, MSG_DATE_BEGIN_BEFORE_CURRENT );
		return false;
	}// if

	m_pBuf->m_recPrefTax.m_fDateValidTo = (LDATE)m_edbDateActiveTo.GetDate();

	if( (LDATE)m_pBuf->m_recPrefTax.m_fDateValidTo < (LDATE)m_pBuf->m_recPrefTax.m_fDateValidFrom
		|| (LDATE)m_pBuf->m_recPrefTax.m_fDateValidTo > lMaxDate )
	{
		CSMsg::Log( eScreen, MSG_END_DATE_ACTIVE_SMALLER_THAN_BEGIN_DATE );
		return false;
	}// if


	if( m_pBuf->m_recPrefTax.m_sPreferenceStatus != (short)eStsDeactivated )
	{
		if( CUR_DATE <(LDATE)m_pBuf->m_recPrefTax.m_fDateValidFrom  )
			m_pBuf->m_recPrefTax.m_sPreferenceStatus = (short)eStsWaiting;
		else if( CUR_DATE > (LDATE)m_pBuf->m_recPrefTax.m_fDateValidTo )
			m_pBuf->m_recPrefTax.m_sPreferenceStatus = (short)eStsInactive;
		else if( CUR_DATE >= (LDATE)m_pBuf->m_recPrefTax.m_fDateValidFrom
			&& CUR_DATE <= (LDATE)m_pBuf->m_recPrefTax.m_fDateValidTo )
			m_pBuf->m_recPrefTax.m_sPreferenceStatus = (short)eStsActive;
	}// else

	if( m_nRdbDaysMonths != (int)eMonthsPeriod && m_nRdbDaysMonths != (int)eDaysPeriod )
	{
		CSMsg::Log( eScreen, MSG_CHOOSE_OPTION_COUNT_MONTHS_DAYS );
		return false;
	}// if

	if( m_nRdbDaysMonths == (int)eMonthsPeriod )
	{
		if( m_nNumMonths <= 0 )
		{
			CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_NUMBER_MONTHS ) ) );
			return false;
		}// if

		m_pBuf->m_recPrefTax.m_nNumMonths	= m_nNumMonths;
		m_pBuf->m_recPrefTax.m_nNumDays		= 0;
	}// if
	else if( m_nRdbDaysMonths == (int)eDaysPeriod )
	{
		if( m_nNumDays <= 0 )
		{
			CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_DAYS_COUNT_EMPL ) ) );
			return false;
		}// if

		m_pBuf->m_recPrefTax.m_nNumMonths	= 0;
		m_pBuf->m_recPrefTax.m_nNumDays		= m_nNumDays;
	}// else if

	m_pBuf->m_recPrefTax.m_sPeriodType = m_nRdbDaysMonths;

	// Периодичност
	lSel = m_cmbPeriodicity.GetCurSel();
	if( lSel < 0 )
	{
		CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_PERIODICAL ) ) );
		return false;
	}// if

	m_pBuf->m_recPrefTax.m_sPeriodicity = (short)m_cmbPeriodicity.GetItemData( lSel );

	if( m_pBuf->m_recPrefTax.m_sPeriodicity <= 0 )
	{
		CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_PERIODICAL ) ) );
		return false;
	}// if

	m_pBuf->m_recPrefTax.m_Hdr.SetBit( STS_DEPENDS_ON_DATE_OPEN, m_chbDependsOnDealOpenDate.GetCheck() > 0 );


	if( m_pBuf->m_recPrefTax.m_nDealType == KCK_DEALS && m_chbGratisSumValidity.GetCheck() > 0 )
	{
		if( m_nGratisMonths <= 0 )
		{
			CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_GRATIS_SUM_VALIDITY_MONTHS ) ) );
			return false;
		}// if

		m_pBuf->m_recPrefTax.m_nGratisMonths = m_nGratisMonths;

		if( m_edbGratisSum.GetSum() <= 0.0 )
		{
			CSMsg::Log( eScreen, MSG_PLEASE_FILL_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_GRATIS_SUM_VALUE ) ) );
			return false;
		}// if

		m_pBuf->m_recPrefTax.m_fGratisSumBgn = m_edbGratisSum.GetSum();

	}// if

	if( m_chbGratisSumValidity.GetCheck() <= 0 )
	{
		m_pBuf->m_recPrefTax.m_nGratisMonths	= 0;
		m_pBuf->m_recPrefTax.m_fGratisSumBgn		= 0;
	}// if

	if( m_chbDependsOnDealOpenDate.GetCheck() > 0 )
	{
		m_pBuf->m_recPrefTax.m_fDateOpenFrom = (LDATE)m_edbDateDealFrom.GetDate();

		if( (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenFrom < MIN_DATE 
			|| (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenFrom > lMaxDate )
		{
			CSMsg::Log( eScreen, MSG_INCORRECT_DATE_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_DATE_OPEN_DEAL_FROM ) ) );
			return false;
		}// if

		m_pBuf->m_recPrefTax.m_fDateOpenTo = (LDATE)m_edbDateDealTo.GetDate();

		if( (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenTo < MIN_DATE 
			|| (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenTo > lMaxDate )
		{
			CSMsg::Log( eScreen, MSG_INCORRECT_DATE_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_DATE_OPEN_DEAL_TO ) ) );
			return false;
		}// if

		if( (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenFrom > (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenTo )
		{
			CSMsg::Log( eScreen, MSG_DATE_DEAL_OPEN_TO_SMALLER_DATE_FROM );
			return false;
		}// if
	}// if
	else
	{
		m_pBuf->m_recPrefTax.m_fDateOpenFrom.Clear();
		m_pBuf->m_recPrefTax.m_fDateOpenTo.Clear();
	}// else

	if( m_nRdbBalanceNumEvents != (short)eBalanceTurnover 
		&& m_nRdbBalanceNumEvents != (short)eEventsCount 
		&& m_nRdbBalanceNumEvents != (short)eNoDependancy
		&& m_nRdbBalanceNumEvents != (short)eDealService )
	{
		CSMsg::Log( eScreen, MSG_CHOOSE_OPTION_DEPENDS_ON_OR_EVENT_COUNT );
		return false;
	}// if

	if( m_pBuf->m_arrPlan.GetCount() <= 0 && m_pBuf->m_recPrefTax.m_fGratisSumBgn <= 0  )
	{
		CSMsg::Log( eScreen, MSG_NO_PREF_PLAN );
		return false;
	}// if

	// Събития и такси
	PREFERENCIAL_EVENTS_TAXES * pPrefEvent = NULL;

	if( m_pBuf->m_arrEventsTaxes.GetCount() <= 0 )
	{
		CSMsg::Log( eScreen, MSG_NO_EVENTS_AND_TAXES_ADDED );
		return FALSE;
	}// if

	for( int i = 0; i < m_pBuf->m_arrEventsTaxes.GetCount(); i++ )
	{
		pPrefEvent = m_pBuf->m_arrEventsTaxes.GetAt( i );

		if ( pPrefEvent->m_lTaxCode > 0 )
			continue;

		if( !m_pBuf->m_arrEventsTaxes.EventHasTaxes( pPrefEvent->m_lEventCode ) )
		{
			CString strName;
			GetSheet()->GetEventName( pPrefEvent->m_lEventCode, strName );
			CSMsg::Log( eScreen, MSG_FOR_EVENT_THERE_ARE_NO_TAXES, &MSGKEY( "0", strName ) );	
			return FALSE;
		}// if
	}// for

	if( m_eDlgMode != eAddNoStdDog && m_eDlgMode!= eEditConditionsOnly && m_eDlgMode != eAddKlientska
		&& !m_pBuf->IsIndividual()
		&& !m_pBuf->IsKlientskaPref()
		&& m_pBuf->m_arrTaxesToStdDeals.GetCount() <= 0 
		&& ( m_pBuf->m_recPrefTax.m_nDealType == RAZPL_DEALS || 
			 m_pBuf->m_recPrefTax.m_nDealType == DEPOS_DEALS || 
			 m_pBuf->m_recPrefTax.m_nDealType == WLOG_DEALS  || 
			 m_pBuf->m_recPrefTax.m_nDealType == KCK_DEALS ) )
	{
		CSMsg::Log( eScreen, MSG_PR_TAX_NO_STD );
		return false;
	}//if

	return true;
}

void CPreferencialTaxMainPage::DoDataExchange( CDataExchange* pDX )
{
	CBasePropertyPage::DoDataExchange( pDX );

	// Код на преференциална такса
	DDX_Control(	pDX,	IDC_EDB_CODE,						m_edbCode );
	DDX_Text(		pDX,	IDC_EDB_CODE,						m_nCode );
	// Име на преференциалната такса
	DDX_Control(	pDX,	IDC_EDB_NAME,						m_edbName );
	// Статус - активна, неактивна, деактивирана или чакаща
	DDX_Control(	pDX,	IDC_EDB_STATUS,						m_edbStatus );
	// Тип на сделка
	DDX_Control(	pDX,	IDC_CMB_DEAL_TYPE,					m_cmbDealType );
	// Обслужваща система
	DDX_Control(	pDX,	IDC_LSB_PREF_SYSTEM,				m_chlbProcessingSystem );
	// Срок на действие от
	DDX_EditDate(	pDX,	IDC_EDB_TERM_FROM,					m_edbDateActiveFrom );
	// Срок на действие до
	DDX_EditDate(	pDX,	IDC_EDB_TERM_TO,					m_edbDateActiveTo );
	// Радиобутони Брой месеци / Брой дни
	DDX_Control(	pDX,	IDC_RDB_MONTHS,						m_rdbMohtns );
	DDX_Control(	pDX,	IDC_RDB_DAYS,						m_rdbDays );
	// Брой месеци
	DDX_Control(	pDX,	IDC_EDB_MONTHS,						m_edbNumMonths );
	DDX_Text(		pDX,	IDC_EDB_MONTHS,						m_nNumMonths );
	// Брой дни
	DDX_Control(	pDX,	IDC_EDB_DAYS,						m_edbNumDays );
	DDX_Text(		pDX,	IDC_EDB_DAYS,						m_nNumDays );
	// Периодичност
	DDX_Control(	pDX,	IDC_CMB_PERIODICITY,				m_cmbPeriodicity );
	// Зависи от дата на откриване на сделката
	DDX_Control(	pDX,	IDC_CHK_DEPENDS_ON_DATE_OPEN,		m_chbDependsOnDealOpenDate );
	// Дата на откриване на сделката от
	DDX_EditDate(	pDX,	IDC_EDB_DATE_OPEN_FROM,				m_edbDateDealFrom );
	// Дата на откриване на сделката до
	DDX_EditDate(	pDX,	IDC_EDB_DATE_OPEN_TO,				m_edbDateDealTo );

	// Гратисна сума
	// Брой месеци за валидност на гратисната сума
	DDX_Control(	pDX,	IDC_CHK_GRATIS_SUM_VALIDITY,		m_chbGratisSumValidity );
	DDX_Control(	pDX,	IDC_EDB_GRATIS_MONTHS,				m_edbGratisMonths );
	DDX_Text(		pDX,	IDC_EDB_GRATIS_MONTHS,				m_nGratisMonths );
	// Размер на гратисната сума
	DDX_Control(	pDX,	IDC_EDB_GRATIS_SUM,					m_edbGratisSum );

	// Радиобутони Зависи от салдо / брой събития / комбинация от сделка и услуга
	DDX_Control(	pDX,	IDC_RDB_DEPENDS_ON,					m_rdbBalance );	
	DDX_Control(	pDX,	IDC_RDB_EVENT_COUNT,				m_rdbNumEvents );
	DDX_Control(	pDX,	IDC_RDB_DEAL_SERVICE_COMB,			m_rdbDealTypeService );
	DDX_Control(	pDX,	IDC_RDB_NO_DEPENDANCY,				m_rdbNoDependancy );

	// Зависи от
	DDX_Control(	pDX,	IDC_CMB_DEPENDS_ON,					m_cmbDependsOn );
	// Комбинация от зделка и услуга
	DDX_Control(	pDX,	IDC_CMB_DEAL_SERVICE_COMB,			m_cmbDealTypeService );
	// Сума над
	DDX_SumEdit(	pDX,	IDC_EDB_SUM_OVER,					m_edbSumOver );
	// Брой събития от
	DDX_Control(	pDX,	IDC_EDB_EVENT_COUNT_FROM,			m_edbNumEventsFrom );
	DDX_Text(		pDX,	IDC_EDB_EVENT_COUNT_FROM,			m_nNumEventsFrom );
	// Брой събития до
	DDX_Control(	pDX,	IDC_EDB_EVENT_COUNT_TO,				m_edbNumEventsТо );
	DDX_Text(		pDX,	IDC_EDB_EVENT_COUNT_TO,				m_nNumEventsTo );
	// Радиобутони процент / твърда сума
	DDX_Control(	pDX,	IDC_RDB_PERCENT,					m_rdbPercent );
	DDX_Control(	pDX,	IDC_RDB_SOLID_SUM,					m_rdbSolidSum );
	// Процент
	DDX_SumEdit(	pDX,	IDC_EDB_PERCENT,					m_edbPercent );
	// Твърда сума
	DDX_SumEdit(	pDX,	IDC_EDB_SOLID_SUM,					m_edbSolidSum );
	// Добавяне
	DDX_Control(	pDX,	IDC_BTN_ADD,						m_btnAdd );
	// Изтриване
	DDX_Control(	pDX,	IDC_BTN_DEL,						m_btnDelete );
	// Преференциален план по събития
	DDX_Control(	pDX,	IDC_LSB_PREF_EVENT_PLAN,			m_lstPlan );					
}

BOOL CPreferencialTaxMainPage::OnInitDialog() 
{
	CBasePropertyPage::OnInitDialog();

	m_edbCode.EnableWindow( FALSE );
	m_edbStatus.EnableWindow( FALSE );
	m_edbNumDays.EnableWindow( FALSE );
	m_edbNumMonths.EnableWindow( FALSE );
	m_edbDateDealFrom.EnableWindow( FALSE );
	m_edbDateDealTo.EnableWindow( FALSE );

	m_cmbDependsOn.EnableWindow( FALSE );
	m_edbSumOver.EnableWindow( FALSE );
	m_edbNumEventsFrom.EnableWindow( FALSE );
	m_edbNumEventsТо.EnableWindow( FALSE );
	m_edbPercent.EnableWindow( FALSE );
	m_edbSolidSum.EnableWindow( FALSE );

	m_chbGratisSumValidity.EnableWindow( TRUE );
	m_edbGratisMonths.EnableWindow( FALSE );
	m_edbGratisSum.EnableWindow( FALSE );
	m_edbGratisSum.SetSum( 0.0 );

	m_edbDateActiveTo.EnableWindow( FALSE );

	m_cmbDealType.EnableWindow( m_eDlgMode == eEditFull || ( m_eDlgMode == eAdd && m_pBuf->m_recPrefTax.m_nDealType <= 0 )  );

	WORD wCol = 0;

	m_lstPlan.InsertColumn( wCol++, CSMsg::GetMsgUsrLang( MSG_SUM_PERCENT ),	LVCFMT_RIGHT, m_lstPlan.GetStringWidth("#############"),		wCol );
	m_lstPlan.InsertColumn( wCol++, CSMsg::GetMsgUsrLang( MSG_DEPENDS_ON ),		LVCFMT_LEFT, m_lstPlan.GetStringWidth("################"),	wCol );
	m_lstPlan.InsertColumn( wCol++, CSMsg::GetMsgUsrLang( MSG_SUM_OVER ),		LVCFMT_RIGHT, m_lstPlan.GetStringWidth("##########"),		wCol );
	m_lstPlan.InsertColumn( wCol++, CSMsg::GetMsgUsrLang( MSG_EVENTS_FROM ),	LVCFMT_RIGHT, m_lstPlan.GetStringWidth("###########"),		wCol );
	m_lstPlan.InsertColumn( wCol++, CSMsg::GetMsgUsrLang( MSG_UNTIL ),			LVCFMT_RIGHT, m_lstPlan.GetStringWidth("##########"),		wCol );

	DWORD dwStyleEx = m_lstPlan.GetExtendedStyle(); 
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	m_lstPlan.SetExtendedStyle( dwStyleEx );

	if( m_eDlgMode == eView 
		|| m_eDlgMode == eViewNoDeactivate
		|| m_eDlgMode == eEditDatesOnly
		|| m_eDlgMode == eEditConditionsOnly )
		EnableControls( FALSE );

	if( !LoadData() )
	{
		CSMsg::Log( eScreen, MSG_ERR_LOAD_PREFERENCE_DATA );
		return FALSE;
	}// if

	return TRUE;
}

BOOL CPreferencialTaxMainPage::OnApply()
{
	if( !Control() )
		return FALSE;

	return TRUE;
}

void CPreferencialTaxMainPage::EnableControls( const BOOL bEnable )
{	
	BOOL bEditDatesOnly = ( m_eDlgMode == eEditDatesOnly );

	m_edbCode.EnableWindow( FALSE );
	m_edbName.EnableWindow( bEnable );
	m_edbStatus.EnableWindow( FALSE );
	m_chlbProcessingSystem.EnableWindow( bEnable );
	m_cmbDealType.EnableWindow( bEnable );
	m_edbDateActiveFrom.EnableWindow( bEnable || ( m_eDlgMode == eEditConditionsOnly && m_pBuf->m_recPrefTax.IsKlientskaPref() ) || ( m_eDlgMode == eEditConditionsOnly && m_pBuf->m_recPrefTax.IsIndividual() ) );

	m_edbDateActiveTo.EnableWindow( bEnable );
	m_rdbMohtns.EnableWindow( bEnable );
	m_rdbDays.EnableWindow( bEnable );

	m_edbNumMonths.EnableWindow( bEnable || ( bEditDatesOnly && m_pBuf->m_recPrefTax.m_sPeriodType == (short)eMonthsPeriod ) );
	m_edbNumDays.EnableWindow( bEnable || ( bEditDatesOnly && m_pBuf->m_recPrefTax.m_sPeriodType == (short)eDaysPeriod ) );
	m_chbGratisSumValidity.EnableWindow( ( bEnable || ( m_eDlgMode == eEditConditionsOnly &&  m_pBuf->m_recPrefTax.m_nDealType == KCK_DEALS ) ) 
											&& m_pBuf->m_arrPlan.GetCount() <= 0 );

	m_cmbPeriodicity.EnableWindow( bEnable );
	m_chbDependsOnDealOpenDate.EnableWindow( bEnable );
	m_edbDateDealFrom.EnableWindow( bEnable );
	m_edbDateDealTo.EnableWindow( bEnable || ( bEditDatesOnly && m_pBuf->m_recPrefTax.m_Hdr.CheckBit( STS_DEPENDS_ON_DATE_OPEN ) == (short)eDaysPeriod ) );
	m_rdbBalance.EnableWindow( bEnable );

	m_rdbNumEvents.EnableWindow( bEnable );
	m_cmbDependsOn.EnableWindow( bEnable );
	m_rdbNoDependancy.EnableWindow( bEnable );
	m_edbSumOver.EnableWindow( bEnable );
	m_edbNumEventsFrom.EnableWindow( bEnable );
	m_edbNumEventsТо.EnableWindow( bEnable );
	m_rdbDealTypeService.EnableWindow( bEnable );
	m_cmbDealTypeService.EnableWindow( bEnable );

	m_rdbPercent.EnableWindow( m_eDlgMode == eEditConditionsOnly );
	m_rdbSolidSum.EnableWindow( m_eDlgMode == eEditConditionsOnly );
	m_edbPercent.EnableWindow( m_eDlgMode == eEditConditionsOnly );
	m_edbSolidSum.EnableWindow( m_eDlgMode == eEditConditionsOnly );
	m_btnAdd.EnableWindow( m_eDlgMode == eEditConditionsOnly );

	m_btnDelete.EnableWindow( m_eDlgMode == eEditConditionsOnly );
}

void CPreferencialTaxMainPage::OnLbsProcSystemSelChange()
{
	if( !m_pBuf )
		return;
	
	for( int i=0; i< m_chlbProcessingSystem.GetCount(); i++ )
	{
		if( m_chlbProcessingSystem.GetItemData( i ) < 0 )
			continue;

		m_pBuf->m_recPrefTax.SetProcessingSystemBit( m_chlbProcessingSystem.GetItemData( i )
										, m_chlbProcessingSystem.GetCheck( i ) > 0 );
	}// for
}

void CPreferencialTaxMainPage::OnCbnDealTypeSelChange()
{
	if( !m_pBuf )
		return;
	
	long lDealType = 0;
	StoreSelDetValueFromComboX( m_cmbDealType, lDealType );

	if(  m_pBuf->m_recPrefTax.m_nDealType != lDealType && m_pBuf->m_arrEventsTaxes.GetCount() > 0  )
		if( !CSMsg::Question( "При смяна на типа сделка ще бъдат премахнати всички събития! Сигурни ли сте?" ) )
		{
			int nIndex =-1;
			for( int i = 0; i< m_cmbDealType.GetCount(); i++ )
			{
				if( m_pBuf->m_recPrefTax.m_nDealType == m_cmbDealType.GetItemData( i ) )
				{
					m_cmbDealType.SetCurSel( i );
					break;
				}// if
			}// for
			
			return;
		}// if

	if( lDealType < 0 )
		return;

	m_pBuf->m_recPrefTax.m_nDealType = lDealType;

	m_chbGratisSumValidity.EnableWindow( m_pBuf->m_recPrefTax.m_nDealType == KCK_DEALS && m_pBuf->m_arrPlan.GetCount() <= 0 );

	int nCurSel = m_cmbDealType.GetCurSel();
	if( nCurSel != CB_ERR && (m_cmbDealType.GetItemData( nCurSel ) == RAZPL_DEALS) )
	{
		m_rdbDealTypeService.ShowWindow( SW_SHOW );
		m_cmbDealTypeService.ShowWindow( SW_SHOW );
	} else if ( m_rdbDealTypeService.GetCheck() == BST_CHECKED ) {
		m_rdbDealTypeService.ShowWindow( SW_HIDE );
		m_cmbDealTypeService.ShowWindow( SW_HIDE );

		m_pBuf->m_arrPlan.Release();
		LoadPlan();
	} else {
		m_rdbDealTypeService.ShowWindow( SW_HIDE );
		m_cmbDealTypeService.ShowWindow( SW_HIDE );
	}
	
	if( m_pBuf->m_recPrefTax.m_nDealType == RAZPL_DEALS && m_cmbDealTypeService.IsWindowVisible() )
		LoadDealTypeBankServiceCombinationsInCombo( m_pBuf->m_recPrefTax.m_nDealType );

	OnBnClickedRdbDependsOn();
	OnBnClickedChkGratisSumValidity();
	GetParent()->PostMessage( ID_WM_DEAL_TYPE_CHANGED, (WPARAM)lDealType );
}

void CPreferencialTaxMainPage::OnBnClickedChkGratisSumValidity()
{
	if( !m_pBuf )
		return;

	BOOL bEnable = m_chbGratisSumValidity.GetCheck() > 0;
	
	if( m_pBuf->m_recPrefTax.m_nDealType != KCK_DEALS )
	{
		m_edbGratisMonths.EnableWindow( FALSE );
		m_nGratisMonths = 0;
		UpdateData( FALSE );
		m_edbGratisSum.EnableWindow( FALSE );
		m_edbGratisSum.SetSum( 0.0 );
		m_chbGratisSumValidity.SetCheck( FALSE );
		bEnable = FALSE;
	}// if

	BOOL bEnableGratisControls = ( m_eDlgMode != eView  
									&& m_eDlgMode != eViewNoDeactivate 
									&& m_eDlgMode != eEditDatesOnly ); 
	
	m_edbGratisMonths.EnableWindow( bEnable && bEnableGratisControls );
	m_edbGratisSum.EnableWindow( bEnable && bEnableGratisControls );

	BOOL bEnableDependanceControls = m_eDlgMode != eView  && m_eDlgMode != eViewNoDeactivate && m_eDlgMode != eEditDatesOnly;

	m_rdbBalance.EnableWindow( !bEnable && bEnableDependanceControls );
	m_rdbNumEvents.EnableWindow( !bEnable && bEnableDependanceControls);
	m_rdbNoDependancy.EnableWindow( !bEnable && bEnableDependanceControls );

	m_btnAdd.EnableWindow( !bEnable && bEnableDependanceControls );
	m_btnDelete.EnableWindow( !bEnable && bEnableDependanceControls );

	m_rdbPercent.EnableWindow( !bEnable && bEnableDependanceControls );
	m_rdbSolidSum.EnableWindow( !bEnable && bEnableDependanceControls );

	if( bEnable )
	{
		m_edbPercent.EnableWindow( FALSE );
		m_edbSolidSum.EnableWindow( FALSE );
	}// if
	else
	{
		if( m_rdbPercent.GetCheck() > 0 )
			m_edbPercent.EnableWindow( bEnableDependanceControls );
		
		if( m_rdbSolidSum.GetCheck() > 0 )
			m_edbSolidSum.EnableWindow( bEnableDependanceControls );
	}// else

	LoadPlan();
}

bool CPreferencialTaxMainPage::LoadData() 
{
	if( !m_pBuf )
		return false;

	CSNomMsgIDCode oStatuses( g_arrPreferenceTaxStatus, _countof( g_arrPreferenceTaxStatus ) );

	LoadDetInComboX( DIALS, NOM, m_pBuf->m_recPrefTax.m_nDealType, m_cmbDealType, FALSE );

	if( !LoadProcessingSystem() )
		return false;

	if( m_pBuf->m_recPrefTax.IsIndividual() )
	{
		CString strDealType = "", strCaption = "";

		for( int i =0; i< m_arrDealTypes.GetCount(); i++ )
			if( m_arrDealTypes.GetAt( i )->lCode == m_pBuf->m_recPrefToDeal.m_lDealType )
			{
				strDealType = m_arrDealTypes.GetAt( i )->szName;
				break;
			}// if

	
		CSMsg::GetMsgUsrLang( strCaption, MSG_PREFERENCIAL_TAX_TO_DEAL_TYPE_NUMBER,
		&MSGKEY("0", strDealType ),
		&MSGKEY("1", m_pBuf->m_recPrefToDeal.m_lDealNum ) );
		
		if( GetParent() )
			GetParent()->SetWindowText( strCaption );
	}// if

	// Ако мода е добавяне на клиеткса преференция
	if( m_eDlgMode == eAddKlientska )
	{
		m_edbName.SetWindowText( m_pBuf->m_recPrefTax.m_szPreferenceName );

		m_edbDateActiveFrom.SetDate( CUR_DATE );
		m_edbDateActiveTo.SetDate( CUR_DATE );
		m_edbDateDealFrom.SetDate( CUR_DATE );
		m_edbDateDealTo.SetDate( CUR_DATE );

		m_rdbPercent.SetCheck( TRUE );
		m_rdbMohtns.SetCheck( TRUE );
		m_rdbNoDependancy.SetCheck( TRUE );
		UpdateData( TRUE );

		m_edbSolidSum.SetSum( 0.0 );
		m_edbSumOver.SetSum( 0.0 );
		m_edbPercent.SetSum( 0.0 );
		m_edbStatus.SetWindowText( oStatuses.GetMsgUserLang( eStsWaiting ) );

		OnBnClickedRdbMonths();
		OnBnClickedChkDependsOnDateOpen();
		OnBnClickedRdbDependsOn();
		OnBnClickedRdbPercent();

		if ( !LoadPeriodicities() )
			return false;

		if( !LoadBalanceTurnover() )
			return false;

		OnCbnDealTypeSelChange();

		return true;
	}//if

	if( m_eDlgMode == eAdd )
	{
		m_edbDateActiveFrom.SetDate( CUR_DATE );
		m_edbDateActiveTo.SetDate( CUR_DATE );
		m_edbDateDealFrom.SetDate( CUR_DATE );
		m_edbDateDealTo.SetDate( CUR_DATE );

		m_rdbPercent.SetCheck( TRUE );
		m_rdbMohtns.SetCheck( TRUE );
		m_rdbNoDependancy.SetCheck( TRUE );
		UpdateData( TRUE );

		OnBnClickedRdbMonths();
		OnBnClickedRdbPercent();
		OnBnClickedRdbDependsOn();

		m_edbSolidSum.SetSum( 0.0 );
		m_edbSumOver.SetSum( 0.0 );
		m_edbPercent.SetSum( 0.0 );
		m_edbStatus.SetWindowText( oStatuses.GetMsgUserLang( eStsWaiting ) );

		if ( !LoadPeriodicities() )
			return false;

		if( !LoadBalanceTurnover() )
			return false;

		m_rdbDealTypeService.ShowWindow( SW_HIDE );
		m_cmbDealTypeService.ShowWindow( SW_HIDE );
		return true;
	}// if

	if( m_eDlgMode != eAdd )
	{
		if( m_pBuf->m_recPrefTax.m_sPeriodType == eMonthsPeriod )
		{
			m_rdbMohtns.SetCheck( BST_CHECKED );
		}
		else
		{
			m_rdbDays.SetCheck( BST_CHECKED );
		}
	}

	if ( m_eDlgMode == eAdd
		|| m_eDlgMode == eAddNoStdDog )
		return false;

	m_nCode					= m_pBuf->m_recPrefTax.m_lCode;
	m_nRdbDaysMonths		= m_pBuf->m_recPrefTax.m_sPeriodType;
	m_nNumMonths			= m_pBuf->m_recPrefTax.m_nNumMonths;
	m_nNumDays				= m_pBuf->m_recPrefTax.m_nNumDays;

	if( m_pBuf->m_arrPlan.GetCount() > 0 )
		m_nRdbBalanceNumEvents	= m_pBuf->m_arrPlan.GetAt(0)->m_sDependType;
	
	if( m_pBuf->m_recPrefTax.m_sPreferenceStatus == (short)eStsDeactivated )
	{
		CString strSts;
		strSts.Format( "%s %s %s", oStatuses.GetMsgUserLang( m_pBuf->m_recPrefTax.m_sPreferenceStatus ) 
									, CSMsg::GetMsgUsrLang( MSG_FROM )
									, m_pBuf->m_recPrefTax.m_iniDeactivate.inic );
		m_edbStatus.SetWindowText( strSts );
	}// if
	else
		m_edbStatus.SetWindowText( oStatuses.GetMsgUserLang( m_pBuf->m_recPrefTax.m_sPreferenceStatus ) );

	m_edbName.SetWindowText( m_pBuf->m_recPrefTax.m_szPreferenceName );
	m_edbDateActiveFrom.SetDate( (LDATE)m_pBuf->m_recPrefTax.m_fDateValidFrom );

	if( m_eDlgMode == eEditConditionsOnly && m_pBuf->m_recPrefTax.IsIndividual() )
		m_edbDateActiveFrom.SetDate( CUR_DATE );
		
	m_edbDateActiveTo.SetDate( (LDATE)m_pBuf->m_recPrefTax.m_fDateValidTo );

	m_chbDependsOnDealOpenDate.SetCheck( m_pBuf->m_recPrefTax.m_Hdr.CheckBit( STS_DEPENDS_ON_DATE_OPEN ) );
	m_edbDateDealFrom.SetDate(  (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenFrom );
	m_edbDateDealTo.SetDate(  (LDATE)m_pBuf->m_recPrefTax.m_fDateOpenTo );

	m_nGratisMonths = m_pBuf->m_recPrefTax.m_nGratisMonths;
	m_edbGratisSum.SetSum( m_pBuf->m_recPrefTax.m_fGratisSumBgn );
	m_chbGratisSumValidity.SetCheck( m_pBuf->m_recPrefTax.m_fGratisSumBgn > 0 );

	UpdateData( FALSE );

	OnBnClickedRdbMonths();
	OnBnClickedChkDependsOnDateOpen();
	OnBnClickedRdbDependsOn();
	OnBnClickedRdbPercent();

	if ( !LoadPlan() )
		return false;

	if ( !LoadPeriodicities( m_pBuf->m_recPrefTax.m_sPeriodicity ) )
		return false;

	short sBalanceTurnover = 0;
	if( m_pBuf->m_arrPlan.GetCount() > 0 )
		sBalanceTurnover = m_pBuf->m_arrPlan.GetAt( 0 )->m_sBalanceTurnoverType;

	if( !LoadBalanceTurnover( sBalanceTurnover ) )
		return false;

	if ( m_eDlgMode != eView && m_eDlgMode != eViewNoDeactivate )
		OnBnClickedChkGratisSumValidity();

	OnCbnDealTypeSelChange( );
	return true;
}

/************************************************************************/
/*  Зарежда комбинациите м/у типове зделки и банкови услуги за тип зделка
/************************************************************************/
bool CPreferencialTaxMainPage::LoadDealTypeBankServiceCombinationsInCombo( const long& lDealType )
{
	if( lDealType <= 0 )
		return false;

	CCSPreferencialDealTypeC oPrefDealTypes;
	if( !oPrefDealTypes.LoadAllInCombo_ByDealType( lDealType, m_cmbDealTypeService ) )
		return false;

	return true;
}

bool CPreferencialTaxMainPage::LoadPeriodicities( const short sPeriodicity )
{
	CSNomMsgIDCode oPeriodicities( g_arrPreferenceTaxPeriodicity, _countof( g_arrPreferenceTaxPeriodicity ) );
	oPeriodicities.LoadInCombo( m_cmbPeriodicity, sPeriodicity );

	if( sPeriodicity == (short)eDayly )
		m_cmbPeriodicity.EnableWindow( FALSE );

	return true;
}

bool CPreferencialTaxMainPage::LoadProcessingSystem()
{
	m_chlbProcessingSystem.ResetContent();

	CSNomMsgIDCode oProcSystem( g_arrProcessingSystem, _countof( g_arrProcessingSystem ) );
	CSCmnRecsArray<long> arrProcSystems;

	int nItem = 0;

	for( INT_PTR i = 0, nCnt = oProcSystem.GetSize(); i < nCnt; i++ )
	{
		if( ( i == nCnt - 1 ) && oProcSystem.LastIsEmpty() )
			break;

		int nCode = oProcSystem.GetCodeByIndex( i );

		if( nCode == (int)eUndefinedProcessingSystem )
			continue;

		nItem = m_chlbProcessingSystem.AddString( oProcSystem.GetMsgUserLangByIndex( i ) );
		m_chlbProcessingSystem.SetItemData( nItem, nCode );

		m_chlbProcessingSystem.SetCheck( nItem, m_pBuf->m_recPrefTax.CheckProcessingSystemBit( nCode ) );
	}// for

	return true;
}

bool CPreferencialTaxMainPage::LoadBalanceTurnover( const short sBalanceTurnoverType )
{
	CSNomMsgIDCode oBalTurnover( g_arrPreferenceTaxBalanceTurnover, _countof( g_arrPreferenceTaxBalanceTurnover ) );
	oBalTurnover.LoadInCombo( m_cmbDependsOn, sBalanceTurnoverType );

	return true;
}

bool CPreferencialTaxMainPage::LoadPlan()
{
	CCSPreferencialDealTypeC oPrefHelper;
	m_lstPlan.DeleteAllItems();

	CString strBuf = "";
	int nSubItem = 1;

	m_pBuf->m_arrPlan.SetCompFunction( PREFERENCIAL_PLANS::compareByEventCountSumOver );
	m_pBuf->m_arrPlan.QSort();

	CSNomMsgIDCode oBalanceTurnover( g_arrPreferenceTaxBalanceTurnover, _countof( g_arrPreferenceTaxBalanceTurnover ) );

	if( m_pBuf->m_arrPlan.GetCount() > 0 )
	{
		if( m_pBuf->m_arrPlan.GetAt( 0 )->m_sDependType == (short)eBalanceTurnover )
		{
			m_edbSumOver.EnableWindow(  m_eDlgMode == eAdd || m_eDlgMode == eEditFull ||  m_eDlgMode == eEditNoDealType || m_eDlgMode == eEditConditionsOnly || m_eDlgMode == eAddKlientska || m_eDlgMode == eEditKlientska );
			m_rdbBalance.SetCheck( TRUE );
		}// if

		if( m_pBuf->m_arrPlan.GetAt( 0 )->m_sDependType == (short)eEventsCount )
		{
			m_edbNumEventsFrom.EnableWindow(  m_eDlgMode == eAdd || m_eDlgMode == eEditFull ||  m_eDlgMode == eEditNoDealType || m_eDlgMode == eEditConditionsOnly || m_eDlgMode == eAddKlientska || m_eDlgMode == eEditKlientska );
			m_edbNumEventsТо.EnableWindow(  m_eDlgMode == eAdd || m_eDlgMode == eEditFull ||  m_eDlgMode == eEditNoDealType || m_eDlgMode == eEditConditionsOnly || m_eDlgMode == eAddKlientska || m_eDlgMode == eEditKlientska );
			m_rdbNumEvents.SetCheck( TRUE );
		}// if

		if( m_pBuf->m_arrPlan.GetAt( 0 )->m_sDependType == (short)eDealService )
		{
			m_cmbDealTypeService.EnableWindow( m_eDlgMode == eAdd || m_eDlgMode == eEditFull ||  m_eDlgMode == eEditNoDealType || m_eDlgMode == eEditConditionsOnly || m_eDlgMode == eAddKlientska || m_eDlgMode == eEditKlientska );
			m_rdbDealTypeService.SetCheck( BST_CHECKED );

			oPrefHelper.LoadAllInCombo_ByDealType( m_pBuf->m_recPrefTax.m_nDealType, m_cmbDealTypeService );
			CCSPreferencialDealTypeC::SelectInCombo_ByCode( m_pBuf->m_arrPlan.GetAt(0)->m_nPrefDealTypeCode, m_cmbDealTypeService );
		}

		if(  m_pBuf->m_arrPlan.GetAt( 0 )->m_sDependType == (short)eNoDependancy )
		{
			m_rdbNoDependancy.SetCheck( TRUE );
		}

		UpdateData( TRUE );
	}// if
	else
	{
		m_rdbSolidSum.SetCheck( FALSE );
		m_rdbPercent.SetCheck( TRUE );
		m_rdbNumEvents.SetCheck( FALSE );
		m_rdbBalance.SetCheck( FALSE );
		m_rdbNoDependancy.SetCheck( TRUE );
		UpdateData( TRUE );

		OnBnClickedRdbPercent();
		OnBnClickedRdbDependsOn();
	}// else

	BOOL bEnableDependanceControls = ( m_pBuf->m_arrPlan.GetCount() <= 0 )
							&& m_eDlgMode != eView  
							&& m_eDlgMode != eViewNoDeactivate 
							&& m_eDlgMode != eEditDatesOnly
							&& m_chbGratisSumValidity.GetCheck() <= 0;

	m_rdbBalance.EnableWindow( bEnableDependanceControls );
	m_rdbNumEvents.EnableWindow( bEnableDependanceControls );
	m_rdbNoDependancy.EnableWindow( bEnableDependanceControls );
	m_cmbDependsOn.EnableWindow( bEnableDependanceControls && m_rdbBalance.GetCheck() > 0 );
	m_rdbDealTypeService.EnableWindow( bEnableDependanceControls );

	PREFERENCIAL_DEAL_TYPE recPrefDealType;
	for( INT_PTR i = 0, nCount = m_pBuf->m_arrPlan.GetCount() ; i < nCount ; i++ )
	{
		nSubItem = 1;

		PREFERENCIAL_PLANS *pItem = m_pBuf->m_arrPlan.GetAt( i );

		if( !pItem )
			return false;

		if( pItem->m_sPreferenceType == (short)ePercent )
			strBuf.Format( "%.2f %%", pItem->m_fPercent );
		else
			strBuf.Format( "%.2f ", pItem->m_fSumSolid );

		// Сума/процент
		int nLCIndex = m_lstPlan.InsertItem( m_lstPlan.GetItemCount(), strBuf );

		strBuf = "";

		if( pItem->m_sDependType == (short)eBalanceTurnover )
		{
			strBuf = oBalanceTurnover.GetMsgUserLang( pItem->m_sBalanceTurnoverType );
		} else if ( pItem->m_sDependType == (short)eDealService ) {
			if( !oPrefHelper.LoadRecord( pItem->m_nPrefDealTypeCode, recPrefDealType ) )
				return false;

			strBuf.Format( _T("%d.%s"), recPrefDealType.nCode, recPrefDealType.szPrefName );
		}

		// Зависи от
		m_lstPlan.SetItemText( nLCIndex, nSubItem++, strBuf );

		strBuf = "";

		if( pItem->m_sDependType == (short)eBalanceTurnover )
			strBuf.Format( "%.2f", pItem->m_fSumOver );

		// Сума над
		m_lstPlan.SetItemText( nLCIndex, nSubItem++, strBuf );

		strBuf = "";

		if( pItem->m_sDependType == (short)eEventsCount )
			strBuf.Format( "%d", pItem->m_nEventCountFrom );

		// Събития от
		m_lstPlan.SetItemText( nLCIndex, nSubItem++, strBuf );

		strBuf = "";

		if( pItem->m_sDependType == (short)eEventsCount )
			strBuf.Format( "%d", pItem->m_nEventCountTo );

		// Събития до
		m_lstPlan.SetItemText( nLCIndex, nSubItem++, strBuf );
	}// for

	return true;
}

void CPreferencialTaxMainPage::OnBnClickedAdd()
{
	if( !m_pBuf )
		return;

	UpdateData( TRUE );

	long lBalTurnover = 0;

	// Дали е избрана опция от радиобутоните брой събития/зависи от
	if( m_nRdbBalanceNumEvents != (short)eEventsCount
		&& m_nRdbBalanceNumEvents != (short)eBalanceTurnover 
		&& m_nRdbBalanceNumEvents != (short)eNoDependancy 
		&& m_nRdbBalanceNumEvents != (short)eDealService )
	{
		CSMsg::Log( eScreen, MSG_CHOOSE_OPTION_DEPENDS_ON_OR_EVENT_COUNT ); 
		return;
	}// if

	// За избрана опция 'Без допълнителни условия за преференцията' не може да имаме повече от един елемент
	if( m_nRdbBalanceNumEvents == (short)eNoDependancy && m_pBuf->m_arrPlan.GetCount() > 0 )
	{
		CSMsg::Log( eScreen, MSG_FOR_OPTION_NO_DEPENDENCY_CANT_HAVE_MORE_THAN_ONE_INPLAN );
		return;
	}// if

	// Дали е избрана опция от радиобутоните сума/процент
	if( m_nRdbPercentSolidSum != (short)eSolidSum
		&& m_nRdbPercentSolidSum != (short)ePercent )
	{
		CSMsg::Log( eScreen, MSG_CHOOSE_OPTION_PERCENT_OR_SOLID_SUM );
		return;
	}// if

	// Ако е брой събития, проверяваме дали брой 'от' е 
	if( m_nRdbBalanceNumEvents == (short)eEventsCount )
	{
		if( m_nNumEventsFrom != m_pBuf->m_arrPlan.GetMaxEventCountTo() +1 )
		{
			CSMsg::Log( eScreen, MSG_INNCORECT_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_NUMBER_EVENTS_FROM ) ) );
			m_nNumEventsFrom = m_pBuf->m_arrPlan.GetMaxEventCountTo() + 1;
			m_edbNumEventsFrom.SetFocus();
			UpdateData( FALSE );
			return;
		}//if

		if( m_nNumEventsFrom > m_nNumEventsTo )
		{	
			CSMsg::Log( eScreen, MSG_NUM_EVENTS_TO_SMALLER_THAN_FROM );
			m_nNumEventsTo = m_nNumEventsFrom +1;
			m_edbNumEventsТо.SetFocus();
			UpdateData( FALSE );
			return;
		}// if

	}// if

	//	Ако е по салдо/оборот, проверяваме дали е сума над е по-голяма от последната за салдо или оборот
	if( m_nRdbBalanceNumEvents == (short)eBalanceTurnover )
	{
		if( StoreSelDetValueFromComboX( m_cmbDependsOn, lBalTurnover ) == CB_ERR || lBalTurnover <= 0 )
		{
			CSMsg::Log( eScreen, MSG_INNCORECT_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_DEPENDS_ON ) ) );
			m_cmbDependsOn.SetFocus();
			return;
		}// if

		if( m_edbSumOver.GetSum() <= m_pBuf->m_arrPlan.GetMaxSumOver( (short)lBalTurnover ) )
		{
			CSNomMsgIDCode oBalanceTurnover( g_arrPreferenceTaxBalanceTurnover, _countof( g_arrPreferenceTaxBalanceTurnover ) );
			CString strType = "";
			strType = oBalanceTurnover.GetMsgUserLang( (short)lBalTurnover );
			CSMsg::Log( eScreen, MSG_SUM_OVER_FOR_TYPE_MUST_BE_BIGGER_THAN_LAST, &MSGKEY( "0", strType ), &MSGKEY( "1", strType ) );
			m_edbSumOver.SetFocus();
			return;
		}// if

	}// if

	// Ако е избрано 'Комбинация м/у зделка и услуга' проверяваме дали имаме валидна селекция
	// и при опит за добавяне дали избраният елемент вече не е добавен
	if( m_nRdbBalanceNumEvents == (short)eDealService )
	{
		int nCurSel = m_cmbDealTypeService.GetCurSel();
		if( nCurSel == CB_ERR )
		{
			m_cmbDealTypeService.SetFocus();
			return;
		} // if

		if( m_cmbDealTypeService.GetItemData( nCurSel ) <= 0 )
		{
			m_cmbDealTypeService.SetFocus();
			return;
		} // if

		int nPrefDealTypeCode = 
			(int)m_cmbDealTypeService.GetItemData( m_cmbDealTypeService.GetCurSel() );
		if( m_pBuf->m_arrPlan.GetCount() > 0 )
		{
			for( int i = 0; i < m_pBuf->m_arrPlan.GetCount() > 0; i++ )
			{
				PREFERENCIAL_PLANS *pRec = m_pBuf->m_arrPlan.GetAt( i );
				if( pRec == NULL )
					continue;

				if( pRec->m_nPrefDealTypeCode == nPrefDealTypeCode )
				{
					CSMsg::Log( eScreen, MSG_PREF_PLAN_HAS_COMBINATION );
					return;
				} // if
			} // for
		} // if
	} // if
 
	//	Ако е сума, проверяваме дали е по-голяма от нула
	if( m_nRdbPercentSolidSum == (short)eSolidSum )
	{
		if( m_edbSolidSum.GetSum() < 0.0 )
		{
			CSMsg::Log( eScreen, MSG_INNCORECT_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_FIXED_SUM ) ) );
			m_edbSolidSum.SetFocus();
			return;
		}// if
	}// if

	//	Ако е процент, проверяваме дали е по-голям от нула
	if( m_nRdbPercentSolidSum == (short)ePercent )
	{
		if( m_edbPercent.GetSum() < 0.0 )
		{
			CSMsg::Log( eScreen, MSG_INNCORECT_VALUE_IN_FIELD, &MSGKEY( "0", CSMsg::GetMsgUsrLang( MSG_PERCENT ) ) );
			m_edbPercent.SetFocus();
			return;
		}// if
	}// if

	// Създаваме нов елемент
	PREFERENCIAL_PLANS *pNewItem = new PREFERENCIAL_PLANS();
	if( !pNewItem )
		return;

	pNewItem->m_lCode			= 0;
	pNewItem->m_lPreferenceCode = m_pBuf->m_recPrefTax.m_lCode;
	pNewItem->m_sDependType		= m_nRdbBalanceNumEvents;

	if( m_nRdbBalanceNumEvents == (short)eEventsCount )
	{
		pNewItem->m_nEventCountFrom = m_nNumEventsFrom;
		pNewItem->m_nEventCountTo = m_nNumEventsTo;

		m_nNumEventsFrom = m_nNumEventsTo +1;
		m_nNumEventsTo = m_nNumEventsFrom;
	}// if

	if( m_nRdbBalanceNumEvents == (short)eBalanceTurnover )
	{
		pNewItem->m_sBalanceTurnoverType = (short)lBalTurnover;
		pNewItem->m_fSumOver = m_edbSumOver.GetSum();
	}// if

	if( m_nRdbBalanceNumEvents == (short)eDealService )
	{
		pNewItem->m_nPrefDealTypeCode = 
			(int)m_cmbDealTypeService.GetItemData( m_cmbDealTypeService.GetCurSel() );
	}

	pNewItem->m_sPreferenceType = m_nRdbPercentSolidSum;

	if( m_nRdbPercentSolidSum == (short)eSolidSum )
		pNewItem->m_fSumSolid = m_edbSolidSum.GetSum();

	if( m_nRdbPercentSolidSum == (short)ePercent )
		pNewItem->m_fPercent = m_edbPercent.GetSum();	

	m_pBuf->m_arrPlan.Add( pNewItem );

	if( m_pBuf->m_arrPlan.GetCount() > 0 )
	{
		m_chbGratisSumValidity.EnableWindow( FALSE );
		m_edbGratisMonths.SetWindowText( "0" );
		m_edbGratisSum.SetSum( 0.0 );
	}// if

	LoadPlan();

	UpdateData( FALSE );
}

void CPreferencialTaxMainPage::OnBnClickedDel()
{
	if( !m_pBuf )
		return;

	int nSelection = GetSelected( m_lstPlan );

	if( nSelection == LB_ERR
		|| AfxMessageBox( CSMsg::GetMsgUsrLang( MSG_BFROFC_3681 ), MB_ICONQUESTION|MB_YESNO ) != IDYES  )
		return;

	PREFERENCIAL_PLANS *pItem = m_pBuf->m_arrPlan.GetAt( nSelection );

	delete pItem;
	m_pBuf->m_arrPlan.RemoveAt( nSelection );

	if( m_pBuf->m_arrPlan.GetCount() <= 0
		&& m_eDlgMode != eView 
		&& m_eDlgMode != eViewNoDeactivate
		&& m_eDlgMode != eEditDatesOnly
		&& m_pBuf->m_recPrefTax.m_nDealType == KCK_DEALS  )
	{
		m_chbGratisSumValidity.EnableWindow( TRUE );
	}// if

	LoadPlan();
}

void CPreferencialTaxMainPage::CalculateEndDate()
{
	UpdateData( TRUE );

	LDATE lBeginDate = 0, lEndDate = 0;

	if( m_chbDependsOnDealOpenDate.GetCheck() > 0 )
		lBeginDate = m_edbDateDealTo.GetDate();
	else
		lBeginDate = m_edbDateActiveFrom.GetDate();

	if( lBeginDate <= MIN_DATE )
		return;

	if( m_nRdbDaysMonths != (int)eDaysPeriod
		&& m_nRdbDaysMonths != (int)eMonthsPeriod )
		return;

	if( m_nRdbDaysMonths == (int)eDaysPeriod )
	{
		LoadPeriodicities((short)eDayly );
		lEndDate = korldat( lBeginDate, m_nNumDays, LIHWA_365_365 );
	}// if
	else if( m_nRdbDaysMonths == (int)eMonthsPeriod )
	{
		m_cmbPeriodicity.EnableWindow( m_eDlgMode == eEditFull 
										|| m_eDlgMode == eEditNoDealType 
										|| m_eDlgMode == eAdd  
										|| m_eDlgMode == eAddNoStdDog
										|| m_eDlgMode == eAddKlientska 
										|| m_eDlgMode == eEditKlientska );

		lEndDate = korlmnt( lBeginDate, m_nNumMonths, LIHWA_365_365 );
	}// else

	m_edbDateActiveTo.SetDate( lEndDate );
}

void CPreferencialTaxMainPage::OnEnKillfocusEdbTermFrom()
{
	CalculateEndDate();
}

void CPreferencialTaxMainPage::OnEnKillfocusEdbMonths()
{
	CalculateEndDate();
}

void CPreferencialTaxMainPage::OnEnKillfocusEdbDays()
{
	CalculateEndDate();
}

void CPreferencialTaxMainPage::OnBnClickedRdbMonths()
{
	if( m_eDlgMode == eView
		|| m_eDlgMode == eViewNoDeactivate
		|| m_eDlgMode == eEditConditionsOnly )
		return;

	if( m_rdbMohtns.GetCheck() > 0 )
	{
		m_nRdbDaysMonths = eMonthsPeriod;
	}
	else if ( m_rdbDays.GetCheck() > 0 )
	{
		m_nRdbDaysMonths = eDaysPeriod;
	}

	m_edbNumMonths.EnableWindow( m_rdbMohtns.GetCheck() > 0 );
	m_edbNumDays.EnableWindow( m_rdbDays.GetCheck() > 0 );

	CalculateEndDate();
}

void CPreferencialTaxMainPage::OnBnClickedChkDependsOnDateOpen()
{
	if( m_eDlgMode == eView
		|| m_eDlgMode == eViewNoDeactivate )
		return;

	if( m_eDlgMode != eEditDatesOnly )
	{
		m_edbDateDealFrom.EnableWindow( m_chbDependsOnDealOpenDate.GetCheck() > 0 );
		m_edbDateDealTo.EnableWindow( m_chbDependsOnDealOpenDate.GetCheck() > 0 );
	}

	CalculateEndDate();
}

void CPreferencialTaxMainPage::OnEnKillfocusEdbDateOpenTo()
{
	CalculateEndDate();
}

void CPreferencialTaxMainPage::OnBnClickedRdbDependsOn()
{
	if( m_eDlgMode == eView
		|| m_eDlgMode == eViewNoDeactivate
		|| m_eDlgMode == eEditDatesOnly )
		return;

	if( m_pBuf->m_arrPlan.GetCount() <= 0 )
		m_cmbDependsOn.EnableWindow( m_rdbBalance.GetCheck() > 0 );

	m_edbSumOver.EnableWindow( m_rdbBalance.GetCheck() > 0 );

	m_edbNumEventsFrom.EnableWindow( m_rdbNumEvents.GetCheck() > 0 );
	m_edbNumEventsТо.EnableWindow( m_rdbNumEvents.GetCheck() > 0 );

	m_rdbDealTypeService.SetCheck( BST_UNCHECKED );
	m_cmbDealTypeService.EnableWindow( m_rdbDealTypeService.GetCheck() > 0 );

	if( m_rdbNoDependancy.GetCheck() > 0 && m_pBuf->m_arrPlan.GetCount() <= 0 )
	{
		m_pBuf->m_arrPlan.Release();
		m_lstPlan.DeleteAllItems();
	}// if

	if ( m_rdbBalance.GetCheck() > 0 )
	{
		m_nRdbBalanceNumEvents = eBalanceTurnover;
	}
	else if ( m_rdbNumEvents.GetCheck() > 0 )
	{
		m_nRdbBalanceNumEvents = eEventsCount;
	}
	else if( m_rdbNoDependancy.GetCheck() > 0 )
	{
		m_nRdbBalanceNumEvents = eNoDependancy;
	}
}

void CPreferencialTaxMainPage::OnBnClickedRdbDependsOnDealTypeService()
{
	if( m_eDlgMode == eView
		|| m_eDlgMode == eViewNoDeactivate
		|| m_eDlgMode == eEditDatesOnly )
		return;

	m_rdbBalance.SetCheck( BST_UNCHECKED );
	m_rdbNumEvents.SetCheck( BST_UNCHECKED );
	m_rdbNoDependancy.SetCheck( BST_UNCHECKED );

	if( m_pBuf->m_arrPlan.GetCount() <= 0 )
		m_cmbDependsOn.EnableWindow( m_rdbBalance.GetCheck() > 0 );

	m_edbSumOver.EnableWindow( m_rdbBalance.GetCheck() > 0 );

	m_edbNumEventsFrom.EnableWindow( m_rdbNumEvents.GetCheck() > 0 );
	m_edbNumEventsТо.EnableWindow( m_rdbNumEvents.GetCheck() > 0 );

	if( m_rdbNoDependancy.GetCheck() > 0 && m_pBuf->m_arrPlan.GetCount() <= 0 )
	{
		m_pBuf->m_arrPlan.Release();
		m_lstPlan.DeleteAllItems();
	}// if

	m_cmbDealTypeService.EnableWindow( m_rdbDealTypeService.GetCheck() > 0 );
	if( m_rdbDealTypeService.GetCheck() > 0 )
	{
		m_nRdbBalanceNumEvents = eDealService;
	}

	LoadDealTypeBankServiceCombinationsInCombo( m_pBuf->m_recPrefTax.m_nDealType );
}

void CPreferencialTaxMainPage::OnBnClickedRdbPercent()
{
	if( m_eDlgMode == eView
		|| m_eDlgMode == eViewNoDeactivate
		|| m_eDlgMode == eEditDatesOnly )
		return;

	if( m_rdbPercent.GetCheck() > 0 )
	{
		m_nRdbPercentSolidSum = ePercent;
	} else if ( m_rdbSolidSum.GetCheck() > 0 ) {
		m_nRdbPercentSolidSum = eSolidSum;
	}

	m_edbPercent.EnableWindow( m_rdbPercent.GetCheck() > 0 && m_chbGratisSumValidity.GetCheck() <= 0 );
	m_edbSolidSum.EnableWindow( m_rdbSolidSum.GetCheck() > 0 && m_chbGratisSumValidity.GetCheck() <= 0 );
}
#pragma endregion MainPage

//////////////////////////////////////////////////////////////////////////////////////////////////
//	CPreferencialTaxEventsTaxesPage
BEGIN_LANG_WND( CPreferencialTaxEventsTaxesPage )
	WND( WND_TITLE,							MSG_EVENTS_AND_TAXES			),
	WND( IDC_STT_EVENTS_AND_TAXES_BY_PREF,	MSG_PREFERENCE_TAXES_AND_EVENTS	),
END_LANG_WND()

IMPLEMENT_DYNAMIC( CPreferencialTaxEventsTaxesPage, CBasePropertyPage )

CPreferencialTaxEventsTaxesPage::CPreferencialTaxEventsTaxesPage( CPreferencialTaxFullRec & recPrefTax, const short sDlgMode, long lSelStdDogCode /*= NO_DETCODE*/, CWnd* pParent )
: CBasePropertyPage( CPreferencialTaxEventsTaxesPage::IDD, GetHISTANCEBfrofCDLL() )
, m_eDlgMode( (ePrefTaxDlgMode)sDlgMode )
{
	m_pBuf = &recPrefTax;
	m_arrEvents.SetCompFunction( CODE_NAME::compByCode );
	m_lSelStdDogCode = lSelStdDogCode;
}
CPreferencialTaxEventsTaxesPage::~CPreferencialTaxEventsTaxesPage()
{
}

void CPreferencialTaxEventsTaxesPage::DoDataExchange( CDataExchange* pDX )
{
	CBasePropertyPage::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_TREE_EVENTS_TAXES, m_treeEventsTaxes );
}

BEGIN_MESSAGE_MAP( CPreferencialTaxEventsTaxesPage, CBasePropertyPage )
	ON_NOTIFY(	NM_RCLICK,		IDC_TREE_EVENTS_TAXES, OnNMRclickTree )
	ON_COMMAND( ID_ADD_EVENT,	&CPreferencialTaxEventsTaxesPage::OnTreeAddEvent )
	ON_COMMAND( ID_DELETE_EVENT,&CPreferencialTaxEventsTaxesPage::OnTreeRemoveEvent )
	ON_COMMAND( ID_ADD_TAX,		&CPreferencialTaxEventsTaxesPage::OnTreeAddTax )
	ON_COMMAND( ID_DELETE_TAX,	&CPreferencialTaxEventsTaxesPage::OnTreeRemoveTax )
	ON_MESSAGE( ID_WM_DEAL_TYPE_CHANGED, OnDealTypeChanged )
END_MESSAGE_MAP()

LRESULT CPreferencialTaxEventsTaxesPage::OnDealTypeChanged( WPARAM wParam, LPARAM lParam )
{
	if( !m_pBuf )
		return FALSE;
	
	m_pBuf->m_arrEventsTaxes.Release();

	m_treeEventsTaxes.DeleteAllItems();

	return TRUE;
}

BOOL CPreferencialTaxEventsTaxesPage::OnInitDialog() 
{
	CBasePropertyPage::OnInitDialog();

	if( !LoadTree() )
	{
		CSMsg::Log( eScreen, MSG_ERR_VISUALIZE_EVENTS_TAXES );
		m_treeEventsTaxes.EnableWindow( FALSE );	
	}// if

	return TRUE;
}

bool CPreferencialTaxEventsTaxesPage::LoadTree()
{
	if( !m_pBuf )
		return false;
	
	HTREEITEM hTreeItem = NULL;
	PREFERENCIAL_EVENTS_TAXES *pItem = NULL;
	CString strEventName, strBuf;

	m_treeEventsTaxes.DeleteAllItems();

	for( int i = 0; i < m_pBuf->m_arrEventsTaxes.GetCount(); i ++ )
	{
		pItem = m_pBuf->m_arrEventsTaxes.GetAt( i );

		if( !pItem )
			continue;
		
		if( pItem->m_lTaxCode > 0 )
			continue;

		if( !GetSheet()->GetEventName( pItem->m_lEventCode, strEventName ) )
			continue;

		strBuf.Format( " %06d %s ", pItem->m_lEventCode, strEventName );

		hTreeItem = m_treeEventsTaxes.InsertItem( strBuf );
		m_treeEventsTaxes.SetItemData( hTreeItem, (DWORD_PTR)pItem );

		if( !LoadTaxes( hTreeItem, pItem->m_lEventCode ) )
			return false;
	}// for

	return true;
}

bool CPreferencialTaxEventsTaxesPage::LoadTaxes( HTREEITEM hParentItem, const long lEventCode )
{
	PREFERENCIAL_EVENTS_TAXES *pItem = NULL;
	CString strTaxName = "", strBuf = "";
	HTREEITEM hTreeItem = NULL;

	for( int i = 0; i < m_pBuf->m_arrEventsTaxes.GetCount(); i ++ )
	{
		pItem = m_pBuf->m_arrEventsTaxes.GetAt( i );

		if( !pItem )
			continue;

		if( pItem->m_lTaxCode <= 0 )
			continue;

		if( pItem->m_lEventCode != lEventCode )
			continue;

		if( !GetTaxName( pItem->m_lTaxCode, strTaxName ) )
			return false;

		strBuf.Format( " %06d %s ", pItem->m_lTaxCode, strTaxName );

		hTreeItem = m_treeEventsTaxes.InsertItem( strBuf, hParentItem );
		
		m_treeEventsTaxes.SetItemData( hTreeItem, (DWORD_PTR)pItem );
	}// for
	
	return true;
}

bool CPreferencialTaxEventsTaxesPage::GetTaxName( long lTaxCode, CString & strName )
{
	if( !fnCallRpc( CParameterT< long >( lTaxCode )
		, RPC_BFROF_GET_TAX_NAME_BY_CODE
		, CParameterT< CString >( strName ) ) )
		return false;
	
	return true;
}

bool CPreferencialTaxEventsTaxesPage::GetTaxName( const long lTaxCode, const CSCmnRecsArray< CODE_NAME > & arrTaxesCodeNames, CString & strName )
{
	strName = "";
	
	for( int i = 0; i < arrTaxesCodeNames.GetCount(); i++ )
		if( arrTaxesCodeNames.GetAt( i )->lCode == lTaxCode )
		{
			strName = arrTaxesCodeNames.GetAt( i )->szName;
			break;
		}// if 

	return strName != "";
}

void CPreferencialTaxEventsTaxesPage::OnNMRclickTree( NMHDR* pNMHDR, LRESULT* pResult )
{
	if ( m_eDlgMode != eEditFull
		&& m_eDlgMode != eEditNoDealType
		&& m_eDlgMode != eAdd
		&& m_eDlgMode != eAddKlientska
		&& m_eDlgMode != eEditKlientska )
		return;

	CPoint pos;

	GetCursorPos( &pos );
	m_treeEventsTaxes.ScreenToClient( &pos );
	HTREEITEM hSelectedItem = m_treeEventsTaxes.HitTest( pos );

	if ( !hSelectedItem )
	{
		GetCursorPos( &pos );
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu( MF_STRING, ID_ADD_EVENT, CSMsg::GetMsgUsrLang( MSG_ADD_EVENT_TYPE ) );
		menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this );
		return;
	}// if

	m_treeEventsTaxes.SelectItem( hSelectedItem );
	m_treeEventsTaxes.ClientToScreen( &pos );
	
	CMenu menu;
	menu.CreatePopupMenu();

	// Избрано е тип събитие
	if( m_treeEventsTaxes.GetParentItem( hSelectedItem ) == NULL )
	{
		menu.AppendMenu( MF_STRING, ID_ADD_EVENT, CSMsg::GetMsgUsrLang( MSG_ADD_EVENT_TYPE ) );
		menu.AppendMenu( MF_STRING, ID_DELETE_EVENT, CSMsg::GetMsgUsrLang( MSG_BFROFC_3055 ) );
		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, ID_ADD_TAX, CSMsg::GetMsgUsrLang( MSG_ADD_TAX ) );
	}//if
	else // Избрана е такса
	{
		menu.AppendMenu( MF_STRING, ID_DELETE_TAX, CSMsg::GetMsgUsrLang( MSG_BFROFC_3055 ) );
	}// else

	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this );
}

void CPreferencialTaxEventsTaxesPage::OnTreeAddEvent()
{
	if( !m_pBuf )
		return;

	CSCmnRecsArray< CODE_NAME > arrEvents;

	BOOL bIsInList = FALSE;

	for( int i = 0; i < GetSheet()->m_arrEvents.GetCount(); i++ )
	{
		bIsInList = FALSE;

		for( int j =0; j < m_pBuf->m_arrEventsTaxes.GetCount(); j++ )	
		{
			if( m_pBuf->m_arrEventsTaxes.GetAt( j )->m_lTaxCode > 0 )
				continue;

			if( m_pBuf->m_arrEventsTaxes.GetAt( j )->m_lEventCode == GetSheet()->m_arrEvents.GetAt( i )->lCode )
			{
				bIsInList = TRUE;
				break;
			}// if
		}// for

		if( bIsInList )
			continue;

		CODE_NAME * pCodeName = new CODE_NAME();
		pCodeName->lCode = GetSheet()->m_arrEvents.GetAt( i )->lCode;
		ourstrncpy( pCodeName->szName, GetSheet()->m_arrEvents.GetAt( i )->szName, sizeof( pCodeName->szName ) - 1 );
			
		arrEvents.Add( pCodeName );
	}// for

	if( arrEvents.GetCount() <= 0 )
	{
		CSMsg::GetMsgUsrLang( MSG_ERR_LOAD_EVENTS_OR_ALL_ADDED );
		return;
	}// if

	CCodeNameChooseDlg oDlg( arrEvents, CSMsg::GetMsgUsrLang( MSG_CHOOSE_EVENTS ), TRUE );

	if( oDlg.DoModal() != IDOK )
		return;

	CInt64Array arrSelectedEventCodes;
	
	oDlg.GetSelectedCodes( arrSelectedEventCodes );

	if( arrSelectedEventCodes.GetSize() <= 0 )
		return;

	HTREEITEM hTreeItem = NULL;

	CString strName = "", strBuf = "";

	for( int i = 0; i < arrSelectedEventCodes.GetSize(); i++ )
	{
		if( !GetSheet()->GetEventName( (long)arrSelectedEventCodes.GetAt( i ), strName ) )
			break;

		PREFERENCIAL_EVENTS_TAXES * pNewItem = new PREFERENCIAL_EVENTS_TAXES();

		pNewItem->m_lPreferenceCode = m_pBuf->m_recPrefTax.m_lCode;
		pNewItem->m_lEventCode = (long)arrSelectedEventCodes.GetAt( i );
		
		m_pBuf->m_arrEventsTaxes.Add( pNewItem );

		strBuf.Format( " %06d %s ",pNewItem->m_lEventCode , strName );
			
		hTreeItem = m_treeEventsTaxes.InsertItem( strBuf );
		m_treeEventsTaxes.SetItemData( hTreeItem, (DWORD_PTR)pNewItem );
	}// for
}

bool CPreferencialTaxEventsTaxesPage::LoadTaxesForKlietskaPref( long lEventCode, long lDealType, long lDealNum, bool bStandartTaxesOnly, CSCmnRecsArray< CODE_NAME > & arrTaxesCodeNames )
{	
	long lRet = 0;
	CRpcBufArray arrIn, arrOut;

	CSNomMsgIDCode oProcSystem( g_arrProcessingSystem, _countof( g_arrProcessingSystem ) );
	CSCmnRecsArray<long> arrProcSystems;

	for( INT_PTR i = 0, nCnt = oProcSystem.GetSize(); i < nCnt; i++ )
	{
		if( ( i == nCnt - 1 ) && oProcSystem.LastIsEmpty() )
			break;

		int nCode = oProcSystem.GetCodeByIndex( i );

		if( nCode == (int)eUndefinedProcessingSystem )
			continue;

		if( m_pBuf->m_recPrefTax.CheckProcessingSystemBit( nCode ) )
		{
			arrProcSystems.Add( new long( (long)nCode ) );
		}// if
	}// for

	arrIn << lEventCode << lDealType << lDealNum << bStandartTaxesOnly ;

	arrProcSystems.Serialize( arrIn );

	try 
	{
		lRet = RPCBfrofCDispatchHandler( MyHandle(), RPCSYS( RPC_BFROF_GET_TAXES_NAME_CODES_BY_DEALTYPE_AND_CODE ), &arrIn, &arrOut );
	} // try
	catch(...) 
	{
		glShowErrMsg( APP_CLIENTTYPE, NULL, "CPreferencialTaxEventsTaxesPage::LoadTaxesForKlietskaPref", NULL );
		lRet = 0;
	} // catch

	if( lRet <= 0 )
		return false;

	for( int i = 0, nCount = arrOut.GetCnt(); i < nCount ; i++ )
	{
		long lSize = 0;

		CODE_NAME *pBuf = (CODE_NAME*)arrOut.GetAt( i, lSize );

		ASSERT( lSize == sizeof( CODE_NAME ) );

		CODE_NAME * pNewItem = new CODE_NAME();
		*pNewItem = *pBuf;

		arrTaxesCodeNames.Add( pNewItem );
	} // for

	return true;
}

bool CPreferencialTaxEventsTaxesPage::LoadTaxes( const long lEventType, const bool bStandartTaxesOnly/*=true*/, CSCmnRecsArray< CODE_NAME > & arrTaxesCodeNames )
{
	long lRet = 0;
	CRpcBufArray arrIn, arrOut;

	CSNomMsgIDCode oProcSystem( g_arrProcessingSystem, _countof( g_arrProcessingSystem ) );
	CSCmnRecsArray<long> arrProcSystems;

	for( INT_PTR i = 0, nCnt = oProcSystem.GetSize(); i < nCnt; i++ )
	{
		if( ( i == nCnt - 1 ) && oProcSystem.LastIsEmpty() )
			break;

		int nCode = oProcSystem.GetCodeByIndex( i );

		if( nCode == (int)eUndefinedProcessingSystem )
			continue;

		if( m_pBuf->m_recPrefTax.CheckProcessingSystemBit( nCode ) )
		{
			arrProcSystems.Add( new long( (long)nCode ) );
		}// if
	}// for
	
	arrIn << lEventType << (long)m_pBuf->m_recPrefTax.m_nDealType << m_pBuf->m_recPrefTax.m_lCode << m_lSelStdDogCode << bStandartTaxesOnly ;
	
	m_pBuf->m_arrTaxesToStdDeals.Serialize( arrIn );
	
	arrProcSystems.Serialize( arrIn );

	try 
	{
		lRet = RPCBfrofCDispatchHandler( MyHandle(), RPCSYS( RPC_BFROF_GET_TAXES_NAME_CODES_BYEVENT_TYPE ), &arrIn, &arrOut );
	} // try
	catch(...) 
	{
		glShowErrMsg( APP_CLIENTTYPE, NULL, "CPreferencialTaxEventsTaxesPage::LoadEventsByDealType", NULL );
		lRet = 0;
	} // catch

	if( lRet <= 0 )
		return false;

	for( int i = 0, nCount = arrOut.GetCnt(); i < nCount ; i++ )
	{
		long lSize = 0;

		CODE_NAME *pBuf = (CODE_NAME*)arrOut.GetAt( i, lSize );

		ASSERT( lSize == sizeof( CODE_NAME ) );

		CODE_NAME * pNewItem = new CODE_NAME();
		*pNewItem = *pBuf;

		arrTaxesCodeNames.Add( pNewItem );
	} // for

	return true;
}

void CPreferencialTaxEventsTaxesPage::OnTreeRemoveEvent()
{
	HTREEITEM hSelectedItem = m_treeEventsTaxes.GetSelectedItem();

	if ( hSelectedItem == NULL )
		return;

	if( m_treeEventsTaxes.GetParentItem( hSelectedItem ) != NULL )
		return;
	
	RemoveItem( hSelectedItem );
}

void CPreferencialTaxEventsTaxesPage::OnTreeAddTax()
{
	HTREEITEM hSelectedItem = m_treeEventsTaxes.GetSelectedItem();

	if ( hSelectedItem == NULL )
		return;

	if( m_treeEventsTaxes.GetParentItem( hSelectedItem ) != NULL )
		return;

	PREFERENCIAL_EVENTS_TAXES *pEvent =	(PREFERENCIAL_EVENTS_TAXES*)m_treeEventsTaxes.GetItemData( hSelectedItem );

	if( !pEvent || pEvent->m_lTaxCode > 0  || pEvent->m_lEventCode <= 0 )
		return;

	CSCmnRecsArray<CODE_NAME> arrTaxes;
	CSCmnRecsArray<CODE_NAME> arrTaxesFiltered;

	if( m_pBuf->m_recPrefTax.IsKlientskaPref() )
	{
		if( !LoadTaxesForKlietskaPref( pEvent->m_lEventCode, m_pBuf->m_recPrefTax.m_nDealType, m_pBuf->m_recPrefToDeal.m_lDealNum, false, arrTaxes ) )
			return;
	}
	else {
		if( !LoadTaxes( pEvent->m_lEventCode, m_pBuf->m_recPrefTax.m_nDealType != KCK_DEALS, arrTaxes  ) )
			return;
	}//else

	BOOL bIsInList = FALSE;

	for( int i = 0; i < arrTaxes.GetCount(); i++ )
	{
		bIsInList = FALSE;

		for( int j =0; j < m_pBuf->m_arrEventsTaxes.GetCount(); j++ )	
			if( m_pBuf->m_arrEventsTaxes.GetAt( j )->m_lTaxCode == arrTaxes.GetAt( i )->lCode )
			{
				bIsInList = TRUE;
				break;
			}// if

		if( bIsInList )
			continue;

		CODE_NAME * pCodeName = new CODE_NAME();
		pCodeName->lCode = arrTaxes.GetAt( i )->lCode;
		ourstrncpy( pCodeName->szName, arrTaxes.GetAt( i )->szName, sizeof( pCodeName->szName ) - 1 );

		arrTaxesFiltered.Add( pCodeName );
	}// for
	arrTaxesFiltered.SetCompFunction( CODE_NAME::compByCode );

	arrTaxesFiltered.QSort();

	CCodeNameChooseDlg oDlg( arrTaxesFiltered, CSMsg::GetMsgUsrLang( MSG_CHOOSE_TAXES ), TRUE );

	if( oDlg.DoModal() != IDOK )
		return;

	CInt64Array arrSelectedTax;

	oDlg.GetSelectedCodes( arrSelectedTax );

	if( arrSelectedTax.GetSize() <= 0 )
		return;

	HTREEITEM hInsertedItem = NULL;

	CString strName = "", strBuf = "";

	for( int i = 0; i < arrSelectedTax.GetSize(); i++ )
	{
		PREFERENCIAL_EVENTS_TAXES * pNewItem = new PREFERENCIAL_EVENTS_TAXES();

		pNewItem->m_lPreferenceCode = m_pBuf->m_recPrefTax.m_lCode;
		pNewItem->m_lEventCode		= pEvent->m_lEventCode;
		pNewItem->m_lTaxCode		= (long)arrSelectedTax.GetAt( i );

		m_pBuf->m_arrEventsTaxes.Add( pNewItem );

		GetTaxName( pNewItem->m_lTaxCode, arrTaxesFiltered, strName );	

		strBuf.Format( " %06d %s ",pNewItem->m_lTaxCode , strName );

		hInsertedItem = m_treeEventsTaxes.InsertItem( strBuf, hSelectedItem );
		m_treeEventsTaxes.SetItemData( hInsertedItem, (DWORD_PTR)pNewItem );
	}// for
}

void CPreferencialTaxEventsTaxesPage::OnTreeRemoveTax()
{
	HTREEITEM hSelectedItem = m_treeEventsTaxes.GetSelectedItem();

	if ( hSelectedItem == NULL )
		return;

	if( m_treeEventsTaxes.GetParentItem( hSelectedItem ) == NULL )
		return;

	RemoveItem( hSelectedItem );
	
}

void CPreferencialTaxEventsTaxesPage::RemoveItem( const HTREEITEM hSelectedItem )
{
	PREFERENCIAL_EVENTS_TAXES *pItem = (PREFERENCIAL_EVENTS_TAXES*)m_treeEventsTaxes.GetItemData( hSelectedItem );

	if( !pItem )
		return;

	if( !m_pBuf )
		return;

	long lEventCode = 0;

	for ( int i = 0; i < m_pBuf->m_arrEventsTaxes.GetCount(); i++)
	{
		PREFERENCIAL_EVENTS_TAXES* pEvtTax = m_pBuf->m_arrEventsTaxes[i];

		if ( pEvtTax == pItem )
		{
			// Ако е събитие, махаме таксите от масива
			if( pEvtTax->m_lTaxCode <= 0 )
				lEventCode = pEvtTax->m_lEventCode;

			m_pBuf->m_arrEventsTaxes.RemoveAt(i);
			delete pEvtTax;

			m_treeEventsTaxes.DeleteItem( hSelectedItem );
			break;
		}// if
	}// for

	// Ако е събитие, махаме таксите от масива
	if( lEventCode > 0 )
		for ( int i = m_pBuf->m_arrEventsTaxes.GetCount() - 1; i >=0 ; i-- )
		{
			if( m_pBuf->m_arrEventsTaxes.GetAt( i ) == NULL )
				break;
			
			if( m_pBuf->m_arrEventsTaxes.GetAt( i )->m_lEventCode == lEventCode )
			{	
				delete m_pBuf->m_arrEventsTaxes.GetAt( i );
				m_pBuf->m_arrEventsTaxes.RemoveAt( i );
			}// if
		}// for
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//	CPreferencialTaxEventsTaxesPage
BEGIN_LANG_WND( CPreferencialTaxStgDogsPage )
	WND( WND_TITLE,				MSG_ASSIGN_TO_STG_DOGS	),
	WND( IDC_BTN_ADD_STD_DOGS,	MSG_BFROFC_3054	),
	WND( IDC_BTN_DEL_STD_DOG,	MSG_BFROFC_3055	),
END_LANG_WND()

IMPLEMENT_DYNAMIC( CPreferencialTaxStgDogsPage, CBasePropertyPage )

CPreferencialTaxStgDogsPage::CPreferencialTaxStgDogsPage( CPreferencialTaxFullRec & recPrefTax
														 , const short sPrefTaxDlgMode/*ePrefTaxDlgMode*/
														 , CWnd* pParent/* = NULL */
														 , const BOOL bEditStdDogs/* = TRUE*/)
	 : CBasePropertyPage( CPreferencialTaxStgDogsPage::IDD, GetHISTANCEBfrofCDLL() )
	 , m_eDlgMode( (ePrefTaxDlgMode)sPrefTaxDlgMode )
	 , m_bEditStdDogs( bEditStdDogs )
{
	m_pBuf					= &recPrefTax;
	m_arrStdDogs.SetCompFunction( CODE_NAME::compByCode );
	m_pBuf->m_arrTaxesToStdDeals.SetCompFunction( PREFERENCIAL_TAXES_TO_STD_DEALS::compByDealCode );
}

CPreferencialTaxStgDogsPage::~CPreferencialTaxStgDogsPage()
{
}

void CPreferencialTaxStgDogsPage::DoDataExchange( CDataExchange* pDX )
{
	CBasePropertyPage::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_LST_STD_DOGS,			m_listStdDogs );
	DDX_Control( pDX, IDC_BTN_ADD_STD_DOGS,		m_btnAdd );
	DDX_Control( pDX, IDC_BTN_DEL_STD_DOG,		m_btnDel );
}

BEGIN_MESSAGE_MAP( CPreferencialTaxStgDogsPage, CBasePropertyPage )
	ON_BN_CLICKED( IDC_BTN_ADD_STD_DOGS,	&CPreferencialTaxStgDogsPage::OnBnClickedAddStdDogs )
	ON_BN_CLICKED( IDC_BTN_DEL_STD_DOG,		&CPreferencialTaxStgDogsPage::OnBnClickedDelStdDog )
	ON_MESSAGE( ID_WM_DEAL_TYPE_CHANGED,	OnDealTypeChanged )
END_MESSAGE_MAP()

void CPreferencialTaxStgDogsPage::OnBnClickedAddStdDogs()
{
	if( !EnsureStdDogs() )
		return;

	CSCmnRecsArray< CODE_NAME > arrStdDogs;

	BOOL bIsInList = FALSE;

	for( int i = 0; i < m_arrStdDogs.GetCount(); i++ )
	{
		bIsInList = FALSE;

		for( int j = 0; j < m_pBuf->m_arrTaxesToStdDeals.GetCount(); j++ )	
		{
			if( m_pBuf->m_arrTaxesToStdDeals.GetAt( j )->m_lStdDogCode == m_arrStdDogs.GetAt( i )->lCode )
			{
				bIsInList = TRUE;
				break;
			}// if
		}// for

		if( bIsInList )
			continue;

		CODE_NAME * pCodeName = new CODE_NAME();
		pCodeName->lCode = m_arrStdDogs.GetAt( i )->lCode;
		ourstrncpy( pCodeName->szName, m_arrStdDogs.GetAt( i )->szName, sizeof( pCodeName->szName ) - 1 );

		arrStdDogs.Add( pCodeName );
	}// for


	CCodeNameChooseDlg oDlg( arrStdDogs, CSMsg::GetMsgUsrLang( MSG_CHOOSE_STD_DOGS ), TRUE );

	if( oDlg.DoModal() != IDOK )
		return;

	CInt64Array arrSelectedStdDogCodes;

	oDlg.GetSelectedCodes( arrSelectedStdDogCodes );

	if( arrSelectedStdDogCodes.GetSize() <= 0 )
		return;

	int nSubItem = 1;
	CString strName = "", strBuf = "";

	for( int i = 0; i < arrSelectedStdDogCodes.GetSize(); i++ )
	{
		if( !GetStdDogName( (long)arrSelectedStdDogCodes.GetAt( i ), strName ) )
			break;

		PREFERENCIAL_TAXES_TO_STD_DEALS * pNewItem = new PREFERENCIAL_TAXES_TO_STD_DEALS();

		pNewItem->m_lPreferencialTaxCode	= m_pBuf->m_recPrefTax.m_lCode;
		pNewItem->m_lStdDogCode				= (long)arrSelectedStdDogCodes.GetAt( i );
		pNewItem->m_lDealType				= (long)m_pBuf->m_recPrefTax.m_nDealType;

		m_pBuf->m_arrTaxesToStdDeals.Add( pNewItem );

		strBuf.Format( "%6.6d",pNewItem->m_lStdDogCode );

		int nLCIndex = m_listStdDogs.InsertItem( m_listStdDogs.GetItemCount(), strBuf );
		
		nSubItem = 1;
		// Име
		m_listStdDogs.SetItemText( nLCIndex, nSubItem++, strName );

		m_listStdDogs.SetItemData( nLCIndex, (DWORD_PTR)pNewItem );
	}// for
}

void CPreferencialTaxStgDogsPage::OnBnClickedDelStdDog()
{
	POSITION pos = m_listStdDogs.GetFirstSelectedItemPosition();

	if( pos == NULL)
		return;

	CInt64Array arrIndexes;

	for( UINT i = 0; i <  m_listStdDogs.GetSelectedCount(); i++ )
		arrIndexes.Add( m_listStdDogs.GetNextSelectedItem( pos ) );

	for( int i = 0; i <  arrIndexes.GetSize(); i++ )
	{
		if( arrIndexes.GetAt( i ) == LB_ERR )
			return;

		PREFERENCIAL_TAXES_TO_STD_DEALS *pItem = (PREFERENCIAL_TAXES_TO_STD_DEALS*)m_listStdDogs.GetItemData( (int)arrIndexes.GetAt( i ) );

		for( int j=0; j < m_pBuf->m_arrTaxesToStdDeals.GetCount(); j++ )
			if( pItem == m_pBuf->m_arrTaxesToStdDeals.GetAt( j ) )
			{
				delete pItem;
				m_pBuf->m_arrTaxesToStdDeals.RemoveAt( j );
			}

	}// for

	LoadList();
}

LRESULT CPreferencialTaxStgDogsPage::OnDealTypeChanged( WPARAM wParam, LPARAM lParam )
{
	if( !m_pBuf )
		return FALSE;

	m_pBuf->m_arrTaxesToStdDeals.Release();

	m_listStdDogs.DeleteAllItems();

	long lDealType = (long)wParam;

	m_arrStdDogs.Release();

	LoadStdDogsByDealType( lDealType, m_arrStdDogs );

	m_arrStdDogs.QSort();

	return TRUE;
}

BOOL CPreferencialTaxStgDogsPage::OnInitDialog() 
{
	CBasePropertyPage::OnInitDialog();

	WORD wCol = 0;

	m_listStdDogs.InsertColumn( wCol++, CSMsg::GetMsgUsrLang( MSG_FOREIGN_EXC_TRANS_SHOW_HIST_CODE ),	LVCFMT_RIGHT,	m_listStdDogs.GetStringWidth("#########"),		wCol );
	m_listStdDogs.InsertColumn( wCol++, CSMsg::GetMsgUsrLang( MSG_STD_DO_NAME ),						LVCFMT_LEFT,	m_listStdDogs.GetStringWidth("########################################"),	wCol );

	DWORD dwStyleEx = m_listStdDogs.GetExtendedStyle(); 
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	m_listStdDogs.SetExtendedStyle( dwStyleEx );

	if( ( m_eDlgMode != eAdd
			&& m_eDlgMode != eEditFull
			&& m_eDlgMode != eEditNoDealType 
		)
		|| !m_bEditStdDogs 
	  )
	{
		m_btnAdd.EnableWindow( FALSE );	
		m_btnDel.EnableWindow( FALSE );	
	}// if

	if( !LoadList() )
	{
		CSMsg::Log( eScreen, MSG_ERR_VISUALIZE_EVENTS_TAXES );

		m_btnAdd.EnableWindow( FALSE );	
		m_btnDel.EnableWindow( FALSE );	
	}// if

	return TRUE;
}

bool CPreferencialTaxStgDogsPage::EnsureStdDogs()
{
	if( m_arrStdDogs.GetCount() > 0 )
		return true;

	if( !LoadStdDogsByDealType( m_pBuf->m_recPrefTax.m_nDealType, m_arrStdDogs ) )
		return false;
	
	return true;
}

bool CPreferencialTaxStgDogsPage::LoadStdDogsByDealType( const long lDealType, CSCmnRecsArray < CODE_NAME > & arrStdDogs )
{
	WORD wNom = 0;

	switch ( lDealType )
	{
		case RAZPL_DEALS:
			wNom = NOM_STD_DOG_RAZPL;
			break;
		case DEPOS_DEALS:
			wNom = NOM_STD_DOG_DEPOS;
			break;
		case WLOG_DEALS:
			wNom = NOM_STD_DOG_WLOGS;
			break;
		case KCK_DEALS:
			wNom = NOM_INV_BROKERAGE_CONTRACT_TYPE;
			break;
	}// switch

	if( wNom <= 0 )
		return false;

	DetInArray oArr( wNom, NOM );
	oArr.MkArray();

	for( int i = 0, nCount = oArr.GetCount(); i < nCount ; i++ )
	{
		CODE_NAME *pBuf = new CODE_NAME();

		pBuf->lCode = oArr.GetCode( i );
		ourstrncpy( pBuf->szName, (LPCTSTR)oArr.GetName( i ), sizeof( pBuf->szName ) );

		arrStdDogs.Add( pBuf );
	} // for

	return true;
}

bool CPreferencialTaxStgDogsPage::GetStdDogName( const long lStdDogCode, CString &strName )
{
	strName = "";

	if( !EnsureStdDogs() )
		return false;
	
	for( int i =0; i < m_arrStdDogs.GetCount();  i++ )
		if( m_arrStdDogs.GetAt( i )->lCode == lStdDogCode )
		{
			strName = m_arrStdDogs.GetAt( i )->szName;
			break;
		}//if

	return strName != "";
}

bool CPreferencialTaxStgDogsPage::LoadList()
{
	if( !m_pBuf )
		return false;

	m_pBuf->m_arrTaxesToStdDeals.QSort();

	m_listStdDogs.DeleteAllItems();

	PREFERENCIAL_TAXES_TO_STD_DEALS *pItem = NULL;
	CString  strBuf = "";
	int nSubItem =1;

	for( int i = 0; i < m_pBuf->m_arrTaxesToStdDeals.GetCount(); i ++ )
	{
		pItem = m_pBuf->m_arrTaxesToStdDeals.GetAt( i );

		if( !pItem )
			continue;

		strBuf.Format( "%6.6d", pItem->m_lStdDogCode );

		int nLCIndex = m_listStdDogs.InsertItem( m_listStdDogs.GetItemCount(), strBuf );

		strBuf = "";

		if( !GetStdDogName( pItem->m_lStdDogCode, strBuf ) )
			continue;

		nSubItem = 1;
		// Име
		m_listStdDogs.SetItemText( nLCIndex, nSubItem++, strBuf );

		m_listStdDogs.SetItemData( nLCIndex, (DWORD_PTR)pItem );

	}// for

	return true;
}


#pragma region Sheet
//////////////////////////////////////////////////////////////////////////////////////////////////
//	CPreferencialTaxSheet
IMPLEMENT_DYNAMIC( CPreferencialTaxSheet, PrShComm )

CPreferencialTaxSheet::CPreferencialTaxSheet( CPreferencialTaxFullRec &oBuf
					, const short sDlgMode/*ePrefTaxDlgMode*/
					, long lSelStdDogCode /*= NO_DETCODE*/
					, CWnd* pParent/* = NULL*/, UINT iSelectPage /* = 0*/ )
:PrShComm( CSMsg::GetMsgUsrLang( MSG_PREFERENCIAL_TAX_DATA ) , pParent, iSelectPage )
, m_oMainPage( oBuf, sDlgMode, this )
, m_oEventsTaxesPage( oBuf, sDlgMode, lSelStdDogCode, this )
, m_eMode( (ePrefTaxDlgMode)sDlgMode )
{
	m_pBuf = &oBuf;
	AddPage( &m_oMainPage );
	AddPage( &m_oEventsTaxesPage );

	m_arrEvents.SetCompFunction( CODE_NAME::compByCode );
	
	if( oBuf.m_recPrefTax.m_nDealType > 0 )
	{
		LoadEventsByDealType( oBuf.m_recPrefTax.m_nDealType, m_arrEvents );
		m_arrEvents.QSort();
	}
}

BEGIN_MESSAGE_MAP( CPreferencialTaxSheet, PrShComm )
	ON_MESSAGE( ID_WM_DEAL_TYPE_CHANGED, OnDealTypeChanged )
END_MESSAGE_MAP()

BOOL CPreferencialTaxSheet::OnInitDialog()
{
	PrShComm::OnInitDialog();

	if( !m_pBuf )
		return FALSE;

	CPropertyPage* parent = GetActivePage();
	if( !parent )
		return TRUE;

	BOOL bDisableDeactivate = (  m_eMode != eView
						|| ( m_pBuf->m_recPrefTax.m_sPreferenceStatus != (short)eStsActive  
							&& m_pBuf->m_recPrefTax.m_sPreferenceStatus != (short)eStsWaiting ) );

	m_DsblCtrl.VirtEnableCtrl( parent, IDC_BTN_DEACTIVATE , !bDisableDeactivate );

	return TRUE;
}

LRESULT CPreferencialTaxSheet::OnDealTypeChanged( WPARAM wParam, LPARAM lParam )
{
	m_arrEvents.Release();

	long lDealType = (long)wParam;

	LoadEventsByDealType( lDealType, m_arrEvents );
	m_arrEvents.QSort();

	if( IsWindow( m_oEventsTaxesPage.GetSafeHwnd() ) )
		m_oEventsTaxesPage.PostMessage( ID_WM_DEAL_TYPE_CHANGED, wParam, lParam );

	return TRUE;
}

bool CPreferencialTaxSheet::LoadEventsByDealType( long lDealType, CSCmnRecsArray < CODE_NAME > & arrEvents )
{
	long lRet = 0;
	CRpcBufArray arrIn, arrOut;
	arrIn << lDealType;

	try 
	{
		lRet = RPCBfrofCDispatchHandler( MyHandle(), RPCSYS( RPC_BFROF_GET_EVENTS_NAME_CODES_BY_DEALTYPE ), &arrIn, &arrOut );
	} // try
	catch(...) 
	{
		glShowErrMsg( APP_CLIENTTYPE, NULL, "CPreferencialTaxEventsTaxesPage::LoadEventsByDealType", NULL );
		lRet = 0;
	} // catch

	if( lRet <= 0 )
		return false;

	for( int i = 0, nCount = arrOut.GetCnt(); i < nCount ; i++ )
	{
		long lSize = 0;

		CODE_NAME *pBuf = (CODE_NAME*)arrOut.GetAt( i, lSize );

		ASSERT( lSize == sizeof( CODE_NAME ) );

		arrEvents.Add( (CODE_NAME*)memcpy( 
			new CODE_NAME, pBuf, sizeof( CODE_NAME ) ) );
	} // for

	return true;
}

void CPreferencialTaxSheet::AddIfNeed( CAPT_ID*& p_it, int& newcnt )
{
	int nOldSize = newcnt;
	newcnt += 3;

	CAPT_ID *lp = new CAPT_ID[newcnt];
	memcpy( lp, p_it, sizeof( CAPT_ID ) * nOldSize );
	delete [] p_it;
	p_it = lp;

	// Първия правим история на корекциите
	p_it[ IND_BUT_EDIT ].capt = (char*)gl_MSG( MSG_HIST_ACC );
	p_it[ IND_BUT_EDIT ].id = IDC_BTN_TAX_HIST;
	p_it[ IND_BUT_EDIT ].style =  WS_CHILD | WS_VISIBLE;

	// Вторият деактивиране
	p_it[ IND_BUT_WRITE ].capt = (char*)gl_MSG( MSG_CSSAFEBOX_4200 );
	p_it[ IND_BUT_WRITE ].id = IDC_BTN_DEACTIVATE;
	p_it[ IND_BUT_WRITE ].style = WS_CHILD | WS_VISIBLE ;

	// Скриваме третия
	p_it[ IND_BUT_REJECT ].style &= ~WS_VISIBLE;

	// Предпоследния да е запис
	p_it[ nOldSize ].pCB = &m_btnОк;
	p_it[ nOldSize ].capt = (char*)gl_MSG( MSG_SAFE );
	p_it[ nOldSize ].id = IDOK;
	p_it[ nOldSize ].style = WS_CHILD | WS_VISIBLE;

	if( m_eMode == eView || m_eMode == eViewNoDeactivate )
		p_it[ nOldSize ].style |= WS_DISABLED;

	nOldSize++;

	// Последния му задаваме да е Отказ
	p_it[ nOldSize ].pCB = &m_btnExit;
	p_it[ nOldSize ].capt = (char*)gl_MSG( MSG_CANCEL_AMP_1 );
	p_it[ nOldSize ].id = IDCANCEL;
	p_it[ nOldSize ].style = WS_CHILD | WS_VISIBLE;


	nOldSize++;

	char * pszSeriazlization = new char [MAX_PATH];
	ourstrncpy( pszSeriazlization, "XML serialization", MAX_PATH-1 );

	p_it[ nOldSize ].pCB = &m_btnXmlSerialize;
	p_it[ nOldSize ].capt = pszSeriazlization;
	p_it[ nOldSize ].id = IDC_BTN_PREF_XML;
	p_it[ nOldSize ].style = WS_CHILD | WS_VISIBLE;
	
}

bool CPreferencialTaxSheet::GetEventName( const long lEventCode, CString & strName )
{
	strName = "";

	for( int i=0; i< m_arrEvents.GetCount(); i++ )	
		if( m_arrEvents.GetAt( i )->lCode == lEventCode )
		{
			strName = m_arrEvents.GetAt( i )->szName;
			break;
		}// if

	return strName != "";
}

BOOL CPreferencialTaxSheet::OnCommand( WPARAM wParam, LPARAM lParam )
{
	WORD wNotifyCode = HIWORD( wParam );	// notification code 
	UINT nID = LOWORD( wParam );			// item, control, or accelerator identifier 

	if( wNotifyCode == BN_CLICKED )
	{
		switch( nID )
		{
		case IDC_BTN_TAX_HIST:
			ShowHistorySql( ePreferencialTaxes, GetString( "%ld", m_pBuf->m_recPrefTax.GetKeyValue() ) );
			break;
		case IDC_BTN_DEACTIVATE:
			DeactivatePreferencialTax();
			break;
		case IDC_BTN_PREF_XML:
			DoXmlSerialization();
		} // switch
	} // if

	return PrShComm::OnCommand( wParam, lParam );
}

void CPreferencialTaxSheet::DeactivatePreferencialTax()
{
	if( !m_pBuf )
		return;

	if( m_pBuf->m_recPrefTax.IsKlientskaPref() && !pGetUser()->GetAccess( ACCESS_Deactivate_Klientska_Preference, TRUE ) )
		return;

	m_pBuf->m_recPrefTax.m_sPreferenceStatus = (short)eStsDeactivated;
	m_pBuf->m_recPrefTax.m_fDeactivateDate = CUR_DATE;
	m_pBuf->m_recPrefTax.m_iniDeactivate = *pGetUserIni();

	EndDialog( IDOK );
}



#pragma endregion Sheet

///////////////////////////////////////////////////////////////////////////////////////////////////
// CPreferencialTaxSheetStdDogAssign
IMPLEMENT_DYNAMIC( CPreferencialTaxSheetStdDogAssign, CPreferencialTaxSheet )

CPreferencialTaxSheetStdDogAssign::CPreferencialTaxSheetStdDogAssign( CPreferencialTaxFullRec &oBuf
								  , const short sPrefTaxDlgMode/*ePrefTaxDlgMode*/
								  , CWnd* pParent/* = NULL*/, UINT iSelectPage /*= 0*/
								  , const BOOL bEditStdDogs /*= TRUE*/)
:CPreferencialTaxSheet( oBuf, sPrefTaxDlgMode, NO_DETCODE, pParent, iSelectPage )
, m_oStdDogsPage( oBuf, sPrefTaxDlgMode, this , bEditStdDogs )
{
	if( !oBuf.m_recPrefTax.IsIndividual() && !oBuf.m_recPrefTax.IsKlientskaPref() )
		AddPage( &m_oStdDogsPage );
}

LRESULT CPreferencialTaxSheetStdDogAssign::OnDealTypeChanged( WPARAM wParam, LPARAM lParam )
{
	CPreferencialTaxSheet::OnDealTypeChanged( wParam, lParam );
	
	if( IsWindow( m_oStdDogsPage.GetSafeHwnd() ) )
		m_oStdDogsPage.PostMessage( ID_WM_DEAL_TYPE_CHANGED, wParam, lParam );

	return TRUE;
}

CXmlEditDlg::CXmlEditDlg( CWnd* pParent /*=NULL*/ )
			: CBaseDialog( CXmlEditDlg::IDD, GetHISTANCEBfrofCDLL(), pParent )
{
}

void CXmlEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_XML_EDIT_EDB_XML, m_edbXml );
}


BOOL CXmlEditDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CString strBuf;
	strBuf = (LPCTSTR)m_bstr;
	m_edbXml.SetWindowText( strBuf );

	return TRUE;
}

void CXmlEditDlg::OnOK()
{
	CString strBuf;
	m_edbXml.GetWindowText( strBuf );
	m_bstr = strBuf;

	__super::OnOK();
}