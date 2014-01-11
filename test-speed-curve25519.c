#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "curve25519-donna.h"

#define ITERATIONS 10000
#define PASSES 5
#define OUTLIER_THRESHOLD (MICRO_SECONDS_PER_SECOND / 100)
#define MICRO_SECONDS_PER_SECOND 1000000

static uint64_t
time_now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t ret = tv.tv_sec;
	ret *= MICRO_SECONDS_PER_SECOND;
	ret += tv.tv_usec;
	return ret;
}

uint8_t private_key[32], public[32], peer[32], output[32];
static const uint8_t basepoint[32] = {9};
unsigned i, j;
uint64_t sum = 0, sum_squares = 0, skipped = 0, mean, ops;
double sd;

static void
run_pass(int pass, int totalPasses) {
	sum = sum_squares = skipped = 0;
	uint64_t total_delta = 0;

	for (i = 0; i < ITERATIONS; ++i) {
		const uint64_t start = time_now();
		curve25519_donna(output, peer, public);
		const uint64_t end = time_now();
		const uint64_t delta = end - start;
		total_delta += delta;
		if (delta > OUTLIER_THRESHOLD) {
			// something terrible happened (task switch etc)
			skipped++;
			continue;
		}
		sum += delta;
		sum_squares += (delta * delta);
	}

	mean = sum / ((uint64_t) ITERATIONS);
	sd = sqrt((double)(sum_squares/((uint64_t) ITERATIONS) - mean * mean));
	double totalSeconds = (double)total_delta / MICRO_SECONDS_PER_SECOND;
	ops = ITERATIONS / totalSeconds;
	printf("pass %d/%d: mean:%lluus sd:%fus ops/s:%llu skipped:%llu\n",
		   pass,
		   totalPasses,
		   mean,
		   sd,
		   ops,
		   skipped);
}

int
main(int argc, char **argv) {
	memset(private_key, 42, sizeof(private_key));
	memset(peer, 0, sizeof(peer));
	private_key[0] &= 248;
	private_key[31] &= 127;
	private_key[31] |= 64;
	curve25519_donna(public, private_key, basepoint);

	for (j = 0; j < PASSES; j++) {
		run_pass(j + 1, PASSES);
	}

	return 0;
}
