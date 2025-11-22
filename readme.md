**Readme**

B3
AES in C language

100% free !

 - clone this branch command  
`git clone -b actual https://github.com/WealthPaddyfield/AES.git`
  
 - gcc compile command(client_side)  
`gcc -Iinclude client_side/src/*.c -o compiled/client.out`  
- gcc compile command(server_side)  
`gcc -Iinclude server_side/src/*.c -o compiled/server.out`  
  
- change key length

|Key length| NR | NK | NB | WORDS |  
|128 bit | 10  | 4 | 4 | 44 |  
|192 bit | 12  | 6 | 4 | 52 |  
|256 bit | 14  | 8 | 4 | 60 |  
  
To change the key length, modify the **AES_NK** and **AES_NB** values in *aes_key_config.h*  
※Need to change both side. use appropiate key too.
 
