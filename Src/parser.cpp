#include "stdafx.h"
#include <regex>

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
		bool SplitIPv6HostAndPort(const std::string input, std::string& host, std::string& port)
		{
			bool bRet = false;
			// Extract Ipv6 hostname and optional port
			//\todo check this regular expression
			std::regex base_regex("^\\[([A-Fa-f0-9:]+)\\][:]*([0-9]*)$");
			std::smatch base_match;

			if (std::regex_match(input, base_match, base_regex)) {
				if (base_match.size() == 3)
				{
					host = base_match[1].str();
					port = base_match[2].str();
					bRet = true;
				}
				else if (base_match.size() == 2)
				{
					host = base_match[1].str();
					bRet = true;
				}
			}
			return bRet;
		}
		///\TODO improve percent encoding validation
		bool ValidateSchema(const std::string& input, std::string& errors)
		{
			//Permitted characters within a URI are the ASCII characters for the lowercase and uppercase letters
			//of the modern English alphabet, the Arabic numerals, hyphen, period, underscore, and tilde.
			//Octets represented by any other character must be percent-encoded
			if (std::regex_match(input, std::regex("^[A-Za-z0-9-._~%]*$")))
			{
				return true;
			}
			errors = "Schema contains invalid characters";
			return false;
		}
		///\TODO Validate port is in range 0-65535
		bool ValidatePort(const std::string& port, std::string& errors)
		{
			if (std::regex_match(port, std::regex("^[0-9]*$")))
			{
				return true;
			}
			errors = "Port contains invalid characters";
			return false;
		}
		bool ValidateUserPasswordCharacters(const std::string& input)
		{
			//The characters !$&'()*+,;= are permitted by generic URI syntax to be used unencoded in the user information, host, and path as delimiters
			if (std::regex_match(input, std::regex("^[A-Za-z0-9-._~%!$&'()*+,;=]*$")))
			{
				return true;
			}
			return false;
		}
		bool ValidateUser(const std::string& input, std::string& errors)
		{
			if (!ValidateUserPasswordCharacters(input))
			{
				errors = "Username contains invalid characters";
				return false;
			}
			return true;
		}
		bool ValidatePassword(const std::string& input, std::string& errors)
		{
			if (!ValidateUserPasswordCharacters(input))
			{
				errors = "Password contains invalid characters";
				return false;
			}
			return true;
		}
		bool ValidateHost(const std::string& input, std::string& errors)
		{
			//The characters !$&'()*+,;= are permitted by generic URI syntax to be used unencoded in the user information, host, and path as delimiters
			if (std::regex_match(input, std::regex("^[A-Za-z0-9-._~%!$&'()*+,;=]*$")))
			{
				//hostname or IPv4
				return true;
			}
			else if (std::regex_match(input, std::regex("^[A-Fa-f0-9:]+$")))
			{
				//IPv6
				return true;
			}
			errors = "Host contains invalid characters";
			return false;
		}
		bool ValidatePath(const std::string& input, std::string& errors)
		{
			//The characters !$&'()*+,;= are permitted by generic URI syntax to be used unencoded in the user information, host, and path as delimiters
			//Additionally, : and @ may appear unencoded within the path, query, and fragment
			if (std::regex_match(input, std::regex("^[A-Za-z0-9-._~%!$&'()*+,;=:@/]*$")))
			{
				return true;
			}
			errors = "Path contains invalid characters";
			return false;
		}
		bool ValidateQueryFragmentCharacters(const std::string& input)
		{
			//Additionally, : and @ may appear unencoded within the path, query, and fragment
			//and ? and / may appear unencoded as data within the query or fragment.
			if (std::regex_match(input, std::regex("^[A-Za-z0-9-._~%!$&'()*+,;=:@?/]*$")))
			{
				return true;
			}
			return false;
		}
		bool ValidateQuery(const std::string& input, std::string& errors)
		{
			if (!ValidateQueryFragmentCharacters(input))
			{
				errors = "Query contains invalid characters";
				return false;
			}
			return true;
		}
		bool ValidateFragment(const std::string& input, std::string& errors)
		{
			if (!ValidateQueryFragmentCharacters(input))
			{
				errors = "Fragment contains invalid characters";
				return false;
			}
			return true;
		}
		bool ValidateData(const URIData& parsedURI, URIData& output)
		{
			std::string errors;
			bool bRet(true);
			bRet = bRet && ValidateSchema(parsedURI.schema, errors);
			bRet = bRet && ValidateUser(parsedURI.user, errors);
			bRet = bRet && ValidatePassword(parsedURI.password, errors);
			bRet = bRet && ValidateHost(parsedURI.host, errors);
			bRet = bRet && ValidatePort(parsedURI.port, errors);
			bRet = bRet && ValidatePath(parsedURI.path, errors);
			bRet = bRet && ValidateQuery(parsedURI.query, errors);
			bRet = bRet && ValidateFragment(parsedURI.fragment, errors);
			if (bRet)
			{
				//use compiler provided copy constructor for now
				//ensure we don't pass partially parsed data back to the client
				output = parsedURI;
			}
			else
			{
				output.errors = errors;
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
	bool URIParser::Parse(const std::string URI, URIData& output)
	{
		URIData parsedURI;
		bool bRet(!URI.empty());
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
										if (!SplitOnFirst(':', userPassword, parsedURI.user, parsedURI.password))
										{
											//Allow username with no password
											parsedURI.user = userPassword;
										}
									}
								}
								else
								{
									hostPort = authority;
								}

								if (!hostPort.empty())
								{
									if (!SplitIPv6HostAndPort(hostPort, parsedURI.host, parsedURI.port))
									{
										//This is Ipv4 or hostname
										if (!SplitOnFirst(':', hostPort, parsedURI.host, parsedURI.port))
										{
											//We only have host
											parsedURI.host = hostPort;
										}
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
								if (!SplitOnFirst('#', remainingURI, parsedURI.query, parsedURI.fragment))
								{
									parsedURI.query = remainingURI;
								}
								
							}
							else if (!SplitOnFirst('#', remainingURI, parsedURI.path, parsedURI.fragment))
							{
								parsedURI.path = remainingURI;
							}
						}
					}
				}
			}
		}
		if (!bRet)
		{
			output.errors = "Failed to parse URI";
		}
		else
		{
			bRet = ValidateData(parsedURI, output);
		}
		return bRet;
	}

}