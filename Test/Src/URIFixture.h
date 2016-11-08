#ifndef URI_PARSER__TEST_FIXTURE_H
#define URI_PARSER__TEST_FIXTURE_H
#include "..\..\Src\URIParser.h"

class URIFixture
{
public:
	URIFixture();
	~URIFixture();
};

void URIParserCallback(const URIParserUserData* userData, const ParsedURI* parsedURI);

#endif