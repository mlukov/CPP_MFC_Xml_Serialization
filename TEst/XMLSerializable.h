// XMLSerializable.h

#pragma once
#include "CSXMLSerializer.h"
#include "XmlTypeInfo.h"


/// <summary>Абстрактен клас за XML сериализация/десериализация</summary>
class DLLINTRFC CXMLSerializable
{
public:
	/// <summary>Стандартен конструктор</summary>
	CXMLSerializable();

	/// <summary>Сериализиране</summary>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	virtual bool SerializeXML( CSXMLSerializer &oXmlSerializer, const LPCTSTR szFieldName = NULL );

	/// <summary>Де-сериализиране</summary>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	virtual bool DeserializeXML( CSXMLSerializer &oXmlSerializer, IN const _bstr_t *pbstrXml = NULL, const LPCTSTR szFieldName = NULL );

	/// <summary>Йерархична информация за структурата на типа</summary>
	const CXmlTypeInfo & GetTypeInfo();

protected:
	/// <summary>Наименованието на типа</summary>
	virtual LPCTSTR GetTypeName() const = 0;

	/// <summary>Големина на типа</summary>
	virtual const long GetSize() const = 0;

	virtual void * GetPoiner() const = 0;

	/// <summary>Запълване на описанието на типа</summary>
	virtual void FillTypeInfo() = 0;

	/// <summary>Задаване на типа данни</summary>
	/// <param name="eDataType">Тип данни на полето <seealso cref="XMLDataType"/> </param>
	void SetDataType( const XMLDataType	eDataType );


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
	/// <param name="nSubTypeSize">Големина на подтипа(само за низови полета)</param>
	/// <returns>Функцията връща индекса на добавения в масива запис при успех и -1 при грешка</returns>
	INT_PTR AddCPtrArrayInfo(	const LPCTSTR		szFieldName,
		const int			nOffset,
		const LPCTSTR		szSubTypeFieldName,
		const XMLDataType	eSubTypeDataType,  
		const CXmlTypeInfo &oSubTypeInfo,
		const int			nSubTypeSize );

	/// <returns>Функцията трябва да връща true при успех и false при грешка</returns>
	virtual bool SerializeFields( CSXMLSerializer &oXmlSerializer );

	/// <returns>Функцията трябва да връща true при успех и false при грешка</returns>
	virtual bool DeserializeFields( CSXMLSerializer &oXmlSerializer );

	/// <summary>Име на типа(за сериализация)</summary>
	CString			m_strTypeName;

	/// <summary>Йерархична информация за структурата на типа</summary>
	CXmlTypeInfo	m_oXmlTypeInfo;

	bool m_bInfoInitialized;
};



//////////////////////////////////////////////////////////////////////////
class DLLINTRFC INICXmlSerilizable : public CXMLSerializable
{
public:
	void FillTypeInfo();

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	LPCTSTR GetTypeName() const { return "INIC"; }
};

//////////////////////////////////////////////////////////////////////////
class DLLINTRFC FDATEXmlSerilizable : public CXMLSerializable
{
public:
	void FillTypeInfo();

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	LPCTSTR GetTypeName() const { return "FDATE"; }
};

//////////////////////////////////////////////////////////////////////////
class DLLINTRFC CSRECHDRXmlSerilizable : public CXMLSerializable
{
public:
	void FillTypeInfo();

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	LPCTSTR GetTypeName() const { return "CSRECHDR"; }
};


//////////////////////////////////////////////////////////////////////////
class DLLINTRFC CSHISTINFOXmlSerilizable : public CXMLSerializable
{
public:
	void FillTypeInfo();

	const long GetSize() const { return sizeof( *this ); }

	void * GetPoiner() const { return (void*)this; }

	LPCTSTR GetTypeName() const { return "CSHISTINFO"; }
};
