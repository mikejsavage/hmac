/*
 * Public domain
 * HMAC-SHA256 implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <err.h>
#include "sha256.h"

#define INNER_PADDING '\x36'
#define OUTER_PADDING '\x5c'

#define BLOCK_LENGTH 64

static void usage( const char * const prog ) {
	errx( 1, "usage: %s <key>", prog );
}

static void normalize_key( uint8_t * const key, const char * const input_key ) {
	const size_t len = strlen( input_key );

	if( len <= BLOCK_LENGTH ) {
		memcpy( key, input_key, len );
		memset( key + len, '\0', BLOCK_LENGTH - len );
	}
	else {
		struct sha256 s;
		sha256_init( &s );
		sha256_update( &s, input_key, len );
		sha256_sum( &s, key );
		memset( key + SHA256_DIGEST_LENGTH, '\0', BLOCK_LENGTH - SHA256_DIGEST_LENGTH );
	}
}

int main( int argc, char ** argv ) {
	if( argc != 2 )
		usage( argv[ 0 ] );

	uint8_t key[ BLOCK_LENGTH ];
	normalize_key( key, argv[ 1 ] );

	struct sha256 inner_s;
	sha256_init( &inner_s );

	uint8_t inner_key[ BLOCK_LENGTH ];
	uint8_t outer_key[ BLOCK_LENGTH ];
	for( size_t i = 0; i < BLOCK_LENGTH; i++ ) {
		inner_key[ i ] = key[ i ] ^ INNER_PADDING;
		outer_key[ i ] = key[ i ] ^ OUTER_PADDING;
	}

	sha256_update( &inner_s, inner_key, BLOCK_LENGTH );

	uint8_t buf[ BUFSIZ ];
	size_t n;
	while( ( n = fread( buf, 1, sizeof( buf ), stdin ) ) > 0 )
		sha256_update( &inner_s, buf, n );
	if( ferror( stdin ) )
		err( 1, "error reading stdin" );

	uint8_t inner_hash[ SHA256_DIGEST_LENGTH ];
	sha256_sum( &inner_s, inner_hash );

	struct sha256 outer_s;
	sha256_init( &outer_s );
	sha256_update( &outer_s, outer_key, BLOCK_LENGTH );
	sha256_update( &outer_s, inner_hash, SHA256_DIGEST_LENGTH );

	uint8_t hmac[ SHA256_DIGEST_LENGTH ];
	sha256_sum( &outer_s, hmac );
	for( size_t i = 0; i < SHA256_DIGEST_LENGTH; i++ )
		printf( "%02x", hmac[ i ] );
	putchar( '\n' );

	return 0;
}
