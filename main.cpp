#include <graphics.h>
#include <conio.h>
#include <iostream>
using namespace std;
#define LEN 10

#define xLen 800
#define yLen 600
const int gameSpeed = 100;
struct SNAKE{
    int x[xLen * yLen];
    int y[xLen * yLen];
    int node;  // 蛇长度
    bool die = false;
}snake;


void initGame(){
//    初始化窗口
    initgraph(xLen, yLen);

//    初始蛇
    snake.x[0] = 100;
    snake.y[0] = 100;
    snake.node = 1;
}


void playGame(){
    char key, tend = 'd';  // 初始方向



    while (true){
        while (!kbhit()) {
            //    显示初始蛇
            cleardevice();
            fillrectangle(snake.x[0] - 5, snake.y[0] + 5, snake.x[0] + 5, snake.y[0] - 5);
            cout << snake.x[0] << ' ' << snake.y[0] << ' ' << tend << endl;
            Sleep(gameSpeed);
            switch (tend) {
                case 'd': {
                    snake.x[0] += 10;
                    break;
                }
                case 'a': {
                    snake.x[0] -= 10;
                    break;
                }
                case 'w': {
                    snake.y[0] -= 10;
                    break;
                }
                case 's': {
                    snake.y[0] += 10;
                    break;
                }


            }
        }

        key = getch();
        if ( (key == 'a' || key == 'A') && tend != 'd') {
            tend = 'a';
        } else if ( (key == 'd' || key == 'D') && tend != 'a') {
            tend = 'd';
        } else if ( (key == 'w' || key == 'W') && tend != 's') {
            tend = 'w';
        } else if ( (key == 's' || key == 'S') && tend != 'w') {
            tend = 's';
        }


        if (snake.die) {  // 蛇死亡
            break;
        }


    }
}

void closeGame(){
    //    关闭窗口
    closegraph();
}
int main() {
    initGame();
    playGame();
    closeGame();

    return 0;
}
