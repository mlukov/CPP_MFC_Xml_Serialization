// PreferencialTaxes.h
//
#include "EventsAndActionsStr.h"
#pragma once
#include "XMLSerializable.h"

/// <summary> Зависимост от дата на откриване</summary>
#define STS_DEPENDS_ON_DATE_OPEN		11 // Зависимост от дата на откриване
/// <summary> Индивидуална преференция</summary>
#define STS_INDIVIDUAL_PREF				12 // Индивидуална преференция
/// <summary> Клиентска преференция </summary>
#define STS_KLIENTSKA_PREF				13 // Клиентска преференция


/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES

/// <summary>Зависи от - 'салдо/оборот', брой на събития или тип сделка + услуга</summary>
enum eDependType
{ 
	/// <summary>Салдо/оборот.</summary>
	eBalanceTurnover = 0,
	/// <summary>Брой на събития.</summary>
	eEventsCount = 1 ,	
	/// <summary>Без допълнителни условия за преференцията</summary>
	eNoDependancy = 2,
	/// <summary> Tип сделка + услуга (PREFERENCIAL_DEAL_TYPE) </summary>
	eDealService = 3
 };

/// <summary> Тип период - месеци или дни.</summary>
enum ePeriodType
{ 
	/// <summary>Тип период месеци.</summary>
	eMonthsPeriod = 0, 
	/// <summary>Тип период дни.</summary>
	eDaysPeriod = 1  
};

/// <summary>Периодичност(Цикличност) на зануляване на броячите -без, дневно, месечно, годишно</summary>
enum ePeriodicity
{ 
	/// <summary>Без периодичност.</summary>
	eNoPeriodicity = 1, 
	/// <summary>Дневно.</summary>
	eDayly = 2, 
	/// <summary>Месечно.</summary>
	eMonthly = 3, 
	/// <summary>Годишно.</summary>
	eAnnualy = 4 
};

/// <summary>Тип на преференцията - процент, или твърда сума.</summary>
enum ePreferenceType
{ 
	/// <summary>Тип на преференцията - процент.</summary>
	ePercent = 0,
	/// <summary>Тип на преференцията - твърда сума.</summary> 
	eSolidSum = 1 
};

/// <summary>Зависи от салдо, кредитен оборот или дебитен оборот.</summary>
enum eBalanceTurnoverType
{ 
	/// <summary>Зависи от салдо.</summary>
	eBalanceType = 1, 
	/// <summary>Зависи от кредитен оборот.</summary>
	eKreditTurnover = 2, 
	/// <summary>Зависи от дебитен оборот.</summary>
	eDebitTurnover = 3 
};

/// <summary>Статус на преференцията - активна, неактивна, деактивирана или чакаща.</summary> 
enum ePreferenceStatus
{ 
	/// <summary>Действаща преференция.</summary>
	eStsActive = 1,		
	/// <summary>Със изтекъл срок на валидност.</summary>
	eStsInactive = 2,	
	/// <summary>Прекратена преференция от потребител на системата.</summary>
	eStsDeactivated = 3,
	/// <summary>Параметризирана преференция с бъдеща начална дата</summary>
	eStsWaiting = 4	
};

/// <summary> Структура 'Преференциални такси' </summary>
struct BFROFEXP PREFERENCIAL_TAXES
{
	/// <summary>Стандартен хедър</summary>
	CSRECHDR			m_Hdr;		
	/// <summary>Полета за история на корекциите</summary>
	CSHISTINFO			m_histInfo;		
	/// <summary>Уникален код на преференциалната такса в регистъра</summary>
	long				m_lCode;			
	/// <summary>Име на преференцията</summary>
	PREFERERENCENAME	m_szPreferenceName;
	/// <summary>Статус на преференцията - активна, деактивирана или чакаща( стойностите са в изброител ePreferenceStatus )</summary>
	/// <seealso cref="ePreferenceStatus">
	short				m_sPreferenceStatus;
	/// <summary>Тип на сделка за която се отнася</summary>
	int					m_nDealType;	
	/// <summary>Валидна от</summary>
	FDATE				m_fDateValidFrom; 
	/// <summary>Валидна до</summary>
	FDATE				m_fDateValidTo;		
	/// <summary>Дата на откриване от</summary>
	FDATE				m_fDateOpenFrom;
	/// <summary>Дата на откриване до</summary>
	FDATE				m_fDateOpenTo;		
	/// <summary>Тип период - месеци или дни( стойностите са в изброител ePeriodType )</summary>
	/// <seealso cref="ePeriodType">
	short				m_sPeriodType;		
	/// <summary>Брой месеци</summary>
	int					m_nNumMonths;	
	/// <summary>Брой Дни</summary>
	int					m_nNumDays;				
	/// <summary>Цикличност - дневно, месечно, годишно, ( стойностите са в изброител ePeriodicity )</summary>
	/// <seealso cref="ePeriodicity">
	short				m_sPeriodicity;		
	/// <summary>Инициали на потребителя деактивиракл преференцията</summary>
	INIC				m_iniDeactivate;
	/// <summary>Дата на последна промяна.</summary>
	FDATE				m_fDeactivateDate;	
	/// <summary>Код на стандартна преференция ( ако е индивидуална )</summary>
	long				m_lStdPreferenceCode;
	/// <summary>Системи на преференцията - основна система/интернет система ( битовете са в изброител eProcessingSystems )</summary>
	/// <seealso cref="eProcessingSystems">
	STATUS				m_stsProcessingSystem;	
	/// <summary>Гратисна сума в лева</summary>
	SUMA				m_fGratisSumBgn;
	/// <summary>Брой месеци за валидност на гратисната сума</summary>
	int					m_nGratisMonths;

