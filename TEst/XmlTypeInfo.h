// XmlTypeInfo.h
#pragma once

/// <summary>������ ����� �� �������� �� ���������</summary>
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

/// <summary>����� ��� ���������� �� ������ �����</summary>
class DLLINTRFC CXMLTypeInfoArray: public CSmartTypedPtrArray< CPtrArray, CXmlTypeInfo > 
{

public:
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
	/// <param name="nSubTypeSize">�������� �� �������</param>
	/// <returns>��������� ����� ������� �� ��������� � ������ ����� ��� ����� � -1 ��� ������</returns>
	INT_PTR AddCPtrArrayInfo(	const LPCTSTR		szFieldName,
								const int			nOffset,
								const LPCTSTR		szSubTypeFieldName,
								const XMLDataType	eSubTypeDataType,  
								const CXmlTypeInfo &oSubTypeInfo,
								const int			nSubTypeSize );

private:
	/// <summary>�������� �� ������� �� ������ ���������� � ������</summary>
	/// <returns>��������� ����� true ���  � false ��� ������</returns>
	bool CanAddOffset( const int nOffset, const LPCTSTR szFieldName );
};


/// <summary>���� �� �������� �� ���������</summary>
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
	/// <summary>����������� ��� ������ ���������</summary>
	/// <param name="DataType">��� ����� �� ������ <seealso cref="XMLDataType"/> </param>
	/// <param name="szFieldName">������������ �� ������(������ ������� �� �� �������� �� ������������)</param>
	/// <param name="nOffset">���������� �� ������ � �����������</param>
	/// <param name="nSize">������ �� ������(���� �� ������ ������)</param>
	/// <param name="szSubTypeName">��� �� ������� �� ������(���� �� ������)</param>
	CXmlTypeInfo(	const XMLDataType	eDataType,  
					const LPCTSTR	szFieldName,
					const int		nOffset, 
					const int		nSize = 0, 
					const LPCTSTR	szSubTypeName = NULL);

	/// <summary>����������� �� �����</summary>
	CXmlTypeInfo( const CXmlTypeInfo & oXmlInfo );

	CXmlTypeInfo & operator = ( const CXmlTypeInfo & oXmlInfo );

	const CXmlTypeInfo * GetTypeInfo( const LPCTSTR	szFieldName ) const;

	bool	IsSimpleType()	const;
	bool	IsCPtrArray()	const;
	bool	IsComplexType() const;

	int GetTypeSize() const;

public:
	/// <summary>��� ����� �� ������ <seealso cref="XMLDataType"/> </summary>
	XMLDataType		m_eDataType;
	/// <summary>������������ �� ������(������ ������� �� �� �������� �� ������������)</summary>
	CString			m_strFieldName;
	/// <summary>��� �� ������� �� ������(���� �� ������)</summary>
	CString			m_strSubTypeName;
	/// <summary>������ �� ������( �� ������ ������ � �������� �� ����� )</summary>
	int				m_nSize;
	/// <summary>���������� �� ������ � �����������</summary>
	int				m_nOffset;

	/// <summary>����� � ������ �� ���-����</summary>
	CXMLTypeInfoArray	m_oArrInfos;
};
