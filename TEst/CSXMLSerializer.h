// XMLSerializer.h

#pragma once

#import <msxml6.dll> named_guids

class CXMLSerializable;
class CStructSerializer;

/// <summary>Kлас за XML сериализация/десериализация</summary>
class DLLINTRFC CSXMLSerializer
{
	friend class CXMLSerializable;
	friend class CStructSerializer;
public:
	/// <summary>Стандартен конструктор</summary>
	CSXMLSerializer();

	/// <summary>Сериализиране на едно поле</summary>
	/// <param name="szFieldName">Име на член променливата</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool SerializeField( const LPCSTR szFieldName, const LPCTSTR szValue );

	/// <summary>Сериализация към XML на C++ структура описана в gl_arrCSStructFields </summary>
	/// <param name="oXmlSerializer">Обект който извършва XML сериализацията <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="szStructName"> Име на C++ структура описана в gl_arrCSStructFields </param>
	/// <param name="szFieldName">Име на член променлива ако де-сериализираме член променлива на обект</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool SerializeStruct( IN BYTE		*	pStruct
		, IN long			lStructSize
		, IN LPCTSTR		szStructName
		, IN const LPCSTR	szFieldName = NULL );

	/// <summary>Де-сериализиране на едно поле</summary>
	/// <param name="szFieldName">Име на член променливата</param>
	/// <param name="szValue">Стойност (изходящ параметър)</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT LPCTSTR szValue, const int lSize );

	/// <summary>Де-сериализиране на едно поле</summary>
	/// <param name="szFieldName">Име на член променливата</param>
	/// <param name="lValue">Стойност (изходящ параметър)</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT long lValue );

	/// <summary>Де-сериализиране на едно поле</summary>
	/// <param name="szFieldName">Име на член променливата</param>
	/// <param name="dValue">Стойност (изходящ параметър)</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT double dValue );

	/// <summary>Де-сериализиране на едно поле</summary>
	/// <param name="szFieldName">Име на член променливата</param>
	/// <param name="fValue">Стойност (изходящ параметър)</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT float fValue );

	/// <summary>Десериализация на XML към C++ структура описана в gl_arrCSStructFields</summary>
	/// <param name="oXmlSerializer">Обект който извършва XML десериализацията <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="szStructName"> Име на C++ структура описана в gl_arrCSStructFields </param>
	/// <param name="szFieldName">Име на член променлива ако де-сериализираме член променлива на обект</param>
	/// <param name="bstrXml">Целият XML ако директно де-сериализираме структура.</param>
	/// <param name="bDontSearchChild">Да не търси подклон(вече е извлечен)</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool DeserializeStruct(   IN BYTE		*	pStruct
							, IN long			lStructSize
							, IN LPCTSTR		szStructName
							, IN const LPCTSTR	szFieldName = NULL
							, IN const _bstr_t* pBstrXml	= NULL
							, IN const bool		bDontSearchChild = false );


	/// <summary>Добавяне на нов клон в дървото на документа</summary>
	/// <param name="szFieldName">Име на член променливата</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool AddChildNode( const LPCTSTR szFieldName );

	/// <summary>Преминаване към под-клон на текущия в дървото на документа</summary>
	/// <param name="szFieldName">Име на член променливата</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool GetChildNode( const LPCTSTR szFieldName );

	/// <summary>Извличане на под-клоните на текущия в дървото на документа<</summary>
	/// <returns>Функцията връща броя на подклоните при успех и нула при грешка</returns>
	long GetChildNodes();

	/// <summary>Преминаване към под-клон на текущия в дървото на документа</summary>
	/// <param name="lIndex">Индекс на подклона</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool GetChildNodeAt( const long lIndex );

	/// <summary>Връщане към по-горен клон в дървото на документа<</summary>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool MoveOneNodeUp();
	
	/// <summary>Взимане на XML документа след сериализация</summary>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool GetXML( OUT _bstr_t &bstrXml );

private:
	/// <summary>Инициализация на данните преди сериализация/десериализация</summary>
	/// <param name="bstrXml">XML за десериализиране или сериализиране</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool Initialize( IN const _bstr_t &bstrXml );

	/// <summary>Инициализиран ли е обекта</summary>
	/// <returns>Функцията връща true ако обекта е инициализиран и ако не е</returns>
	bool IsInitialized() const;

	/// <summary>Задаване на XSI атрибут</summary>
	/// <param name="variantValue">Стойност на антрибута</param>
	void SetRootNodeXSIAttribute( LPCTSTR szValue );

	/// <summary>Задаване на XSI атрибут</summary>
	/// <param name="variantValue">Стойност на антрибута</param>
	void SetRootNodeXSDAttribute( LPCTSTR szValue );

	/// <summary>Добавяне на главния клон на документа ( <?xml version='1.0' encoding='UTF-16'> )</summary>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool AddProcessingNode();

	/// <summary>Извличане на текста на текущият клон на дървото</summary>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool GetCurNodeText( OUT BSTR &bstr );

	/// <summary>Задаване на текста на текущият клон на дървото</summary>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool SetCurNodeText( const LPCTSTR szNodeText );

	
protected:
	/// <summary>Документ</summary> 
	MSXML2::IXMLDOMDocument2Ptr		m_pXMLDoc; 
	/// <summary>Корена на дървото на XML документа</summary> 
	MSXML2::IXMLDOMElementPtr		m_pRootNode;

	/// <summary>Текущ елемент от дървото на XML документа</summary> 
	MSXML2::IXMLDOMElementPtr		m_pCurrentNode;

	MSXML2::IXMLDOMNodeListPtr		m_pChildsList;
	
	/// <summary>Наименование на корена на дървото което ще обработваме</summary> 
	CString							m_strRootTagName;

	/// <summary>Инициализиран ли е обекта</summary>
	bool m_bInitialized;

	/// <summary>Зададен ли е XSI атрибута</summary>
	bool m_bRootNodeXSIAttributeSet;

	/// <summary>Зададен ли е XSD атрибута</summary>
	bool m_bRootNodeXSDAttributeSet;

	/// <summary>Добавен ли е главния клон на документа ( <?xml version='1.0' encoding='UTF-16'> )</summary>
	bool m_bProcessingNodeAdded;
};
