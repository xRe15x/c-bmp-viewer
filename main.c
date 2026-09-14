#include <stdio.h>
#include <stdint.h>

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
    uint32_t compression;
    uint32_t imageSize; // Size of image (while compressed if present)
    uint32_t XpixelsPerM; // Useful for physical printing
    uint32_t YpixelsPerM; // Useful for physical printing
    uint32_t coloursUsed;
    uint32_t coloursImportant;
    // Need colour table and raster data too
} InfoHeader;

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

    // Printing
    printf("File Header\nSignature: %u, File size: %u, Reserved: %u, Data offset: %u\n", fileHeader.signature, fileHeader.fileSize, fileHeader.reserved, fileHeader.dataOffset);
    printf("Info Header\nSize: %u, Width: %u, Height: %u, Planes: %u, Bit count: %u, ", infoHeader.size, infoHeader.width, infoHeader.height, infoHeader.planes, infoHeader.bitCount);
    printf("Compression: %u, Image size: %u, X pixels per meter: %u, Y pixels per meter: %u, ", infoHeader.compression, infoHeader.imageSize, infoHeader.XpixelsPerM, infoHeader.YpixelsPerM);
    printf("Colours used: %u, Colours important: %u\n", infoHeader.coloursUsed, infoHeader.coloursImportant);
    
    return 0;
}