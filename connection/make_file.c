#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    FILE *fp;
    long fileSize = 1024 * 1024 * 1024 / 4;
    long i;
    int random_num;

    srand((unsigned int)time(NULL));
   
    fp =  fopen("random_data.bin","wb");


    

    for (i = 0; i < fileSize; i++){
        random_num = rand();
        fwrite(&random_num,sizeof(int),1,fp);
    }

    fclose(fp);
    return 0;
}
