#include "stdafx.h"
#include "Test.h"
#include "URIFixture.h"
#include "parser.h"
BOOST_AUTO_TEST_SUITE(URIParserTest)

//Examples of absolute URIs from wikipedia
//https://example.org/absolute/URI/with/absolute/path/to/resource.txt
//ftp://example.org/resource.txt
//urn:ISSN:1535–3613

BOOST_FIXTURE_TEST_CASE(SimpleParse, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	parser.Parse("urn:ISSN:1535–3613", parsedData);
	BOOST_CHECK_EQUAL(parsedData.schema, "urn");
	BOOST_CHECK_EQUAL(parsedData.host, "ISSN");
	BOOST_CHECK_EQUAL(parsedData.path, "1535–3613");
}

BOOST_AUTO_TEST_SUITE_END()