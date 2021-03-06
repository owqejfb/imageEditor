#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct pictureFile
{
    //the header information
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;

    //Pixel Information
    unsigned char* pixelFile;

    //creates pictureFile from data
    pictureFile(string fileName);

    //creates output file for each task
    void finalOutput(string fileName, int totalBytes, unsigned char* newPicture);
};


pictureFile::pictureFile(string fileName) {

    ifstream picture(fileName, ios_base::binary);

    picture.read(&idLength, 1);
    picture.read(&colorMapType, 1);
    picture.read(&dataTypeCode, 1);
    picture.read((char*)&colorMapOrigin, 2);
    picture.read((char*)&colorMapLength, 2);
    picture.read(&colorMapDepth, 1);
    picture.read((char*)&xOrigin, 2);
    picture.read((char*)&yOrigin, 2);
    picture.read((char*)&width, 2);
    picture.read((char*)&height, 2);
    picture.read(&bitsPerPixel, 1);
    picture.read(&imageDescriptor, 1);

    //1 pixel = 3 bytes, 200x300 = 60,000 pixels, 180,000 bytes
    pixelFile = new unsigned char[width * height * 3];

    //picture goes B-G-R in a .tga picture
    int totalPixels = width * height * 3;
   
    //cout << totalPixels << endl;
    //cout << "Width: " << width << endl;
    //cout << "Height: " << height << endl;

    //fill in pixel information into pixelData
    for (int i = 0; i < totalPixels; i++) {
        picture.read((char*)&pixelFile[i], 1);
    }
}

void pictureFile::finalOutput(string fileName, int totalBytes, unsigned char* newPicture) {

    //picture object

    ofstream picture(fileName, ios_base::binary);

    picture.write(&idLength, 1);
    picture.write(&colorMapType, 1);
    picture.write(&dataTypeCode, 1);
    picture.write((char*)&colorMapOrigin, 2);
    picture.write((char*)&colorMapLength, 2);
    picture.write(&colorMapDepth, 1);
    picture.write((char*)&xOrigin, 2);
    picture.write((char*)&yOrigin, 2);
    picture.write((char*)&width, 2);
    picture.write((char*)&height, 2);
    picture.write(&bitsPerPixel, 1);
    picture.write(&imageDescriptor, 1);

    for (unsigned int i = 0; i < totalBytes; i++) {
        picture.write((char*)&newPicture[i], 1);
    }
}


