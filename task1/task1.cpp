#include <iostream>
#include <fstream>
#include <cstring>
#include <openssl/des.h>
#include <vector>

using namespace std;

// it calculates the number of bytes needed to pad the data to a multiple of 8
void addPadding(char* data, size_t dataSize) {
    size_t paddingSize = 8 - (dataSize % 8);
    // cast is done because memset only accepts chars
    memset(data + dataSize, static_cast<char>(paddingSize), paddingSize);
}

// Remove padding from the decrypted data
void removePadding(char* data, size_t dataSize) {
    size_t paddingSize = static_cast<size_t>(data[dataSize - 1]);
    if (paddingSize <= 8) {
        memset(data + dataSize - paddingSize, 0, paddingSize);
    }
}

int main() {
    const char* inputFile = "input.txt";
    const char* encryptedFile = "encrypted.txt";
    const char* decryptedFile = "decrypted.txt";
    const char* desKey = "mysecret"; // 8-byte key (56 bits)


    ifstream inputFileStream(inputFile, ios::binary); // input from the input file
    ofstream encryptedFileStream(encryptedFile, ios::binary); // output to encrypted file

    DES_cblock key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    DES_key_schedule schedule;
    DES_set_key_unchecked(&key, &schedule);

    char buffer[8];
    size_t bytesRead;
    vector<int> readSizes; // to store the values read for every loop iteration

    // reads and gets the number of bytes read
    while ((bytesRead = inputFileStream.read(buffer, 8).gcount()) > 0) {

        readSizes.push_back(bytesRead); //adds # of bytes to vector to be used in decryption
        // add padding if necessary
        if (bytesRead < 8) {
            addPadding(buffer, bytesRead);
        }

        // encrypts using DES and writes to eccryption file
        char ciphertext[8];
        DES_ecb_encrypt((DES_cblock*)buffer, (DES_cblock*)ciphertext, &schedule, DES_ENCRYPT);
        encryptedFileStream.write(ciphertext, 8);
    }

    inputFileStream.close();
    encryptedFileStream.close();
    

    /***********************************    Decryption    ***************************************/

    ifstream inputEmcrypted(encryptedFile, ios::binary); // the input is the encrypted file
    ofstream decryptedFileStream(decryptedFile, ios::binary); // the output is the decrypted file

    int count = 0;
    while ((bytesRead = inputEmcrypted.read(buffer, 8).gcount()) > 0) {

        char decryptedText[8];
        DES_ecb_encrypt((DES_cblock*)buffer, (DES_cblock*)decryptedText, &schedule, DES_DECRYPT);

        // remove padding after decryption
        if (readSizes[count] < 8) {
            removePadding(decryptedText, bytesRead);
        }

        // writes to deccrypted after removing the padding the exact number
        // of original bytes read using the readSizes vector
        decryptedFileStream.write(decryptedText, readSizes[count]);

        count++;
    }

    
    decryptedFileStream.close();

    return 0;
}