	/// <summary>Стандартен конструктор</summary>
	PREFERENCIAL_TAXES();
	/// <summary>Конструктор за копие</summary>
	/// <param name=recPrefTax>Структурата от която ще копираме стойностите</param>
	PREFERENCIAL_TAXES( const PREFERENCIAL_TAXES & recPrefTax );

	/// <summary>Стойността на ключовото поле на записа.</summary>
	/// <returns>Стойността на ключовото поле на записа.</returns>
	long GetKeyValue() const ;

	/// <summary>Задава стойност на ключовото поле на записа.</summary>
	/// <param name=lKeyValue>Новата стойност на ключовото поле на записа.</param>
	/// <returns>Новата стойност на ключовото поле на записа.</returns>
	long SetKeyValue( const long lKeyValue );

	/// <summary> Дали преференциалната такса е индивидуална</summary>
	/// <returns>Ако е индивидуална- връща истина, ако е стандартна - лъжа.</returns>
	BOOL IsIndividual(){ return m_Hdr.CheckBit( STS_INDIVIDUAL_PREF ); }

	/// <summary> Дали преференциалната такса е клиентска</summary>
	/// <returns>Ако е клиентска - връща истина, ако не е - лъжа.</returns>
	BOOL IsKlientskaPref(){ return m_Hdr.CheckBit( STS_KLIENTSKA_PREF ); }

	/// <summary> Връща стринг с типа на преференцията Стандартна / Индивидуална / Клиентска </summary>
	LPCTSTR GetType();

	/// <summary> Проверка за вдингат бит по поле обслужваща система</summary>
	/// <param name="nBit">Бита който проверяваме</param>
	/// <returns>TRUE - вдигнат, FALSE - не е.</returns>
	BOOL CheckProcessingSystemBit( int nBit ) const
	{
		ASSERT( nBit >= 0 && nBit < STATUSSIZE );

		return CSBitVector( STATUSSIZE, m_stsProcessingSystem )[ nBit ];
	}

	/// <summary> Вдига или сваля бит за обслужваща система</summary>
	/// <param name="nBit">Бита</param>
	/// <param name="flg">Флаг дали да е вдигнат или не</param>
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
/// <summary>Справочна структура за преференциални такси с</summary>
struct BFROFEXP PREFERENCIAL_TAXES_SPR : public PREFERENCIAL_TAXES
{
	/// <summary>Зависи от дата на откриване</summary>
	bool	m_bDependsOnDateOpen;			 
	/// <summary>Има преференциален план</summary>
	bool	m_bPreferencePlan;				 
	/// <summary>Име на типа сделка</summary>
	char	m_szDealTypeName[ ALLDETNAMES + 1 ]; 
	/// <summary>Зависи от салдо, кредитен оборот или дебитен оборот</summary>
	/// <seealso cref="eBalanceTurnoverType">
	short	m_sBalanceTurnoverType;				

	/// <summary>Стандартен конструктор</summary>
	PREFERENCIAL_TAXES_SPR();
	/// <summary>Конструктор за копие от PREFERENCIAL_TAXES</summary>
	/// <param name="recPrefTax">Структурата от която ще копираме</param>
	/// <seealso cref="PREFERENCIAL_TAXES">
	PREFERENCIAL_TAXES_SPR( const PREFERENCIAL_TAXES &recPrefTax );
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_DEAL_SPR
/// <summary>Справочна структура за преференциални такси със флаг за стандартна преференция по сделка</summary>
struct BFROFEXP PREFERENCIAL_TAXES_TO_DEAL_SPR : public PREFERENCIAL_TAXES
{
	/// <summary>Флаг за стандартна преференция по сделка</summary>
	bool	m_bStdPrefOnDeal;
			
