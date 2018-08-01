// PreferencialTaxes.h
//
#include "EventsAndActionsStr.h"
#pragma once
#include "XMLSerializable.h"

/// <summary> ���������� �� ���� �� ���������</summary>
#define STS_DEPENDS_ON_DATE_OPEN		11 // ���������� �� ���� �� ���������
/// <summary> ������������ �����������</summary>
#define STS_INDIVIDUAL_PREF				12 // ������������ �����������
/// <summary> ��������� ����������� </summary>
#define STS_KLIENTSKA_PREF				13 // ��������� �����������


/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES

/// <summary>������ �� - '�����/������', ���� �� ������� ��� ��� ������ + ������</summary>
enum eDependType
{ 
	/// <summary>�����/������.</summary>
	eBalanceTurnover = 0,
	/// <summary>���� �� �������.</summary>
	eEventsCount = 1 ,	
	/// <summary>��� ������������ ������� �� �������������</summary>
	eNoDependancy = 2,
	/// <summary> T�� ������ + ������ (PREFERENCIAL_DEAL_TYPE) </summary>
	eDealService = 3
 };

/// <summary> ��� ������ - ������ ��� ���.</summary>
enum ePeriodType
{ 
	/// <summary>��� ������ ������.</summary>
	eMonthsPeriod = 0, 
	/// <summary>��� ������ ���.</summary>
	eDaysPeriod = 1  
};

/// <summary>������������(����������) �� ���������� �� �������� -���, ������, �������, �������</summary>
enum ePeriodicity
{ 
	/// <summary>��� ������������.</summary>
	eNoPeriodicity = 1, 
	/// <summary>������.</summary>
	eDayly = 2, 
	/// <summary>�������.</summary>
	eMonthly = 3, 
	/// <summary>�������.</summary>
	eAnnualy = 4 
};

/// <summary>��� �� ������������� - �������, ��� ������ ����.</summary>
enum ePreferenceType
{ 
	/// <summary>��� �� ������������� - �������.</summary>
	ePercent = 0,
	/// <summary>��� �� ������������� - ������ ����.</summary> 
	eSolidSum = 1 
};

/// <summary>������ �� �����, �������� ������ ��� ������� ������.</summary>
enum eBalanceTurnoverType
{ 
	/// <summary>������ �� �����.</summary>
	eBalanceType = 1, 
	/// <summary>������ �� �������� ������.</summary>
	eKreditTurnover = 2, 
	/// <summary>������ �� ������� ������.</summary>
	eDebitTurnover = 3 
};

/// <summary>������ �� ������������� - �������, ���������, ������������ ��� ������.</summary> 
enum ePreferenceStatus
{ 
	/// <summary>��������� �����������.</summary>
	eStsActive = 1,		
	/// <summary>��� ������� ���� �� ���������.</summary>
	eStsInactive = 2,	
	/// <summary>���������� ����������� �� ���������� �� ���������.</summary>
	eStsDeactivated = 3,
	/// <summary>��������������� ����������� � ������ ������� ����</summary>
	eStsWaiting = 4	
};

