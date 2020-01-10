#pragma once

#include "IReplyCORSHeadersBuilderService.h"

#include <set>
#include <string>


namespace systelab { namespace web_server {
	class CORSConfiguration;
}}

namespace systelab { namespace web_server { namespace httplib {

	class ReplyCORSHeadersBuilderService : public IReplyCORSHeadersBuilderService
	{
	public:
		ReplyCORSHeadersBuilderService(const CORSConfiguration&);
		virtual ~ReplyCORSHeadersBuilderService();

		void addCORSHeaders(const Request&, Reply&) const;

	private:
		std::string getCommaSeparatedList(const std::set<std::string>& items) const;

	private:
		const CORSConfiguration& m_corsConfiguration;
	};

}}}
