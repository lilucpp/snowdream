#include "example_sm3.h"
#include <openssl/evp.h>
#include "utility.h"

void evp_digest()
{
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = EVP_sm3();
	char mess1[] = "abc";
	char mess2[] = "Hello World\n";
	unsigned char md_value[EVP_MAX_MD_SIZE];
	int md_len;

	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
	EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
	EVP_DigestFinal_ex(mdctx, md_value, (unsigned int *)&md_len);
	EVP_MD_CTX_destroy(mdctx);
	EVP_cleanup();

	print_text(md_value, md_len);
	return;
}

void evp_sm3()
{
	unsigned char testbuf[] = "abc";
	long testbuflen = strlen((char*)testbuf);
	unsigned char dgst[EVP_MAX_MD_SIZE];
	unsigned int dgstlen;
	dgstlen = sizeof(dgst);

	if (!EVP_Digest(testbuf, testbuflen, dgst, &dgstlen, EVP_sm3(), NULL)) {
		return;
	}
	print_text(dgst, dgstlen);
	return;
}