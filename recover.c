#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // check if the command line argument is correct
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // open memory card file
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file\n");
    }

    const int block_size = 512;
    int counter = 0;
    uint8_t buffer[block_size];
    bool flag = false;
    FILE *img;
    char filename[8];

    // reading the entire card and looking for a jpeg file
    while (fread(buffer, sizeof(uint8_t), block_size, card) == block_size)
    {
        // checking if the file is a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (flag == true)
            {
                fclose(img);
            }
            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create image file\n");
                return 1;
            }
            flag = true;
            counter++;
        }

        // Writing to the file
        if (flag == true && img != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), block_size, img);
        }
    }

    // closing the file
    if (flag == true)
    {
        fclose(img);
    }
    fclose(card);
    return 0;
}
