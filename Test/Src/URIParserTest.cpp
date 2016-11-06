#include "stdafx.h"
#include "Test.h"
#include "URIFixture.h"
//#include "..\..\Src\parser.h"
BOOST_AUTO_TEST_SUITE(URIParserTest)

//Examples of absolute URIs from wikipedia
//https://example.org/absolute/URI/with/absolute/path/to/resource.txt
//ftp://example.org/resource.txt
//urn:ISSN:1535–3613

BOOST_FIXTURE_TEST_CASE(SimpleParse, URIFixture)
{
	//URIPARSER::URIParser parser;
	//URIDataType output = { 0 };
	//parser.Parse("urn:ISSN:1535–3613", output);
	BOOST_CHECK_EQUAL(0, 0);
}

BOOST_AUTO_TEST_SUITE_END()