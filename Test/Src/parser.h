#ifndef URI_PARSER_H
#define URI_PARSER_H
#include <string>
namespace URIPARSER
{
	struct URIData
	{
		std::string schema;
		std::string user;
		std::string password;
		std::string host;
		std::string port;
		std::string path;
		std::string query;
		std::string fragment;
	};

	class URIParser
	{
	public:
		bool Parse(const std::string URI, URIData& parsedURI);
	};
}
#endif