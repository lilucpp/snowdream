#include "utility.h"
#include "openssl\ossl_typ.h"
#include "openssl\bio.h"
#include "openssl\ec.h"
#include <exception>

void print_text(unsigned char * text, int len)
{
	for (int i = 0; i < len; i++) {
		printf("%02X", text[i]);
	}
	printf("\n");
}

void print_key(EC_KEY* ec_key)
{
	BIO *out = NULL;
	out = BIO_new_fp(stderr, BIO_NOCLOSE);
	EC_KEY_print(out, ec_key,4);
	BIO_printf(out, "\n");
	BIO_free(out);
}
