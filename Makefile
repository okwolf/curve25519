clean:
	rm -f *.o *.a *.pp test-curve25519-donna test-speed-curve25519-donna

test: test-donna test-speed-donna

curve25519-donna.a: curve25519-donna.o
	ar -rc curve25519-donna.a curve25519-donna.o

curve25519-donna.o: curve25519-donna.c
	gcc -O2 -c curve25519-donna.c -Wall

test-donna: test-curve25519-donna
	./test-curve25519-donna | head -123456 | tail -1

test-speed-donna: test-speed-curve25519-donna
	./test-speed-curve25519-donna

test-curve25519-donna: test-curve25519.c curve25519-donna.a
	gcc -o test-curve25519-donna test-curve25519.c curve25519-donna.a -Wall

test-speed-curve25519-donna: test-speed-curve25519.c curve25519-donna.a
	gcc -o test-speed-curve25519-donna test-speed-curve25519.c curve25519-donna.a -Wall -lm
