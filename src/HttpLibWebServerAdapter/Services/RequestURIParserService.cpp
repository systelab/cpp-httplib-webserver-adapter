#include "RequestURIParserService.h"

#include "WebServerAdapterInterface/Model/Request.h"

#include <regex>
#include <sstream>


namespace systelab { namespace web_server { namespace httplib {

	RequestURIParserService::RequestURIParserService()
	{
	}

	RequestURIParserService::~RequestURIParserService() = default;

	bool RequestURIParserService::parse(Request& request) const
	{
		std::string in = request.getURI();

		std::string out;
		out.reserve(in.size());

		for (std::size_t i = 0; i < in.size(); ++i)
		{
			if (in[i] == '%')
			{
				if (i + 3 <= in.size())
				{
					int value = 0;
					std::istringstream is(in.substr(i + 1, 2));

					if (is >> std::hex >> value)
					{
						out += static_cast<char>(value);
						i += 2;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else if (in[i] == '+')
			{
				out += ' ';
			}
			else
			{
				out += in[i];
			}
		}

		size_t posDotDot = out.find("..");
		if (posDotDot != std::string::npos)
		{
			size_t posQuery = out.find("?");
			if (posQuery == std::string::npos || posQuery > posDotDot)
			{
				return false;
			}
		}

		bool bad = (out.size() == 0) || (out[0] != '/');
		if (bad)
		{
			return false;
		}

		request.setURI(out);
		parseQueryString(request);

		return true;
	}

	void RequestURIParserService::parseQueryString(Request& request) const
	{
		std::string requestURI = request.getURI();
		if (requestURI.find("?") != std::string::npos)
		{
			std::string requestURIWithoutQueryStrings = requestURI.substr(0, requestURI.find("?"));
			request.setURI(requestURIWithoutQueryStrings);

			std::string queryStrings = requestURI.substr(requestURIWithoutQueryStrings.size());
			std::regex pattern("([\\w+%]+)=([^&]*)");
			auto words_begin = std::sregex_iterator(queryStrings.begin(), queryStrings.end(), pattern);
			auto words_end = std::sregex_iterator();

			for (std::sregex_iterator i = words_begin; i != words_end; i++)
			{
				std::string name = (*i)[1].str();
				std::string value = (*i)[2].str();
				request.getQueryStrings().addItem(name, value);
			}
		}
	}

}}}
