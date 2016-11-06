#include "stdafx.h"
#include "URIFixture.h"
#include "test.h"

URIFixture::URIFixture()
{
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