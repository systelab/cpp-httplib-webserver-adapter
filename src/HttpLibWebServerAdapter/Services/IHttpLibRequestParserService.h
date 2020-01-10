#pragma once

#include <memory>
#include <vector>


namespace httplib {
	struct Request;
}

namespace systelab { namespace web_server {
	class Request;
}}

namespace systelab { namespace web_server { namespace httplib {

	class IHttpLibRequestParserService
	{
	public:
		virtual ~IHttpLibRequestParserService() = default;

		virtual std::unique_ptr<Request> parse(const ::httplib::Request&) const = 0;
	};

}}}

