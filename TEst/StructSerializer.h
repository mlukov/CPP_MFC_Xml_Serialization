// TypeSerializer.h

#pragma once
#include "CSXMLSerializer.h"
#include "XmlTypeInfo.h"

/// <summary>���� �� ������������ � �������������� ��� � �� XML 
/// �� ��������� ������� � gl_arrCSStructFields ( StructData h/cpp )</summary>
class DLLINTRFC CStructSerializer
{
public:
	/// <summary>���������� �����������</summary>
	CStructSerializer();
	
	/// <summary>�������������� �� XML ��� C++ ��������� ������� � gl_arrCSStructFields</summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML ���������������� <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="oXmlInfo">������������ �� ���� ����� ��-��������������  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="szFieldName">��� �� ���� ���������� ��� ��-������������� ���� ���������� �� �����</param>
	/// <param name="bstrXml">������ XML ��� �������� ��-������������� ���������.</param>
	/// <param name="bDontSearchChild">�� �� ����� �������(���� � ��������)</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool Deserialize( CSXMLSerializer	&	oXmlSerializer
					, IN BYTE			*	pStruct
					, const long			lStructSize
					, const CXmlTypeInfo&	oXmlInfo
					, const _bstr_t		*	pBstrXml = NULL
					, const bool			bDontSearchChild = false );

	/// <summary>������������ ��� XML �� C++ ��������� ������� � gl_arrCSStructFields </summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML �������������� <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="oXmlInfo">������������ �� ���� ����� �������������� <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="szFieldName">��� �� ���� ���������� ��� ��-������������� ���� ���������� �� �����</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool Serialize(IN	CSXMLSerializer		&	oXmlSerializer,
						const BYTE			*	pStruct,
						const long				lStructSize,
						const CXmlTypeInfo	&	oXmlInfo,
						const LPCSTR			szFieldName = NULL );
protected:

/// ��-�������������

	/// <summary> ��-������������� �� ������� ��� ( ���-����� � ��������� )</summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML ��������������/��-�������������� 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� ����� ��-�������������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="szStructName"> ��� �� C++ ��������� ������� � gl_arrCSStructFields </param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	//bool DeserializeTopType( CSXMLSerializer	&	oXmlSerializer
	//						, BYTE			*	pStruct
	//						, long				lStructSize
	//						, LPCTSTR			szStructName );

	/// <summary> �� ������������� �� ��� </summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML ��������������/��-�������������� 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� ����� ��-�������������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="oXmlInfo">������������ �� ���� ����� ��-��������������  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nOffset">���������� � ����</param>
	/// <param name="bDontSearchChild">�� �� ����� �������(���� � ��������)</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool DeserializeType(IN CSXMLSerializer		&	oXmlSerializer,
							BYTE				*	pStruct,
							const long				lStructSize,
							const CXmlTypeInfo	&	oXmlInfo,
							const long				nOffset,
							const bool				bDontSearchChild = false);

	/// <summary> �������� �� �������� �� ������ � ����</summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML ��������������/��-�������������� 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� ����� ��-�������������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="oXmlInfo">������������ �� ���� ����� ��-��������������  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nCurrentOffset"> ������ ���������� ��� �����������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool SetValueFromSerializer( IN CSXMLSerializer &oSerializer
								, BYTE * pStruct
								, long lStructSize
								, const CXmlTypeInfo	&	oXmlInfo
								, const long				nOffset  ) ;

	
/// �������������
	/// <summary> ������������� �� ������� ��� ( ���-����� � ��������� ��� ������� � ����� )</summary>
	/// <param name="oXmlSerializer">����� ����� �������� XML ��������������/��-�������������� 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� �����  ������������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="szStructName"> ��� �� C++ ��������� ������� � gl_arrCSStructFields </param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	//bool SerializeTopType(	CSXMLSerializer	&	oXmlSerializer
	//						, BYTE			*	pStruct
	//						, long				lStructSize
	//						, LPCTSTR			szStructName );

	/// <summary> ������������� �� ��� </summary>
	/// <param name="oXmlSerializer"> ����� ����� �������� XML ��������������/��-�������������� 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> �������� ��� ����������� ����� �������������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="oXmlInfo">������������ �� ���� ����� ��������������  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nOffset"> ������ ���������� ��� �����������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool SerializeType(IN	CSXMLSerializer		&	oXmlSerializer,
							const BYTE			*	pStruct,
							const long				lStructSize,
							const CXmlTypeInfo	&	oXmlInfo,
							const long				nOffset );

	/// <summary> ��������� �� �������� �� ����������� ����� �������������� </summary>
	/// <param name="pStruct"> �������� ��� ����������� ����� �������������� </param>
	/// <param name="lStructSize"> �������� �� ����������� </param>
	/// <param name="strValue"> ����������� �������� (������� ���������) </param>
	/// <param name="oXmlInfo">������������ �� ���� ����� ��������������  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nOffset">������ ���������� ��� �����������</param>
	/// <returns>��������� ����� true ��� ����� � false ��� ������</returns>
	bool GetValue(	const BYTE			*	pStruct,
					const long				lStructSize,
				OUT	CString				&	strValue, 
					const CXmlTypeInfo	&	oXmlInfo,
					const int				nOffset ) ;

};
