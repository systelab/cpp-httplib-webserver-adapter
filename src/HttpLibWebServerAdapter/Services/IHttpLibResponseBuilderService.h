#pragma once

#include <string>
#include <vector>


namespace httplib {
	struct Response;
}

namespace systelab { namespace web_server {
	class Reply;
}}

namespace systelab { namespace web_server { namespace httplib {

	class IHttpLibResponseBuilderService
	{
	public:
		virtual ~IHttpLibResponseBuilderService() = default;

		virtual void build(const Reply&, ::httplib::Response&) const = 0;
	};

}}}

