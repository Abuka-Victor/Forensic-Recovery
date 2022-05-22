#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // To Check for proper usage
    // Accept only one command line argument which is the forensic file name
    if (argc > 2)
    {
        printf("Only the name of the file is allowed\n");
        return 1;
    }

    // To Check That the user is coperative
    if (argc < 2)
    {
        printf("Usage: ./reover <forensic_file_name>\n");
        return 1;
    }

    // Open The Memory Card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }

    // Read
    uint8_t arr[512];
    int i = 0;
    char *filename = malloc(7 * sizeof(char));
    FILE *img;
    while (fread(arr, 512, 1, card) != 0)
    {
        if (arr[0] == 0xff && arr[1] == 0xd8 && arr[2] == 0xff && (arr[3] & 0xf0) == 0xe0)
        {
            // Found A Header
            if (i == 0)
            {
                // First File
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w");
                i++;
                fwrite(arr, 512, 1, img);
            }
            else
            {
                // Other Files
                fclose(img);
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w");
                i++;
                fwrite(arr, 512, 1, img);
            }
        }
        else
        {
            // Already Open writing
            fwrite(arr, 512, 1, img);
        }
    }
    fclose(img);
    fclose(card);
    free(filename);
}