/// <summary> ��������� '�������������� �����' </summary>
struct BFROFEXP PREFERENCIAL_TAXES
{
	/// <summary>���������� �����</summary>
	CSRECHDR			m_Hdr;		
	/// <summary>������ �� ������� �� ����������</summary>
	CSHISTINFO			m_histInfo;		
	/// <summary>�������� ��� �� ���������������� ����� � ���������</summary>
	long				m_lCode;			
	/// <summary>��� �� �������������</summary>
	PREFERERENCENAME	m_szPreferenceName;
	/// <summary>������ �� ������������� - �������, ������������ ��� ������( ����������� �� � ��������� ePreferenceStatus )</summary>
	/// <seealso cref="ePreferenceStatus">
	short				m_sPreferenceStatus;
	/// <summary>��� �� ������ �� ����� �� ������</summary>
	int					m_nDealType;	
	/// <summary>������� ��</summary>
	FDATE				m_fDateValidFrom; 
	/// <summary>������� ��</summary>
	FDATE				m_fDateValidTo;		
	/// <summary>���� �� ��������� ��</summary>
	FDATE				m_fDateOpenFrom;
	/// <summary>���� �� ��������� ��</summary>
	FDATE				m_fDateOpenTo;		
	/// <summary>��� ������ - ������ ��� ���( ����������� �� � ��������� ePeriodType )</summary>
	/// <seealso cref="ePeriodType">
	short				m_sPeriodType;		
	/// <summary>���� ������</summary>
	int					m_nNumMonths;	
	/// <summary>���� ���</summary>
	int					m_nNumDays;				
	/// <summary>���������� - ������, �������, �������, ( ����������� �� � ��������� ePeriodicity )</summary>
	/// <seealso cref="ePeriodicity">
	short				m_sPeriodicity;		
	/// <summary>�������� �� ����������� ������������ �������������</summary>
	INIC				m_iniDeactivate;
	/// <summary>���� �� �������� �������.</summary>
	FDATE				m_fDeactivateDate;	
	/// <summary>��� �� ���������� ����������� ( ��� � ������������ )</summary>
	long				m_lStdPreferenceCode;
	/// <summary>������� �� ������������� - ������� �������/�������� ������� ( �������� �� � ��������� eProcessingSystems )</summary>
	/// <seealso cref="eProcessingSystems">
	STATUS				m_stsProcessingSystem;	
	/// <summary>�������� ���� � ����</summary>
	SUMA				m_fGratisSumBgn;
	/// <summary>���� ������ �� ��������� �� ���������� ����</summary>
	int					m_nGratisMonths;

	/// <summary>���������� �����������</summary>
	PREFERENCIAL_TAXES();
	/// <summary>����������� �� �����</summary>
	/// <param name=recPrefTax>����������� �� ����� �� �������� �����������</param>
	PREFERENCIAL_TAXES( const PREFERENCIAL_TAXES & recPrefTax );

	/// <summary>���������� �� ��������� ���� �� ������.</summary>
	/// <returns>���������� �� ��������� ���� �� ������.</returns>
	long GetKeyValue() const ;

	/// <summary>������ �������� �� ��������� ���� �� ������.</summary>
	/// <param name=lKeyValue>������ �������� �� ��������� ���� �� ������.</param>
	/// <returns>������ �������� �� ��������� ���� �� ������.</returns>
	long SetKeyValue( const long lKeyValue );

	/// <summary> ���� ���������������� ����� � ������������</summary>
	/// <returns>��� � ������������- ����� ������, ��� � ���������� - ����.</returns>
	BOOL IsIndividual(){ return m_Hdr.CheckBit( STS_INDIVIDUAL_PREF ); }

	/// <summary> ���� ���������������� ����� � ���������</summary>
	/// <returns>��� � ��������� - ����� ������, ��� �� � - ����.</returns>
	BOOL IsKlientskaPref(){ return m_Hdr.CheckBit( STS_KLIENTSKA_PREF ); }

	/// <summary> ����� ������ � ���� �� ������������� ���������� / ������������ / ��������� </summary>
	LPCTSTR GetType();

	/// <summary> �������� �� ������� ��� �� ���� ���������� �������</summary>
	/// <param name="nBit">���� ����� �����������</param>
	/// <returns>TRUE - �������, FALSE - �� �.</returns>
	BOOL CheckProcessingSystemBit( int nBit ) const
	{
		ASSERT( nBit >= 0 && nBit < STATUSSIZE );

		return CSBitVector( STATUSSIZE, m_stsProcessingSystem )[ nBit ];
	}

	/// <summary> ����� ��� ����� ��� �� ���������� �������</summary>
	/// <param name="nBit">����</param>
	/// <param name="flg">���� ���� �� � ������� ��� ��</param>
	void SetProcessingSystemBit( int nBit, BOOL flg )
	{
		ASSERT( nBit >= 0 && nBit < STATUSSIZE );
		CSBitVector bv( STATUSSIZE, m_stsProcessingSystem );
		bv[ (Pos)nBit ] = flg;
	}

