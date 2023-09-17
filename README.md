# A steganography library in C

Steganography is the art of hiding information within other data, like concealing text within images. Our C library enables seamless steganographic operations for secure data embedding and extraction.

The library offers steganography using a straightforward algorithm.

Supports only BMP file format.

BMP library: <a href="https://github.com/mattflow/cbmp" target="_blank"> https://github.com/mattflow/cbmp </a>

# Prerequisites

- gcc, clang, or another C compiler
- Make
- lm (lib math)

## Example encoding

```c
#include "../include/encrypt.h"
#include "../include/define.h"

st_encrypt *st = st_encrypt_create();

if (st_encrypt_set_input(st, "./assets/input.bmp") != ST_NO_ERROR)
{
    printf("Input file doesn't exists\n");
    exit(EXIT_FAILURE);
}

st_encrypt_set_output(st, "./assets/output.bmp");

if (st_encrypt_set_message(st, "This is a secret message!") != ST_NO_ERROR)
{
    printf("Message must be ASCII\n");
    exit(EXIT_FAILURE);
}

__u_short error = st_encrypt_save(st);

if (error != ST_NO_ERROR)
{
    printf("Error: %d\n", error);
    exit(EXIT_FAILURE);
}

st_encrypt_close(st);

printf("File saved\n");
```

## Example decoding

```c
#include "../include/decrypt.h"
#include "../include/define.h"

st_decrypt *st = st_decrypt_create();

st_decrypt_set_input(st, "./assets/output.bmp");

char *message = st_decrypt_get_message(st);

printf("The secret message: %s\n", message);

st_decrypt_close(st);

```

## Build

1. Clone the steganography repository:

```bash
$ git clone https://github.com/baygin/steganography.git
```

2. Change into the steganography directory:

```bash
$ cd steganography
```

3. Build the steganography library:

```bash
$ make
```

## How to use

### Encode

```bash
$ ./bin/steganography encode ./assets/input.bmp ./assets/output.bmp "the secret message"
```

### Decode

```bash
$ ./bin/steganography decode ./assets/output.bmp
```

## Contributing

If you'd like to contribute to the development of this algorithm, your contributions are welcome.

## License

The library is licensed under the GPL3. See <a href="https://github.com/baygin/steganography/blob/master/LICENSE" target="blank"> LICENSE </a> for more information.