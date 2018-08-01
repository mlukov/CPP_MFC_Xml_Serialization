// XMLSerializer.h

#pragma once

#import <msxml6.dll> named_guids

class CXMLSerializable;
class CStructSerializer;

/// <summary>K��� �� XML ������������/��������������</summary>
class DLLINTRFC CSXMLSerializer
{
	friend class CXMLSerializable;
	friend class CStructSerializer;
public:
	/// <summary>���������� �����������</summary>
	CSXMLSerializer();

	/// <summary>������������� �� ���� ����</summary>
	/// <param name="szFieldName">��� �� ���� ������������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool SerializeField( const LPCSTR szFieldName, const LPCTSTR szValue );

	/// <summary>������������ ��� XML �� C++ ��������� ������� � gl_arrCSStructFields </summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML �������������� <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="szStructName"> ��� �� C++ ��������� ������� � gl_arrCSStructFields </param>
	/// <param name="szFieldName">��� �� ���� ���������� ��� ��-������������� ���� ���������� �� �����</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool SerializeStruct( IN BYTE		*	pStruct
		, IN long			lStructSize
		, IN LPCTSTR		szStructName
		, IN const LPCSTR	szFieldName = NULL );

	/// <summary>��-������������� �� ���� ����</summary>
	/// <param name="szFieldName">��� �� ���� ������������</param>
	/// <param name="szValue">�������� (������� ���������)</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT LPCTSTR szValue, const int lSize );

	/// <summary>��-������������� �� ���� ����</summary>
	/// <param name="szFieldName">��� �� ���� ������������</param>
	/// <param name="lValue">�������� (������� ���������)</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT long lValue );

	/// <summary>��-������������� �� ���� ����</summary>
	/// <param name="szFieldName">��� �� ���� ������������</param>
	/// <param name="dValue">�������� (������� ���������)</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT double dValue );

	/// <summary>��-������������� �� ���� ����</summary>
	/// <param name="szFieldName">��� �� ���� ������������</param>
	/// <param name="fValue">�������� (������� ���������)</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool DeserializeField( const LPCSTR szFieldName, OUT float fValue );

	/// <summary>�������������� �� XML ��� C++ ��������� ������� � gl_arrCSStructFields</summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML ���������������� <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="szStructName"> ��� �� C++ ��������� ������� � gl_arrCSStructFields </param>
	/// <param name="szFieldName">��� �� ���� ���������� ��� ��-������������� ���� ���������� �� �����</param>
	/// <param name="bstrXml">������ XML ��� �������� ��-������������� ���������.</param>
	/// <param name="bDontSearchChild">�� �� ����� �������(���� � ��������)</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool DeserializeStruct(   IN BYTE		*	pStruct
							, IN long			lStructSize
							, IN LPCTSTR		szStructName
							, IN const LPCTSTR	szFieldName = NULL
							, IN const _bstr_t* pBstrXml	= NULL
							, IN const bool		bDontSearchChild = false );


	/// <summary>�������� �� ��� ���� � ������� �� ���������</summary>
	/// <param name="szFieldName">��� �� ���� ������������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool AddChildNode( const LPCTSTR szFieldName );

	/// <summary>����������� ��� ���-���� �� ������� � ������� �� ���������</summary>
	/// <param name="szFieldName">��� �� ���� ������������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool GetChildNode( const LPCTSTR szFieldName );

	/// <summary>��������� �� ���-������� �� ������� � ������� �� ���������<</summary>
	/// <returns>��������� ����� ���� �� ���������� ��� ����� � ���� ��� ������</returns>
	long GetChildNodes();

	/// <summary>����������� ��� ���-���� �� ������� � ������� �� ���������</summary>
	/// <param name="lIndex">������ �� ��������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool GetChildNodeAt( const long lIndex );

	/// <summary>������� ��� ��-����� ���� � ������� �� ���������<</summary>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool MoveOneNodeUp();
	
	/// <summary>������� �� XML ��������� ���� ������������</summary>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool GetXML( OUT _bstr_t &bstrXml );

private:
	/// <summary>������������� �� ������� ����� ������������/��������������</summary>
	/// <param name="bstrXml">XML �� ��������������� ��� �������������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool Initialize( IN const _bstr_t &bstrXml );

	/// <summary>������������� �� � ������</summary>
	/// <returns>��������� ����� true ��� ������ � ������������� � ��� �� �</returns>
	bool IsInitialized() const;

	/// <summary>�������� �� XSI �������</summary>
	/// <param name="variantValue">�������� �� ���������</param>
	void SetRootNodeXSIAttribute( LPCTSTR szValue );

	/// <summary>�������� �� XSI �������</summary>
	/// <param name="variantValue">�������� �� ���������</param>
	void SetRootNodeXSDAttribute( LPCTSTR szValue );

	/// <summary>�������� �� ������� ���� �� ��������� ( <?xml version='1.0' encoding='UTF-16'> )</summary>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool AddProcessingNode();

	/// <summary>��������� �� ������ �� �������� ���� �� �������</summary>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool GetCurNodeText( OUT BSTR &bstr );

	/// <summary>�������� �� ������ �� �������� ���� �� �������</summary>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool SetCurNodeText( const LPCTSTR szNodeText );

	
protected:
	/// <summary>��������</summary> 
	MSXML2::IXMLDOMDocument2Ptr		m_pXMLDoc; 
	/// <summary>������ �� ������� �� XML ���������</summary> 
	MSXML2::IXMLDOMElementPtr		m_pRootNode;

	/// <summary>����� ������� �� ������� �� XML ���������</summary> 
	MSXML2::IXMLDOMElementPtr		m_pCurrentNode;

	MSXML2::IXMLDOMNodeListPtr		m_pChildsList;
	
	/// <summary>������������ �� ������ �� ������� ����� �� �����������</summary> 
	CString							m_strRootTagName;

	/// <summary>������������� �� � ������</summary>
	bool m_bInitialized;

	/// <summary>������� �� � XSI ��������</summary>
	bool m_bRootNodeXSIAttributeSet;

	/// <summary>������� �� � XSD ��������</summary>
	bool m_bRootNodeXSDAttributeSet;

	/// <summary>������� �� � ������� ���� �� ��������� ( <?xml version='1.0' encoding='UTF-16'> )</summary>
	bool m_bProcessingNodeAdded;
};
