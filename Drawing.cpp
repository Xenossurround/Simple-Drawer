#include <graphics.h>
#include <conio.h>
#include <math.h>

#define BLACK RGB(0, 0, 0)
#define WHITE RGB(255, 255, 255)
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define BLUE RGB(0, 0, 255)

enum Tool { PEN, RECTANGLE, CIRCLE, ERASER };

Tool currentTool = PEN;
COLORREF currentColor = BLACK;
int startX, startY;
bool isDrawing = false;
int previewX, previewY; 
IMAGE canvasBuffer;

void initCanvas() {
    initgraph(800, 600);
    setbkcolor(WHITE);   
    cleardevice();      
}

void saveCanvasToBuffer() {
    getimage(&canvasBuffer, 0, 0, 800, 600);
}

void restoreCanvasFromBuffer() {
    putimage(0, 0, &canvasBuffer);
}

void drawLine(int x1, int y1, int x2, int y2) {
    setlinecolor(currentColor);
    line(x1, y1, x2, y2);
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    setlinecolor(currentColor);
    rectangle(x1, y1, x2, y2);
}

void drawCircle(int x1, int y1, int x2, int y2) {
    setlinecolor(currentColor);
    int radius = (int)sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    circle(x1, y1, radius);
}

void erase(int x, int y) {
    setlinecolor(WHITE);
    setfillcolor(WHITE);
    fillcircle(x, y, 10); 
}

void clearPreview() {
    
    restoreCanvasFromBuffer();
}

void drawPreview() {
    if (currentTool == RECTANGLE) {
        drawRectangle(startX, startY, previewX, previewY);
    }
    else if (currentTool == CIRCLE) {
        drawCircle(startX, startY, previewX, previewY);
    }
}


int main() {
    initCanvas();

    while (true) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();

            if (msg.uMsg == WM_LBUTTONDOWN) {
                isDrawing = true;
                startX = msg.x;
                startY = msg.y;

                saveCanvasToBuffer();
            }

            if (msg.uMsg == WM_LBUTTONUP) {
                isDrawing = false;

                if (currentTool == RECTANGLE) {
                    drawRectangle(startX, startY, msg.x, msg.y);
                }
                else if (currentTool == CIRCLE) {
                    drawCircle(startX, startY, msg.x, msg.y);
                }
            }

            if (msg.uMsg == WM_MOUSEMOVE) {
                if (isDrawing) {
                   
                    if (currentTool == PEN) {
                        drawLine(startX, startY, msg.x, msg.y);
                        startX = msg.x; 
                        startY = msg.y;
                    }
                    else if (currentTool == ERASER) {
                        erase(msg.x, msg.y);
                    }
                    else if (currentTool == RECTANGLE || currentTool == CIRCLE) {
                        clearPreview(); 
                        previewX = msg.x;
                        previewY = msg.y;
                        drawPreview(); 
                    }
                }
            }
        }

        
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
            case '1': currentTool = PEN; break;
            case '2': currentTool = RECTANGLE; break;
            case '3': currentTool = CIRCLE; break;
            case '4': currentTool = ERASER; break;
            case 'r': currentColor = RED; break;
            case 'g': currentColor = GREEN; break;
            case 'b': currentColor = BLUE; break;
            case 'c': cleardevice(); break; 
            case 27: return 0; 
            }
        }
    }

    closegraph(); 
    return 0;
}
