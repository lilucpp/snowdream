/*
cpp-httplib(重载、inline)、cpr（response， request, session, sync, async, callbase,timeout)
postman:
	params(url?key(encode)=val(encode))
	header: endcode(ken), encode(value)
	body:
	url=scheme+host:port/path?query str
*/
#pragma once
#include <string>
#include <memory>
#include <regex>
#include <mutex>
#include <array>
#include <thread>
#include <curl/curl.h>


namespace httphelp {
	class CurlHolder;
	class Response;

	namespace detail {
		struct ci {
			bool operator()(const std::string& s1, const std::string& s2) const {
				return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(),
					s2.end(),
					[](unsigned char c1, unsigned char c2) {
						return ::tolower(c1) < ::tolower(c2);
					});
			}
		};

		template<typename T>
		const std::string GetContent(const T& container, std::shared_ptr<CurlHolder> curl) {
			std::string content{};
			for (const auto& e : container) {
				if (!content.empty()) {
					content += "&";
				}
				content += e.first + "=" + curl->urlEncode(e.second);
			}

			return content;
		}
	}

	using http_off_t = curl_off_t;
	using AsyncResponse = std::future<Response>;
	using Url = std::string;
	using Params = std::multimap<std::string, std::string>;
	using Headers = std::multimap<std::string, std::string, detail::ci>;
	using Body = std::string;
	using Match = std::smatch;
	using Timeout = long;

	struct CurlHolder {
	private:
		/**
		 * Mutex for curl_easy_init().
		 * curl_easy_init() is not thread save.
		 * References:
		 * https://curl.haxx.se/libcurl/c/curl_easy_init.html
		 * https://curl.haxx.se/libcurl/c/threadsafe.html
		 **/

		 // Avoids initalization order problems in a static build
		static std::mutex& curl_easy_init_mutex_() {
			static std::mutex curl_easy_init_mutex_;
			return curl_easy_init_mutex_;
		}

	public:
		CURL* handle{ nullptr };
		std::array<char, CURL_ERROR_SIZE> error{};

		CurlHolder();
		CurlHolder(const CurlHolder& other) = default;
		CurlHolder(CurlHolder&& old) noexcept = default;
		~CurlHolder();

		CurlHolder& operator=(CurlHolder&& old) noexcept = default;
		CurlHolder& operator=(const CurlHolder& other) = default;

		/**
		 * Uses curl_easy_escape(...) for escaping the given string.
		 **/
		std::string urlEncode(const std::string& s) const;

		/**
		 * Uses curl_easy_unescape(...) for unescaping the given string.
		 **/
		std::string urlDecode(const std::string& s) const;
	};

	class Response {
	private:
		std::shared_ptr<CurlHolder> curl_{nullptr};

	public:
		long status_code{};
		std::string text{};
		std::string url{};
		std::string error{};
		double elapsed{};
		http_off_t uploaded_bytes{};
		http_off_t downloaded_bytes{};

		Response() = default;
		Response(std::shared_ptr<CurlHolder> curl curl);
		Response(const Response& other) = default;
		Response(Response&& old) noexcept = default;
		~Response() noexcept = default;

		Response& operator=(Response&& old) noexcept = default;
		Response& operator=(const Response& other) = default;
	};

	class Client : public std::enable_shared_from_this <Client> {
	public:
		// Universal interface
		explicit Client(const std::string& scheme_host_port);
		// HTTP only interface
		explicit Client(const std::string& host, int port);
		Client(const Client& other) = delete;

		Client(Client&&) = default;
		Client& operator=(const Client& other) = delete;

		~Client();

		Response Get(const std::string& path);
		Response Get(const std::string& path, const Headers& headers);
		Response Get(const std::string& path, const Params& params, const Headers& headers);

		Response Post(const std::string& path);
		Response Post(const std::string& path, const std::string& body);
		Response Post(const std::string& path, const Params& params);
		Response Post(const std::string& path, const Headers& headers);
		Response Post(const std::string& path, const Params& params, const Headers& headers);
		Response Post(const std::string& path, const Params& params, const std::string& body);
		Response Post(const std::string& path, const Headers& headers, const std::string& body);
		Response Post(const std::string& path, const Params& params, const Headers& headers, const std::string& body);

		Response Put(const std::string& path);
		Response Put(const std::string& path, const std::string& body);
		Response Put(const std::string& path, const Params& params);
		Response Put(const std::string& path, const Headers& headers);
		Response Put(const std::string& path, const Params& params, const Headers& headers);
		Response Put(const std::string& path, const Params& params, const std::string& body);
		Response Put(const std::string& path, const Headers& headers, const std::string& body);
		Response Put(const std::string& path, const Params& params, const Headers& headers, const std::string& body);



	private:
		Response MakeRequest();
		void PrepareCommon(const std::string& method, const std::string& path, const Params& params);
		void PrepareData(const Headers& headers, const std::string& body);
		void SetTimeout(long timeout);

	private:
		std::string scheme_host_port_;
		std::string scheme_;
		std::string host_;
		std::string port_;
		std::shared_ptr<CurlHolder> curl_;
	};

	class Session : public std::enable_shared_from_this<Session> {
	public:
		Session();
		Session(const Session& other) = delete;
		Session(Session&& old) = default;

		~Session() = default;

		Session& operator=(Session&& old) noexcept = default;
		Session& operator=(const Session& other) = delete;

		void SetUrl(const Url& url);
		void SetUrl(Url&& url);
		void SetParams(const Params& params);
		void SetParams(Params&& params);
		void SetHeaders(const Headers& header);
		void SetHeaders(Headers&& header);
		void SetBody(const Body& body);
		void SetBody(Body&& body);
		void SetTimeout(const Timeout& timeout);

		Response Get();
		Response Post();
		Response Put();

		AsyncResponse GetAsync();
		AsyncResponse PostAsync();
		AsyncResponse PutAsync();


	private:
		std::shared_ptr<CurlHolder> curl_;
    	Url url_;
		Params params_;
		Headers headers_;
		Body body_;
		Timeout timeout_;

		void PrepareGet();
		void PreparePost();
		void PreparePut();
		void prepareCommon();
		void SetHeaderInternal();
		Response Complete(CURLcode curl_error);
		Response makeRequest();
	};

	inline void Session::SetUrl(const Url& url) {
		url_=url;
	}

	inline void Session::SetUrl(Url&& url) {
		url_=std::move(url);
	}

	inline void Session::SetParams(const Params& params) {
		params_=params;
	}

	inline void Session::SetParams(Params&& params) {
		params_=std::move(params);
	}

	inline void Session::SetHeaders(const Headers& header) {
		headers_=header;
	}

	inline void Session::SetHeaders(Headers&& header) {
		headers_=std::move(header);
	}

	inline void Session::SetBody(const Body& body) {
		body_=body;
	}

	inline void Session::SetBody(Body&& body) {
		body_=std::move(body);
	}

	inline void Session::SetTimeout(const Timeout& timeout) {
		timeout_=timeout;
	}

	inline void Session::PrepareGet() {
		curl_easy_setopt(curl_->handle, CURLOPT_CUSTOMREQUEST, "GET");
		prepareCommon();
	}

	inline void Session::PreparePost() {
		curl_easy_setopt(curl_->handle, CURLOPT_CUSTOMREQUEST, "POST");
		prepareCommon();
	}

	inline void Session::PreparePut() {
		curl_easy_setopt(curl_->handle, CURLOPT_CUSTOMREQUEST, "PUT");
		prepareCommon();
	}

	inline void Session::prepareCommon() {
		assert(curl_->handle);

		// Set Header:
    	SetHeaderInternal();

		// Set params

		// Set body


	}



	inline CurlHolder::CurlHolder() {
		/**
		 * Allow multithreaded access to CPR by locking curl_easy_init().
		 * curl_easy_init() is not thread safe.
		 * References:
		 * https://curl.haxx.se/libcurl/c/curl_easy_init.html
		 * https://curl.haxx.se/libcurl/c/threadsafe.html
		 **/
		curl_easy_init_mutex_().lock();
		// NOLINTNEXTLINE (cppcoreguidelines-prefer-member-initializer) since we need it to happen inside the lock
		handle = curl_easy_init();
		curl_easy_init_mutex_().unlock();

		assert(handle);
	}

	inline CurlHolder::~CurlHolder() {
		curl_easy_cleanup(handle);
	}

	inline std::string CurlHolder::urlEncode(const std::string& s) const {
		assert(handle);
		char* output = curl_easy_escape(handle, s.c_str(), static_cast<int>(s.length()));
		if (output) {
			std::string result = output;
			curl_free(output);
			return result;
		}
		return "";
	}

	inline std::string CurlHolder::urlDecode(const std::string& s) const {
		assert(handle);
		char* output = curl_easy_unescape(handle, s.c_str(), static_cast<int>(s.length()), nullptr);
		if (output) {
			std::string result = output;
			curl_free(output);
			return result;
		}
		return "";
	}

	inline Response::Response(CurlHolder curl) : curl_(std::move(curl)) {
		assert(curl_);
    	assert(curl_->handle);

		curl_easy_getinfo(curl_->handle, CURLINFO_RESPONSE_CODE, &status_code);
    	curl_easy_getinfo(curl_->handle, CURLINFO_TOTAL_TIME, &elapsed);
		char* url_string{ nullptr };
		curl_easy_getinfo(curl_->handle, CURLINFO_EFFECTIVE_URL, &url_string);
		url = url_string;
#if LIBCURL_VERSION_NUM >= 0x073700
		curl_easy_getinfo(curl_->handle, CURLINFO_SIZE_DOWNLOAD_T, &downloaded_bytes);
   		curl_easy_getinfo(curl_->handle, CURLINFO_SIZE_UPLOAD_T, &uploaded_bytes);
#else
		double downloaded_bytes_double, uploaded_bytes_double;
		curl_easy_getinfo(curl_->handle, CURLINFO_SIZE_DOWNLOAD, &downloaded_bytes_double);
		curl_easy_getinfo(curl_->handle, CURLINFO_SIZE_UPLOAD, &uploaded_bytes_double);
		downloaded_bytes = downloaded_bytes_double;
		uploaded_bytes = uploaded_bytes_double;
#endif
	}

	// Universal client implementation
	inline Client::Client(const std::string& scheme_host_port) : curl_(new CurlHolder()) {
		const static std::regex re(
			R"((?:([a-z]+):\/\/)?(?:\[([\d:]+)\]|([^:/?#]+))(?::(\d+))?)");

		std::smatch m;
		if (std::regex_match(scheme_host_port, m, re)) {
			scheme_ = m[1].str();
			if (!scheme_.empty() && (scheme_ != "http" && scheme_ != "https")) {
				std::string msg = "'" + scheme_ + "' scheme is not supported.";
				throw std::invalid_argument(msg);
				return;
			}

			auto is_ssl = scheme_ == "https";

			host_ = m[2].str();
			if (host_.empty()) { host_ = m[3].str(); }

			auto port_str = m[4].str();
			port_ = !port_str.empty() ? std::stoi(port_str) : (is_ssl ? 443 : 80);
		}
		else {
			std::string msg = "'" + scheme_host_port + "' is invalid address.";
			throw std::invalid_argument(msg);
		}
	}
	inline Client::Client(const std::string& host, int port) : scheme_("http"), host_(host), port_(port_), curl_(new CurlHolder()) {
		scheme_host_port_ = scheme_ + "//" + host_ + ":" + port_;
	}
	inline Client::~Client() {}

	inline Response Client::Get(const std::string& path) {
		return Get(path, Params(), Headers());
	}

	inline Response Client::Get(const std::string& path, const Headers& headers) {
		return Get(path, Params(), headers);
	}

	inline Response Client::Get(const std::string& path, const Params& params, const Headers& headers) {
		PrepareCommon("GET", path, params);
		PrepareData(headers, std::string());

		return MakeRequest();
	}

	inline Response Client::MakeRequest() {
		// todo
		return Response();
	}

	inline void Client::PrepareCommon(const std::string& method, const std::string& path, const Params& params) {
		call_easy_curl_setopt(curl_->handle, CURLOPT_CUSTOMREQUEST, method.c_str());

		std::string url = scheme_host_port_ + path;
		if (!params.empty()) {
			url += "?";
			url += detail::GetContent(params, curl_);
		}

		//call_easy_curl_setopt(curl_->handle, CURLOPT_URL, serveraddr.c_str());
		call_easy_curl_setopt(curl_->handle, CURLOPT_FOLLOWLOCATION, 1L);
		call_easy_curl_setopt(curl_->handle, CURLOPT_DEFAULT_PROTOCOL, "https");
		/* complete within 180 seconds */
		call_easy_curl_setopt(curl_->handle, CURLOPT_TIMEOUT, 180L);
		/* complete connection within 20 seconds */
		call_easy_curl_setopt(curl_->handle, CURLOPT_CONNECTTIMEOUT, 20L);
	}




	// Get methods
	template <typename... Ts>
	Response Get(Ts&&... ts) {
		//Session session;
		//priv::set_option(session, std::forward<Ts>(ts)...);
		//return session.Get();
		return Response();
	}

}  // namespace httphelp