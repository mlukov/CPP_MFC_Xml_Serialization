// XmlTypeInfo.h
#pragma once

/// <summary>Типове данни за описване на структури</summary>
enum XMLDataType
{
	XMLDataTypeNONE		= 0,
	XMLDataTypeInt		= 1,
	XMLDataTypeLong		= 2,
	XMLDataTypeShort	= 3,
	XMLDataTypeWORD		= 4,
	XMLDataTypeDWORD	= 5,
	XMLDataTypeDouble	= 6,
	XMLDataTypeFloat	= 7,
	XMLDataTypeChar		= 8,
	XMLDataTypeSTATUS	= 9,
	XMLDataTypeCPtrArray= 10,
	XMLDataTypeComplex	= 11
};

class CXmlTypeInfo;

/// <summary>Масив със информация за типове данни</summary>
class DLLINTRFC CXMLTypeInfoArray: public CSmartTypedPtrArray< CPtrArray, CXmlTypeInfo > 
{

public:
	/// <summary>Добавяне на прост тип данни в описанието на типа</summary>
	/// <param name="eDataType">Тип данни на полето <seealso cref="XMLDataType"/> </param>
	/// <param name="szFieldName">Наименование на полето(такова каквото ще се използва за сериализация)</param>
	/// <param name="nOffset">Отместване на полето в структурата</param>
	/// <param name="nSize">Размер на полето(само за низови полета)</param>
	/// <returns>Функцията връща индекса на добавения в масива запис при успех и -1 при грешка</returns>
	INT_PTR AddSimpleInfo(	const XMLDataType	eDataType,  
							const LPCTSTR		szFieldName,
							const int			nOffset, 
							const int			nSize = 0, 
							const LPCTSTR		szSubTypeName = NULL );

	/// <summary>Добавяне на сложен тип данни в описанието на типа</summary>
	/// <param name="oTypeInfo">Информация за подтипа който добавяме <seealso cref="CXmlTypeInfo"/> </param>
	/// <param name="szFieldName">Наименование на полето(такова каквото ще се използва за сериализация)</param>
	/// <param name="nOffset">Отместване на полето в структурата</param>
	/// <returns>Функцията връща индекса на добавения в масива запис при успех и -1 при грешка</returns>
	INT_PTR AddComplexInfo( const CXmlTypeInfo &oTypeInfo,
							const LPCTSTR		szFieldName,
							const int			nOffset );

	/// <summary>Добавяне на тип масив със сложен тип данни в описанието на типа</summary>
	/// <param name="szFieldName">Наименование на полето(такова каквото ще се използва за сериализация)</param>
	/// <param name="nOffset">Отместване на масива в структурата</param>
	/// <param name="szSubTypeFieldName">Наименование на под-полето(такова каквото ще се използва за сериализация)</param>
	/// <param name="eSubTypeDataType">Тип на подтипа <seealso cref="XMLDataType"/> </param>
	/// <param name="oSubTypeInfo">Информация за подтипа който добавяме<seealso cref="CXmlTypeInfo"/> </param>
	/// <param name="nSubTypeSize">Големина на подтипа</param>
	/// <returns>Функцията връща индекса на добавения в масива запис при успех и -1 при грешка</returns>
	INT_PTR AddCPtrArrayInfo(	const LPCTSTR		szFieldName,
								const int			nOffset,
								const LPCTSTR		szSubTypeFieldName,
								const XMLDataType	eSubTypeDataType,  
								const CXmlTypeInfo &oSubTypeInfo,
								const int			nSubTypeSize );

private:
	/// <summary>Проверка за наличие на същото отместване в масива</summary>
	/// <returns>Функцията връща true при  и false при грешка</returns>
	bool CanAddOffset( const int nOffset, const LPCTSTR szFieldName );
};


/// <summary>Клас за описване на структури</summary>
class DLLINTRFC CXmlTypeInfo
{
public:

	CXmlTypeInfo()
	{
		m_eDataType			= XMLDataTypeNONE;
		m_strFieldName		= "";
		m_strSubTypeName	= "";
		m_nSize				= 0;
		m_nOffset			= 0;
	}
	/// <summary>Конструктор със всички аргументи</summary>
	/// <param name="DataType">Тип данни на полето <seealso cref="XMLDataType"/> </param>
	/// <param name="szFieldName">Наименование на полето(такова каквото ще се използва за сериализация)</param>
	/// <param name="nOffset">Отместване на полето в структурата</param>
	/// <param name="nSize">Размер на полето(само за низови полета)</param>
	/// <param name="szSubTypeName">Име на подтипа на полето(само за масиви)</param>
	CXmlTypeInfo(	const XMLDataType	eDataType,  
					const LPCTSTR	szFieldName,
					const int		nOffset, 
					const int		nSize = 0, 
					const LPCTSTR	szSubTypeName = NULL);

	/// <summary>Конструктор за копие</summary>
	CXmlTypeInfo( const CXmlTypeInfo & oXmlInfo );

	CXmlTypeInfo & operator = ( const CXmlTypeInfo & oXmlInfo );

	const CXmlTypeInfo * GetTypeInfo( const LPCTSTR	szFieldName ) const;

	bool	IsSimpleType()	const;
	bool	IsCPtrArray()	const;
	bool	IsComplexType() const;

	int GetTypeSize() const;

public:
	/// <summary>Тип данни на полето <seealso cref="XMLDataType"/> </summary>
	XMLDataType		m_eDataType;
	/// <summary>Наименование на полето(такова каквото ще се използва за сериализация)</summary>
	CString			m_strFieldName;
	/// <summary>Име на подтипа на полето(само за масиви)</summary>
	CString			m_strSubTypeName;
	/// <summary>Размер на полето( за низови полета и елементи на масив )</summary>
	int				m_nSize;
	/// <summary>Отместване на полето в структурата</summary>
	int				m_nOffset;

	/// <summary>Масив с полета на под-типа</summary>
	CXMLTypeInfoArray	m_oArrInfos;
};
