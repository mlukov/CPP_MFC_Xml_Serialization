// XMLSerializable.h

#pragma once
#include "CSXMLSerializer.h"
#include "XmlTypeInfo.h"


/// <summary>���������� ���� �� XML ������������/��������������</summary>
class DLLINTRFC CXMLSerializable
{
public:
	/// <summary>���������� �����������</summary>
	CXMLSerializable();

	/// <summary>�������������</summary>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	virtual bool SerializeXML( CSXMLSerializer &oXmlSerializer, const LPCTSTR szFieldName = NULL );

	/// <summary>��-�������������</summary>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	virtual bool DeserializeXML( CSXMLSerializer &oXmlSerializer, IN const _bstr_t *pbstrXml = NULL, const LPCTSTR szFieldName = NULL );

	/// <summary>���������� ���������� �� ����������� �� ����</summary>
	const CXmlTypeInfo & GetTypeInfo();

protected:
	/// <summary>�������������� �� ����</summary>
	virtual LPCTSTR GetTypeName() const = 0;

	/// <summary>�������� �� ����</summary>
	virtual const long GetSize() const = 0;

	virtual void * GetPoiner() const = 0;

	/// <summary>��������� �� ���������� �� ����</summary>
	virtual void FillTypeInfo() = 0;

	/// <summary>�������� �� ���� �����</summary>
	/// <param name="eDataType">��� ����� �� ������ <seealso cref="XMLDataType"/> </param>
	void SetDataType( const XMLDataType	eDataType );


	/// <summary>�������� �� ����� ��� ����� � ���������� �� ����</summary>
	/// <param name="eDataType">��� ����� �� ������ <seealso cref="XMLDataType"/> </param>
	/// <param name="szFieldName">������������ �� ������(������ ������� �� �� �������� �� ������������)</param>
	/// <param name="nOffset">���������� �� ������ � �����������</param>
	/// <param name="nSize">������ �� ������(���� �� ������ ������)</param>
	/// <returns>��������� ����� ������� �� ��������� � ������ ����� ��� ����� � -1 ��� ������</returns>
	INT_PTR AddSimpleInfo(	const XMLDataType	eDataType,  
		const LPCTSTR		szFieldName,
		const int			nOffset, 
		const int			nSize = 0, 
		const LPCTSTR		szSubTypeName = NULL );

	/// <summary>�������� �� ������ ��� ����� � ���������� �� ����</summary>
	/// <param name="oTypeInfo">���������� �� ������� ����� �������� <seealso cref="CXmlTypeInfo"/> </param>
	/// <param name="szFieldName">������������ �� ������(������ ������� �� �� �������� �� ������������)</param>
	/// <param name="nOffset">���������� �� ������ � �����������</param>
	/// <returns>��������� ����� ������� �� ��������� � ������ ����� ��� ����� � -1 ��� ������</returns>
	INT_PTR AddComplexInfo( const CXmlTypeInfo &oTypeInfo,
		const LPCTSTR		szFieldName,
		const int			nOffset );

	/// <summary>�������� �� ��� ����� ��� ������ ��� ����� � ���������� �� ����</summary>
	/// <param name="szFieldName">������������ �� ������(������ ������� �� �� �������� �� ������������)</param>
	/// <param name="nOffset">���������� �� ������ � �����������</param>
	/// <param name="szSubTypeFieldName">������������ �� ���-������(������ ������� �� �� �������� �� ������������)</param>
	/// <param name="eSubTypeDataType">��� �� ������� <seealso cref="XMLDataType"/> </param>
	/// <param name="oSubTypeInfo">���������� �� ������� ����� ��������<seealso cref="CXmlTypeInfo"/> </param>
	/// <param name="nSubTypeSize">�������� �� �������(���� �� ������ ������)</param>
	/// <returns>��������� ����� ������� �� ��������� � ������ ����� ��� ����� � -1 ��� ������</returns>
	INT_PTR AddCPtrArrayInfo(	const LPCTSTR		szFieldName,
		const int			nOffset,
		const LPCTSTR		szSubTypeFieldName,
		const XMLDataType	eSubTypeDataType,  
		const CXmlTypeInfo &oSubTypeInfo,
		const int			nSubTypeSize );

	/// <returns>��������� ������ �� ����� true ��� ����� � false ��� ������</returns>
	virtual bool SerializeFields( CSXMLSerializer &oXmlSerializer );

	/// <returns>��������� ������ �� ����� true ��� ����� � false ��� ������</returns>
	virtual bool DeserializeFields( CSXMLSerializer &oXmlSerializer );

	/// <summary>��� �� ����(�� ������������)</summary>
	CString			m_strTypeName;

	/// <summary>���������� ���������� �� ����������� �� ����</summary>
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
