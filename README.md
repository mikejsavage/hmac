This is a command line tool for computing SHA256 HMACs. It has no
licensing restrictions.

### Usage

`hmacsha256` reads data from standard input, and takes a key as an
argument then does what you would expect. For example:

	$ echo "hello world" | ./hmacsha256 "best key"
	d345364418f757b735b58ebc5d2dde98edf1c91a77a7909019ad05a99177b37f
