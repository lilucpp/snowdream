#pragma once

#include <stdio.h>
#include <cstring>
#include "openssl\ossl_typ.h"

	void print_text(unsigned char * text, int len);
	void print_key(EC_KEY* ec_key);