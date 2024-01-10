
#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <map>

class HttpRequestParser
{
	public:
		HttpRequestParser();
		~HttpRequestParser();

		HttpRequest				parseHttpRequest(std::string request);
		void					parseRequestLine(std::string &requestLine, std::string &method, std::string &uri, std::string &version);
		const std::string		parseMethod(std::string &requestLine);
		const std::string		parseVersion(std::string &requestLine);
		const std::string		parseUri(std::string &requestLine);
		const std::string		getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind);
		void					parseHeaders(const std::string &request, std::map<std::string, std::string> &headers);
		void					findBody(std::string newLine, bool &bodyFound);
		void					parseBody(std::string newLine, std::string &body);
		int						compareMethod(std::string method, std::string &requestLine);
};
