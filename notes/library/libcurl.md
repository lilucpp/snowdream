# cURL

## [URI](https://zh.wikipedia.org/wiki/%E7%BB%9F%E4%B8%80%E8%B5%84%E6%BA%90%E6%A0%87%E5%BF%97%E7%AC%A6)
统一资源标识符（英语：Uniform Resource Identifier，缩写：URI）在电脑术语中是一个用于标识某一互联网资源名称的字符串。
该种标识允许用户对网络中（一般指万维网）的资源通过特定的协议进行交互操作。URI的最常见的形式是统一资源定位符（URL），经常指定为非正式的网址。更罕见的用法是统一资源名称（URN），其目的是通过提供一种途径。用于在特定的名字空间资源的标识，以补充网址。
### 文法
URI文法由URI协议名（例如http、ftp、mailto、file），一个冒号，和协议对应的内容所构成。特定的协议定义了协议内容的语法和语义，而所有的协议都必须遵循一定的URI文法通用规则，亦即为某些专门目的保留部分特殊字符。URI文法同时也就各种原因对协议内容加以其他的限制，例如，保证各种分层协议之间的协同性。百分号编码也为URI提供附加信息。

通用URI的格式如下：
[协议名]://用户名:密码@主机名:端口/路径?查询参数#片段ID

下图展示了两个 URI 例子及它们的组成部分。
```
                    hierarchical part
        ┌───────────────────┴─────────────────────┐
                    authority               path
        ┌───────────────┴───────────────┐┌───┴────┐
  abc://username:password@example.com:123/path/data?key=value&key2=value2#fragid1
  └┬┘   └───────┬───────┘ └────┬────┘ └┬┘           └─────────┬─────────┘ └──┬──┘
scheme  user information     host     port                  query         fragment

  urn:example:mammal:monotreme:echidna
  └┬┘ └──────────────┬───────────────┘
scheme              path
```
## [URL](https://zh.wikipedia.org/wiki/%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81)

百分号编码（英语：Percent-encoding），又称：URL编码（URL encoding）是特定上下文的统一资源定位符 （URL）的编码机制，实际上也适用于统一资源标志符（URI）的编码。
URI的字符类型
URI所允许的字符分作保留与未保留。保留字符是那些具有特殊含义的字符，例如：斜线字符用于URL（或URI）不同部分的分界符；未保留字符没有这些特殊含义。百分号编码把保留字符表示为特殊字符序列。上述情形随URI与URI的不同版本规格会有轻微的变化。

RFC 3986 section 2.2 保留字符 (2005年1月)
```
!	*	'	(	)	;	:	@	&	=	+	$	,	/	?	#	[	]
```

RFC 3986 section 2.3 未保留字符 (2005年1月)
```
A	B	C	D	E	F	G	H	I	J	K	L	M	N	O	P	Q	R	S	T	U	V	W	X	Y	Z
a	b	c	d	e	f	g	h	i	j	k	l	m	n	o	p	q	r	s	t	u	v	w	x	y	z
0	1	2	3	4	5	6	7	8	9	-	_	.	~	
```
**URI中的其它字符必须用百分号编码。**

### 百分号编码
[code](../../code_snippets/url_encoder.h)
```c++
static inline void Encode(const std::string &in, std::string *out, bool uppercase = false) {
    std::stringstream ss;

    for (std::string::const_iterator it = in.begin(); it != in.end(); ++it) {
      if (kUnreservedChar[*it]) {
        ss << *it;
      } else {
        ss << '%' << std::setfill('0') << std::hex;
        if (uppercase) ss << std::uppercase;
        ss << (int)*it;
      }
    }

    out->assign(ss.str());
  }
```

