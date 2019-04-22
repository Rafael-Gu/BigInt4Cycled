#pragma once
#include <string>

using namespace std;

struct DigitQuad
{
	short data;
	DigitQuad *prev;
	DigitQuad *next;
};

struct BigInt
{
	DigitQuad *head;
	DigitQuad *end;
	int block;
};

BigInt *BIntCreate();
void BIntFree(BigInt *&);
void BIntAssign(BigInt *&, string);
void BIntClear(BigInt *);
BigInt *BIntCopy(BigInt *);
BigInt *BIntAdd(BigInt *, BigInt *);
bool BIntIsNorm(BigInt *);
void BIntNormalize(BigInt *);
string BIntParse(BigInt *);
bool BIntEmpty(BigInt *);

bool StringCheck(string);
string StringCull(string);
string StringFix(string);
