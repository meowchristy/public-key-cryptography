# public key cryptography

implementation of schmidt-somoa algorithms

## building
```
  $ make
  $ make all
```

## running
```
  $ ./keygen
    -b             : specifies the minimum bits needed for the public modulus n
    -i             : specifies the number Miller-Rabin iterations for testing primes
    -n             : specifies the public key file
    -d             : specifies the private key file
    -s             : specifies the random seed for the random initialization
    -v             : verbose, optional statistics
    -h             : displays program synopsis and usage
```
```
  $ ./encrypt -i <input file> -o <output file>
    -i <input file>  : specifies input to be compressed
    -o <output file> : specifies output of compressed input
    -n               : specifies the file containing the public key
    -v               : verbose, optional
    -h               : displays program synopsis and usage
```
```
  $ ./decrypt -i <input file> -o <output file>
    -i <input file>  : specifies input to be compressed
    -o <output file> : specifies output of compressed input
    -n               : specifies the file containing the private key
    -v               : verbose, optional
    -h               : displays program synopsis and usage
```
- Do note, typing multiple arguments will work as well, but any argument with -h will only display a help message.

## error

- If the user does not specify a specific file or specfics a non-existent file, the program will give a segmentation fault. 
