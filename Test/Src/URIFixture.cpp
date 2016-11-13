#include "stdafx.h"
#include "URIFixture.h"
#include "test.h"
#include "..\..\Src\parser.h"

void URIParserCallback(const URIParserUserData* userData, const ParsedURI* parsedURI)
{

}

URIFixture::URIFixture()
{
	TOnParsedCallback callbackFn = *URIParserCallback;
	URIParserInitialise(&callbackFn);
	try
	{
		std::stringstream stream;
		for (int count = 0; count < boost::unit_test::framework::master_test_suite().argc; ++count)
		{
			stream << boost::unit_test::framework::master_test_suite().argv[count];
		}
	}
	catch (std::runtime_error error)
	{
		BOOST_TEST_MESSAGE("Setup URIParserTest fixture failed");
	}
}

URIFixture::~URIFixture()
{
	URIParserUninitialise();
}

void URIFixture::CheckParseFails(const std::string& URI)
{
	URIPARSER::URIData parsedData;
	URIPARSER::URIParser parser;
	BOOST_CHECK_EQUAL(parser.Parse(URI, parsedData), false);
	BOOST_CHECK_EQUAL(parsedData.schema, "");
	BOOST_CHECK_EQUAL(parsedData.user, "");
	BOOST_CHECK_EQUAL(parsedData.password, "");
	BOOST_CHECK_EQUAL(parsedData.host, "");
	BOOST_CHECK_EQUAL(parsedData.port, "");
	BOOST_CHECK_EQUAL(parsedData.path, "");
	BOOST_CHECK_EQUAL(parsedData.query, "");
	BOOST_CHECK_EQUAL(parsedData.fragment, "");
	BOOST_CHECK_NE(parsedData.errors, "");
}