	CString GetProcessingSystemsAsString() const;

};

//////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_TAXESXmlSerializable
class BFROFEXP PREFERENCIAL_TAXESXmlSerializable : public CXMLSerializable
{
public:
	LPCTSTR GetTypeName() const { return "PREFERENCIAL_TAXES"; };

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	void FillTypeInfo();
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_SPR
/// <summary>��������� ��������� �� �������������� ����� �</summary>
struct BFROFEXP PREFERENCIAL_TAXES_SPR : public PREFERENCIAL_TAXES
{
	/// <summary>������ �� ���� �� ���������</summary>
	bool	m_bDependsOnDateOpen;			 
	/// <summary>��� �������������� ����</summary>
	bool	m_bPreferencePlan;				 
	/// <summary>��� �� ���� ������</summary>
	char	m_szDealTypeName[ ALLDETNAMES + 1 ]; 
	/// <summary>������ �� �����, �������� ������ ��� ������� ������</summary>
	/// <seealso cref="eBalanceTurnoverType">
	short	m_sBalanceTurnoverType;				

	/// <summary>���������� �����������</summary>
	PREFERENCIAL_TAXES_SPR();
	/// <summary>����������� �� ����� �� PREFERENCIAL_TAXES</summary>
	/// <param name="recPrefTax">����������� �� ����� �� ��������</param>
	/// <seealso cref="PREFERENCIAL_TAXES">
	PREFERENCIAL_TAXES_SPR( const PREFERENCIAL_TAXES &recPrefTax );
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_DEAL_SPR
/// <summary>��������� ��������� �� �������������� ����� ��� ���� �� ���������� ����������� �� ������</summary>
struct BFROFEXP PREFERENCIAL_TAXES_TO_DEAL_SPR : public PREFERENCIAL_TAXES
{
	/// <summary>���� �� ���������� ����������� �� ������</summary>
	bool	m_bStdPrefOnDeal;
			
	/// <summary>���������� �����������</summary>
	PREFERENCIAL_TAXES_TO_DEAL_SPR();
	/// <summary>����������� �� ����� �� PREFERENCIAL_TAXES</summary>
	/// <param name="recPrefTax">����������� �� ����� �� ��������</param>
	/// <seealso cref="PREFERENCIAL_TAXES">
	PREFERENCIAL_TAXES_TO_DEAL_SPR( const PREFERENCIAL_TAXES &recPrefTax );
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_SPR_FILT
const int BV_PREFERENCIAL_TAXES_SPR_FILT_DEAL_TYPE		= 0;
const int BV_PREFERENCIAL_TAXES_SPR_FILT_PRODUCT		= 1;
const int BV_PREFERENCIAL_TAXES_SPR_FILT_CODE			= 2;
const int BV_PREFERENCIAL_TAXES_SPR_FILT_STATUS			= 3;
const int BV_PREFERENCIAL_TAXES_SPR_FILT_TYPE			= 4;

#define STANDART_PREFERENCE 1
#define INDIVIDUAL_PREFERENCE 2
#define KLIENTSKA_PREFERENCE 3

struct BFROFEXP PREFERENCIAL_TAXES_SPR_FILT
{
	long		m_lDealType;		// ��� ������
	long		m_lProductCode;		// ��� ���������� �������
	long		m_lCode;			// ��� �� �������������� �����
	short		m_sStatus;			// ������
	long		m_lPrefType;		// 1 - ���������� 2 - ������������ 3 - ���������

	DWORD		m_bvSelected;
	BOOL		m_bReadOnly;

