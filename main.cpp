#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <windows.h>
#include <queue>
using namespace std;

// 常量，变量， 结构体
#define xLen 800
#define yLen 600
int gameSpeed = 50;
bool capslock = false;

// ai, bfs
bool aiSwitch = false;
int map[xLen / 10][yLen / 10];
int state[xLen / 10][yLen / 10];
typedef struct _step {
    int x, y;
} step;
queue<step> qu;
int dx[4] = {10, 0, -10, 0};
int dy[4] = {0, 10, 0, -10};


struct SNAKE{
    int x[xLen * yLen];
    int y[xLen * yLen];
    int node;  // 蛇长度
    bool die = false;
}snake;


struct FOOD{
    int x,y;
    bool yes = true;
}food;




// 初始化
void initGame(){
    // 设置随机数种子
    srand(time(NULL));
    food.yes = true; // 确保每次新的开始都会刷新食物

    // 蛇风格
    setlinecolor(RED);


    //    初始蛇,下标0为蛇头
    snake.x[0] = 100;
    snake.y[0] = 100;
    snake.x[1] = 90;
    snake.y[1] = 100;
    snake.x[2] = 80;
    snake.y[2] = 100;
    snake.node = 3;
    snake.die = false;

    //    切换大写
    if (!(GetKeyState(VK_CAPITAL) & 0x0001)) {
        // 如果 Caps Lock 当前未被激活，模拟按下 Caps Lock 键来激活大写锁定
        capslock = true;
        keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }
}


static void playGame(){
    char key, tend = 'd';  // 初始方向
    int h_x, h_y; // 存储蛇头坐标


    while (true){
        while (!kbhit()) {
            // 生成食物
            if (food.yes) {
                food.x = rand() % xLen - 10;
                food.y = rand() % yLen - 10;
                // 保证为整格
                while (food.x % 10 != 0 || food.x < 10) ++food.x;
                while (food.y % 10 != 0 || food.y < 10) ++food.y;
                food.yes = false;
            }
            cleardevice();

            // 显示食物
            setfillcolor(GREEN);
            fillrectangle(food.x - 5,food.y + 5,food.x + 5,food.y - 5);

            //    显示蛇
            for (int i = 0; i < snake.node; ++i) {
                i == 0 ? setfillcolor(RED): setfillcolor(WHITE);
                fillrectangle(snake.x[i] - 5, snake.y[i] + 5, snake.x[i] + 5, snake.y[i] - 5);
            }


            // ai换方向
            if (aiSwitch) {
                if (snake.x[0] == food.x && snake.y[0] == food.y) break;
                // 蛇头为起点，食物为终点，蛇身为墙
                int beginX = snake.x[0] / 10, beginY = snake.y[0] / 10, endX = food.x / 10, endY = food.y / 10;
                memset(map, 0, sizeof(map)); // 地图清零
                memset(state, 0, sizeof(state)); // 状态清零

                // 初始化地图
                for (int i = 0; i < snake.node; ++i) {
                    map[snake.x[i] / 10][snake.y[i] / 10] = 1;
                }

                // 记录蛇头出发到达每一格时的前一个格子
                step prev[xLen/10][yLen/10];

                // 蛇头入队
                qu.push({beginX, beginY});
                state[beginX][beginY] = 1;

                while (!qu.empty()) {
                    step head = qu.front();
                    qu.pop();


                    // 判断是否到食物的位置
                    if (head.x == endX && head.y == endY) {
                        // 沿着prev数组回溯到蛇头
                        while (!(prev[head.x][head.y].x == beginX && prev[head.x][head.y].y == beginY)) {
                            head = prev[head.x][head.y];
                        }

                        // 设定下一步的方向
                        if(head.x == beginX && head.y + 1 == beginY) tend = 'w';
                        if(head.x + 1 == beginX && head.y == beginY) tend = 'a';
                        if(head.x == beginX && head.y - 1 == beginY) tend = 's';
                        if(head.x - 1 == beginX && head.y == beginY) tend = 'd';
                        break;
                    }

                    // 搜索当前节点的四个方向
                    for (int f = 0; f < 4; ++f) {
                        int t_x = head.x + dx[f] / 10;
                        int t_y = head.y + dy[f] / 10;

                        if (t_x < 0 || t_y < 0 || t_x >= xLen / 10 || t_y >= yLen / 10) continue; // 越界检查
                        if (map[t_x][t_y]) continue; // 检查是否是蛇身
                        if (state[t_x][t_y]) continue; // 检查是否已经访问过

                        qu.push({t_x, t_y});
                        state[t_x][t_y] = 1;
                        prev[t_x][t_y] = head; // 记录前一步
                    }
                }

                // 清除队列
                while (!qu.empty()) qu.pop();
            }


            h_x = snake.x[0];
            h_y = snake.y[0];
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


            for (int i = snake.node - 1; i > 1; --i) {
                snake.x[i] = snake.x[i-1];
                snake.y[i] = snake.y[i-1];
            }
            snake.x[1] = h_x;
            snake.y[1] = h_y;


            // 吃食物
            if (snake.x[0] == food.x && snake.y[0] == food.y) {
                snake.node++;
                for (int i = snake.node - 1; i > 0; --i) {
                    snake.x[i] = snake.x[i-1];
                    snake.y[i] = snake.y[i-1];
                }
                // 添加食物到蛇头部
                snake.x[0] = food.x;
                snake.y[0] = food.y;
                // 重新生成食物
                food.yes = true;
            }


            // 判断是否撞墙
            if (snake.x[0] < 10 || snake.x[0] > xLen-10|| snake.y[0] < 10 || snake.y[0] > yLen-10) {
                snake.die = true;
                return;
            }

            // 判断自撞，第四截开始判断
            for (int i = 3; i < snake.node; ++i) {
                if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]){
                    snake.die = true;
                    return;
                }
            }

        }


        key = getch();
        // Esc退出，Esc对应ASCII表值为27
        if (key == 27) return;

        // 按下空格暂停
        if (key == ' ')
            while (true){
                outtextxy(xLen/2-50, yLen/2,"游戏已暂停");
                key = getch();
                if (key == 27) return;
                if (key == ' ') break;
            }


        // 换方向
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



bool printScore(){
    char F[100];
    sprintf(F, "得分：%d", snake.node-3 < 0 ? 0: snake.node-3);
    outtextxy(20, 60,F);
    outtextxy(20, 30,"1.结束, 2.继续, 3,AI");
    if (aiSwitch) outtextxy(20, 90, "AI已开启");
    else outtextxy(20,90, "AI已关闭");
    char key;
    while (true) {
        key = getch();
        if (key == 27) {
            return true;
        }
        if (key == '1') {
            return true;
        } else if (key == '2') {
            return false;
        } else if (key == '3') {
            aiSwitch = !aiSwitch;
            printScore();
            return false;
        }
    }
}


void closeGame(){
    outtextxy(0, 0, "Game over!");

    getch();
    // 关闭窗口
    closegraph();

    // 关闭大写
    if (capslock) {
        // 如果 Caps Lock 当前未被激活，模拟按下 Caps Lock 键来激活大写锁定
        keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }
}
int main() {
    bool isPlay = true;
    initgraph(xLen, yLen);
    isPlay =  !printScore();
    while (isPlay) {
        initGame();
        playGame();
        if (printScore()) break;
    }
    closeGame();
    return 0;
}
