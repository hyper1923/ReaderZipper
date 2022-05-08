#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>




struct FILE_H {
    char fileName[24];
    uint32_t fileSize = 0;
    char* fileData;
};

std::vector<FILE_H*> files;

uint32_t writedFiles = 0;


void Write(const char* fileName){
    /*
    * Getting the file size.
    */
    FILE* f = fopen(fileName,"rb");
    fseek(f, 0, SEEK_END);
    uint32_t fileSize = ftell(f);
    fclose(f);

    /*
    * Make some settings about new file.
    */
    FILE_H* file = new FILE_H();
    strcpy(file->fileName, fileName);
    file->fileSize = fileSize;
    file->fileData = (char*)malloc(fileSize);

    f = fopen(fileName, "rb");
    fread(file->fileData, fileSize, 1, f);
    fclose(f);


    files.push_back(file);
}

void Export() {
    FILE* f = fopen("Data.bin", "wb");
    fwrite("PACN", 4, 1, f);
    uint32_t filesTotal = files.size();
    fwrite(&filesTotal, 4, 1, f);
    for (int i = 0; i < files.size(); i++) {
        fwrite(files.at(i)->fileName, 24, 1, f);
        fwrite(&files.at(i)->fileSize, 4, 1, f);
        fwrite(files.at(i)->fileData, files.at(i)->fileSize, 1, f);
    }
    fclose(f);
}


void Read() {
    FILE* f = fopen("Data.bin", "rb");
    char header[4];
    fread(header, 4, 1, f);
    uint32_t totalFiles = 0;
    fread(&totalFiles, 4, 1, f);


    for (int i = 0; i < totalFiles; i++) {
        char name[24];
        FILE_H* file = new FILE_H;
        uint32_t fileSize = 0;
        fread(name, 24, 1, f);
        fread(&fileSize, 4, 1, f);
        strcpy(file->fileName, name);
        file->fileSize = fileSize;
        file->fileData = (char*)malloc(fileSize);
        fread(file->fileData, fileSize, 1, f);
        files.push_back(file);
    }

    for (int i = 0; i < files.size(); i++) {
        printf("[%s] [%d KB(s)]\n", files.at(i)->fileName, files.at(i)->fileSize / 1024);
    }
    fclose(f);
}

void ExportFile(const char *name) {
    for (int i = 0; i < files.size(); i++) {
        if (strcmp(name,files.at(i)->fileName) == 0) {
            FILE* f = fopen(name, "wb");
            fwrite(files[i]->fileData, files[i]->fileSize, 1, f);
            fclose(f);
        }
    }
}


int main(int argc, char** argv)
{
    for (int i = 1; i <= argc; i++) {
        if (strcmp(argv[i], "-write") == 0) {
            Write(argv[i + 1]);
        }
        if (strcmp(argv[i], "-export") == 0) {
            Export();
        }
        if (strcmp(argv[i], "-read") == 0) {
            Read();
        }
        if (strcmp(argv[i], "-exportfile") == 0) {
            ExportFile(argv[i+1]);
        }
    }
}