#ifndef URI_PARSE_MANAGER_H
#define URI_PARSE_MANAGER_H
#include "parser.h"
#include "URIParserTypes.h"

namespace URIPARSER
{
	class URIParseManager
	{
	public:
		URIParseManager(TOnParsedCallback callback) : callback_(callback) {}
		bool Parse(const char* uri);
	private:
		void ParseInThread(std::string uri);
		URIParser parser_;
		TOnParsedCallback callback_;
	};
}

#endif