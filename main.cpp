#include <iostream>
#include <fstream>
#include <cstdlib> // дл€ использование генератора случайных чисел
#include <ctime>   // функции времени для инициализации генератора с. ч.
#include <locale>
#include <ctype.h>
#include <stdio.h>



using namespace std;

struct Result {
    int result;
    int x;
    int y;
};

struct Previous {
    int previousX = -1;
    int previousY = -1;
    // true - по горизонтали, false - по вертикали;
    bool direction;
    int counter = 0;
};

struct Point {
    int x;
    int y;
};


void initBoard();
void readBoard(int**, int);
bool isHit(int**, int, int);
Result hit(Previous*, int**, Point* (*)());
int hit(int**, int, int);
int checkShip(int**, int, int);
void printBoard(int**);
Result hitAgain(Previous*, int, int, int**);
int** getSyncBoard(int**);
int final(int**);
void noShot(int**, int, int);
char up(char);

Point* randdSimple();
Point* randdCross();
Point* randdCross2();

//‘актическа€ доска игрока.
int** board1 = new int*[10];    // массив указателей;
//‘актическа€ доска компьютера.
int** board2 = new int*[10];    // массив указателей;
//ƒоска игрока.
int** board3 = new int*[10];    // массив указателей;
//ƒоска компьютера.
int** board4 = new int*[10];    // массив указателей;

int** randoma = new int*[10];

int**** sync = new int***[2];

//¬ыбор алгоритма игры
Point* (*randdAlgoritm[])() = {&randdSimple, &randdCross, &randdCross2};


int main()
{
    srand(unsigned(time(0))); // инициализации генератора с. ч.
    for (int i = 0; i < 10; i++) {
        board1[i] = new int[10];     // инициализация указателей
    }

    for (int i = 0; i < 10; i++) {
        board2[i] = new int[10];     // инициализация указателей
    }

    for (int i = 0; i < 10; i++) {
        board3[i] = new int[10];     // инициализация указателей
    }

    for (int i = 0; i < 10; i++) {
        board4[i] = new int[10];     // инициализация указателей
    }

    for (int i = 0; i < 10; i++) {
        randoma[i] = new int[10];     // инициализация указателей
    }

    sync[0] = new int**[2];
    sync[1] = new int**[2];

    sync[0][0] = board3;
    sync[0][1] = board2;
    sync[1][0] = board4;
    sync[1][1] = board1;

    setlocale(LC_ALL, "Russian");

    //initBoard();
    readBoard(board1, 1);
    readBoard(board2, 2);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board3[i][j] = 0;
            board4[i][j] = 0;
        }
    }

    //hit();

    printBoard(board1);
    cout << "=================================" << endl;
    printBoard(board3);

    Previous computer1;
    Previous computer2;


    Point* (*computer1Algoritm) () = randdCross2;
    Point* (*computer2Algoritm) () = randdCross2;

    while(true) {
        int x;
        char X;
        int y;
        int human = -1;


        do {
            cout << "x = ";
            cin >> X ;
            x = up(X) - 'A';
            cout << "y = ";
            cin >> y;
            y--;
            if (x > 9 || x < 0 || y > 9 || y < 0) {
                cout << "!!!" << endl;
                continue;
            }
            human = hit(board3, x, y);
            cout << "You: " << endl << "8 is missed(result = 0)" << endl << "2 is damaged(result = 1, and if it was killed = 2) " << endl << "7 is blind zone " << endl << "x: " << (char)(x + 'A') << " y: " << (y + 1) << " result: " << human << endl;
            printBoard(board3);
            if (human == 4) {
                getchar();
                //	exit(0);
            }
        } while (human != 0);

        Result computer;

        do {
            computer = hit(&computer2, board3, computer2Algoritm);
            cout << "Computer2: " << endl << "x: " << (char)(computer.x + (int)'A') << " y: " << computer.y+1 << " result: " << computer.result << endl;
            if (computer.result != 3) {
                printBoard(board3);
                //		printBoard(board2);
            }
            if (computer.result == 4) {
                getchar();
                //		exit(0);
            }
        } while (computer.result != 0);


        /*do {
            computer = hit(&computer1, board4, computer1Algoritm);
            cout << "Computer1: " << endl << "x: " << (char)(computer.x + (int)'A') << " y: " << computer.y+1 << " result: " << computer.result << endl;
            if (computer.result != 3) {
                printBoard(board4);
                //printBoard(board1);
            }
            if (computer.result == 4) {
                getchar();
        //		exit(0);
            }
        } while (computer.result != 0);*/
    }




    getchar();
    return 0;
}


