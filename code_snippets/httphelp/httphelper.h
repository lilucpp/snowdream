#pragma once

#include <string>
#include <memory>
#include <regex>
#include <mutex>
#include <array>
#include <future>
#include <map>
#include <thread>
#include <assert.h>
#include <curl/curl.h>
#include "threadpool.h"
#include "singleton.h"


namespace httphelp {
	struct CurlHolder;
	class Response;
	class async;
	class GlobalThreadPool;

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

		size_t writeFunction(char* ptr, size_t size, size_t nmemb, std::string* data) {
			size *= nmemb;
			data->append(ptr, size);
			return size;
		}

		class GlobalThreadPool : public ThreadPool {
			CPR_SINGLETON_DECL(GlobalThreadPool)
		protected:
			GlobalThreadPool() = default;

		public:
			~GlobalThreadPool() override = default;
		};

		// NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
		CPR_SINGLETON_IMPL(GlobalThreadPool)


			/**
				* Return a future, calling future.get() will wait task done and return RetType.
				* async(fn, args...)
				* async(std::bind(&Class::mem_fn, &obj))
				* async(std::mem_fn(&Class::mem_fn, &obj))
				**/
			template <class Fn, class... Args>
		auto async(Fn&& fn, Args&&... args) {
			return GlobalThreadPool::GetInstance()->Submit(std::forward<Fn>(fn), std::forward<Args>(args)...);
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

	const Timeout DEFAULT_TIMEOUT = 180;

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
		struct curl_slist* chunk{ nullptr };
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
		std::shared_ptr<CurlHolder> curl_{ nullptr };

	public:
		long status_code{};
		std::string text{};
		std::string url{};
		std::string error{};
		double elapsed{};
		http_off_t uploaded_bytes{};
		http_off_t downloaded_bytes{};

		Response() = default;
		Response(std::shared_ptr<CurlHolder> curl, const std::string& text, std::string&& error);
		Response(const Response& other) = default;
		Response(Response&& old) noexcept = default;
		~Response() noexcept = default;

		Response& operator=(Response&& old) noexcept = default;
		Response& operator=(const Response& other) = default;
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
		void SetTimeout(const Timeout& second);

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
		std::string response_string_;

		void PrepareGet();
		void PreparePost();
		void PreparePut();
		void prepareCommon();
		void SetHeaderInternal();
		Response Complete(CURLcode curl_error);
		Response makeRequest();
	};

	inline Session::Session() : curl_(new CurlHolder()), timeout_(DEFAULT_TIMEOUT) {
		curl_easy_setopt(curl_->handle, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl_->handle, CURLOPT_ERRORBUFFER, curl_->error.data());

#if LIBCURL_VERSION_NUM >= 0x071900
		curl_easy_setopt(curl_->handle, CURLOPT_TCP_KEEPALIVE, 1L);
#endif
	}

	inline void Session::SetUrl(const Url& url) {
		url_ = url;
	}

	inline void Session::SetUrl(Url&& url) {
		url_ = std::move(url);
	}

	inline void Session::SetParams(const Params& params) {
		params_ = params;
	}

	inline void Session::SetParams(Params&& params) {
		params_ = std::move(params);
	}

	inline void Session::SetHeaders(const Headers& header) {
		headers_ = header;
	}

	inline void Session::SetHeaders(Headers&& header) {
		headers_ = std::move(header);
	}

	inline void Session::SetBody(const Body& body) {
		body_ = body;
	}

	inline void Session::SetBody(Body&& body) {
		body_ = std::move(body);
	}

	inline void Session::SetTimeout(const Timeout& second) {
		timeout_ = second;
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

		// set Header:
		SetHeaderInternal();

		// set Url
		if (!params_.empty()) {
			Url new_url = url_ + "?";// +detail::GetContent(params_, curl_);
			std::string content{};
			for (const auto& e : params_) {
				if (!content.empty()) {
					content += "&";
				}
				std::string str = e.second;
				if (str.empty())
					content += e.first;
				else
					content += e.first + "=" + curl_->urlEncode(str);
			}
			new_url += content;

			curl_easy_setopt(curl_->handle, CURLOPT_URL, new_url.c_str());
		}
		else {
			curl_easy_setopt(curl_->handle, CURLOPT_URL, url_.c_str());
		}

		curl_easy_setopt(curl_->handle, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_->handle, CURLOPT_DEFAULT_PROTOCOL, "https");

		// set Timeout
		curl_easy_setopt(curl_->handle, CURLOPT_TIMEOUT, timeout_);
		/* complete connection within 20 seconds */
		curl_easy_setopt(curl_->handle, CURLOPT_CONNECTTIMEOUT, 20L);

		// set Body
		curl_easy_setopt(curl_->handle, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(body_.size()));
		curl_easy_setopt(curl_->handle, CURLOPT_POSTFIELDS, body_.c_str());

		curl_easy_setopt(curl_->handle, CURLOPT_WRITEFUNCTION, detail::writeFunction);
		curl_easy_setopt(curl_->handle, CURLOPT_WRITEDATA, &response_string_);
	}

