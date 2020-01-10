#pragma once


namespace systelab { namespace web_server {
	class Request;
}}

namespace systelab { namespace web_server { namespace httplib {

	class IRequestURIParserService
	{
	public:
		virtual ~IRequestURIParserService() = default;

		virtual bool parse(Request&) const = 0;
	};

}}}