	PREFERENCIAL_TAXES_SPR_FILT(){ memset( this, 0x00, sizeof(*this) ); }
};

// ������������ - ������, �������, �������
static MSGID_CODE g_arrPreferenceTaxPeriodicity[] =
{
	{ MSG_NO_PERIODICITY,	eNoPeriodicity	},
	{ MSG_DAILY,			eDayly			},
	{ MSG_PER_MONTHLY,		eMonthly		},
	{ MSG_PER_YEARLY,		eAnnualy		}
};

// ������ �� �����, �������� ������ ��� ������� ������
static MSGID_CODE g_arrPreferenceTaxBalanceTurnover[] =
{
	{ MSG_SALDO_COL,		eBalanceType	},
	{ MSG_CREDIT_TURNOVER,	eKreditTurnover	},
	{ MSG_DEBIT_TURNOVER,	eDebitTurnover	}
};

// ������ �� ������������� - �������, ���������, ������������ ��� ������
static MSGID_CODE g_arrPreferenceTaxStatus[] =
{
	{ MSG_PART_ACTIV,			eStsActive		},	// ��������� �����������;
	{ MSG_STS_INACTIVE,			eStsInactive	},	// ��� ������� ���� �� ���������;
	{ MSG_STS_DEACTIVATED,		eStsDeactivated},	// ���������� ����������� �� ���������� �� ���������;
	{ MSG_ACCSYS_STILL_PENDING,	eStsWaiting	}		// ��������������� ����������� � ������ ������� ����.
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_PLANS
struct BFROFEXP PREFERENCIAL_PLANS
{
	CSRECHDR			m_Hdr;					// ���������� �����
	CSHISTINFO			m_histInfo;				// ������ �� ������� �� ����������
	long				m_lCode;				// �������� ��� � ���������
	long				m_lPreferenceCode;		// ��� �� �������������
	// ePreferenceType
	short				m_sPreferenceType;		// ��� �� ������������� - �������, ��� ������ ����
	SUMA				m_fPercent;				// ������� �� �������������
	SUMA				m_fSumSolid;			// ������ ���� �� �������������
	// eDependType
	short				m_sDependType;			// ������ �� - '�����/������' ��� ���� �� �������
	// eBalanceTurnoverType
	short				m_sBalanceTurnoverType;	// ������ �� �����, �������� ������ ��� ������� ������
	SUMA				m_fSumOver;				// ���� ���
	int					m_nEventCountFrom;		// ���� ������� ��
	int					m_nEventCountTo;		// ���� ������� ��
	int					m_nPrefDealTypeCode;	// ��� �� ������� 'PREFERENCIAL_DEAL_TYPE'

	PREFERENCIAL_PLANS();
	PREFERENCIAL_PLANS( const PREFERENCIAL_PLANS & recPlan );

	long GetKeyValue() const ;
	long SetKeyValue( const long lKeyValue );
	int CompareStructs( const PREFERENCIAL_PLANS &oRec );

	static int compareByEventCountSumOver( const void* p1, const void* p2 );
};

//////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_PLANSXmlSerializable
class BFROFEXP PREFERENCIAL_PLANSXmlSerializable : public CXMLSerializable
{
public:
	LPCTSTR GetTypeName() const { return "PREFERENCIAL_PLANS"; };

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	void FillTypeInfo();
};

/// <summary> ����� �� ��������� '������� � ����� �� �������������� �����' </summary>
class BFROFEXP CPreferencePlanArray : public CSCmnRecsArrayComparable< PREFERENCIAL_PLANS >
{
public:
	int GetMaxEventCountTo();	// ����������� ���� ������� �� � ������
	/* m_sBalanceTurnover - eBalanceTurnoverType */ 
	SUMA GetMaxSumOver( const short m_sBalanceTurnover );		// ������������ ���� ��� � ������ �� ��� // ����� ��� ������
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_EVENTS_TAXES
/// <summary> ��������� '������� � ����� �� �������������� �����' </summary>
struct BFROFEXP PREFERENCIAL_EVENTS_TAXES
{
	CSRECHDR			m_Hdr;					// ���������� �����
	CSHISTINFO			m_histInfo;				// ������ �� ������� �� ����������
	long				m_lCode;				// �������� ��� � ���������
	long				m_lPreferenceCode;		// ��� �� �������������
	long				m_lEventCode;			// ��� �� �������
	long				m_lTaxCode;				// ��� �� �����

	PREFERENCIAL_EVENTS_TAXES();
	PREFERENCIAL_EVENTS_TAXES( const PREFERENCIAL_EVENTS_TAXES & recPlan );

	long GetKeyValue() const ;
	long SetKeyValue( const long lKeyValue );
	int CompareStructs( const PREFERENCIAL_EVENTS_TAXES &oRec ){ return GetKeyValue() - oRec.GetKeyValue(); }
};

///////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_EVENTS_TAXESXmlSerializable
class BFROFEXP PREFERENCIAL_EVENTS_TAXESXmlSerializable : public CXMLSerializable
{
public:
	LPCTSTR GetTypeName() const { return "PREFERENCIAL_EVENTS_TAXES"; };

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	void FillTypeInfo();
};

/////////////////////////////////////////////////////////////////////////////
// class CPreferenceEventsTaxesArray
class BFROFEXP CPreferenceEventsTaxesArray : public CSCmnRecsArrayComparable< PREFERENCIAL_EVENTS_TAXES >
{
public:
	bool EventHasTaxes( const long lEventType );

	bool HasDoubledTax( const long lTaxCode );
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_STD_DEALS
/// <summary> ��������� '�������������� ����� ��� ���������� ��������' </summary>
struct BFROFEXP PREFERENCIAL_TAXES_TO_STD_DEALS
{
	CSRECHDR			m_Hdr;					// ���������� �����
	CSHISTINFO			m_histInfo;				// ������ �� ������� �� ����������
	long				m_lCode;				// �������� ��� � ���������
	long				m_lDealType;			// ��� �� ������
	long				m_lStdDogCode;			// ��� �� ���������� �������
	long				m_lPreferencialTaxCode;	// ��� �� �����

	PREFERENCIAL_TAXES_TO_STD_DEALS();
	PREFERENCIAL_TAXES_TO_STD_DEALS( const PREFERENCIAL_TAXES_TO_STD_DEALS & recPlan );

	long GetKeyValue() const ;
	long SetKeyValue( const long lKeyValue );
	int CompareStructs( const PREFERENCIAL_TAXES_TO_STD_DEALS &oRec ){ return GetKeyValue() - oRec.GetKeyValue(); }

	static int compByDealCode( const void* p1, const void* p2 )
	{
		PREFERENCIAL_TAXES_TO_STD_DEALS* pPrefStd1 = *(PREFERENCIAL_TAXES_TO_STD_DEALS**)p1;
		PREFERENCIAL_TAXES_TO_STD_DEALS* pPrefStd2 = *(PREFERENCIAL_TAXES_TO_STD_DEALS**)p2;

		if( pPrefStd1->m_lStdDogCode > pPrefStd2->m_lStdDogCode )
			return 1;

		if( pPrefStd1->m_lStdDogCode < pPrefStd2->m_lStdDogCode )
			return -1;

		return 0;
	}
};

///////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_TAXES_TO_STD_DEALSXmlSerializable
class BFROFEXP PREFERENCIAL_TAXES_TO_STD_DEALSXmlSerializable : public CXMLSerializable
{
public:
	LPCTSTR GetTypeName() const { return "PREFERENCIAL_TAXES_TO_STD_DEALS"; };

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	void FillTypeInfo();
};

class BFROFEXP CPreferenceTaxesToStdDealsArray : public CSCmnRecsArrayComparable< PREFERENCIAL_TAXES_TO_STD_DEALS >
{
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_STD_DEALS_EXT
struct BFROFEXP PREFERENCIAL_TAXES_TO_STD_DEALS_EXT: public PREFERENCIAL_TAXES_TO_STD_DEALS
{
	short				m_sPrefStatus;			// ������ �� �������������
	PREFERERENCENAME	m_szPreferenceName;		// ��� �� �������������
	FDATE				m_fDateValidFrom;		// ������� ��
	FDATE				m_fDateValidTo;			// ������� ��

	PREFERENCIAL_TAXES_TO_STD_DEALS_EXT();
	PREFERENCIAL_TAXES_TO_STD_DEALS_EXT( const PREFERENCIAL_TAXES_TO_STD_DEALS & recPrefTaxToStd );
	PREFERENCIAL_TAXES_TO_STD_DEALS_EXT( const PREFERENCIAL_TAXES_TO_STD_DEALS_EXT & recPrefTaxToStdExt );
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_STD_DEALS
struct BFROFEXP PREFERENCIAL_TAXES_TO_DEALS
{
	CSRECHDR			m_Hdr;					// ���������� �����
	CSHISTINFO			m_histInfo;				// ������ �� ������� �� ����������
	long				m_lCode;				// �������� ��� � ���������
	long				m_lDealType;			// ��� �� ������
	long				m_lDealNum;				// ����� �� ������
	long				m_lPreferencialTaxCode;	// ��� �� �����

	PREFERENCIAL_TAXES_TO_DEALS();
	PREFERENCIAL_TAXES_TO_DEALS( const PREFERENCIAL_TAXES_TO_DEALS & recPlan );

	long GetKeyValue() const ;
	long SetKeyValue( const long lKeyValue );
	int CompareStructs( const PREFERENCIAL_TAXES_TO_DEALS &oRec ){ return GetKeyValue() - oRec.GetKeyValue(); }

	CString GetRegValue() const 
	{ 
		return GetString( "%ld", GetKeyValue() ); 
	}

};

///////////////////////////////////////////////////////////////////////////
// PREFERENCIAL_TAXES_TO_DEALSXmlSerializable
class BFROFEXP PREFERENCIAL_TAXES_TO_DEALSXmlSerializable : public CXMLSerializable
{
public:
	LPCTSTR GetTypeName() const { return "PREFERENCIAL_TAXES_TO_DEALS"; };

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	void FillTypeInfo();
};

/////////////////////////////////////////////////////////////////////////////
// class CPreferencialTaxFullRec
class BFROFEXP CPreferencialTaxFullRec : public ISerializable, public CXMLSerializable
{
public:
	CPreferencialTaxFullRec();
	CPreferencialTaxFullRec( const CPreferencialTaxFullRec & oPrefTaxFullRec );

public:
	PREFERENCIAL_TAXES				m_recPrefTax;
	PREFERENCIAL_TAXES_TO_DEALS		m_recPrefToDeal;
	
	CPreferencePlanArray			m_arrPlan;
	CPreferenceEventsTaxesArray		m_arrEventsTaxes;
	CPreferenceTaxesToStdDealsArray	m_arrTaxesToStdDeals;
	
public:
	void Copy( const CPreferencialTaxFullRec & oPrefTaxFullRec );
	// ISerializable �������������
	virtual BOOL Serialize( ISerializer &serializer ) const ;
	virtual BOOL Deserialize( ISerializer &serializer );

	// CXMLSerializable �������������
	LPCTSTR GetTypeName() const;

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	void	FillTypeInfo();


	BOOL IsIndividual(){ return m_recPrefTax.IsIndividual(); }

	BOOL IsKlientskaPref() { return m_recPrefTax.IsKlientskaPref(); }


	CString GetRegValue() const 
	{ 
		return GetString( "%ld - %s", m_recPrefTax.GetKeyValue(), m_recPrefTax.m_szPreferenceName ); 
	}

	bool operator == ( const CPreferencialTaxFullRec & src ) const;
 	bool operator != ( const CPreferencialTaxFullRec & src ) const
 	{
 		return !( *this == src ); 
	}
};


/////////////////////////////////////////////////////////////////////////////
// struct TAXES_ON_DEAL_PREFERENCE_COUNTERS
/// <summary></summary>
struct BFROFEXP TAXES_ON_DEAL_PREFERENCE_COUNTERS
{
	CSRECHDR			m_Hdr;					// ���������� �����
	CSHISTINFO			m_histInfo;				// ������ �� ������� �� ����������
	long				m_lCode;				// �������� ��� � ���������
	long				m_lDealType;			// ��� �� ������
	long				m_lDealNum;				// ����� �� ������
	long				m_lEventCount;			// ���� �������
	FDATE				m_fDateLastEvent;		// ���� �� �������� ���������� �� ���������
	long				m_lPrefCode;			// ��� �� �������������

	TAXES_ON_DEAL_PREFERENCE_COUNTERS();
	TAXES_ON_DEAL_PREFERENCE_COUNTERS( const TAXES_ON_DEAL_PREFERENCE_COUNTERS & recEvtOnDealCount );

	long GetKeyValue() const ;
	long SetKeyValue( const long lKeyValue );
	int CompareStructs( const TAXES_ON_DEAL_PREFERENCE_COUNTERS &oRec ){ return GetKeyValue() - oRec.GetKeyValue(); }
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT
const int BV_PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT_PREF_CODE		= 0;
const int BV_PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT_EVENT_TYPE	= 1;
const int BV_PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT_TAX_CODE		= 2;
const int BV_PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT_PROC_SYSTEM	= 3;
const int BV_PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT_DEAL_TYPE		= 4;
const int BV_PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT_STD_CODE		= 5;
const int BV_PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT_TYPE			= 6;

/// <summary>��������� �� ���������� �� ������� '������� � ����� ��� �����������'</summary>
struct BFROFEXP PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT
{
	/// <summary>��� �� �����������</summary>
	long		m_lPreferenceCode;	
	/// <summary>��� �� �������</summary>
	long		m_lEventType;	
	/// <summary>��� �� �����</summary>	
	long		m_lTaxCode;	
	/// <summary>���������� �������</summary>		
	short		m_sProcSystem;	
	/// <summary>��� ������</summary>		
	long		m_lDealType;
	/// <summary>��� ���������� �������</summary>	
	long		m_lStdCndCode;
	/// <summary> ��� �����������</summary>
	long		m_lPrefType;

	/// <summary>������ �� ��������� ������ ��� �������</summary>
	DWORD		m_bvSelected;

	PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT(){ memset( this, 0x00, sizeof(*this) ); }
};


/////////////////////////////////////////////////////////////////////////////
// struct TAXES_ON_DEAL_PREFERENCE_COUNTERS

/// <summary>��������� ��������� �� ������� '������� � ����� ��� �����������'</summary>
struct BFROFEXP PREFERENCIAL_TAXES_EVENT_TAXES_SPR
{
	/// <summary>��� �� �������������� �����</summary>
	long				m_lCode;				
	/// <summary>��� �� �������������� �����</summary>
	PREFERERENCENAME	m_szPreferenceName;	
	/// <summary>��� �� �������</summary>
	long				m_lEventCode;	
	/// <summary>��� �� �������</summary>
	char				m_szEventName[ EVENT_NAME_MAX_LEN + 1 ];			
	/// <summary>��� �� �����</summary>
	long				m_lTaxCode;				
	/// <summary>��� �� �����</summary>
	TAXNAME				m_szTaxName;			
	/// <summary>���������� ������� �� �����������</summary>
	STATUS				m_sStsProcessingSystem;	
	/// <summary>��� ������ <summary>
	long				m_lDealType;
	/// <summary>���������� ������� </summary>
	long				m_lStdContr;
	/// <summary>��� �������������� ����� </summary>
	long				m_lType;

	/// <summary> �������� �� ������� ��� �� ���� ���������� �������</summary>
	/// <param name="nBit">���� ����� �����������</param>
	/// <returns>TRUE - �������, FALSE - �� �.</returns>
	BOOL CheckProcessingSystemBit( int nBit ) const
	{
		ASSERT( nBit >= 0 && nBit < STATUSSIZE );

		return CSBitVector( STATUSSIZE, m_sStsProcessingSystem )[ nBit ];
	}

	PREFERENCIAL_TAXES_EVENT_TAXES_SPR(){ memset( this, 0, sizeof( *this ) ); }
};