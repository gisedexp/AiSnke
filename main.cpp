#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <windows.h>
using namespace std;

// ������������ �ṹ��
#define xLen 800
#define yLen 600
int gameSpeed = 50;
bool capslock = false;
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
            // �ж��Ƿ�ײǽ
            if (snake.x[0] < 10 || snake.x[0] > xLen-10|| snake.y[0] < 10 || snake.y[0] > yLen-10) {
                snake.die = true;
                break;
            }
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

        }
        if (snake.die) {  // ������
            break;
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

    }
}



bool printScore(){
    char F[100];
    sprintf(F, "�÷֣�%d", snake.node-3);
    outtextxy(20, 60,F);
    outtextxy(20, 30,"1.����, 2.����");
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
    int i = 0;
    initgraph(xLen, yLen);

    while (true) {
        initGame();
        playGame();
        if (printScore()) break;
    }
    closeGame();
    return 0;
}
