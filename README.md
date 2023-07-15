Christy Miao 
cmiao4
Winter 2023
# Assignment 5 - Public Key Cryptography

This program aims to create three programs: a key generator, an encryptor, and decryptor. The key generator, keygen, is expected to generate a key, producing Schmidt-Samoa public and private key pairs. The encrytor, encrypt, will encrypt files using a public key, and the decryptor, decrypt will decrypt the encrypted files using the corresponding private key. There are also two libraries: numtheory, and ss. Numtheory holds functions related to the mathematics behind ss, and ss will contain the implementations of routines for SS. A random state module will be used in each of the program. 

## Building

To build, run '$ make or '$ make all' on the command line within the assignment 5's directory. This should create three executable binary files titled 'keygen', 'encrypt', and 'decrypt', which the user can then run. If you would like to create one of the executable binary files, run '$ make' followed by 'keygen', 'encrypt', or 'decrypt'. 

## Running

Typing '$ ./keygen', followed by one or more of the following valid arguments (-b, -i, -n, -d, -s, -v, -h) will generate a ss public and private key pair.

-b specifies the minimum bits needed for the public modulus n, -i specifies the number Miller-Rabin iterations for testing primes, -n specifies the public key file, -d specifies the private key file, -s specifies the random seed for the random initialization, -v enables verbose output, -h displays program synopsis and usage.

Typing '$ ./encrypt', followed by one or more of the following valid arguments (-i, -o, -n, -v, -h) will encrypt files using a public key.

-i specifies the input file the encrypt, -o specifies the output file the encrypt, -n specifies the file containing the public key, -v enables verbose output, -h displays program synopsis and usage.

Typing '$ ./decrypt', followed by one or more of the following valid arguments (-i, -o, -n, -v, -h) will decrypt the encrypted files using the corresponding private key.

-i specifies the input file the decrypt, -o specifies the output file the decrypt, -n specifies the file containing the private key, -v enables verbose output, -h displays program synopsis and usage.

Do note, typing multiple arguments will work as well, but any argument with -h will only display a help message.

## Error

If the user does not specify a specific file or specfics a non-existent file, the program will give a segmentation fault. 