	/// <summary>Стандартен конструктор</summary>
	PREFERENCIAL_TAXES_TO_DEAL_SPR();
	/// <summary>Конструктор за копие от PREFERENCIAL_TAXES</summary>
	/// <param name="recPrefTax">Структурата от която ще копираме</param>
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
	long		m_lDealType;		// Тип сделка
	long		m_lProductCode;		// Код стандартен договор
	long		m_lCode;			// Код на преференциална такса
	short		m_sStatus;			// Статус
	long		m_lPrefType;		// 1 - Стандартна 2 - Индивидуална 3 - Клиентска

	DWORD		m_bvSelected;
	BOOL		m_bReadOnly;

	PREFERENCIAL_TAXES_SPR_FILT(){ memset( this, 0x00, sizeof(*this) ); }
};

// Периодичност - дневно, месечно, годишно
static MSGID_CODE g_arrPreferenceTaxPeriodicity[] =
{
	{ MSG_NO_PERIODICITY,	eNoPeriodicity	},
	{ MSG_DAILY,			eDayly			},
	{ MSG_PER_MONTHLY,		eMonthly		},
	{ MSG_PER_YEARLY,		eAnnualy		}
};

// Зависи от салдо, кредитен оборот или дебитен оборот
static MSGID_CODE g_arrPreferenceTaxBalanceTurnover[] =
{
	{ MSG_SALDO_COL,		eBalanceType	},
	{ MSG_CREDIT_TURNOVER,	eKreditTurnover	},
	{ MSG_DEBIT_TURNOVER,	eDebitTurnover	}
};

// Статус на преференцията - активна, неактивна, деактивирана или чакаща
static MSGID_CODE g_arrPreferenceTaxStatus[] =
{
	{ MSG_PART_ACTIV,			eStsActive		},	// Действаща преференция;
	{ MSG_STS_INACTIVE,			eStsInactive	},	// Със изтекъл срок на валидност;
	{ MSG_STS_DEACTIVATED,		eStsDeactivated},	// Прекратена преференция от потребител на системата;
	{ MSG_ACCSYS_STILL_PENDING,	eStsWaiting	}		// Параметризирана преференция с бъдеща начална дата.
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_PLANS
struct BFROFEXP PREFERENCIAL_PLANS
{
	CSRECHDR			m_Hdr;					// Стандартен хедър
	CSHISTINFO			m_histInfo;				// Полета за история на корекциите
	long				m_lCode;				// Уникален код в регистъра
	long				m_lPreferenceCode;		// Код на преференцията
	// ePreferenceType
	short				m_sPreferenceType;		// Тип на преференцията - процент, или твърда сума
	SUMA				m_fPercent;				// Процент на преференцията
	SUMA				m_fSumSolid;			// Твърда сума на преференцията
	// eDependType
	short				m_sDependType;			// Зависи от - 'салдо/оборот' или брой на събития
	// eBalanceTurnoverType
	short				m_sBalanceTurnoverType;	// Зависи от салдо, кредитен оборот или дебитен оборот
	SUMA				m_fSumOver;				// Сума над
	int					m_nEventCountFrom;		// Брой събития от
	int					m_nEventCountTo;		// Брой събития до
	int					m_nPrefDealTypeCode;	// Код от таблица 'PREFERENCIAL_DEAL_TYPE'

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

/// <summary> Масив от структури 'Събития и Такси по Преференциални такси' </summary>
class BFROFEXP CPreferencePlanArray : public CSCmnRecsArrayComparable< PREFERENCIAL_PLANS >
{
public:
	int GetMaxEventCountTo();	// Максималния брой събития до в масива
	/* m_sBalanceTurnover - eBalanceTurnoverType */ 
	SUMA GetMaxSumOver( const short m_sBalanceTurnover );		// Максималната сума над в масива по тип // салдо или оборот
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_EVENTS_TAXES
/// <summary> Структура 'Събития и Такси по Преференциални такси' </summary>
struct BFROFEXP PREFERENCIAL_EVENTS_TAXES
{
	CSRECHDR			m_Hdr;					// Стандартен хедър
	CSHISTINFO			m_histInfo;				// Полета за история на корекциите
	long				m_lCode;				// Уникален код в регистъра
	long				m_lPreferenceCode;		// Код на преференцията
	long				m_lEventCode;			// Код на събитие
	long				m_lTaxCode;				// Код на такса

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
/// <summary> Структура 'Преференциални такси към стандартни договори' </summary>
struct BFROFEXP PREFERENCIAL_TAXES_TO_STD_DEALS
{
	CSRECHDR			m_Hdr;					// Стандартен хедър
	CSHISTINFO			m_histInfo;				// Полета за история на корекциите
	long				m_lCode;				// Уникален код в регистъра
	long				m_lDealType;			// Тип на сделка
	long				m_lStdDogCode;			// Код на стандартен договор
	long				m_lPreferencialTaxCode;	// Код на такса

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
	short				m_sPrefStatus;			// Статус на преференцията
	PREFERERENCENAME	m_szPreferenceName;		// Име на преференцията
	FDATE				m_fDateValidFrom;		// Валидна от
	FDATE				m_fDateValidTo;			// Валидна до

	PREFERENCIAL_TAXES_TO_STD_DEALS_EXT();
	PREFERENCIAL_TAXES_TO_STD_DEALS_EXT( const PREFERENCIAL_TAXES_TO_STD_DEALS & recPrefTaxToStd );
	PREFERENCIAL_TAXES_TO_STD_DEALS_EXT( const PREFERENCIAL_TAXES_TO_STD_DEALS_EXT & recPrefTaxToStdExt );
};

/////////////////////////////////////////////////////////////////////////////
// struct PREFERENCIAL_TAXES_TO_STD_DEALS
struct BFROFEXP PREFERENCIAL_TAXES_TO_DEALS
{
	CSRECHDR			m_Hdr;					// Стандартен хедър
	CSHISTINFO			m_histInfo;				// Полета за история на корекциите
	long				m_lCode;				// Уникален код в регистъра
	long				m_lDealType;			// Тип на сделка
	long				m_lDealNum;				// Номер на сделка
	long				m_lPreferencialTaxCode;	// Код на такса

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
	// ISerializable имплементация
	virtual BOOL Serialize( ISerializer &serializer ) const ;
	virtual BOOL Deserialize( ISerializer &serializer );

	// CXMLSerializable имплементация
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
	CSRECHDR			m_Hdr;					// Стандартен хедър
	CSHISTINFO			m_histInfo;				// Полета за история на корекциите
	long				m_lCode;				// Уникален код в регистъра
	long				m_lDealType;			// Тип на сделка
	long				m_lDealNum;				// Номер на сделка
	long				m_lEventCount;			// Брой събития
	FDATE				m_fDateLastEvent;		// Дата на последно настъпване на събитието
	long				m_lPrefCode;			// Код на преференцията

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

/// <summary>Структура за филтриране на справка 'Събития и такси към преференция'</summary>
struct BFROFEXP PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT
{
	/// <summary>Код на преференция</summary>
	long		m_lPreferenceCode;	
	/// <summary>Код на събитие</summary>
	long		m_lEventType;	
	/// <summary>Код на такса</summary>	
	long		m_lTaxCode;	
	/// <summary>Обслужваща система</summary>		
	short		m_sProcSystem;	
	/// <summary>Тип сделка</summary>		
	long		m_lDealType;
	/// <summary>Код Стандартен Договор</summary>	
	long		m_lStdCndCode;
	/// <summary> Тип преференция</summary>
	long		m_lPrefType;

	/// <summary>Битове за избраните полета във филтъра</summary>
	DWORD		m_bvSelected;

	PREFERENCIAL_TAXES_EVENTS_TAXES_SPR_FILT(){ memset( this, 0x00, sizeof(*this) ); }
};


/////////////////////////////////////////////////////////////////////////////
// struct TAXES_ON_DEAL_PREFERENCE_COUNTERS

/// <summary>Справочна структура за справка 'Събития и такси към преференция'</summary>
struct BFROFEXP PREFERENCIAL_TAXES_EVENT_TAXES_SPR
{
	/// <summary>Код на преференциална такса</summary>
	long				m_lCode;				
	/// <summary>Име на преференциална такса</summary>
	PREFERERENCENAME	m_szPreferenceName;	
	/// <summary>Код на събитие</summary>
	long				m_lEventCode;	
	/// <summary>Име на събитие</summary>
	char				m_szEventName[ EVENT_NAME_MAX_LEN + 1 ];			
	/// <summary>Код на такса</summary>
	long				m_lTaxCode;				
	/// <summary>Име на такса</summary>
	TAXNAME				m_szTaxName;			
	/// <summary>Обслужващи системи на преференция</summary>
	STATUS				m_sStsProcessingSystem;	
	/// <summary>Тип сделка <summary>
	long				m_lDealType;
	/// <summary>Стандартен договор </summary>
	long				m_lStdContr;
	/// <summary>Тип преференциална такса </summary>
	long				m_lType;

	/// <summary> Проверка за вдингат бит по поле обслужваща система</summary>
	/// <param name="nBit">Бита който проверяваме</param>
	/// <returns>TRUE - вдигнат, FALSE - не е.</returns>
	BOOL CheckProcessingSystemBit( int nBit ) const
	{
		ASSERT( nBit >= 0 && nBit < STATUSSIZE );

		return CSBitVector( STATUSSIZE, m_sStsProcessingSystem )[ nBit ];
	}

	PREFERENCIAL_TAXES_EVENT_TAXES_SPR(){ memset( this, 0, sizeof( *this ) ); }
};