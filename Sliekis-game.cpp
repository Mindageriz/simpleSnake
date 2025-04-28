#include <iostream>
#include <conio.h> // _getch() funkcijai, user input
#include <windows.h> // Sleep() funkcijai, delsimas
#include <fstream> // Highscore saugojimui teksto faile
#include <ctime> //time funkcija naudojama nustatyti vis skirtingą seed'a random funkcijai

using namespace std;

//pagrindiniai kintamieji
bool gameover;
const int plotis = 20;
const int aukstis = 10;
int x, y, vaisiusX, vaisiusY, score, highscore;
int uodegeleX[100], uodegeleY[100];
int nUodegele;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN }; //naudojamas enum duomenų tipas, kuris turi išanksto priskirtas reikšmes, kurios gali būti nustatytos
eDirecton dir; //priskiriamas pavadinimas krypčiai

void setup() //funkcija naudojama duomenų paruošimui naujam žaidimui
{
    nUodegele = 0;
    gameover = false;
    dir = STOP;
    x = plotis / 2;
    y = aukstis / 2;
    vaisiusX = rand() % plotis;
    vaisiusY = rand() % aukstis;
    score = 0;
}

void spausdinimas() //visko spausdinimo funkcija
{
    system("cls");

    for (int i = 0; i < plotis + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < aukstis; i++)
    {
        for (int j = 0; j < plotis; j++)
        {
            if (j == 0)
                cout << "#"; // Siena
            if (i == y && j == x)
                cout << "O"; // Gyvatėlės galva
            else if (i == vaisiusY && j == vaisiusX)
                cout << "V"; // vaisius, maistas, vadinkit kaip norit
            else
            {
                bool printUodega = false;
                for (int k = 0; k < nUodegele; k++)
                {
                    if (uodegeleX[k] == j && uodegeleY[k] == i)
                    {
                        cout << "o"; // Gyvatėlės kūnas/uodega
                        printUodega = true;
                    }
                }
                if (!printUodega)
                    cout << " ";
            }

            if (j == plotis - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < plotis + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Highscore: " << highscore << endl; // Didžiausi pasiekti taškai
    cout << "Score: " << score << endl; // Šiuo metu pasiekti taškai
}

void pause() // Žaidimo pastabdymo funkcija
{
    bool pause = true;
    system("cls"); //išvalo visą konsolę
    cout << "         G A M E   P A U S E D\n\n";
    cout << "- - - - - - C O N T R O L S - - - - - -\n";
    cout << "|        W         |     P - pause    |\n";
    cout << "|      A S D       |   X - end game   |\n";
    cout << "- - - - - - - - - - - - - - - - - - - -\n\n";
    cout << "BASH the R key to resume";
    while (pause == true) {
        if (_kbhit()) pause = false; //Jei pagauna klaviatūros inputa nutraukia pause() (tiek daug IF'ų, tam, kad greičiau sureaguotų)
        cout << ".";
        if (_kbhit()) pause = false;
        Sleep(300);
        if (_kbhit()) pause = false;
        cout << ".";
        if (_kbhit()) pause = false;
        Sleep(300);
        if (_kbhit()) pause = false;
        cout << ".";
        if (_kbhit()) pause = false;
        Sleep(800);
        if (_kbhit()) pause = false;
        cout << "\b \b"; //ištrina paskutini konsolej atspausdintą simbolį
        if (_kbhit()) pause = false;
        cout << "\b \b";
        if (_kbhit()) pause = false;
        cout << "\b \b";
        if (_kbhit()) pause = false;
        Sleep(300);
        if (_kbhit()) pause = false;
    }
    system("cls");
    cout << "RESUMING IN ";
    cout << "3";
    Sleep(1000);
    cout << "\b \b";
    cout << "2";
    Sleep(1000);
    cout << "\b \b";
    cout << "1";
    Sleep(1000);
    cout << "\b \b";
    system("cls");
    cout << "GO!";
    Sleep(500);
}

void ivestis()
{
    if (_kbhit())
    {
        char key = _getch();

        // patikrina ar inputas nėra priešingas dabartinei ėjimo krypčiai (kitaip patriggerintu gameover taisyklę)
        if ((key == 'a' && dir != RIGHT) ||
            (key == 'd' && dir != LEFT) ||
            (key == 'w' && dir != DOWN) ||
            (key == 's' && dir != UP) ||
            (key == 'x' || key == 'p'))
        {
            switch (key)
            {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameover = true;
                break;
            case 'p':
                pause();
                break;
            }
        }
    }
}

void smegenys(int delay)
{   //uodegegėles perkelimo procesas ir ciklas
    int praeitasX = uodegeleX[0];
    int praeitasY = uodegeleY[0];
    int praeitas2X, praeitas2Y;
    uodegeleX[0] = x;
    uodegeleY[0] = y;

    for (int i = 1; i < nUodegele; i++)
    {
        praeitas2X = uodegeleX[i];
        praeitas2Y = uodegeleY[i];
        uodegeleX[i] = praeitasX;
        uodegeleY[i] = praeitasY;
        praeitasX = praeitas2X;
        praeitasY = praeitas2Y;
    }
    //koordinačių priskyrimo procesas (skaičiuojamas pats ėjimas)
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        Sleep(delay * 1.2); //Šie Sleep() skirti išbalansuoti greitį tarp X ir Y ašių, kadangi konsolėjė i tokius pat matmenis kvadrate telpa mažiau blokų ant Y ašies
        y--;
        break;
    case DOWN:
        Sleep(delay * 1.2);
        y++;
        break;
    default:
        break;
    }
    //"teleportacijos" procesas
    if (x >= plotis) x = 0;
    else if (x < 0) x = plotis - 1;

    if (y >= aukstis) y = 0;
    else if (y < 0) y = aukstis - 1;

    //GameOver tikrinimas
    for (int i = 0; i < nUodegele; i++)
        if (uodegeleX[i] == x && uodegeleY[i] == y)
            gameover = true;

    // Vaisių generavimas
    if (x == vaisiusX && y == vaisiusY)
    {
        score += 10;
        vaisiusX = rand() % plotis;
        vaisiusY = rand() % aukstis;
        nUodegele++;
    }

    // Highscore tikrinimas
    if (score > highscore) highscore = score;
}

int main()
{
    bool PlayAgain = true;
    char playagain;
    string sunkumas;
    int delay = 40;
    ifstream df("highscore.txt");
    df >> highscore;
    df.close();

    while (PlayAgain) {
        srand(time(0));

        cout << "All Time Highscore On This Device: " << highscore << "\n\n";
        cout << "- - - - - - C O N T R O L S - - - - - -\n";
        cout << "|        W         |     P - pause    |\n";
        cout << "|      A S D       |   X - end game   |\n";
        cout << "- - - - - - - - - - - - - - - - - - - -\n\n";
        cout << "Choose difficulty (easy/medium/hard)" << endl;

        cin >> sunkumas;
        if (sunkumas == "easy") {
            delay = 40;
        };
        if (sunkumas == "medium") {
            delay = 20;
        };
        if (sunkumas == "hard") {
            delay = 8;
        };

        system("cls");
        setup();
        while (!gameover)
        {
            spausdinimas();
            ivestis();
            smegenys(delay);
            Sleep(delay); // Pagal sunkumo lygį nustatomas uždelsimas tarp ėjimų
        }


        cout << "G A M E   O V E R\n";
        cout << "Would you like to play again? (y/n)\n";
        cin >> playagain;
        if (playagain == 'n') PlayAgain = false;
        if (playagain == 'y') gameover = false;
        system("cls");
    }
    cout << "G O O D B Y E ;)";
    ofstream rf("highscore.txt");
    rf << highscore;
    rf.close();
    return 0;
}