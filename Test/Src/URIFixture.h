#ifndef URI_PARSER__TEST_FIXTURE_H
#define URI_PARSER__TEST_FIXTURE_H

#include <string>
#include "..\..\Src\URIParser.h"

class URIFixture
{
public:
	URIFixture();
	~URIFixture();

	void CheckParseFails(const std::string& URI);
};

void URIParserCallback(const URIParserUserData* userData, const ParsedURI* parsedURI);

#endif