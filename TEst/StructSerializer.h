// TypeSerializer.h

#pragma once
#include "CSXMLSerializer.h"
#include "XmlTypeInfo.h"

/// <summary>Клас за Сериализация и Десериализация към и от XML 
/// на структури описани в gl_arrCSStructFields ( StructData h/cpp )</summary>
class DLLINTRFC CStructSerializer
{
public:
	/// <summary>Стандартен конструктор</summary>
	CStructSerializer();
	
	/// <summary>Десериализация на XML към C++ структура описана в gl_arrCSStructFields</summary>
	/// <param name="oXmlSerializer">Обект който извършва XML десериализацията <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="oXmlInfo">Информацията за типа който де-сериализаираме  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="szFieldName">Име на член променлива ако де-сериализираме член променлива на обект</param>
	/// <param name="bstrXml">Целият XML ако директно де-сериализираме структура.</param>
	/// <param name="bDontSearchChild">Да не търси подклон(вече е извлечен)</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool Deserialize( CSXMLSerializer	&	oXmlSerializer
					, IN BYTE			*	pStruct
					, const long			lStructSize
					, const CXmlTypeInfo&	oXmlInfo
					, const _bstr_t		*	pBstrXml = NULL
					, const bool			bDontSearchChild = false );

	/// <summary>Сериализация към XML на C++ структура описана в gl_arrCSStructFields </summary>
	/// <param name="oXmlSerializer">Обект който извършва XML сериализацията <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="oXmlInfo">Информацията за типа който сериализаираме <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="szFieldName">Име на член променлива ако де-сериализираме член променлива на обект</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool Serialize(IN	CSXMLSerializer		&	oXmlSerializer,
						const BYTE			*	pStruct,
						const long				lStructSize,
						const CXmlTypeInfo	&	oXmlInfo,
						const LPCSTR			szFieldName = NULL );
protected:

/// Де-сериализиране

	/// <summary> Де-сериализиране на Основен таг ( най-горен в документа )</summary>
	/// <param name="oXmlSerializer">Обект който извършва XML сериализацията/де-сериализацията 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата който де-сериализаираме </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="szStructName"> Име на C++ структура описана в gl_arrCSStructFields </param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	//bool DeserializeTopType( CSXMLSerializer	&	oXmlSerializer
	//						, BYTE			*	pStruct
	//						, long				lStructSize
	//						, LPCTSTR			szStructName );

	/// <summary> Де сериализиране на таг </summary>
	/// <param name="oXmlSerializer">Обект който извършва XML сериализацията/де-сериализацията 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата който де-сериализаираме </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="oXmlInfo">Информацията за типа който де-сериализаираме  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nOffset">Отместване в типа</param>
	/// <param name="bDontSearchChild">Да не търси подклон(вече е извлечен)</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool DeserializeType(IN CSXMLSerializer		&	oXmlSerializer,
							BYTE				*	pStruct,
							const long				lStructSize,
							const CXmlTypeInfo	&	oXmlInfo,
							const long				nOffset,
							const bool				bDontSearchChild = false);

	/// <summary> Задаване на стойност от текста в тага</summary>
	/// <param name="oXmlSerializer">Обект който извършва XML сериализацията/де-сериализацията 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата която де-сериализаираме </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="oXmlInfo">Информацията за типа който де-сериализаираме  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nCurrentOffset"> Текущо отместване във структурата</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool SetValueFromSerializer( IN CSXMLSerializer &oSerializer
								, BYTE * pStruct
								, long lStructSize
								, const CXmlTypeInfo	&	oXmlInfo
								, const long				nOffset  ) ;

	
/// Сериализиране
	/// <summary> Сериализиране на Основен таг ( най-горен в документа или елемент в масив )</summary>
	/// <param name="oXmlSerializer">Обект който извършва XML сериализацията/де-сериализацията 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата който  сериализираме </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="szStructName"> Име на C++ структура описана в gl_arrCSStructFields </param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	//bool SerializeTopType(	CSXMLSerializer	&	oXmlSerializer
	//						, BYTE			*	pStruct
	//						, long				lStructSize
	//						, LPCTSTR			szStructName );

	/// <summary> Сериализиране на таг </summary>
	/// <param name="oXmlSerializer"> Обект който извършва XML сериализацията/де-сериализацията 
	/// <seealso cerf="CSXMLSerializer" /></param>
	/// <param name="pStruct"> Указател към структурата която сериализаираме </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="oXmlInfo">Информацията за типа който сериализаираме  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nOffset"> Текущо отместване във структурата</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool SerializeType(IN	CSXMLSerializer		&	oXmlSerializer,
							const BYTE			*	pStruct,
							const long				lStructSize,
							const CXmlTypeInfo	&	oXmlInfo,
							const long				nOffset );

	/// <summary> Извличане на стойност от структурата който сериализаираме </summary>
	/// <param name="pStruct"> Указател към структурата който сериализаираме </param>
	/// <param name="lStructSize"> Големина на структурата </param>
	/// <param name="strValue"> Извлечената стойност (изходен параметър) </param>
	/// <param name="oXmlInfo">Информацията за типа който сериализаираме  <seealso cref="CXmlTypeInfo"/></param>
	/// <param name="nOffset">Текущо отместване във структурата</param>
	/// <returns>Функцията връща true при успех и false при грешка</returns>
	bool GetValue(	const BYTE			*	pStruct,
					const long				lStructSize,
				OUT	CString				&	strValue, 
					const CXmlTypeInfo	&	oXmlInfo,
					const int				nOffset ) ;

};