	inline void Session::SetHeaderInternal() {
		curl_slist* chunk = nullptr;

		std::string content{};
		for (const auto& e : headers_) {
			std::string header_string = e.first;
			if (e.second.empty()) {
				header_string += ";";
			}
			else {
				header_string += ": " + e.second;
			}
			curl_slist* temp = curl_slist_append(chunk, header_string.c_str());
			if (temp) {
				chunk = temp;
			}
		}

		if (headers_.find("Content-Type") == headers_.end()) {
			curl_slist* temp = curl_slist_append(chunk, "Content-Type: text/plain");
			if (temp) {
				chunk = temp;
			}
		}

		// libcurl would prepare the header "Expect: 100-continue" by default when uploading files larger than 1 MB.
		// Here we would like to disable this feature:
		curl_slist* temp = curl_slist_append(chunk, "Expect:");
		if (temp) {
			chunk = temp;
		}

		curl_easy_setopt(curl_->handle, CURLOPT_HTTPHEADER, chunk);

		curl_slist_free_all(curl_->chunk);
		curl_->chunk = chunk;
	}

	inline Response Session::Complete(CURLcode curl_error) {
		std::string errorMsg = curl_->error.data();
		return Response(curl_, std::move(response_string_), std::move(errorMsg));
	}

	inline Response Session::makeRequest() {
		CURLcode curl_error = curl_easy_perform(curl_->handle);;
		return Complete(curl_error);
	}

	inline Response Session::Get() {
		PrepareGet();
		return makeRequest();
	}

	inline Response Session::Post() {
		PreparePost();
		return makeRequest();
	}

	inline Response Session::Put() {
		PreparePut();
		return makeRequest();
	}

	inline AsyncResponse Session::GetAsync() {
		auto shared_this = shared_from_this();
		return detail::async([shared_this]() { return shared_this->Get(); });
	}

	inline AsyncResponse Session::PostAsync() {
		auto shared_this = shared_from_this();
		return detail::async([shared_this]() { return shared_this->Post(); });
	}
	inline AsyncResponse Session::PutAsync() {
		auto shared_this = shared_from_this();
		return detail::async([shared_this]() { return shared_this->Put(); });
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
		curl_slist_free_all(chunk);
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

	inline Response::Response(std::shared_ptr<CurlHolder> curl, const std::string& text, std::string&& error) : curl_(std::move(curl)),
		text(std::move(text)), error(std::move(error)) {
		assert(curl_);
		assert(curl_->handle);

		curl_easy_getinfo(curl_->handle, CURLINFO_RESPONSE_CODE, &status_code);
		curl_easy_getinfo(curl_->handle, CURLINFO_TOTAL_TIME, &elapsed);
		char* url_string{ nullptr };
		curl_easy_getinfo(curl_->handle, CURLINFO_EFFECTIVE_URL, &url_string);
		if (url_string)
			url = std::string(url_string);
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

	inline Response Get(const std::string& path, const Params& params, const Headers& headers, const Timeout& timeout) {
		Session s;
		s.SetUrl(path);
		s.SetParams(params);
		s.SetHeaders(headers);
		s.SetTimeout(timeout);

		return s.Get();
	}

	inline Response Get(const std::string& path, const Params& params, const Headers& headers) {
		return Get(path, params, headers, DEFAULT_TIMEOUT);
	}

	inline AsyncResponse GetAsync(const std::string& path, const Params& params, const Headers& headers) {
		return detail::async([=]() {return Get(path, params, headers); });
	}

	template <typename Then>
	inline auto GetCallback(Then then, const std::string& path, const Params& params, const Headers& headers) {
		return detail::async([=](Then then_inner) { return then_inner(Get(path, params, headers)); }, std::move(then));
	}

	inline Response Post(const std::string& path, const Params& params, const Headers& headers, const Body& body, const Timeout& timeout) {
		Session s;
		s.SetUrl(path);
		s.SetParams(params);
		s.SetHeaders(headers);
		s.SetBody(body);
		s.SetTimeout(timeout);
		return s.Post();
	}

	inline Response Post(const std::string& path, const Params& params, const Headers& headers, const Body& body) {
		return Post(path, params, headers, body, DEFAULT_TIMEOUT);
	}

	inline AsyncResponse PostAsync(const std::string& path, const Params& params, const Headers& headers, const Body& body) {
		return detail::async([=]() { return Post(path, params, headers, body); });
	}

	template <typename Then>
	inline auto PostCallback(Then then, const std::string& path, const Params& params, const Headers& headers, const Body& body) {
		return detail::async([=](Then then_inner) { return then_inner(Post(path, params, headers, body)); }, std::move(then));
	}

	inline Response Put(const std::string& path, const Params& params, const Headers& headers, const Body& body, const Timeout& timeout) {
		Session s;
		s.SetUrl(path);
		s.SetParams(params);
		s.SetHeaders(headers);
		s.SetBody(body);
		s.SetTimeout(timeout);

		return s.Put();
	}

	inline Response Put(const std::string& path, const Params& params, const Headers& headers, const Body& body) {
		return Put(path, params, headers, body, DEFAULT_TIMEOUT);
	}

	inline AsyncResponse PutAsync(const std::string& path, const Params& params, const Headers& headers, const Body& body) {
		return detail::async([=]() { return Put(path, params, headers, body); });
	}

	template <typename Then>
	inline auto PutCallback(Then then, const std::string& path, const Params& params, const Headers& headers, const Body& body) {
		return detail::async([=](Then then_inner) { return then_inner(Put(path, params, headers, body)); }, std::move(then));
	}

}  // namespace httphelp