#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#pragma pack(push, 1) // Stop padding on structs

typedef struct {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
} FileHeader;

typedef struct {
    uint32_t size; // Size of InfoHeader (should be 40 for my case)
    uint32_t width; // Width of image
    int32_t height; // Height of image
    uint16_t planes; // Amount of colour planes (should be 1)
    uint16_t bitCount; // Aka bit depth, bits per pixel
    uint32_t compression; // Type of compression, in my case I'm not handling compression so this should be 0
    uint32_t imageSize; // Size of image (while compressed if present)
    uint32_t XpixelsPerM; // Useful for physical printing (which I don't need)
    uint32_t YpixelsPerM; // Same as above
    uint32_t coloursUsed; // Colours used, if 0 then 2^bitCount (or 1 << bitCount)
    uint32_t coloursImportant;
    // Could move colour table and raster data here but probably not
} InfoHeader;

typedef struct { // BMP stores RGB as BGR
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved; // Not sure why this exists, probably so it's aligned in 4 bytes and is easier for the CPU to get
} RGBQuad;

#pragma pack(pop) // Restore padding on structs

int main() {
    FILE* file = fopen("example.bmp", "rb");
    if (!file) {
        printf("Error: Unable to get file\n");
        return 1;
    }

    FileHeader fileHeader;
    InfoHeader infoHeader;

    fread(&fileHeader, sizeof(fileHeader), 1, file);
    fread(&infoHeader, sizeof(infoHeader), 1, file);

    // BMP is little endian so the signature would be 0x4D42 not 0x424D
    // I don't know much C or file formats in the first place so for now I will only do monochrome (1 bit) BMP's with no compression (0)
    if (fileHeader.signature != 0x4D42 || infoHeader.size != 40 || infoHeader.planes != 1 || infoHeader.bitCount != 1 || infoHeader.compression != 0) {
        fclose(file);
        return 1;
    }

    // Get colour palette (Normally would need to check bitCount <= 8 but it's guaranteed in my case so it's okay)
    uint8_t numColours = infoHeader.coloursUsed == 0 ? 1 << infoHeader.bitCount : infoHeader.coloursUsed; // << is a way to do exponents
    RGBQuad* colourTable = malloc(sizeof(RGBQuad) * numColours);

    fread(colourTable, sizeof(colourTable), numColours, file);

    // Get raster data
    uint8_t padding = infoHeader.width % 4 != 0 ? infoHeader.width + (4 - (infoHeader.width % 4)) : 0; // Account for padding of 4 bytes (because BMP specification says so)
    fseek(file, fileHeader.dataOffset, SEEK_SET); // Go to start of raster data

    // Printing
    printf("File Header\nSignature: %u, File size: %u, Reserved: %u, Data offset: %u\n", fileHeader.signature, fileHeader.fileSize, fileHeader.reserved, fileHeader.dataOffset);

    printf("Info Header\nSize: %u, Width: %u, Height: %u, Planes: %u, Bit count: %u, ", infoHeader.size, infoHeader.width, infoHeader.height, infoHeader.planes, infoHeader.bitCount);
    printf("Compression: %u, Image size: %u, X pixels per meter: %u, Y pixels per meter: %u, ", infoHeader.compression, infoHeader.imageSize, infoHeader.XpixelsPerM, infoHeader.YpixelsPerM);
    printf("Colours used: %u, Colours important: %u\n", infoHeader.coloursUsed, infoHeader.coloursImportant);

    printf("Colours\n");
    for (uint32_t i = 0; i < numColours; i++) {
        printf("Index %u: Blue: %d, Green: %d, Red: %d\n", i, colourTable[i].blue, colourTable[i].green, colourTable[i].red);
    }
    
    return 0;
}