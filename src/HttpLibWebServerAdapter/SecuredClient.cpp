#include "SecuredClient.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"

#include <httplib/httplib.h>
#include <iostream>


namespace systelab { namespace web_server { namespace httplib {

	SecuredClient::SecuredClient(const std::string& serverAddress, unsigned int port)
		:Client(serverAddress, port)
	{
	}

	SecuredClient::~SecuredClient() = default;

	std::unique_ptr<Reply> SecuredClient::send(const Request& request) const
	{
		::httplib::SSLClient client(m_serverAddress, m_port);
		client.set_timeout_sec(600);
		client.set_read_timeout(600, 1);

		::httplib::Headers httpLibHeaders;
		for (auto& header : request.getHeaders().getHeadersMap())
		{
			if (header.first != "Content-Type")
			{
				httpLibHeaders.emplace(header.first.c_str(), header.second.c_str());
			}
		}

		std::shared_ptr<::httplib::Response> httpLibResponse;
		if (request.getMethod() == "GET")
		{
			httpLibResponse = client.Get(request.getURI().c_str(), httpLibHeaders);
		}
		else if (request.getMethod() == "POST")
		{
			std::string contentType = request.getHeaders().hasHeader("Content-Type") ? request.getHeaders().getHeader("Content-Type") : "text/plain";
			httpLibResponse = client.Post(request.getURI().c_str(), httpLibHeaders, request.getContent(), contentType.c_str());
		}
		else if (request.getMethod() == "PUT")
		{
			std::string contentType = request.getHeaders().hasHeader("Content-Type") ? request.getHeaders().getHeader("Content-Type") : "text/plain";
			httpLibResponse = client.Put(request.getURI().c_str(), httpLibHeaders, request.getContent(), contentType.c_str());
		}
		else if (request.getMethod() == "PATCH")
		{
			std::string contentType = request.getHeaders().hasHeader("Content-Type") ? request.getHeaders().getHeader("Content-Type") : "text/plain";
			httpLibResponse = client.Patch(request.getURI().c_str(), httpLibHeaders, request.getContent(), contentType.c_str());
		}
		else if (request.getMethod() == "DELETE")
		{
			std::string contentType = request.getHeaders().hasHeader("Content-Type") ? request.getHeaders().getHeader("Content-Type") : "text/plain";
			httpLibResponse = client.Delete(request.getURI().c_str(), httpLibHeaders, request.getContent(), contentType.c_str());
		}
		else if (request.getMethod() == "OPTIONS")
		{
			std::string contentType = request.getHeaders().hasHeader("Content-Type") ? request.getHeaders().getHeader("Content-Type") : "text/plain";
			httpLibResponse = client.Options(request.getURI().c_str(), httpLibHeaders);
		}
		else
		{
			throw std::runtime_error("Unsupported method");
		}

		if (!httpLibResponse)
		{
			return nullptr;
		}

		return buildReply(*httpLibResponse);
	}

}}}
