#include "HttpLibResponseBuilderService.h"

#include "WebServerAdapterInterface/Model/Reply.h"

#include <httplib/httplib.h>


namespace systelab { namespace web_server { namespace httplib {

	HttpLibResponseBuilderService::HttpLibResponseBuilderService() = default;
	HttpLibResponseBuilderService::~HttpLibResponseBuilderService() = default;

	void HttpLibResponseBuilderService::build(const Reply& reply, ::httplib::Response& response) const
	{
		response.status = static_cast<int>(reply.getStatus());
		response.body = reply.getContent();

		for (const auto& header : reply.getHeaders())
		{
			response.set_header(header.first.c_str(), header.second.c_str());
		}
	}

}}}