/*
вспомогательный метод, чтобы нарисовать пустую доску.
#Tool
*/
void initBoard() {
    ofstream writefile("board.txt"/*, ios_base::app*/); // создаем объект класса ofstream для записи и связываем его с файлом cppstudio.txt
    if (!writefile.is_open()) // если файл небыл открыт
    {
        cout << "файл не может быть открыт или создан\n"; // напечатать соответствующее сообщение
    }

    writefile << " \tA B C D E F G H I J" << endl; // запись строки в файл
    for (int i = 1; i < 11; i++) {
        writefile << i << "\t0 0 0 0 0 0 0 0 0 0" << endl;
    }

    writefile.close(); // закрываем файл
}

/*
распечатка выбранной доски.
#Tool
*/
void printBoard(int** board) {

    cout << " \tA B C D E F G H I J" << endl;
    for (int i = 1; i < 11; i++) {
        cout << i << "\t";
        for (int j = 0; j < 10; j++) {
            cout << board[i-1][j] << " ";
        }
        cout << endl;
    }
}

/*
«заполнение доски из файла.
#Tool
*/
void readBoard(int** board, int number) {

    char buff[50]; // буфер промежуточного хранения считываемого из файла текста

    sprintf_l(buff, "board%d.txt", number);
    ifstream readfile(buff);

    if (!readfile.is_open()) // если файл не открыть
        cout << "файл не может быть открыт!\n"; // сообщить об этом
    else
    {
        readfile.getline(buff, 50); // папка

        for (int i = 0; i < 10; i++) {
            int helper;
            readfile >> helper;
            for (int j = 0; j < 10; j++) {
                readfile >> board[i][j];
            }
        }


        readfile.close(); // закрываем файл
        //cout << buff << endl; // напечатали эту строку
    }
}

/*
попал/не попал.
#Referee
*/
bool isHit(int** board, int x, int y) {
    return board[y][x] != 0;
}

/*
¬ыстрел.
0 - не попал
1 - –анил
2 - подбил
3 - нельзя
4 - игра закончена
#Referee
*/
int hit(int** board, int x, int  y) {

    //доска с реальными коробл€ми.
    int** syncBoard = getSyncBoard(board);

    if (!isHit(syncBoard, x, y)) {
        board[y][x] = 8;
        syncBoard[y][x] = 8;
        return 0;
    }

    // проверяем, если выстрелили в 2 или 8.
    if (syncBoard[y][x] == 2 || syncBoard[y][x] == 8) {
        return 3;
    }

    board[y][x] = 2;
    syncBoard[y][x] = 2;
    int result = checkShip(syncBoard, x, y);
    int isWin = final(syncBoard);
    return isWin > result ? isWin : result;
}


