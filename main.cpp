#include "windows.h"
#include "window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
//int main(int argc, char** argv) {
  MainWindow program;
  program.loop();
  return 0;
}