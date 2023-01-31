#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Cryptographer.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>

Cryptographer::Cryptographer()
{
}

Cryptographer::~Cryptographer()
{
}

std::string Cryptographer::encrypt(std::string pass)
{
	 std::string gsm, result;
	  CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result));

	  CryptoPP::Weak1::MD5 hash;
	  hash.Update((unsigned char*)&pass[0], pass.size());
	  gsm.resize(hash.DigestSize());
	  hash.Final((unsigned char*)&gsm[0]);

	  CryptoPP::StringSource(gsm, true, new CryptoPP::Redirector(encoder));
	  return result;
}


