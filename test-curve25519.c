/*
 test-curve25519 version 20050915
 D. J. Bernstein
 Public domain.
 
 Tiny modifications by agl
 */

#include <stdio.h>
#include <stdint.h>
#include "curve25519-donna.h"

#define ITERATIONS 10000

void doit(u8 *ek, const u8 *e, const u8 *k)
{
	int i;
	for (i = 0; i < 32; i++) {
		printf("%02x", e[i]);
	}
	printf(" ");
	for (i = 0; i < 32; i++) {
		printf("%02x", k[i]);
	}
	printf(" ");
	curve25519_donna(ek,e,k);
	for (i = 0; i < 32; i++) {
		printf("%02x", ek[i]);
	}
	printf("\n");
}

u8 e1k[32];
u8 e2k[32];
u8 e1e2k[32];
u8 e2e1k[32];
u8 e1[32] = {3};
u8 e2[32] = {5};
u8 k[32] = {9};

int
main()
{
	int i, j;
	for (i = 0; i < ITERATIONS; i++) {
		doit(e1k,e1,k);
		doit(e2e1k,e2,e1k);
		doit(e2k,e2,k);
		doit(e1e2k,e1,e2k);
		for (j = 0; j < 32; j++) {
			if (e1e2k[j] != e2e1k[j]) {
				printf("FAIL!\n");
				return 1;
			}
		}
		for (j = 0; j < 32; j++) e1[j] ^= e2k[j];
		for (j = 0; j < 32; j++) e2[j] ^= e1k[j];
		for (j = 0; j < 32; j++) k[j] ^= e1e2k[j];
	}
	
	return 0;
}