int main()
{

    //Part 1
    unsigned char* topLayerPointer;
    unsigned char* botLayerPointer;
    //cout << "check point : 1\n";

    pictureFile layer1("input/layer1.tga");
    pictureFile pattern1("input/pattern1.tga");

    topLayerPointer = layer1.pixelFile;
    botLayerPointer = pattern1.pixelFile;

    //goes through every BGR value so we need to run width*height*3 times

    int totalBytes = (layer1.height) * (layer1.width) * 3;

    //where we will hold the new picture
    unsigned char* newPicture1 = new unsigned char[totalBytes];

    for (unsigned int i = 0; i < totalBytes; i++) {

        float tempBGR = (((float)topLayerPointer[i] / 255) * ((float)botLayerPointer[i] / 255) * 255) + 0.5f;
        newPicture1[i] = (unsigned char)tempBGR;
    }

    layer1.finalOutput("output/part1.tga", totalBytes, newPicture1);
    delete[] newPicture1;

    //Part 2 (car)-(layer 2)  bot-top
    pictureFile layer2("input/layer2.tga");
    pictureFile car("input/car.tga");

    topLayerPointer = layer2.pixelFile;
    botLayerPointer = car.pixelFile;

    totalBytes = (layer2.height) * (layer2.width) * 3;

    unsigned char* newPicture2 = new unsigned char[totalBytes];

    for (unsigned int i = 0; i < totalBytes; i++) {

        if (botLayerPointer[i] - topLayerPointer[i] < 0) {
            newPicture2[i] = 0;
        }

        else {
            newPicture2[i] = botLayerPointer[i] - topLayerPointer[i];
        }
    }

    layer2.finalOutput("output/part2.tga", totalBytes, newPicture2);
    delete[] newPicture2;

    //Part 3
    pictureFile pattern2("input/pattern2.tga");

    topLayerPointer = layer1.pixelFile;
    botLayerPointer = pattern2.pixelFile;

    totalBytes = (layer1.height) * (layer1.width) * 3;

    //where we will hold the new picture
    unsigned char* newPicture3 = new unsigned char[totalBytes];
    //cout << "check point : 3\n";
    for (unsigned int i = 0; i < totalBytes; i++) {

        float tempBGR = (((float)topLayerPointer[i] / 255) * ((float)botLayerPointer[i] / 255) * 255) + 0.5f;
        newPicture3[i] = (unsigned char)tempBGR;
    }

    pictureFile text("input/text.tga");

    topLayerPointer = newPicture3;
    botLayerPointer = text.pixelFile;

    for (unsigned int i = 0; i < totalBytes; i++) {

        float tempBGR = (1.0f - (1.0f - ((float)topLayerPointer[i] / 255)) * (1.0f - ((float)botLayerPointer[i] / 255))) * 255 + 0.5f;
        newPicture3[i] = (unsigned char)tempBGR;
    }

    layer1.finalOutput("output/part3.tga", totalBytes, newPicture3);


    //Part 4
    pictureFile circles("input/circles.tga");

    topLayerPointer = layer2.pixelFile;
    botLayerPointer = circles.pixelFile;

    unsigned char* newPicture4 = new unsigned char[totalBytes];
    //cout << "check point : 3\n";
    for (unsigned int i = 0; i < totalBytes; i++) {

        float tempBGR = (((float)topLayerPointer[i] / 255) * ((float)botLayerPointer[i] / 255) * 255) + 0.5f;
        newPicture4[i] = (unsigned char)tempBGR;
    }

    topLayerPointer = pattern2.pixelFile;
    botLayerPointer = newPicture4;

    for (unsigned int i = 0; i < totalBytes; i++) {

        if (botLayerPointer[i] - topLayerPointer[i] < 0) {
            newPicture4[i] = 0;
        }

        else {
            newPicture4[i] = botLayerPointer[i] - topLayerPointer[i];
        }
    }

    layer2.finalOutput("output/part4.tga", totalBytes, newPicture4);

    delete[] newPicture4;
    delete[] newPicture3;


    //Part 5
    topLayerPointer = layer1.pixelFile;
    botLayerPointer = pattern1.pixelFile;

    totalBytes = (layer1.height) * (layer1.width) * 3;

    unsigned char* newPicture5 = new unsigned char[totalBytes];

    for (unsigned int i = 0; i < totalBytes; i++) {

        float tempVal = ((float)botLayerPointer[i] / 255);
        if (tempVal <= 0.5) {

            if (2 * (((float)topLayerPointer[i] / 255) * ((float)botLayerPointer[i] / 255) * 255) + 0.5f > 255) {
                newPicture5[i] = 255;
            }
            else {
                newPicture5[i] = (unsigned char)(2 * (((float)topLayerPointer[i] / 255) * ((float)botLayerPointer[i] / 255) * 255) + 0.5f);
            }
        }

        else if (tempVal > 0.5) {
            newPicture5[i] = (unsigned char)((1.0f - 2.0f * (1.0f - ((float)topLayerPointer[i] / 255)) * (1.0f - ((float)botLayerPointer[i] / 255))) * 255 + 0.5f);
        }
    }

    layer1.finalOutput("output/part5.tga", totalBytes, newPicture5);
    delete[] newPicture5;

    //Part 6
    topLayerPointer = car.pixelFile;

    totalBytes = (car.height) * (car.width) * 3;

    unsigned char* newPicture6 = new unsigned char[totalBytes];

    for (unsigned int i = 0; i < totalBytes; i++) {
        newPicture6[i] = topLayerPointer[i];
    }

    for (unsigned int i = 1; i < totalBytes; i += 3) {

        if (newPicture6[i] + 200 > 255) {
            newPicture6[i] = 255;
        }
        else {
            newPicture6[i] = newPicture6[i] + 200;
        }
    }

    car.finalOutput("output/part6.tga", totalBytes, newPicture6);


    //Part 7
    topLayerPointer = car.pixelFile;

    totalBytes = (car.height) * (car.width) * 3;

    unsigned char* newPicture7 = new unsigned char[totalBytes];

    for (unsigned int i = 0; i < totalBytes; i++) {
        newPicture7[i] = topLayerPointer[i];
    }

    for (unsigned int i = 0; i < totalBytes; i += 3) {

        newPicture7[i] = 0;

        if (newPicture7[i + 2] * 4 > 255) {
            newPicture7[i + 2] = 255;
        }
        else {
            newPicture7[2 + i] = newPicture7[2 + i] * 4;
        }
    }

    car.finalOutput("output/part7.tga", totalBytes, newPicture7);
    delete[] newPicture7;

    //Part 8
    unsigned char* tempPointer;
    tempPointer = car.pixelFile;

    totalBytes = (car.height) * (car.width) * 3;

    unsigned char* redChannel = new unsigned char[totalBytes];
    unsigned char* greenChannel = new unsigned char[totalBytes];
    unsigned char* blueChannel = new unsigned char[totalBytes];

    for (unsigned int i = 0; i < totalBytes; i += 3) {

        blueChannel[i] = tempPointer[i];
        blueChannel[i + 1] = tempPointer[i];
        blueChannel[i + 2] = tempPointer[i];

        greenChannel[i] = tempPointer[i + 1];
        greenChannel[i + 1] = tempPointer[i + 1];
        greenChannel[i + 2] = tempPointer[i + 1];

        redChannel[i] = tempPointer[i + 2];
        redChannel[i + 1] = tempPointer[i + 2];
        redChannel[i + 2] = tempPointer[i + 2];
    }

    car.finalOutput("output/part8_b.tga", totalBytes, blueChannel);
    car.finalOutput("output/part8_g.tga", totalBytes, greenChannel);
    car.finalOutput("output/part8_r.tga", totalBytes, redChannel);

    delete[] blueChannel;
    delete[] greenChannel;
    delete[] redChannel;

    //Part 9
    unsigned char* redPointer;
    unsigned char* greenPointer;
    unsigned char* bluePointer;

    pictureFile layer_red("input/layer_red.tga");
    pictureFile layer_green("input/layer_green.tga");
    pictureFile layer_blue("input/layer_blue.tga");

    redPointer = layer_red.pixelFile;
    greenPointer = layer_green.pixelFile;
    bluePointer = layer_blue.pixelFile;

    totalBytes = layer_red.height * layer_red.width * 3;

    unsigned char* newPicture9 = new unsigned char[totalBytes];

    for (unsigned int i = 0; i < totalBytes; i += 3) {

        newPicture9[i + 2] = redPointer[i + 2];
        newPicture9[i + 1] = greenPointer[i + 1];
        newPicture9[i] = bluePointer[i];
    }

    layer_red.finalOutput("output/part9.tga", totalBytes, newPicture9);
    delete[] newPicture9;


    //Part 10

    pictureFile text2("input/text2.tga");
    topLayerPointer = text2.pixelFile;
    totalBytes = (text2.height) * (text2.width) * 3;
    unsigned char* newPicture10 = new unsigned char[totalBytes];
    int index = 0;

    for (int i = totalBytes - 1; i >= 0; i -= 3) {

        newPicture10[index] = topLayerPointer[i - 2];   //should be B
        newPicture10[index + 1] = topLayerPointer[i - 1];   //should be G
        newPicture10[index + 2] = topLayerPointer[i];   //should be R
        index = index + 3;;
    }

    text2.finalOutput("output/part10.tga", totalBytes, newPicture10);
    delete[] newPicture10;

    cout << "end\n";
    return 0;
}