/*
ѕроверка состо€ни€ корабл€.
1 - раненный
2 - подбитый
3 - ошибка
#Referee
*/
int checkShip(int** board, int x, int y) {
    if (board[y][x] == 2) {
        if (y != 9) { // не попали на нижнюю границу
            if (board[y + 1][x] != 0 && board[y + 1][x] != 8) { //вертекальный
                int n = 1;
                while (y + ++n < 10 && board[y + n][x] != 0 && board[y + n][x] != 8) { // ++n - увеличить и потом использовать
                }
                //анализ
                while (y + --n >= 0 && board[y + n][x] != 0 && board[y + n][x] != 8) {
                    if (board[y + n][x] == 1) {
                        return 1;
                    }
                }
                return 2;
            } else if (y != 0 && board[y - 1][x] != 0 && board[y - 1][x] != 8) { // вертекальный
                int n = 1;
                //анализ
                while (y + --n >= 0 && board[y + n][x] != 0 && board[y + n][x] != 8) {
                    if (board[y + n][x] == 1) {
                        return 1;
                    }
                }
                return 2;
            } else {
                if (x != 9) { // не попали на правую границу
                    if (board[y][x + 1] != 0 && board[y][x + 1] != 8) { // горизонтальный.
                        int n = 1;
                        while (x + ++n < 10 && board[y][x + n] != 0 && board[y][x + n] != 8) { // ++n - увеличить и потом использовать
                        }
                        //јнализ
                        while (x + --n >= 0 && board[y][x + n] != 0 && board[y][x + n] != 8) {
                            if (board[y][x + n] == 1) {
                                return 1;
                            }
                        }
                        return 2;
                    } else if (board[y][x - 1] != 0 && board[y][x - 1] != 8) { // горизонтальный.
                        int n = 1;
                        //анализ
                        while (x + --n >= 0 && board[y][x + n] != 0 && board[y][x + n] != 8) {
                            if (board[y][x + n] == 1) {
                                return 1;
                            }
                        }
                        return 2;
                    } else {
                        return 2;
                    }
                } else { // x = 9 попали на правую границу.
                    if (board[y][x - 1] != 0 && board[y][x - 1] != 8) { // горизонтальный.
                        int n = 1;
                        //јнализ
                        while (x + --n >= 0 && board[y][x + n] != 0 && board[y][x + n] != 8) {
                            if (board[y][x + n] == 1) {
                                return 1;
                            }
                        }
                        return 2;
                    } else {
                        return 2;
                    }
                }
            }
        } else { //y = 9 попали на нижнюю границу.
            if (board[y - 1][x] != 0 && board[y - 1][x] != 8) {
                int n = 1;
                //анализ
                while (y + --n >= 0 && board[y + n][x] != 0 && board[y + n][x] != 8) {
                    if (board[y + n][x] == 1) {
                        return 1;
                    }
                }
                return 2;
            } else { // не вертикальный.
                if (x != 9) { // не попали на правую границу
                    if (board[y][x + 1] != 0 && board[y][x + 1] != 8) { // горизонтальный.
                        int n = 1;
                        while (x + ++n < 10 && board[y][x + n] != 0 && board[y][x + n] != 8) { // ++n - увеличить и потом использовать
                        }
                        //јнализ
                        while (x + --n >= 0 && board[y][x + n] != 0 && board[y][x + n] != 8) {
                            if (board[y][x + n] == 1) {
                                return 1;
                            }
                        }
                        return 2;
                    }
                    else if (board[y][x - 1] != 0 && board[y][x - 1] != 8) { // горизонтальный.
                        int n = 1;
                        //јнализ
                        while (x + --n >= 0 && board[y][x + n] != 0 && board[y][x + n] != 8) {
                            if (board[y][x + n] == 1) {
                                return 1;
                            }
                        }
                        return 2;
                    }
                    else { // для однопалубных.
                        return 2;
                    }
                }
                else { // x = 9 попали на правую границу.
                    if (board[y][x - 1] != 0 && board[y][x - 1] != 8) { // горизонтальный.
                        int n = 1;
                        //јнализ
                        while (x + --n >= 0 && board[y][x + n] != 0 && board[y][x + n] != 8) {
                            if (board[y][x + n] == 1) {
                                return 1;
                            }
                        }
                        return 2;
                    }
                    else { // для однопалубных.
                        return 2;
                    }
                }
            }
        }


    } else if (board[y][x] == 1) {
        return 1;
    }

    return 3;
}

/*
ѕроизвольный выстрел.
prev - данные о предыдущем выстреле.
previousX - исходная предыдуща€ координата X.
previousY - исходная предыдуща€ координата Y.
board - текущая доска.
#Computer
*/
Result hitAgain(Previous* prev, int previousX, int previousY, int** board) {
    srand(unsigned(time(0))); // инициализации генератора с. ч.
    int x;
    int y;
    int a;
    int newX;
    int newY;
    if (prev->counter == 0) { //ѕервый выстрел.
        while (true) {
            a = rand() % 4;
            x = a > 1 ? 2 * (a - 2) - 1 : 0;
            y = a < 2 ? 2 * a - 1 : 0;
            newX = previousX + x;
            newY = previousY + y;
            if (newX < 0 || newX > 9 || newY < 0 || newY > 9) { // «а пределы пол€.
                continue;
            }
            if (isHit(board, newX, newY)) { // ”же стрел€л.
                continue;
            }

            break;
        }
    } else { //¬торой и далее выстрелы.
        if (prev->direction == true) {
            newX = previousX + prev->counter / abs(prev->counter);
            newY = previousY;
            if (newX < 0 || newX > 9 || isHit(board, newX, newY)) {
                newX -= 2*prev->counter/abs(prev->counter) + prev->counter;
                prev->counter = -prev->counter / abs(prev->counter);
            }
        }
        else {
            newX = previousX;
            newY = previousY + prev->counter / abs(prev->counter);
            if (newY < 0 || newY > 9 || isHit(board, newX, newY)) {
                newY -= 2 * prev->counter / abs(prev->counter) + prev->counter;
                prev->counter = -prev->counter / abs(prev->counter);
            }
        }
    }
    int result = hit(board, newX, newY);

    if (result == 1) {

        if (prev->counter == 0) {
            prev->counter = x + y; //(x < 0 || y < 0) ? -1 : 1;
        }
        else {
            prev->counter += prev->counter > 0 ? 1 : -1;
        }

        if (prev->counter == 1 || prev->counter == -1) {

            prev->direction = x == 0 ? false : true;
        }

        prev->previousX = newX;
        prev->previousY = newY;

        /*
        ::previousX = newX; // :: - обращение к глобальной переменной.
        ::previousY = newY;*/

    }

    if (result == 2) {

        prev->previousX = -1;
        prev->previousY = -1;

        /*
        ::previousX = -1;
        ::previousY = -1;*/

        prev->counter = 0;
    }
    Result hitResult;
    hitResult.result = result;
    hitResult.x = newX;
    hitResult.y = newY;
    return hitResult;
}