## 其他
有意思的功能有：
1. posman中可以生成libcurl的代码
2. 命令行 后加 --libcurl 也可以生成代码。
```shell
curl http://example.com  --libcurl example.c
```
3. C++ wrapper around libcURL。 [http://www.curlpp.org/](http://www.curlpp.org/)

4. CURLOPT_SSL_VERFYPEER   将会决定curl是否会验证证书的有效性，0 表示不会验证，1 表示会验证。默认值为1。

5. CURLOPT_SSL_VERIFYHOST   

   ```txt
    设置这个选项，将会决定curl是否会验证证书的有效性，0 表示不会验证，1 表示会验证。默认值为1。
   
     CURLOPT_SSL_VERIFYHOST ：
   
   当建立 TLS 和 SSL 连接的时候，服务器会发一个证书来表明它的身份。
   
     值为0时，不管证书的上的名称，也不验证证书是否正确。
   
     值为1时，检查证书中是否包含名称。
   
     值为2的时候，就表示要求服务器返回的证书中包含的服务器名称，必须和curl请求Url上的域名是一样的，否则连接会失败。（有可能弹出上述错误： “SSL peer certificate or SSH remote key was not OK”）
   ```

6. 线程安全
  ```txt
  1. 初始化线程安全
    正常使用流程是先调用curl_global_init初始化资源，而这个函数不是线程安全的。curl_easy_init发现没有做初始化时会自动初始化，多个线程同时进行curl_easy_init时会导致异常。
    curl官方建议在主线程中先做初始化。curl_global_init(CURL_GLOBAL_ALL); 然后在合适的地方调用 curl_global_cleanup(); 
  2. ssl 线程安全
    需要加锁处理，详见示例。
  ```   

## 示例代码

http/https都支持，其中https不验证证书。

```c++
static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
```

1. get

   ```c++
   std::string RESTfulGet(RESTfulURL url, const std::string& data) {
     std::string ret;
   
     CURL *curl;
     CURLcode res;
     curl = curl_easy_init();
     if (curl) {
       std::string curlCheckParams = getServerIp() + getRESTfulURLString(url);
       if(url == RESTfulURL::heartbeat)
         curlCheckParams+=getServerSuffix();// default token
   
       curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
       curl_easy_setopt(curl, CURLOPT_URL, curlCheckParams.c_str());
       curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
       curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
       /* complete within 20 seconds */
       curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
       /* complete connection within 20 seconds */
       curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20L);
       curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   
       struct curl_slist *headers = NULL;
       std::string token("token: ");
       token += getConnectInfo().defaultToken;//access token
   
       headers = curl_slist_append(headers, token.c_str());
       headers = curl_slist_append(headers, "Content-Type: application/json");
       curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
   
       // result callback
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
       curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
   
       //data: body
       curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
       res = curl_easy_perform(curl);
       if (CURLE_OK != res) {
         LOG(WARNING) << "curl get error:" << res << ", url=" << curlCheckParams;
       }
     }
     curl_easy_cleanup(curl);
   
     return ret;
   }
   ```

   

2. post

   ```c++
   std::string RESTfulPost(RESTfulURL url, const std::string& data) {
     CURL *curl;
     CURLcode res;
     std::string ret;
     curl = curl_easy_init();
     if (curl) {
       std::string curlCheckParams = getServerIp() + getRESTfulURLString(url);
       if(url == RESTfulURL::updateDepartAndPerson || url == RESTfulURL::audit ||
          url == RESTfulURL::uninstall)
         curlCheckParams+=getServerSuffix();
   
       qDebug() << "post url:" << curlCheckParams.c_str();
   
       curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
       curl_easy_setopt(curl, CURLOPT_URL, curlCheckParams.c_str());
       curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
       curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
       /* complete within 20 seconds */
       curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
       /* complete connection within 20 seconds */
       curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20L);
   
       struct curl_slist *headers = NULL;
       std::string token("token: ");
       token += getConnectInfo().defaultToken;
       headers = curl_slist_append(headers, token.c_str());
       headers = curl_slist_append(headers, "Content-Type: application/json");
       //fix: failed when body >1024 
       headers = curl_slist_append(headers, "Expect:");
       curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
       curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   
       curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
   
       // result callback
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
       curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
   
       res = curl_easy_perform(curl);
       if (CURLE_OK != res) {
         LOG(WARNING) << "curl post error:" << res << ", url=" << curlCheckParams;
       }
   
     }
     curl_easy_cleanup(curl);
   
     return ret;
   }
   
   ```

3. download file

   ```c++
   static size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
      size_t written = fwrite(ptr, size, nmemb, stream);
      return written;
   }

   std::string downloadFile(const char* url, const char outfilename[FILENAME_MAX], const char* userpwd) {
     std::string ret;
   	CURL *curl;
   	FILE *fp;
   	CURLcode res;
   	curl_global_init(CURL_GLOBAL_ALL);
   	curl = curl_easy_init();
   	if (curl) {
   		fp = fopen(outfilename,"wb");
   		curl_easy_setopt(curl, CURLOPT_URL, url);
   		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);  
   		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
   		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
   		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
           // failed when http server return error code.
           curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
   		if(userpwd != NULL) {
   		  curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
   		}
   		
   		std::string data = getParamsData();
   		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
   
   		res = curl_easy_perform(curl);
   		
   		if(res != CURLE_OK){
         ret = curl_easy_strerror(res);
     		LOG(WARNING) << "curl_easy_perform() failed: " << ret;
   		}
   
   		curl_easy_cleanup(curl);
   		fclose(fp);	
   	}
   	curl_global_cleanup();
   	return ret;
   }
   
   ```

   

4.  upload file

   ```c++
   size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                               void *userp) {
     ((std::string *)userp)->append((char *)contents, size * nmemb);
     return size * nmemb;
   }
   
   bool saveCheckResultimp(const std::string &filepath) {
     CURL *curl;
     CURLcode res;
     std::string ret;
     curl = curl_easy_init();
     if (curl) {
       std::string curlCheckParams =
           getServerPrefix() + getServerIp() + RestFulApi["saveCheckResult"];
       curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
       curl_easy_setopt(curl, CURLOPT_URL, curlCheckParams.c_str());
       curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
       curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
       struct curl_slist *headers = NULL;
       curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
       curl_mime *mime;
       curl_mimepart *part;
       mime = curl_mime_init(curl);
       part = curl_mime_addpart(mime);
       curl_mime_name(part, "file");
       curl_mime_filedata(part, filepath.c_str());
       curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
   
       // result callback
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
       curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
   
       res = curl_easy_perform(curl);
       curl_mime_free(mime);
     }
     curl_easy_cleanup(curl);
   
     nlohmann::json j = std::move(parseString(ret));
     if (j["success"].is_boolean() && j["success"].get<bool>()) {
       return true;
     } else {
       std::cout << "[fail] saveCheckResultimp response fail..." << std::endl;
     }
   
     return false;
   }
   ```

5. [multithread](https://curl.se/libcurl/c/multithread.html)

  ```c++
  #include <stdio.h>
  #include <pthread.h>
  #include <curl/curl.h>
  
  #define NUMT 4
  
  /*
    List of URLs to fetch.
  
    If you intend to use a SSL-based protocol here you might need to setup TLS
    library mutex callbacks as described here:
  
    https://curl.se/libcurl/c/threadsafe.html
  
  */
  const char * const urls[NUMT]= {
    "https://curl.se/",
    "ftp://example.com/",
    "https://example.net/",
    "www.example"
  };
  
  static void *pull_one_url(void *url)
  {
    CURL *curl;
  
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_perform(curl); /* ignores error */
    curl_easy_cleanup(curl);
  
    return NULL;
  }
  
  
  /*
    int pthread_create(pthread_t *new_thread_ID,
    const pthread_attr_t *attr,
    void * (*start_func)(void *), void *arg);
  */
  
  int main(int argc, char **argv)
  {
    pthread_t tid[NUMT];
    int i;
  
    /* Must initialize libcurl before any threads are started */
    curl_global_init(CURL_GLOBAL_ALL);
  
    for(i = 0; i< NUMT; i++) {
      int error = pthread_create(&tid[i],
                                NULL, /* default attributes please */
                                pull_one_url,
                                (void *)urls[i]);
      if(0 != error)
        fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
      else
        fprintf(stderr, "Thread %d, gets %s\n", i, urls[i]);
    }
  
    /* now wait for all threads to terminate */
    for(i = 0; i< NUMT; i++) {
      pthread_join(tid[i], NULL);
      fprintf(stderr, "Thread %d terminated\n", i);
    }
    curl_global_cleanup();
    return 0;
  }
  ```   
6. [threaded-ssl](https://curl.se/libcurl/c/threaded-ssl.html)

  ```c++
  /* <DESC>
  * Show the required mutex callback setups for GnuTLS and OpenSSL when using
  * libcurl multi-threaded.
  * </DESC>
  */
  /* A multi-threaded example that uses pthreads and fetches 4 remote files at
  * once over HTTPS. The lock callbacks and stuff assume OpenSSL <1.1 or GnuTLS
  * (libgcrypt) so far.
  *
  * OpenSSL docs for this:
  *   https://www.openssl.org/docs/man1.0.2/man3/CRYPTO_num_locks.html
  * gcrypt docs for this:
  *   https://gnupg.org/documentation/manuals/gcrypt/Multi_002dThreading.html
  */
  
  #define USE_OPENSSL /* or USE_GNUTLS accordingly */
  
  #include <stdio.h>
  #include <pthread.h>
  #include <curl/curl.h>
  
  #define NUMT 4
  
  /* we have this global to let the callback get easy access to it */
  static pthread_mutex_t *lockarray;
  
  #ifdef USE_OPENSSL
  #include <openssl/crypto.h>
  static void lock_callback(int mode, int type, char *file, int line)
  {
    (void)file;
    (void)line;
    if(mode & CRYPTO_LOCK) {
      pthread_mutex_lock(&(lockarray[type]));
    }
    else {
      pthread_mutex_unlock(&(lockarray[type]));
    }
  }
  
  static unsigned long thread_id(void)
  {
    unsigned long ret;
  
    ret = (unsigned long)pthread_self();
    return ret;
  }
  
  static void init_locks(void)
  {
    int i;
  
    lockarray = (pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() *
                                                  sizeof(pthread_mutex_t));
    for(i = 0; i<CRYPTO_num_locks(); i++) {
      pthread_mutex_init(&(lockarray[i]), NULL);
    }
  
    CRYPTO_set_id_callback((unsigned long (*)())thread_id);
    CRYPTO_set_locking_callback((void (*)())lock_callback);
  }
  
  static void kill_locks(void)
  {
    int i;
  
    CRYPTO_set_locking_callback(NULL);
    for(i = 0; i<CRYPTO_num_locks(); i++)
      pthread_mutex_destroy(&(lockarray[i]));
  
    OPENSSL_free(lockarray);
  }
  #endif
  
  #ifdef USE_GNUTLS
  #include <gcrypt.h>
  #include <errno.h>
  
  GCRY_THREAD_OPTION_PTHREAD_IMPL;
  
  void init_locks(void)
  {
    gcry_control(GCRYCTL_SET_THREAD_CBS);
  }
  
  #define kill_locks()
  #endif
  
  /* List of URLs to fetch.*/
  const char * const urls[]= {
    "https://www.example.com/",
    "https://www2.example.com/",
    "https://www3.example.com/",
    "https://www4.example.com/",
  };
  
  static void *pull_one_url(void *url)
  {
    CURL *curl;
  
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* this example does not verify the server's certificate, which means we
      might be downloading stuff from an impostor */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_perform(curl); /* ignores error */
    curl_easy_cleanup(curl);
  
    return NULL;
  }
  
  int main(int argc, char **argv)
  {
    pthread_t tid[NUMT];
    int i;
    (void)argc; /* we do not use any arguments in this example */
    (void)argv;
  
    /* Must initialize libcurl before any threads are started */
    curl_global_init(CURL_GLOBAL_ALL);
  
    init_locks();
  
    for(i = 0; i< NUMT; i++) {
      int error = pthread_create(&tid[i],
                                NULL, /* default attributes please */
                                pull_one_url,
                                (void *)urls[i]);
      if(0 != error)
        fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
      else
        fprintf(stderr, "Thread %d, gets %s\n", i, urls[i]);
    }
  
    /* now wait for all threads to terminate */
    for(i = 0; i< NUMT; i++) {
      pthread_join(tid[i], NULL);
      fprintf(stderr, "Thread %d terminated\n", i);
    }
  
    kill_locks();
  
    return 0;
  }
  ```
7. 断点续下载
```c++
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <iostream>

#define PART_FILENAME "downloaded_file.part"

// Struct to hold custom data needed for resuming
struct CustomData {
    FILE *file;
    CURL *curl;
};

// Callback function to write data received from curl
static size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

// libcurl 的调试回调函数
int debugCallback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    // 仅处理请求数据
    if (type == CURLINFO_HEADER_OUT) {
        // 将请求数据打印到标准输出
        std::cout << "HTTP Request:\n";
        std::cout.write(data, size);
    }
    return 0;
}

// Function to perform resumable download
int perform_resumable_download(const char *url, const char *output_filename) {
    CURL *curl = curl_easy_init();
    CURLcode res;

    if (!curl) {
        fprintf(stderr, "Failed to initialize libcurl\n");
        return 1;
    }

    FILE *file = fopen(output_filename, "ab");
    if (!file) {
        perror("Failed to open file for writing");
        curl_easy_cleanup(curl);
        return 1;
    }

    // Get the size of existing file
    fseek(file, 0, SEEK_END);
    long resume_from = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Set URL for download
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // 设置调试回调函数
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debugCallback);

        // 启用调试输出
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    if(resume_from !=0) {// Enable resuming from where the partial file left off
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, resume_from);
    } else {
         // Enable resuming from where the partial file left off
        curl_easy_setopt(curl, CURLOPT_RANGE, "0-");
    }

    // curl 拔掉网线不返回错误
    // 原因: https://curl.se/docs/faq.html#Why_doesn_t_curl_return_an_error)
    // 解决方案:  使用CURLOPT_LOW_SPEED_TIME/CURLOPT_LOW_SPEED_LIMIT，https://github.com/curl/curl/issues/8345
    // abort if slower than 30 bytes/sec during 60 seconds
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 60L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 30L);

    // Set write callback function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // Set file handle for writing
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    // Perform the request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        fclose(file);
        curl_easy_cleanup(curl);
        return 1;
    }

    // Close file handle
    fclose(file);

    // Clean up curl handle
    curl_easy_cleanup(curl);

    printf("Downloaded file saved as '%s'\n", output_filename);

    return 0;
}

int main() {
    //const char *url = "";
    const char* url = "xx;
    const char *output_filename = "CheckToolNet.exe";

    // Perform the resumable download
    if (perform_resumable_download(url, output_filename) != 0) {
        fprintf(stderr, "Failed to perform resumable download\n");
        return 1;
    }

    return 0;
}
```


## 资料

1. 维基百科
2. 在线编码解析工具：http://web.chacuo.net/charseturlencode
3. cURL必知必会   微信阅读中可免费查看