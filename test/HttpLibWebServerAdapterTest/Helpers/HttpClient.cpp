#include "stdafx.h"
#include "HttpClient.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"

#include <httplib/httplib.h>
#include <iostream>


namespace systelab { namespace web_server { namespace httplib { namespace test {

	HttpClient::HttpClient() = default;
	HttpClient::~HttpClient() = default;

	std::unique_ptr<Reply> HttpClient::send(const std::string& serverAddress, unsigned int port, const Request& request) const
	{
		::httplib::Client client(serverAddress, port);
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
			throw std::runtime_error("Not implemented");
		}
		else if (request.getMethod() == "PATCH")
		{
			throw std::runtime_error("Not implemented");
		}
		else if (request.getMethod() == "DELETE")
		{
			throw std::runtime_error("Not implemented");
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

	void HttpClient::fillHttplibRequest(const Request& request, ::httplib::Request& httplibRequest) const
	{
		httplibRequest.method = request.getMethod();
		httplibRequest.path = request.getURI();
		httplibRequest.body = request.getContent();

		for (auto& header : request.getHeaders().getHeadersMap())
		{
			httplibRequest.set_header(header.first.c_str(), header.second.c_str());
		}
	}

	std::unique_ptr<Reply> HttpClient::buildReply(::httplib::Response httplibResponse) const
	{
		auto reply = std::make_unique<Reply>();
		reply->setStatus(static_cast<Reply::StatusType>(httplibResponse.status));
		reply->setContent(httplibResponse.body);

		for (auto& header : httplibResponse.headers)
		{
			reply->addHeader(header.first, header.second);
		}

		return reply;
	}

}}}}
