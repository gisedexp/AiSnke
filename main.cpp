#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <windows.h>
#include <queue>
using namespace std;

// ������������ �ṹ��
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
    int node;  // �߳���
    bool die = false;
}snake;


struct FOOD{
    int x,y;
    bool yes = true;
}food;




// ��ʼ��
void initGame(){
    // �������������
    srand(time(NULL));
    food.yes = true; // ȷ��ÿ���µĿ�ʼ����ˢ��ʳ��

    // �߷��
    setlinecolor(RED);


    //    ��ʼ��,�±�0Ϊ��ͷ
    snake.x[0] = 100;
    snake.y[0] = 100;
    snake.x[1] = 90;
    snake.y[1] = 100;
    snake.x[2] = 80;
    snake.y[2] = 100;
    snake.node = 3;
    snake.die = false;

    //    �л���д
    if (!(GetKeyState(VK_CAPITAL) & 0x0001)) {
        // ��� Caps Lock ��ǰδ�����ģ�ⰴ�� Caps Lock ���������д����
        capslock = true;
        keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }
}


static void playGame(){
    char key, tend = 'd';  // ��ʼ����
    int h_x, h_y; // �洢��ͷ����


    while (true){
        while (!kbhit()) {
            // ����ʳ��
            if (food.yes) {
                food.x = rand() % xLen - 10;
                food.y = rand() % yLen - 10;
                // ��֤Ϊ����
                while (food.x % 10 != 0 || food.x < 10) ++food.x;
                while (food.y % 10 != 0 || food.y < 10) ++food.y;
                food.yes = false;
            }
            cleardevice();

            // ��ʾʳ��
            setfillcolor(GREEN);
            fillrectangle(food.x - 5,food.y + 5,food.x + 5,food.y - 5);

            //    ��ʾ��
            for (int i = 0; i < snake.node; ++i) {
                i == 0 ? setfillcolor(RED): setfillcolor(WHITE);
                fillrectangle(snake.x[i] - 5, snake.y[i] + 5, snake.x[i] + 5, snake.y[i] - 5);
            }


            // ai������
            if (aiSwitch) {
                if (snake.x[0] == food.x && snake.y[0] == food.y) break;
                // ��ͷΪ��㣬ʳ��Ϊ�յ㣬����Ϊǽ
                int beginX = snake.x[0] / 10, beginY = snake.y[0] / 10, endX = food.x / 10, endY = food.y / 10;
                memset(map, 0, sizeof(map)); // ��ͼ����
                memset(state, 0, sizeof(state)); // ״̬����

                // ��ʼ����ͼ
                for (int i = 0; i < snake.node; ++i) {
                    map[snake.x[i] / 10][snake.y[i] / 10] = 1;
                }

                // ��¼��ͷ��������ÿһ��ʱ��ǰһ������
                step prev[xLen/10][yLen/10];

                // ��ͷ���
                qu.push({beginX, beginY});
                state[beginX][beginY] = 1;

                while (!qu.empty()) {
                    step head = qu.front();
                    qu.pop();


                    // �ж��Ƿ�ʳ���λ��
                    if (head.x == endX && head.y == endY) {
                        // ����prev������ݵ���ͷ
                        while (!(prev[head.x][head.y].x == beginX && prev[head.x][head.y].y == beginY)) {
                            head = prev[head.x][head.y];
                        }

                        // �趨��һ���ķ���
                        if(head.x == beginX && head.y + 1 == beginY) tend = 'w';
                        if(head.x + 1 == beginX && head.y == beginY) tend = 'a';
                        if(head.x == beginX && head.y - 1 == beginY) tend = 's';
                        if(head.x - 1 == beginX && head.y == beginY) tend = 'd';
                        break;
                    }

                    // ������ǰ�ڵ���ĸ�����
                    for (int f = 0; f < 4; ++f) {
                        int t_x = head.x + dx[f] / 10;
                        int t_y = head.y + dy[f] / 10;

                        if (t_x < 0 || t_y < 0 || t_x >= xLen / 10 || t_y >= yLen / 10) continue; // Խ����
                        if (map[t_x][t_y]) continue; // ����Ƿ�������
                        if (state[t_x][t_y]) continue; // ����Ƿ��Ѿ����ʹ�

                        qu.push({t_x, t_y});
                        state[t_x][t_y] = 1;
                        prev[t_x][t_y] = head; // ��¼ǰһ��
                    }
                }

                // �������
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


            // ��ʳ��
            if (snake.x[0] == food.x && snake.y[0] == food.y) {
                snake.node++;
                for (int i = snake.node - 1; i > 0; --i) {
                    snake.x[i] = snake.x[i-1];
                    snake.y[i] = snake.y[i-1];
                }
                // ���ʳ�ﵽ��ͷ��
                snake.x[0] = food.x;
                snake.y[0] = food.y;
                // ��������ʳ��
                food.yes = true;
            }


            // �ж��Ƿ�ײǽ
            if (snake.x[0] < 10 || snake.x[0] > xLen-10|| snake.y[0] < 10 || snake.y[0] > yLen-10) {
                snake.die = true;
                return;
            }

            // �ж���ײ�����Ľؿ�ʼ�ж�
            for (int i = 3; i < snake.node; ++i) {
                if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]){
                    snake.die = true;
                    return;
                }
            }

        }


        key = getch();
        // Esc�˳���Esc��ӦASCII��ֵΪ27
        if (key == 27) return;

        // ���¿ո���ͣ
        if (key == ' ')
            while (true){
                outtextxy(xLen/2-50, yLen/2,"��Ϸ����ͣ");
                key = getch();
                if (key == 27) return;
                if (key == ' ') break;
            }


        // ������
        if ( (key == 'a' || key == 'A') && tend != 'd') {
            tend = 'a';
        } else if ( (key == 'd' || key == 'D') && tend != 'a') {
            tend = 'd';
        } else if ( (key == 'w' || key == 'W') && tend != 's') {
            tend = 'w';
        } else if ( (key == 's' || key == 'S') && tend != 'w') {
            tend = 's';
        }

        if (snake.die) {  // ������
            break;
        }

    }
}



bool printScore(){
    char F[100];
    sprintf(F, "�÷֣�%d", snake.node-3 < 0 ? 0: snake.node-3);
    outtextxy(20, 60,F);
    outtextxy(20, 30,"1.����, 2.����, 3,AI");
    if (aiSwitch) outtextxy(20, 90, "AI�ѿ���");
    else outtextxy(20,90, "AI�ѹر�");
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
    // �رմ���
    closegraph();

    // �رմ�д
    if (capslock) {
        // ��� Caps Lock ��ǰδ�����ģ�ⰴ�� Caps Lock ���������д����
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
