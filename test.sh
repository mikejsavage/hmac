#! /bin/bash

test_hmac() {
	opensslhmac=`echo -n "$1" | openssl sha256 -hmac "$2" | cut -d ' ' -f 2`
	mymac=`echo -n "$1" | ./hmacsha256 "$2"`

	if [[ "$opensslhmac" != "$mymac" ]]; then
		echo "openssl disagrees!"
		echo "$1" > baddata.txt
		echo "$2" > badkey.txt
		exit 1
	fi
}

echo "Testing short keys"
for i in {0..960}; do
	data=`dd if=/dev/urandom bs=1 count=$i 2> /dev/null`
	keylen=$(($i / 16))
	key=`dd if=/dev/urandom bs=1 count=$keylen 2> /dev/null`

	test_hmac "$data" "$key"
done

echo "Testing 64 byte keys"
for i in {0..1024}; do
	data=`dd if=/dev/urandom bs=1 count=$i 2> /dev/null`
	keylen=64
	key=`dd if=/dev/urandom bs=1 count=$keylen 2> /dev/null`

	test_hmac "$data" "$key"
done

echo "Testing long keys"
for i in {0..1024}; do
	data=`dd if=/dev/urandom bs=1 count=$i 2> /dev/null`
	keylen=$(($i + 64))
	key=`dd if=/dev/urandom bs=1 count=$keylen 2> /dev/null`

	test_hmac "$data" "$key"
done
