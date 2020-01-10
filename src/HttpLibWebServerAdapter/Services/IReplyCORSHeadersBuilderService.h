#pragma once


namespace systelab { namespace web_server {
	class Reply;
	class Request;
}}

namespace systelab { namespace web_server { namespace httplib {

	class IReplyCORSHeadersBuilderService
	{
	public:
		virtual ~IReplyCORSHeadersBuilderService() = default;

		virtual void addCORSHeaders(const Request&, Reply&) const = 0;
	};

}}}

