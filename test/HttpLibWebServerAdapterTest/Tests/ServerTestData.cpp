#include "stdafx.h"
#include "ServerTestData.h"

#include "WebServerAdapterTestUtilities/Builders/RequestBuilder.h"
#include "WebServerAdapterTestUtilities/Builders/ReplyBuilder.h"


using namespace systelab::web_server::test_utility;

namespace systelab { namespace web_server { namespace httplib { namespace test {

	std::vector<ServerTestData> ServerTestDataBuilder::build()
	{
		return {
			{
				RequestBuilder()
					.setMethod("GET").setURI("/rest/api/health")
					.addHeader("Origin", "http://localhost:4200")
					.getEntity(),
				ReplyBuilder()
					.setStatus(Reply::StatusType::OK)
					.addHeader("Content-Type", "application/json")
					.setContent("{ \"status\": \"running\" }")
					.getEntity()
			},
			{
				RequestBuilder()
					.setMethod("POST").setURI("/rest/api/users/login")
					.addHeader("Content-Length", "25").addHeader("Content-Type", "text/plain")
					.setContent("Request content goes here")
					.getEntity(),
				ReplyBuilder()
					.setStatus(Reply::StatusType::CREATED)
					.addHeader("Content-Type", "text/plain").addHeader("Authorization", "Bearer 12345679012345689")
					.setContent("Reply goes here")
					.getEntity()
			},
			{
				RequestBuilder()
					.setMethod("PUT").setURI("/custom/url/here")
					.addHeader("Authorization", "Bearer xxx.yyy.zzz")
					.getEntity(),
				ReplyBuilder()
					.setStatus(Reply::StatusType::NOT_FOUND)
					.addHeader("Content-Type", "application/json").addHeader("Authorization", "Bearer 12345679012345689")
					.setContent("{}")
					.getEntity()
			},
			{
				RequestBuilder()
					.setMethod("PATCH").setURI("/rest/api/patch/method")
					.addHeader("Content-Type", "application/json")
					.setContent("{ \"aaa\": \"bbb\" }")
					.getEntity(),
				ReplyBuilder()
					.setStatus(Reply::StatusType::BAD_REQUEST)
					.addHeader("Content-Type", "application/json").addHeader("Authorization", "Bearer 12345679012345689")
					.setContent("{ \"ccc\": [1,2,3,4,5,6,7,8,9] }")
					.getEntity()
			},
			{
				RequestBuilder()
					.setMethod("DELETE")
					.setURI("/rest/api/users/john")
					.addHeader("CustomHeader", "CustomValue")
					.setContent("Request content goes here")
					.getEntity(),
				ReplyBuilder()
					.setStatus(Reply::StatusType::NO_CONTENT)
					.getEntity()
			},
			{
				RequestBuilder()
					.setMethod("OPTIONS")
					.setURI("/rest/api/users")
					.addHeader("Origin", "http://localhost:4200")
					.getEntity(),
				ReplyBuilder()
					.setStatus(Reply::StatusType::INTERNAL_SERVER_ERROR)
					.addHeader("Access-Control-Allow-Origin", "http://localhost:4200")
					.setContent("CORS reply")
					.getEntity()
			}
		};
	}

}}}}
