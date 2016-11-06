#include "stdafx.h"
#include "parser.h"

namespace URIPARSER
{
//scheme:[//[user:password@]host[:port]][/]path[?query][#fragment]
//Of the ASCII character set, the characters : / ? # [] @ are reserved for use as delimiters of the generic URI components and must be percent - encoded — for example, % 3F for a question mark.
//The characters !$ & ' ( ) * + , ; = are permitted by generic URI syntax to be used unencoded in the user information, host, and path as delimiters.
//Additionally, : and @ may appear unencoded within the path, query, and fragment
//and ? and / may appear unencoded as data within the query or fragment.

	bool URIParser::Parse(const std::string URI, URIData& parsedURI)
	{
		bool bRet(false);

		if (!URI.empty())
		{
			std::string remainingURI;
			const std::size_t pos = URI.find_first_of(':');
			if (pos != std::string::npos)
			{
				//schema
				parsedURI.schema = URI.substr(0, pos);
				remainingURI = URI.substr(pos);
			}
			if (!remainingURI.empty())
			{
				if (remainingURI.substr(0, 2) == "//")
				{
					// Now parse [user:password@]host[:port]
					// remainingURI =
				}
				if (!remainingURI.empty())
				{
					// Now parse [/ ]path
					if (remainingURI[0] == '/')
					{
						//ignore the '/' separator
						remainingURI = remainingURI.substr(1);
					}
					if (!remainingURI.empty())
					{
						const std::size_t pos = URI.find_first_of('?');
						if (pos != std::string::npos)
						{
							//path
							parsedURI.path = remainingURI.substr(0, pos);
							remainingURI = remainingURI.substr(pos);

							if (!remainingURI.empty())
							{
								//now look for [?query][#fragment]
							}
						}
						else
						{
							parsedURI.path = remainingURI;
						}
					}
				}
			}
		}
		return bRet;
	}

}