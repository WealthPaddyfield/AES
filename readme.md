**Readme**

B3
AES in C language

100% free !

 - clone this branch command

`git clone -b conn https://github.com/WealthPaddyfield/AES.git`

 - gcc compile command(encrypt)
`gcc -Iinclude encrypt/src/test.c encrypt/src/addroundkey.c encrypt/src/subbytes.c encrypt/src/mixcolumns.c encrypt/src/getroundkey.c encrypt/src/keyexpansion.c encrypt/src/sbox.c encrypt/src/shiftrows.c -o aes`

- gcc compile command(decrypt)
`gcc -Iinclude decrypt/src/test.c decrypt/src/addroundkey.c decrypt/src/inv_subbytes.c decrypt/src/inv_mixcolumns.c decrypt/src/getroundkey.c decrypt/src/keyexpansion.c decrypt/src/sbox.c decrypt/src/inv_shiftrows.c -o aes_inv`

- change key length

|Key length| NR | NK | NB | WORDS |  
|128 bit | 10  | 4 | 4 | 44 |  
|192 bit | 12  | 6 | 4 | 52 |  
|256 bit | 14  | 8 | 4 | 60 |  
To change the key length, modify the **AES_NK** and **AES_NB** values in *aes_key_config.h*
 
