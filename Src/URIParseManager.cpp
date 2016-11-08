#include "stdafx.h"
#include <thread>
#include "URIParseManager.h"

namespace URIPARSER
{
	bool URIParseManager::Parse(const char* uri)
	{
		std::string strURI(uri);	
		std::thread exec(std::bind(&URIParseManager::ParseInThread, this, strURI));
		return true;
	}
	void URIParseManager::ParseInThread(std::string uri)
	{
		URIPARSER::URIData data;
		parser_.Parse(uri, data);
		//todo copy over data
		const URIParserUserData* userData = { 0 };
		const ParsedURI* parsedURI = nullptr;
		(callback_)(userData, parsedURI);
	}
};
