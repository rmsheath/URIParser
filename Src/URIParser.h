#ifndef URI_PARSER_API_H
#define URI_PARSER_API_H

#include "URIParserTypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	URI_PARSER_DLLEXPORT URIParserVersionNumber URIParserGetVersionNumber();

	URI_PARSER_DLLEXPORT bool URIParserInitialise(TOnParsedCallback* callback);

	URI_PARSER_DLLEXPORT bool URIParserUninitialise();

	URI_PARSER_DLLEXPORT bool ParseURI(const URIParserHostContext hostContext, const char* uri);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif