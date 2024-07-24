// httphelp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "httphelper.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace httphelp;

uint16_t GetPort() {
	// Unassigned port number in the ephemeral range
	return 1984;
}

std::string GetBaseUrl() {
	return "http://127.0.0.1:" + std::to_string(GetPort());
}

TEST(BasicTests, HelloWorldTest) {
	auto url = Url{ GetBaseUrl() + "/resource" };
	auto response = Get(url, Params(), Headers());
	auto expected_text = std::string{ "Hello, World!" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(200, response.status_code);
}

TEST(BasicTests, TimeoutTest) {
	auto url = Url{ GetBaseUrl() + "/resource" };
	auto response = Get(url, Params(), Headers(), Timeout{ 1 });
	auto expected_text = std::string{ "Hello, World!" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(200, response.status_code);
}

TEST(BasicTests, BasicJsonTest) {
	auto url = Url{ GetBaseUrl() + "/basic.json" };
	auto response = Get(url, Params(), Headers());
	auto expected_text = std::string{
			"[\n"
			"  {\n"
			"    \"first_key\": \"first_value\",\n"
			"    \"second_key\": \"second_value\"\n"
			"  }\n"
			"]" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(200, response.status_code);
}

TEST(BasicTests, ResourceNotFoundTest) {
	auto url = Url{ GetBaseUrl() + "/error.html" };
	auto response = Get(url, Params(), Headers());
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(404, response.status_code);
}

TEST(BasicTests, BadHostTest) {
	auto url = Url{ "http://bad_host/" };
	auto response = Get(url, Params(), Headers());
	EXPECT_EQ(std::string{}, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(0, response.status_code);
}

TEST(ParameterTests, SingleParameterTest) {
	auto url = Url{ GetBaseUrl() + "/resource" };
	auto parameters = Params{ {"key", "value"} };
	auto response = Get(url, parameters, Headers());
	auto expected_text = std::string{ "Hello, World!" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(Url{ url + "?key=value" }, response.url);
	EXPECT_EQ(200, response.status_code);
}

TEST(ParameterTests, SingleParameterOnlyKeyTest) {
	auto url = Url{ GetBaseUrl() + "/hello.html" };
	auto parameters = Params{ {"key", ""} };
	auto response = Get(url, parameters, Headers());
	auto expected_text = std::string{ "Hello world!" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(Url{ url + "?key" }, response.url);
	EXPECT_EQ(200, response.status_code);
}

TEST(ParameterTests, MultipleParametersTest) {
	auto url = Url{ GetBaseUrl() + "/hello.html" };
	auto response =
		Get(url, Params{ {"key", "value"}, {"hello", "world"}, {"test", "case"} }, Headers());
	auto expected_text = std::string{ "Hello world!" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(Url{ url + "?hello=world&key=value&test=case" }, response.url);
	EXPECT_EQ(200, response.status_code);
}

TEST(HeaderTests, HeaderReflectMultipleTest) {
	auto url = Url{ GetBaseUrl() + "/header_reflect.html" };
	auto response = Get(url, Params{}, Headers{ {"hello", "world"}, {"key", "value"}, {"test", "case"} });
	auto expected_text = std::string{ "Hello world!" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(200, response.status_code);
}


TEST(UrlEncodedPostTests, AsyncGetTest) {
	auto url = Url{ GetBaseUrl() + "/hello.html" };
	auto future = GetAsync(url, Params(), Headers());
	auto expected_text = std::string{ "Hello world!" };
	auto response = future.get();
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(200, response.status_code);
}

std::chrono::milliseconds sleep_time{ 50 };
std::chrono::seconds zero{ 0 };
TEST(CallbackGetTests, CallbackGetLambdaStatusTest) {
	Url url{ GetBaseUrl() + "/hello.html" };
	int status_code = 0;
	auto future = GetCallback(
		[&status_code](Response r) {
			status_code = r.status_code;
			return r.status_code;
		},
		url, Params(), Headers());
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(status_code, future.get());
}

TEST(CallbackGetTests, CallbackGetLambdaTextTest) {
	Url url{ GetBaseUrl() + "/hello.html" };
	std::string expected_text{};
	auto future = GetCallback(
		[&expected_text](Response r) {
			expected_text = r.text;
			return r.text;
		},
		url, Params(), Headers());
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(expected_text, future.get());
}

int status_callback(int& status_code, Response r) {
	status_code = r.status_code;
	return r.status_code;
}

int status_callback_ref(int& status_code, const Response& r) {
	status_code = r.status_code;
	return r.status_code;
}

std::string text_callback(std::string& expected_text, Response r) {
	expected_text = r.text;
	return r.text;
}

std::string text_callback_ref(std::string& expected_text, const Response& r) {
	expected_text = r.text;
	return r.text;
}

TEST(CallbackGetTests, CallbackGetFunctionStatusTest) {
	Url url{ GetBaseUrl() + "/hello.html" };
	int status_code = 0;
	auto callback = std::function<int(Response)>(std::bind(status_callback, std::ref(status_code), std::placeholders::_1));
	auto future = GetCallback(callback, url, Params(), Headers());
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(status_code, future.get());
}

TEST(CallbackGetTests, CallbackGetFunctionTextTest) {
	Url url{ GetBaseUrl() + "/hello.html" };
	std::string expected_text{};
	auto callback = std::function<std::string(Response)>(std::bind(text_callback, std::ref(expected_text), std::placeholders::_1));
	auto future = GetCallback(callback, url, Params(), Headers());
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(expected_text, future.get());
}

TEST(CallbackGetTests, CallbackGetFunctionStatusReferenceTest) {
	Url url{ GetBaseUrl() + "/hello.html" };
	int status_code = 0;
	auto callback = std::function<int(Response)>(std::bind(status_callback_ref, std::ref(status_code), std::placeholders::_1));
	auto future = GetCallback(callback, url, Params(), Headers());
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(status_code, future.get());
}

TEST(CallbackGetTests, CallbackGetFunctionTextReferenceTest) {
	Url url{ GetBaseUrl() + "/hello.html" };
	std::string expected_text{};
	auto callback = std::function<std::string(Response)>(std::bind(text_callback_ref, std::ref(expected_text), std::placeholders::_1));
	auto future = GetCallback(callback, url, Params(), Headers());
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(expected_text, future.get());
}


TEST(UrlEncodedPostTests, TimeoutPostTest) {
	Url url{ GetBaseUrl() + "/json_post.html" };
	std::string body{ R"({"RegisterObject": {"DeviceID": "65010000005030000001"}})" };
	Response response = Post(url, Params(), Headers{ {"Content-Type", "application/json"} }, Body{ body }, Timeout{ 3 });
	std::string expected_text{ R"({"RegisterObject": {"DeviceID": "65010000005030000001"}})" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(201, response.status_code);
}

TEST(CallbackPostTests, CallbackPostLambdaStatusTest) {
	Url url{ GetBaseUrl() + "/json_post.html" };
	Body body("{ \"x\", \"5\" }");
	int status_code = 0;
	auto future = PostCallback(
		[&status_code](Response r) {
			status_code = r.status_code;
			return r.status_code;
		},
		url, Params(), Headers(), body);
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(status_code, future.get());
}

TEST(CallbackPostTests, CallbackPostLambdaTextTest) {
	Url url{ GetBaseUrl() + "/json_post.html" };
	Body body("{ \"x\", \"5\" }");
	std::string expected_text{};
	auto future = PostCallback(
		[&expected_text](Response r) {
			expected_text = r.text;
			return r.text;
		},
		url, Params(), Headers(), body);
	std::this_thread::sleep_for(sleep_time);
	EXPECT_EQ(future.wait_for(zero), std::future_status::ready);
	EXPECT_EQ(expected_text, future.get());
}

TEST(PutTests, PutTest) {
	Url url{ GetBaseUrl() + "/put.html" };
	Body body("{ \"x\": \"5\" }");
	Response response = Put(url, Params(), Headers(), body);
	std::string expected_text{ "{ \"x\": \"5\" }" };
	EXPECT_EQ(expected_text, response.text);
	EXPECT_EQ(url, response.url);
	EXPECT_EQ(200, response.status_code);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


