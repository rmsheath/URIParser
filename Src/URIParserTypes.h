#ifndef URI_PARSER_TYPES_H
#define URI_PARSER_TYPES_H

#ifdef _WIN32
#ifndef URI_PARSER_DLLEXPORT
#ifdef BUILDING_URI_PARSER_DLL
#define URI_PARSER_DLLEXPORT __declspec(dllexport)
#else
#define URI_PARSER_DLLEXPORT __declspec(dllimport)
#endif
#endif
#else
#ifndef URI_PARSER_DLLEXPORT
#define URI_PARSER_DLLEXPORT
#endif
#endif

typedef const char* URIParserVersionNumber;
typedef void* URIParserHostContext;
typedef const char * URIParserStringType;

typedef struct URIParserUserDataT
{
	URIParserHostContext HostContext;
} URIParserUserData;

struct ParsedURI
{
	URIParserStringType Schema;
	URIParserStringType User;
	URIParserStringType Password;
	URIParserStringType Host;
	URIParserStringType Port;
	URIParserStringType Path;
	URIParserStringType Query;
	URIParserStringType Fragment;
	URIParserStringType Errors;
	bool Success;
};

typedef void(*TOnParsedCallback)(const URIParserUserData* userData, const ParsedURI* parsedURI);
//TOnParsedCallback OnParsedCallback;

#endif