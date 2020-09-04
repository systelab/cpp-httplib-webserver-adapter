#include "HttpLibRequestParserService.h"

#include "RequestURIParserService.h"

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
		request->setURI(httpLibRequest.target);
		request->setContent(httpLibRequest.body);

		for (const auto& header : httpLibRequest.headers)
		{
			request->getHeaders().addHeader(header.first, header.second);
		}

		RequestURIParserService uriParserService;
		uriParserService.parse(*request);

		return request;
	}

}}}
