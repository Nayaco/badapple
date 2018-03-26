#include <windows.h>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <cmath>
#include <iostream>
using namespace std;
const int picnum = 1754;
typedef struct map{
    uint32_t h,w;
}MAP;
MAP mapinfo;
RGBQUAD *pixels;
bool READFILE(const char* filename){
    FILE *f = fopen(filename,"rb");
    BITMAPINFOHEADER bi;
    BITMAPFILEHEADER bf;
    fread(&bf,sizeof(BITMAPFILEHEADER),1,f);
    fread(&bi,sizeof(BITMAPINFOHEADER),1,f);
    if(bi.biBitCount!=24)return 0;
    if(bi.biCompression!=BI_RGB)return 0;
    int height = bi.biHeight;
    int width = bi.biWidth;
    mapinfo.h = height;
    mapinfo.w = width;
    pixels = new RGBQUAD[height*width];
    uint32_t rowSize = (bi.biBitCount * width + 31) / 32 * 4;
    uint8_t *line = new uint8_t[rowSize];
    for(int y =  0; y < height; y++){
        fread(line,rowSize,1,f);
        for(int x = 0; x < width; x++){
            uint8_t *color = line + x*3;  
            RGBQUAD *pixel = &pixels[(height-y-1) * width+x];
            pixel->rgbBlue = color[0];
            pixel->rgbGreen = color[1];
            pixel->rgbRed = color[2];
        }
    }

    delete[] line;
    fclose(f);
    return 1;
}
RGBQUAD getcolor(int x, int y, int w, int h,const RGBQUAD *pixels, MAP info){
    int r = 0, g = 0, b = 0;
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            RGBQUAD pixel = pixels[(i+y)*info.w + j+x];
            r += pixel.rgbRed;
            g += pixel.rgbGreen;
            b += pixel.rgbBlue;
        }
    }
    return RGBQUAD{(BYTE)(r/(h*w)),(BYTE)(g/(h*w)),(BYTE)(b/(h*w))};
}

char pixeltochar(const RGBQUAD pixel){
    static char const *colors = "Qdogc*;:-. ";
    int len = strlen(colors);
    int level = 0xFF / len;
    int grey =  (pixel.rgbBlue+pixel.rgbGreen+pixel.rgbRed)/3;
    int greylevel = grey/level;
    if(greylevel>=len)greylevel = len-1;
    return colors[greylevel];
}
void makemap(const char *filename,const MAP info, int zoom, const RGBQUAD *pixels){
    FILE* f = fopen(filename,"a+");
    int Wid = info.w/zoom;
    int Hig = info.h/zoom;
    for(int y = 0; y < info.h/zoom; y++){
        for(int x = 0; x < info.w/zoom; x++){
            RGBQUAD pixel = getcolor(x*zoom,y*zoom,zoom,zoom,pixels,info);
            fprintf(f,"%c",pixeltochar(pixel));
            //printf("%c",pixeltochar(pixel));
        }
        fprintf(f,"\n");
        //printf("\n");
    }
    fclose(f);
    return;
}
int main(){
    char filename[255];
    for(int i = 0; i < picnum; i++){
        printf("%d  ",i+1);
        if((i+1)%8==0)printf("\n");
        sprintf(filename,"p/%04d.bmp",i+1);
        READFILE(filename);
        makemap("badapple.txt",mapinfo,8,pixels);
    }
    return 0;
}