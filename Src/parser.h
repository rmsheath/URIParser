#ifndef URI_PARSER_H
#define URI_PARSER_H
#include <string>

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

	class URI_PARSER_DLLEXPORT URIParser
	{
	public:
		bool Parse(const std::string URI, URIData& parsedURI);
	};
}
#endif