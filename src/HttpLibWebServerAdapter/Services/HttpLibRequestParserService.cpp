#include "HttpLibRequestParserService.h"

#include "WebServerAdapterInterface/Model/Request.h"

#include <httplib/httplib.h>


namespace systelab { namespace web_server { namespace httplib {

	HttpLibRequestParserService::HttpLibRequestParserService() = default;
	HttpLibRequestParserService::~HttpLibRequestParserService() = default;

	std::unique_ptr<Request> HttpLibRequestParserService::parse(const ::httplib::Request& httpLibRequest) const
	{
		auto request = std::make_unique<Request>();
		request->setHttpVersionMajor(1); // httpLibRequest.version
		request->setHttpVersionMinor(1);
		request->setMethod(httpLibRequest.method);
		request->setURI(httpLibRequest.path);
		request->setContent(httpLibRequest.body);

		for (auto& it = httpLibRequest.headers.begin(); it != httpLibRequest.headers.end(); ++it)
		{
			request->getHeaders().addHeader(it->first, it->second);
		}

		return request;
	}

}}}