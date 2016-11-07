#include "stdafx.h"
#include "Test.h"
#include "URIFixture.h"
#include "..\..\Src\parser.h"
BOOST_AUTO_TEST_SUITE(URIParserTest)

//\TODO: Add test case to check data is not altered if parse fails - Fix this in parse() i.e. use a copy of parsedURI
//\TODO: identify more faliure scenarios and add tests
//\TODO: Add more tests for port, user, password, query, fragment
//\TODO: Add tests to validate correct use of the ASCII character set following these rules:
// the characters : / ? # [] @ are reserved for use as delimiters of the generic URI components and must be percent - encoded — for example, % 3F for a question mark.
// The characters !$ & ' ( ) * + , ; = are permitted by generic URI syntax to be used unencoded in the user information, host, and path as delimiters.
// Additionally, : and @ may appear unencoded within the path, query, and fragment
// and ? and / may appear unencoded as data within the query or fragment.
//\TODO: Refactor URIParser::Parse
//\TODO: consider handling wide-strings
//\TODO: Add C DLL and C++ wrapper with portable data structures for access from other application

BOOST_FIXTURE_TEST_CASE(FTPWithUsernamePasswordAndPort, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("ftp://admin:123456@example.org:21/resource.txt", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "ftp");
	BOOST_CHECK_EQUAL(parsedData.user, "admin");
	BOOST_CHECK_EQUAL(parsedData.password, "123456");
	BOOST_CHECK_EQUAL(parsedData.host, "example.org");
	BOOST_CHECK_EQUAL(parsedData.port, "21");
	BOOST_CHECK_EQUAL(parsedData.path, "resource.txt");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");

}

BOOST_FIXTURE_TEST_CASE(HTTPSWithQueryAndPort, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("https://clients.mindbodyonline.com:8080/classic/home?studioid=285893", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "https");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "clients.mindbodyonline.com");
	BOOST_CHECK_EQUAL(parsedData.port, "8080");
	BOOST_CHECK_EQUAL(parsedData.path, "classic/home");
	BOOST_CHECK_EQUAL(parsedData.query, "studioid=285893");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");

}

BOOST_FIXTURE_TEST_CASE(HTTPSWithFragment, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("https://en.wikipedia.org/wiki/Uniform_Resource_Identifier#Notes", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "https");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "en.wikipedia.org");
	BOOST_CHECK_EQUAL(parsedData.port, "");
	BOOST_CHECK_EQUAL(parsedData.path, "wiki/Uniform_Resource_Identifier");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "Notes");
}

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