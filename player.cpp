#include <windows.h>
#include <fstream>
#include <time.h>
#include <mmsystem.h>
#include <iostream>
#define LL long long
using namespace std;
const int FPS = 8; 
const int Width = 64;
const int Height = 48;
static const char _Filename[13] = "badapple.txt";
static const char _Audioname[13] = "badapple.wav";
struct fpsctr{
    int _fps;
    LL _nowTime;
    int _frame;
    int _argTime;
    LL _fromTime;
    fpsctr(){}
    fpsctr(int fps) {_fps = fps;_nowTime = GetTickCount();_argTime = 1000/_fps;}
    void reset(){_nowTime = GetTickCount();_frame = 0;_fromTime = _nowTime;}
    void delay(){
        _frame++;
        _nowTime = GetTickCount();
        int _btime = _fromTime + _frame*_argTime - _nowTime;
        if(_btime>0)Sleep(_btime);
    }
};
void _Resetcurse(){
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle,coord);
}
void _Audioplayer(const char *_filename);
void _Loop(const char *_filename,const int _fps,const int _height){
    FILE *f = fopen(_filename,"r");
    if(f==NULL)exit(1);
    char _buffer[512];
    fpsctr _ctr(_fps);
    _Audioplayer(_Audioname);
    Sleep(10);
    _ctr.reset();
    while(!feof(f)){
        _Resetcurse();
        for(int i = 0; i < _height; i++){
            fgets(_buffer, 512, f);
            cout<<_buffer;
        }
        _ctr.delay();
    }
}
void _Audioplayer(const char *_filename){
    sndPlaySound(_filename, SND_FILENAME | SND_ASYNC);
}
int main(){
    ios::sync_with_stdio(false);

    _Loop(_Filename,FPS,Height);
    return 0;
}