
#pragma once

#include "WebServer.hpp"
#include "Exceptions.hpp"

class HttpRequest
{
	public:

		enum METHOD
		{
			NONE,
			GET,
			POST,
			DELETE
		};

		~HttpRequest(void);
		HttpRequest(const HttpRequest& rhs);
		HttpRequest(HttpRequest::METHOD method,
					const std::string& version,
					const std::string& uri,
					const std::string& host,
					const std::string body,
					const int& contentLength);

		HttpRequest&	operator=(const HttpRequest& rhs);

		HttpRequest::METHOD			getMethod() const;
		const std::string			getVersion() const;
		const std::string			getUri() const;
		const std::string			getHost() const;
		const std::string			getBody() const;
		const int&					getContentLength() const;

	private:
		static const std::string	_allowedMethods[];

		HttpRequest::METHOD				_method;
		const std::string			_version;
		const std::string			_uri;
		const std::string			_host;
		const std::string			_body;
		const int					_contentLength;

		HttpRequest(void);

		static void					isValid(const HttpRequest& request);

};
