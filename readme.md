**Readme**

B3
AES in C language

100% free !

 - clone this branch command

`git clone -b dev-tomida https://github.com/WealthPaddyfield/AES.git`

 - gcc compile command(encrypt)

`gcc -Iinclude encrypt/src/main.c encrypt/src/addroundkey.c encrypt/src/subbytes.c encrypt/src/mixcolumns.c encrypt/src/getroundkey.c encrypt/src/keyexpansion.c encrypt/src/sbox.c encrypt/src/shiftrows.c -o aes`

- gcc compile command(decrypt)

`gcc -Iinclude decrypt/src/main.c decrypt/src/addroundkey.c decrypt/src/inv_subbytes.c decrypt/src/inv_mixcolumns.c decrypt/src/getroundkey.c decrypt/src/keyexpansion.c decrypt/src/sbox.c decrypt/src/inv_shiftrows.c -o aes_inv`
