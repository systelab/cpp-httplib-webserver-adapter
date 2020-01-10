#pragma once

#include "IRequestURIParserService.h"
#include <memory>


namespace systelab { namespace web_server { namespace httplib {

	class RequestURIParserService : public IRequestURIParserService
	{
	public:
		RequestURIParserService();
		virtual ~RequestURIParserService();

		bool parse(Request&) const;

	private:
		void parseQueryString(Request&) const;
	};

}}}

