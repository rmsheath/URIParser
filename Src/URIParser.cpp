#include "stdafx.h"
#include <memory>
#include "URIParser.h"
#include "parser.h"

const char URI_PARSER_API_VERSION_NUMBER[] = "1.0.0";
std::shared_ptr<URIPARSER::URIParser> parser_;

URIParserVersionNumber URIParserGetVersionNumber()
{
	return URI_PARSER_API_VERSION_NUMBER;
}

bool URIParserInitialise(TOnParsedCallback callback)
{
	if (!parser_)
	{
		parser_ = std::make_shared<URIPARSER::URIParser>();
	}
	return true;
}

bool URIParserUninitialise()
{
	if (parser_)
	{
		parser_.reset();
	}
	return true;
}

bool ParseURI(const URIParserHostContext hostContext, const char* uri)
{
	if (parser_)
	{
		URIPARSER::URIData data;
		parser_->Parse(uri, data);
		return true;
	}
	return false;
}