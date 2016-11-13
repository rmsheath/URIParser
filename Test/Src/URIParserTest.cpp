#include "stdafx.h"
#include "Test.h"
#include "URIFixture.h"
#include "..\..\Src\parser.h"
BOOST_AUTO_TEST_SUITE(URIParserTest)

///\TODO: identify more failure scenarios and add tests
///\TODO: Move validation so it happens at the point each element is parsed
///\TODO: Add C DLL and C++ wrapper with portable data structures for access from other application
///\TODO: Test support for URI references - Possibly add an option to flag if this is supported 
///\TODO: Consider adding an option to decode percent encoded characters in the output

BOOST_FIXTURE_TEST_CASE(BadPercentEncodedCharactersAreNotAllowedInHostName, URIFixture)
{
	//Regex for permitted chars [A-Za-z0-9-._~]
	//Plus %encoded e.g.%20 space
	//delims= / ? # [] @
	//user, password, host, and path can also contain !$ & ' ( ) * + , ; =
	//path, query, and fragment can contain : and @
	CheckParseFails("ftp://admin@exa%Z0mple.org:21/resource.txt");
}

BOOST_FIXTURE_TEST_CASE(PercentEncodedCharactersAreAllowedInHostName, URIFixture)
{
	//Regex for permitted chars [A-Za-z0-9-._~]
	//Plus %encoded e.g.%20 space
	//delims= / ? # [] @
	//user, password, host, and path can also contain !$ & ' ( ) * + , ; =
	//path, query, and fragment can contain : and @
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("ftp://admin@exa%20mple.org:21/resource.txt", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "ftp");
	BOOST_CHECK_EQUAL(parsedData.user, "admin");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "exa%20mple.org");
	BOOST_CHECK_EQUAL(parsedData.port, "21");
	BOOST_CHECK_EQUAL(parsedData.path, "resource.txt");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
	BOOST_CHECK_EQUAL(parsedData.errors, "");
}

BOOST_FIXTURE_TEST_CASE(InvalidCharactersShouldFail, URIFixture)
{
	//Regex for permitted chars [A-Za-z0-9-._~]
	//Plus %encoded e.g.%20 space
	//delims= / ? # [] @
	//user, password, host, and path can also contain !$ & ' ( ) * + , ; =
	//path, query, and fragment can contain : and @
	CheckParseFails("ftp://admin@exa mple.org:21/resource.txt");
}

BOOST_FIXTURE_TEST_CASE(InvalidSchemaCharactersShouldFail, URIFixture)
{
	//scheme, consisting of a sequence of characters beginning with a letter and followed by any combination of letters, digits, plus(+), period(.), or hyphen(-).
	CheckParseFails("ft^p://admin@example.org:21/resource.txt");

}

BOOST_FIXTURE_TEST_CASE(FTPWithUsernamePasswordPortAndBadIPv6ShouldFail, URIFixture)
{
	CheckParseFails("ftp://admin@[2620::0:ccc::2]:21/resource.txt");
}

BOOST_FIXTURE_TEST_CASE(FTPWithUsernamePasswordPortAndIPv6, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("ftp://admin@[2620:0:ccc::2]:21/resource.txt", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "ftp");
	BOOST_CHECK_EQUAL(parsedData.user, "admin");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "2620:0:ccc::2");
	BOOST_CHECK_EQUAL(parsedData.port, "21");
	BOOST_CHECK_EQUAL(parsedData.path, "resource.txt");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
	BOOST_CHECK_EQUAL(parsedData.errors, "");
}

BOOST_FIXTURE_TEST_CASE(FTPWithUsernamePasswordPortAndIPv4, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("ftp://admin@127.0.0.1:21/resource.txt", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "ftp");
	BOOST_CHECK_EQUAL(parsedData.user, "admin");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "127.0.0.1");
	BOOST_CHECK_EQUAL(parsedData.port, "21");
	BOOST_CHECK_EQUAL(parsedData.path, "resource.txt");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
	BOOST_CHECK_EQUAL(parsedData.errors, "");

}

BOOST_FIXTURE_TEST_CASE(FTPWithUsernamePortAndNoPassword, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("ftp://admin@example.org:21/resource.txt", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "ftp");
	BOOST_CHECK_EQUAL(parsedData.user, "admin");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "example.org");
	BOOST_CHECK_EQUAL(parsedData.port, "21");
	BOOST_CHECK_EQUAL(parsedData.path, "resource.txt");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
	BOOST_CHECK_EQUAL(parsedData.errors, "");

}

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
	BOOST_CHECK_EQUAL(parsedData.errors, "");

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
	BOOST_CHECK_EQUAL(parsedData.errors, "");

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
	BOOST_CHECK_EQUAL(parsedData.errors, "");
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
	BOOST_CHECK_EQUAL(parsedData.errors, "");
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
	BOOST_CHECK_EQUAL(parsedData.errors, "");
}

BOOST_FIXTURE_TEST_CASE(ISSNParse, URIFixture)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse("urn:ISSN:1535-3613", parsedData), true);
	BOOST_CHECK_EQUAL(parsedData.schema, "urn");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "");
	BOOST_CHECK_EQUAL(parsedData.port, "");
	BOOST_CHECK_EQUAL(parsedData.path, "ISSN:1535-3613");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
	BOOST_CHECK_EQUAL(parsedData.errors, "");
}

BOOST_FIXTURE_TEST_CASE(EmptyParse, URIFixture)
{
	CheckParseFails("");
}

BOOST_AUTO_TEST_SUITE_END()