all: hmacsha256

hmacsha256: hmacsha256.c sha256.c
	gcc -O2 -Wall -Wextra -std=c99 -o hmacsha256 hmacsha256.c sha256.c

clean:
	rm -f hmacsha256
