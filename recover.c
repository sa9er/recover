#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    char *fileName = argv[1];
    if (fileName == NULL)
    {
        printf("Usage: ./recover fileName\n");
        return 1;
    }
    if (!(argc <= 2))
    {
        printf("Usage: ./recover fileName");
        return 2;
    }
    FILE *f = fopen(fileName, "r");
    if (f == NULL)
    {
        printf("file not found!");
        return 3;
    }

    BYTE B512[512];
    int countJPEGs = 0;
    char *JPEG_title = malloc(8 * sizeof(BYTE));
    FILE *newJPEG = NULL;

    while (fread(B512, (sizeof(BYTE)), 512, f) == 512)
    {
        if (B512[0] == 0xff && B512[1] == 0xd8 && B512[2] == 0xff && (B512[3] & 0xf0) == 0xe0)
        {
            if (countJPEGs > 0)
            {
                fclose(newJPEG);
            }
            sprintf(JPEG_title, "%03i.jpg", countJPEGs);
            newJPEG = fopen(JPEG_title, "w");
            countJPEGs++;
            if (newJPEG == NULL)
            {
                printf("new jpeg could not be created");
                return 5;
            }
        }
        if (countJPEGs > 0)
        {
            fwrite(B512, sizeof(BYTE), 512, newJPEG);
        }
    }
    free(JPEG_title);
    fclose(newJPEG);
    fclose(f);
    return 0;
}