/*
ѕроизвольный выстрел.
prev - данные о предыдущем выстреле.
board - текущая доска.
randd - функция случайного выбора точки.
#Computer
*/
Result hit(Previous* prev, int** board, Point* (*randd)()) {
    Result hitResult;

    if (prev->previousX != -1 && (*prev).previousY != -1) {
        hitResult = hitAgain(prev, prev->previousX, prev->previousY, board);
    } else {


        int x;
        int y;
        do {
            Point* point = randd();
            x = point->x;
            y = point->y;
        } while (board[y][x] == 2 || board[y][x] == 8 || board[y][x] == 7);

        int result = hit(board, x, y);

        if (result == 1) {
            prev->previousX = x;
            prev->previousY = y;
        }

        if (result == 2) {
            prev->previousX = -1;
            prev->previousY = -1;
        }
        //if (3)

        hitResult.result = result;
        hitResult.x = x;
        hitResult.y = y;
    }

    if (hitResult.result == 2) {
        noShot(board, hitResult.x, hitResult.y);
    }

    return hitResult;
}


int** getSyncBoard(int** board) {
    for (int i = 0; i < 2; i++) {
        if (sync[i][0] == board) {
            return sync[i][1];
        }
    }
}

int final(int** board) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (board[i][j] == 1) {
                return -4;
            }
        }
    }
    cout << "Game is over" << endl;
    return 4;
}

void noShot(int** board,int x,int y) {
    if (board[y][x] == 2) {
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (y + j >= 0 && y + j <= 9 && x + i >= 0 && x + i <= 9) {
                    if (board[y + j][x + i] == 0) {
                        board[y + j][x + i] = 7;
                    }
                    if (board[y + j][x + i] == 2) {
                        board[y][x] = 7;
                        noShot(board, x + i, y + j);
                        board[y][x] = 2;
                    }
                }
            }
        }
    }
}

char up(char z) {
    if (z <= 'Z' && z >= 'A') {
        return z;
    }
    return z + 'A' - 'a';
}

Point* randdSimple() {
    Point point;
    srand(unsigned(rand()*time(0))); // инициализации генератора с. ч.
    point.x = rand() % 10;
    srand(unsigned(rand()*time(0))); // инициализации генератора с. ч.
    point.y = rand() % 100 / 10;
    return &point;
}
int counter = 0;

Point* randdCross() {
    Point point;
    if (counter > 19) {
        point = *randdSimple();
        return &point;
    }
    if (counter < 10) {
        point.x = counter;
        point.y = counter++;
    } else {
        point.x = 19 - counter++;
        point.y = 9 - point.x;
    }


    return &point;
}
int counter1 = 0;

Point* randdCross2() {
    Point point;

    if (counter1 > 23) {
        point = *randdSimple();
        return &point;
    }

    if (counter1 < 4) {
        point.x = counter1;
        point.y = -counter1 + 3 ;
    }

    if (counter1 > 3 && counter1 < 12) {
        point.x = counter1 - 4 ;
        point.y = -counter1 + 11 ;
    }

    if (counter1 > 11 && counter1 < 20) {
        point.x = counter1 - 10;
        point.y = -counter1 + 21;
    }
    if (counter1 > 19 && counter1 < 24) {
        point.x = counter1 - 14;
        point.y = -counter1 + 29;
    }
    counter1++;
    return &point;
}
