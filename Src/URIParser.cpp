#include "stdafx.h"
#include <memory>
#include "URIParser.h"
#include "URIParseManager.h"

const char URI_PARSER_API_VERSION_NUMBER[] = "1.0.0";
std::shared_ptr<URIPARSER::URIParseManager> parserManager_;

URIParserVersionNumber URIParserGetVersionNumber()
{
	return URI_PARSER_API_VERSION_NUMBER;
}

bool URIParserInitialise(TOnParsedCallback* callback)
{
	if (!parserManager_ && callback)
	{
		parserManager_ = std::make_shared<URIPARSER::URIParseManager>(*callback);
	}
	return true;
}

bool URIParserUninitialise()
{
	if (parserManager_)
	{
		parserManager_.reset();
	}
	return true;
}
///\todo get the URIParserHostContext working so it is provided in the callback as an identifier
bool ParseURI(const URIParserHostContext hostContext, const char* uri)
{
	if (parserManager_)
	{
		return parserManager_->Parse(uri);
	}
	return false;
}