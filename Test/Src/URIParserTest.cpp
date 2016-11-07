#include "stdafx.h"
#include "Test.h"
#include "URIFixture.h"
#include "parser.h"
BOOST_AUTO_TEST_SUITE(URIParserTest)

//Examples of absolute URIs from wikipedia
BOOST_FIXTURE_TEST_CASE(FTPparse, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("ftp://example.org/resource.txt", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "ftp");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "example.org");
	BOOST_CHECK_EQUAL(parsedData.port, "");
	BOOST_CHECK_EQUAL(parsedData.path, "resource.txt");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
}

BOOST_FIXTURE_TEST_CASE(HTTPparse, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("https://example.org/absolute/URI/with/absolute/path/to/resource.txt", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "https");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "example.org");
	BOOST_CHECK_EQUAL(parsedData.port, "");
	BOOST_CHECK_EQUAL(parsedData.path, "absolute/URI/with/absolute/path/to/resource.txt");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");

}

BOOST_FIXTURE_TEST_CASE(ISSNParse, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("urn:ISSN:1535–3613", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "urn");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "");
	BOOST_CHECK_EQUAL(parsedData.port, "");
	BOOST_CHECK_EQUAL(parsedData.path, "ISSN:1535–3613");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
}

BOOST_FIXTURE_TEST_CASE(EmptyParse, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("", parsedData), false);
	BOOST_CHECK_EQUAL(parsedData.schema, "");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "");
	BOOST_CHECK_EQUAL(parsedData.port, "");
	BOOST_CHECK_EQUAL(parsedData.path, "");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
}
BOOST_AUTO_TEST_SUITE_END()