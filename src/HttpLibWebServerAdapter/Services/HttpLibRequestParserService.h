#pragma once

#include "IHttpLibRequestParserService.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace web_server { namespace httplib {

	class HttpLibRequestParserService : public IHttpLibRequestParserService
	{
	public:
		HttpLibRequestParserService();
		virtual ~HttpLibRequestParserService();

		std::unique_ptr<Request> parse(const ::httplib::Request&) const override;
	};

}}}
