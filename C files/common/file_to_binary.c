#include <stdio.h>

int main(void)
{
    unsigned char buf;
    FILE *file;

    file = fopen("test.txt", "rb");
    while (fread(&buf, 1, 1, file) > 0)
    {
        printf("%02x", buf);
    }
    
    //fread(&buf, sizeof(buf), 1, file);
    fclose(file);


    return 0;
}