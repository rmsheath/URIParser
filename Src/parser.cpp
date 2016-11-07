#include "stdafx.h"
#include "parser.h"

namespace URIPARSER
{
	namespace
	{
		/// <summary>Splits the input string at the first split character returning the text before and after the split in the before and after respectively</summary>
		/// <param name="split">The character to search for when splitting the input string</param>
		/// <param name="input">The string to be split</param>
		/// <param name="before">The string before the spit character</param>
		/// <param name="after">The string after the spit character</param>
		/// <returns>True if the split character is found, false otherwise</returns>
		bool SplitOnFirst(const char split, const std::string input, std::string& before, std::string& after)
		{
			bool bRet = false;
			const std::string inputCopy = input;// so the same string can be passed in to before or after
			const std::size_t pos = inputCopy.find_first_of(split);
			if (pos != std::string::npos)
			{
				before = inputCopy.substr(0, pos);
				if (inputCopy.size() > (pos + 1))
				{
					after = inputCopy.substr(pos + 1);
				}
				bRet = true;
			}
			return bRet;
		}
	}

	/// <summary>Parses a URI of the form scheme:[//[user:password@]host[:port]][/]path[?query][#fragment] as per https://en.wikipedia.org/wiki/Uniform_Resource_Identifier
	/// Of the ASCII character set, the characters : / ? # [] @ are reserved for use as delimiters of the generic URI components and must be percent - encoded — for example, % 3F for a question mark.
	/// The characters !$ & ' ( ) * + , ; = are permitted by generic URI syntax to be used unencoded in the user information, host, and path as delimiters.
	/// Additionally, : and @ may appear unencoded within the path, query, and fragment
	/// and ? and / may appear unencoded as data within the query or fragment.
	/// </summary>
	/// <returns>True if the URI parse was successful, false otherwise</returns>
	bool URIParser::Parse(const std::string URI, URIData& parsedURI)
	{
		bool bRet(!URI.empty());
		//\TODO: use a copy of parsedURI so it is left unchanged in faliure scenario
		//\TODO: identify more faliure scenarios and add add tests
		//\TODO: Add tests for port, user, password, query, fragment
		//\TODO: Refactor
		//\TODO: Move into another project
		//\TODO: Add DLL and wrapper with portable data structures for access from other application
		if (bRet)
		{
			std::string remainingURI;
			if (SplitOnFirst(':', URI, parsedURI.schema, remainingURI))
			{
				if (!remainingURI.empty())
				{
					if ((remainingURI.size() > 1) && (remainingURI.substr(0, 2) == "//"))
					{
						//trim "//"
						remainingURI = remainingURI.substr(2);

						if(!remainingURI.empty())
						{
							// Now parse [user:password@]host[:port]
							std::string authority;
							if (SplitOnFirst('/', remainingURI, authority, remainingURI))
							{
								std::string userPassword, hostPort;
								if (SplitOnFirst('@', authority, userPassword, hostPort))
								{
									if (!userPassword.empty())
									{
										if (!SplitOnFirst(':', authority, parsedURI.user, parsedURI.password))
										{
											//malformed URI
											bRet = false;
										}
									}
								}
								else
								{
									hostPort = authority;
								}

								if (!hostPort.empty())
								{
									if (!SplitOnFirst(':', hostPort, parsedURI.host, parsedURI.port))
									{
										//We only have host
										parsedURI.host = hostPort;
									}
								}

							}
						}
					}
					if (!remainingURI.empty())
					{
						// Now parse [/ ]path[?query][#fragment]
						if (remainingURI[0] == '/')
						{
							//ignore the '/' separator
							remainingURI = remainingURI.substr(1);
						}
						if (!remainingURI.empty())
						{
							if (SplitOnFirst('?', remainingURI, parsedURI.path, remainingURI))
							{
								//path is assigned
								//We have a query
								if (SplitOnFirst('#', remainingURI, parsedURI.query, parsedURI.fragment))
								{

								}

							}
							else if (SplitOnFirst('#', remainingURI, parsedURI.path, parsedURI.fragment))
							{
								//there is no query
							}
							else
							{
								parsedURI.path = remainingURI;
							}
						}
					}
				}
			}
		}
		return bRet;
	}

}