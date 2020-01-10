#pragma once

#include <memory>


namespace systelab { namespace web_server {
	class Request;
	class Reply;
}}

namespace systelab { namespace web_server { namespace httplib {

	class IRequestHandlingService
	{
	public:
		virtual ~IRequestHandlingService() = default;

		virtual std::unique_ptr<Reply> processRequest(const Request&) const = 0;
	};

}}}

