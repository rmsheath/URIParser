#include "stdafx.h"
#include <regex>
#include <boost/asio.hpp>
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
			///\ todo check this regular expression
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
		bool ValidateSchema(const std::string& input, std::string& errors)
		{
			//scheme = ALPHA *(ALPHA / DIGIT / "+" / "-" / ".")
			if (std::regex_match(input, std::regex("^([A-Za-z][A-Za-z0-9+-.]*)*$")))
			{
				return true;
			}
			errors = "Schema contains invalid characters";
			return false;
		}
		bool ValidatePort(const std::string& port, std::string& errors)
		{
			///\TODO Consider validating the port is in range 0-65535
			if (std::regex_match(port, std::regex("^[0-9]*$")))
			{
				return true;
			}
			errors = "Port contains invalid characters";
			return false;
		}
		bool ValidateUserPasswordCharacters(const std::string& input)
		{
			// userinfo = *(unreserved / pct-encoded / sub-delims / ":")
			// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
			// pct-encoded = "%" HEXDIG HEXDIG
			// sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="

			// Note: This implies ':' is allowed within user or password any number of times.
			// Previously this was used as a delimiter for user:password
			// It seems embedded passwords have been deprecated 
			// For now, use ':' as user password separator and omit ':' from the regex
			if (std::regex_match(input, std::regex("^(([A-Za-z0-9-._~!$&'()*+,;=])|(%[0-9A-Fa-f]{2,2}))*$")))
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
			// host = IP-literal / IPv4address / reg-name
			if (std::regex_match(input, std::regex("^(([A-Za-z0-9-._~!$&'()*+,;=])|(%[0-9A-Fa-f]{2,2}))*$")))
			{
				// We have a Hostname aka reg-name or IPV4Address:
				// reg-name = *(unreserved / pct-encoded / sub-delims)
				// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
				// pct-encoded = "%" HEXDIG HEXDIG
				// sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
				
				// IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
				// dec-octet = number in range 0-255
				// Cannot validate IPv4Adddress because it is subset of the format for reg-name
				// i.e. if it was badly formated IPv4 it would still match as a reg-name
				// e.g. 2000.1.1.255 is an invalid Ipv4Address but a valid reg-name
				return true;
			}
			else if (std::regex_match(input, std::regex("^[A-Fa-f0-9:]+$")))
			{
				//This is an IPv6address:
				//Use existing IPAddress parser to validate it rather than re-invent it.
				try
				{
					//This will parse IPv4 and IPv6 Addresses but we know it has no '.' characters
					//from the regex so it must be a valid IPv6 address if this doesn't throw
					boost::asio::ip::address::from_string(input);
				}
				catch (const std::exception&)
				{
					errors = "Host contains badly formatted IPv6Address";
					return false;
				}
				return true;
			}
			else if (std::regex_match(input, std::regex("^v[A-Fa-f0-9]+\\.([a-zA-Z0-9-._~!$&'()*+,;=:])+$")))
			{
				// This is a IPvFuture Address
				// IPvFuture = "v" 1 * HEXDIG "." 1 * (unreserved / sub-delims / ":")
				// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
				// sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
				return true;
			}
			
			errors = "Host contains invalid characters";
			return false;
		}
		bool ValidatePath(const std::string& input, std::string& errors)
		{
			// path = path-abempty; begins with "/" or is empty
			//			/ path-absolute; begins with "/" but not "//"
			//			/ path-noscheme; begins with a non-colon segment
			//			/ path-rootless; begins with a segment
			//			/ path-empty; zero characters

			// path-abempty = *("/" segment)
			// path-absolute = "/"[segment-nz *("/" segment)]
			// path-noscheme = segment-nz-nc *("/" segment)
			// path-rootless = segment-nz *("/" segment)
			// path-empty = 0<pchar>

			// segment = *pchar
			// segment-nz = 1 * pchar
			// segment-nz-nc = 1 * (unreserved / pct-encoded / sub-delims / "@")
			// ; non-zero-length segment without any colon ":"
			// pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
			// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
			// pct-encoded = "%" HEXDIG HEXDIG
			// sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="

			//The characters !$&'()*+,;= are permitted by generic URI syntax to be used unencoded in the user information, host, and path as delimiters
			//Additionally, : and @ may appear unencoded within the path, query, and fragment
			if (std::regex_match(input, std::regex("^(([A-Za-z0-9-._~!$&'()*+,;=:@/])|(%[0-9a-fA-F]{2,2}))*$")))
			{
				return true;
			}
			errors = "Path contains invalid characters";
			return false;
		}
		bool ValidateQueryFragmentCharacters(const std::string& input)
		{
			// query = *(pchar / "/" / "?")
			// fragment = *(pchar / "/" / "?")
			// pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
			// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
			// pct-encoded = "%" HEXDIG HEXDIG
			// sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="

			if (std::regex_match(input, std::regex("^(([A-Za-z0-9-._~!$&'()*+,;=:@?/])|(%[0-9a-fA-F]{2,2}))*$")))
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

	//  More precise definition: https://tools.ietf.org/html/rfc3986#section-3
	//  Appendix A. Collected ABNF for URI:
	//
	//	URI = scheme ":" hier-part["?" query]["#" fragment]
	//	hier-part = "//" authority path-abempty
	//	/ path-absolute
	//	/ path-rootless
	//	/ path-empty
	//	URI-reference = URI / relative-ref
	//	absolute-URI = scheme ":" hier-part["?" query]
	//	relative-ref = relative-part["?" query]["#" fragment]
	//	relative-part = "//" authority path-abempty
	//	/ path-absolute
	//	/ path-noscheme
	//	/ path-empty

	//	scheme = ALPHA *(ALPHA / DIGIT / "+" / "-" / ".")

	//	authority = [userinfo "@"] host[":" port]
	//	userinfo = *(unreserved / pct-encoded / sub-delims / ":")
	//	host = IP-literal / IPv4address / reg-name
	//	port = *DIGIT

	//	IP-literal = "[" (IPv6address / IPvFuture) "]"

	//	IPvFuture = "v" 1 * HEXDIG "." 1 * (unreserved / sub-delims / ":")

	//	IPv6address = 6(h16 ":") ls32
	//	/ "::" 5(h16 ":") ls32
	//	/ [h16] "::" 4(h16 ":") ls32
	//	/ [*1(h16 ":") h16] "::" 3(h16 ":") ls32
	//	/ [*2(h16 ":") h16] "::" 2(h16 ":") ls32
	//	/ [*3(h16 ":") h16] "::"    h16 ":"   ls32
	//	/ [*4(h16 ":") h16] "::"              ls32
	//	/ [*5(h16 ":") h16] "::"              h16
	//	/ [*6(h16 ":") h16] "::"

	//	h16 = 1 * 4HEXDIG
	//	ls32 = (h16 ":" h16) / IPv4address
	//	IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
	//	dec-octet = DIGIT; 0-9
	//	/ %x31-39 DIGIT; 10-99
	//	/ "1" 2DIGIT; 100-199
	//	/ "2" %x30-34 DIGIT; 200-249
	//	/ "25" %x30-35; 250-255

	//	reg-name = *(unreserved / pct-encoded / sub-delims)

	//	path = path-abempty; begins with "/" or is empty
	//	/ path-absolute; begins with "/" but not "//"
	//	/ path-noscheme; begins with a non-colon segment
	//	/ path-rootless; begins with a segment
	//	/ path-empty; zero characters

	//	path-abempty = *("/" segment)
	//	path-absolute = "/"[segment-nz *("/" segment)]
	//	path-noscheme = segment-nz-nc *("/" segment)
	//	path-rootless = segment-nz *("/" segment)
	//	path-empty = 0<pchar>

	//	segment = *pchar
	//	segment-nz = 1 * pchar
	//	segment-nz-nc = 1 * (unreserved / pct-encoded / sub-delims / "@")
	//	; non-zero-length segment without any colon ":"

	//	pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
	//	query = *(pchar / "/" / "?")
	//	fragment = *(pchar / "/" / "?")
	//	pct-encoded = "%" HEXDIG HEXDIG
	//	unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
	//	reserved = gen-delims / sub-delims
	//	gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
	//	sub-delims = "!" / "$" / "&" / "'" / "(" / ")"
	//	/ "*" / "+" / "," / ";" / "="

	/// <summary>Parses a URI of the form scheme:[//[user:password@]host[:port]][/]path[?query][#fragment] as per https://en.wikipedia.org/wiki/Uniform_Resource_Identifier
	/// Of the ASCII character set, the characters : / ? # [] @ are reserved for use as delimiters of the generic URI components and must be percent-encoded - for example, % 3F for a question mark.
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