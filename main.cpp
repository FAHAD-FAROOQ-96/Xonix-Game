/*
  FINAL PROJECT
  
SUBMITTED BY:
     FAHAD FAROOQ
      
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <cmath>
using namespace sf;

const int M = 25;
const int N = 40;
bool clockwise = true;
int grid[M][N] = {0};
int ts = 18;

struct Enemy
{
    int x, y, dx, dy;
    int zz_counter = 0;
    int zz_period = 50;

    float angle = 0.0f;
    int centerX = 300, centerY = 300;
    float radius = 100.0f;

    Enemy()
    {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;

        if (dx == 0)
            dx = 1;
        if (dy == 0)
            dy = 1;
    }

    void move(int elapsed, int index, int totalEnemies)
    {
        if (elapsed >= 30)
        {
            if (index < totalEnemies / 2)
                zigzag();
            else
                circular();
        }
        else
        linear();
        boundary();
    }

    void linear()
    {
        x += dx;
        if (x < 0 || x >= N * ts)
            dx = -dx;
        if (y < 0 || y >= M * ts)
            dy = -dy;

        int gx = x / ts;
        int gy = y / ts;
        if (gx >= 0 && gx < N && gy >= 0 && gy < M && grid[gy][gx] == 1)
        {
            dx = -dx;
            x += dx;
        }

        y += dy;
        gx = x / ts;
        gy = y / ts;
        if (gx >= 0 && gx < N && gy >= 0 && gy < M && grid[gy][gx] == 1)
        {
            dy = -dy;
            y += dy;
        }
        boundary();
    }

    void zigzag()
    {
        x += dx;
        y += dy;

        if (x < 0 || x >= N * ts)
            dx = -dx;
        if (y < 0 || y >= M * ts)
            dy = -dy;

        int gx = x / ts;
        int gy = y / ts;
        if (gx >= 0 && gx < N && gy >= 0 && gy < M && grid[gy][gx] == 1)
        {
            dx = -dx;
            dy = -dy;
            x += dx;
            y += dy;
        }

        zz_counter++;
        if (zz_counter >= zz_period)
        {
            dy = -dy;
            zz_counter = 0;
        }
        boundary();
    }

    void circular()
    {
        if (clockwise)
            angle += 0.04f;
        else
            angle -= 0.04f;

        if (angle > 6.28f)
            angle = 0;
        if (angle < 0)
            angle = 6.28f;

        float fx = centerX + radius * cos(angle);
        float fy = centerY + radius * sin(angle);
        int newX = fx + 0.5;
        int newY = fy + 0.5;

        int gx = newX / ts;
        int gy = newY / ts;

        if (gx >= 0 && gx < N && gy >= 0 && gy < M && grid[gy][gx] != 1)
        {
            x = newX;
            y = newY;
        }
        else
        {
            clockwise = !clockwise;
        }
        boundary();
    }

    void speedx2()
    {
        if (abs(dx) < 10)
        {
            dx = (dx > 0) ? dx + 2 : dx - 2;
        }
        if (abs(dy) < 10)
        {
            dy = (dy > 0) ? dy + 2 : dy - 2;
        }
        boundary();
    }

    void boundary()
    {
        if (x < ts)
            x = ts;
        if (y < ts)
            y = ts;
        if (x > (N - 1) * ts)
            x = (N - 1) * ts;
        if (y > (M - 1) * ts)
            y = (M - 1) * ts;
    }
};

/*
void drop(int y, int x)
{
    if (y < 0 || y >= M || x < 0 || x >= N)
        return;

    if (grid[y][x] == 0)
        grid[y][x] = -1;
    if (y > 0 && grid[y - 1][x] == 0)
        drop(y - 1, x);
    if (y < M - 1 && grid[y + 1][x] == 0)
        drop(y + 1, x);
    if (x > 0 && grid[y][x - 1] == 0)
        drop(y, x - 1);
    if (x < N - 1 && grid[y][x + 1] == 0)
        drop(y, x + 1);
}
*/

void drop(int y, int x) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    if (grid[y][x] != 0) return;

    grid[y][x] = -1;

    drop(y - 1, x);
    drop(y + 1, x);
    drop(y, x - 1);
    drop(y, x + 1);
}

int main()
{

    int move=0;
    int move1=0;
    srand(time(0));
    static bool initialized = false;
    static bool initialized1 = false;

    sf::Font font;

    if (!font.loadFromFile("images/apercumovistarbold.ttf"));

    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);
    ///////////main menu declarations
    sf::Text npc[4];
    std::string labels[4] = {"Start 1 Game", "Select Level", "ScoreBoard", "Mode"};
    ////////////////end menu declaration
    sf::Text npc1[4];
    std::string labels1[4] = {"Restart", "ScoreBoard", "Exit"};

    ////////////////player declaration
    sf::Text modes2[2];
    std::string lmodes2[2] = {"2 Player", "1 Player"};
    /////////////difficluty level declarartion
    sf::Text dif[4];
    std::string mer[4] = {"Easy", "Medium", "Difficult", "continuous"};

    /////////////difficulty level controller//////////////////////////
    bool easy = false;
    bool medium = false;
    bool hard = false;
    bool continous = false;
    ////////////////////////////score holder/////////////////////////////////
    int immdediate = 0;
    int immdediate2 = 0;
    int occurence = 0;
    int occurence1 = 0;
    bool checker=false;
///////////////////////////////////////////////player dlecteed
    bool x1single=false;
    bool x2single=false;

    bool player1winner=false;
    bool player2winner=false;
    int threshold = 10;
    int threshold1 = 10;
    bool x2time1 = false;
    bool x4time1 = false;

    bool x2time2 = false;
    bool x4time2 = false;
    int selection = 0;
    ///////////////////////////////////////////////////////////
    for (int i = 0; i < 4; i++)
    {
        npc[i].setPosition(200, 100 + i * 50);
        npc[i].setFillColor(sf::Color::White);
        npc[i].setCharacterSize(38);
        npc[i].setFont(font);
    }
    //////////////////////////////////////////////////////////
    for (int i = 0; i < 2; i++)
    {
        modes2[i].setPosition(200, 100 + i * 50);
        modes2[i].setFillColor(sf::Color::White);
        modes2[i].setCharacterSize(38);
        modes2[i].setFont(font);
    }
    /////////////////////////////////////////////////////////////
    for (int i = 0; i < 4; i++)
    {
        npc1[i].setPosition(200, 100 + i * 50);
        npc1[i].setFillColor(sf::Color::White);
        npc1[i].setCharacterSize(38);
        npc1[i].setFont(font);
    }
    //////////////////////////////////////////////////////////////
    for (int i = 0; i < 4; i++)
    {
        dif[i].setPosition(100, 100 + i * 50);
        dif[i].setFillColor(sf::Color::White);
        dif[i].setCharacterSize(38);
        dif[i].setFont(font);
    }

    Texture t1, t2, t3;
    if (!t1.loadFromFile("images/tiles.png"))
    {
        std::cout << "Failed to load tiles.png" << std::endl;
    }
    if (!t2.loadFromFile("images/gameover.png"))
    {
        std::cout << "Failed to load gameover.png" << std::endl;
    }
    if (!t3.loadFromFile("images/enemy.png"))
    {
        std::cout << "Failed to load enemy.png" << std::endl;
    }

    ////////////////score counters declared from 0
    int iscore = 0;
    int i2score = 0;
    bool lemda = false;

    ///////////// displaying score /////////////////
    Text score;
    Text scorep2;
    bool reading = false;

    score.setFont(font);
    score.setPosition(150, 250);
    score.setCharacterSize(42);
    score.setFillColor(sf::Color::Black);
    score.setOutlineColor(Color::Yellow);
    score.setOutlineThickness(2);

    scorep2.setFont(font);
    scorep2.setPosition(480, 250);
    scorep2.setCharacterSize(42);
    scorep2.setFillColor(sf::Color::Black);
    scorep2.setOutlineColor(Color::Red);
    scorep2.setOutlineThickness(2);

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    int gameplay = 0;
    /*
    0==no gameplay
    1==gameplay 2p player
    2==difficluty level
    3==exit game
    6== end menu
    4== scorebaord printing
    7== gameplay 1p player
    */
    int menuindex = 0;

    int difficulty = 0;
    int difer = 0;
    int difermenu = 0;
    int endmenu = 0;

    const int maxEnemies = 20;
    int enemyCount = 2;
    Enemy a[maxEnemies];

    // Initialize all enemies
    for (int i = 0; i < maxEnemies; i++)
    {
        a[i] = Enemy();
        // Set enemy centers for circular movement
        a[i].centerX = 200 + (i % 5) * 50;
        a[i].centerY = 200 + (i / 5) * 50;
    }

    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;

    int x2 = N - 6, y2 = 0, dx2 = 0, dy2 = 0;

    float timer = 0, delay = 0.07;
    Clock clock;

    // Game timer for tracking elapsed seconds
    Clock gameTimer;
    int x2speed = -1;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int powerholder1 = 0;
    int powerholder2 = 0;
    int next = 50;
    int next2 = 50;

    
    int powereredtimer1 = 0;
    int powereredtimer2 = 0;

    bool enemystop = false;
    bool enemystop1 = false;
    bool playeronefrozen = false;
    bool playertwofrozen = false;
    bool oponnet1 = false;
    bool oponnet2 = false;

    int increases = 2;

    sf::Text powerforplayer2;
    powerforplayer2.setFont(font);
    powerforplayer2.setCharacterSize(20);
    powerforplayer2.setFillColor(Color::Yellow);
    powerforplayer2.setPosition(580, 20);

    sf::Text powerUpText1;
    powerUpText1.setFont(font);
    powerUpText1.setCharacterSize(20);
    powerUpText1.setFillColor(Color::Red);
    powerUpText1.setPosition(20, 20);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool checker1=false;
    sf::Text input1;

    ///////////////////////////////////////////Score BoARD iNITATIED
    input1.setPosition(300, 150);
    input1.setFillColor(sf::Color::Yellow);
    input1.setCharacterSize(30);
    input1.setFont(font);
    int scores[100] = {0}; // Initialize all scores to 0
    int scorecount = 0;
    const int scoreboarddispnumbermax = 5;
    std::string scoreboarddisp = " ";

    // File reading and score extraction
    std::ifstream input("ScoreBoard.txt");
    if (!input)
    {
        std::cout << "failed to execute the file.\n";
    }
    else
    {
        std::string line;
        // jo file write ho rahi hai wo window open ke under hai it is being done using string so convert them to integers
        while (getline(input, line) && scorecount < 100)
        {
            int num = 0;
            bool valid = true;

            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] >= '0' && line[i] <= '9')
                {
                    num = num * 10 + (line[i] - '0');
                }
                else
                {
                    valid = false;
                    break;
                }
            }

            if (valid)
            {
                scores[scorecount] = num;
                scorecount++;
            }
        }
        input.close();

        for (int i = 0; i < scorecount - 1; i++)
        {
            for (int j = 0; j < scorecount - i - 1; j++)
            {
                if (scores[j] < scores[j + 1])
                {
                    int temp = scores[j];
                    scores[j] = scores[j + 1];
                    scores[j + 1] = temp;
                }
            }
        }
        

        for (int i = 0; i < scoreboarddispnumbermax && i < scorecount; i++)
        {
            scoreboarddisp += std::to_string(scores[i]) + "\n";
        }
    }

    input1.setString(scoreboarddisp);
    ///////////////////////////SCORE BAORD READ AFTER DECENDING ORDERED

    int lastEnemyIncreaseTime = -1;
    int lastEnemyIncreaseTime1 = -1;

    /////////////////////////////////////TIME CREATED BOR BOTH THE MODES

    int incrementer = 0; //////////////////////////TIMER
    int incrementer1 = 0;
    int holder = 60; // EQUALED TO THE FPS

    /////////TEXT TO DIPLAY THE TIEMR CREATED

    Text timerText;

    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setOutlineColor(Color::Black);
    timerText.setOutlineThickness(2);
    timerText.setPosition(300, 100);

    //////////////////////TIMER FOR SINGLE PLAYER GAME
    Text timerText1;

    timerText1.setFont(font);
    timerText1.setCharacterSize(24);
    timerText1.setFillColor(Color::White);
    timerText1.setOutlineColor(Color::Black);
    timerText1.setOutlineThickness(2);
    timerText1.setPosition(300, 100);

    /////////////////////Move 
    Text moveCounter;

    moveCounter.setFont(font);                 
    moveCounter.setCharacterSize(24);           
    moveCounter.setFillColor(Color::White);
    moveCounter.setOutlineColor(Color::Black);  
    moveCounter.setOutlineThickness(2);         
    moveCounter.setPosition(130, 10);
    Text moveCounter1;

    moveCounter1.setFont(font);                 
    moveCounter1.setCharacterSize(24);           
    moveCounter1.setFillColor(Color::White);
    moveCounter1.setOutlineColor(Color::Black);  
    moveCounter1.setOutlineThickness(2);         
    moveCounter1.setPosition(400, 10);
    
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Adding sound to game --> help taken form youtube

    sf::SoundBuffer buffer; // Correct class name

    if (!buffer.loadFromFile("images/output.wav"))
    {
        std::cout << "unabel to load the fmusic  file!" << std::endl;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        ////////////////IF GAME WINDOW OPEN TIME INITIATE THEN ONLY
        if (gameplay == 1)
        {
            incrementer++;
        }
        /////////TIMER CONVERETD TO STRING USING STRING LIBS
        int secs = incrementer / holder;
        timerText.setString("Time: " + std::to_string(secs) + "s");

        ////////////////// Speed boost every 20 seconds for 2-player mode //////////////////
        if (gameplay == 1 && secs > 0 && secs % 20 == 0 && secs != x2speed)
        {
            for (int i = 0; i < enemyCount; i++)
            {
                a[i].speedx2();
            }
            x2speed = secs;
        }

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            //////////////////////////////////////////////////////////Game menu
            if (gameplay == 0)
            {
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Down)
                    {
                        menuindex++;
                        if (menuindex > 3)
                        {
                            menuindex = 0;
                        }
                    }
                    if (e.key.code == Keyboard::Up)
                    {
                        menuindex--;
                        if (menuindex < 0)
                        {
                            menuindex = 3;
                        }
                    }
                    if (e.key.code == Keyboard::Enter)
                    {
                        if (menuindex == 0)
                        {
                            gameplay = 1;
                            // Reset game
                            gameTimer.restart();
                            move=0;   // resetting to zero after game ends
                            move1=0;   // resetting to zero after game ends
                            x2speed = -1;

                            for (int i = 0; i < M; i++)
                            {
                                for (int j = 0; j < N; j++)
                                {
                                    grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
                                }
                            }
                            x = 10;
                            y = 0;
                            dx = 0;
                            dy = 0;

                            x2 = N - 6, y2 = 0, dx2 = 0, dy2 = 0;
                            Game = true;

                            enemyCount = 4;
                            if (enemyCount > maxEnemies)
                                enemyCount = maxEnemies;

                            for (int i = 0; i < maxEnemies; i++)
                            {
                                a[i] = Enemy();
                                a[i].centerX = 200 + (i % 5) * 50;
                                a[i].centerY = 200 + (i / 5) * 50;
                            }
                        }
                        else if (menuindex == 1)
                        {
                            gameplay = 2;
                        }

                        else if (menuindex == 2)
                        {
                            gameplay = 4;
                        }

                        if (menuindex == 3)
                        {
                            gameplay = 7;
                        }
                    }
                }
            }
            //////////////////////////////////////////////////////////Difficulty menu

            else if (gameplay == 2)
            {
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Down)
                    {
                        difermenu++;
                        if (difermenu > 3)
                        {
                            difermenu = 0;
                        }
                    }

                    if (e.key.code == Keyboard::Up)
                    {
                        difermenu--;
                        if (difermenu < 0)
                        {
                            difermenu = 3;
                        }
                    }

                    if (e.key.code == Keyboard::Enter)
                    {
                        if (difermenu == 0)
                        {
                            easy = true;
                            medium = false;
                            hard = false;
                            continous = false;
                        }

                        if (difermenu == 1)
                        {
                            medium = true;
                            easy = false;
                            hard = false;
                            continous = false;
                        }

                        if (difermenu == 2)
                        {
                            hard = true;
                            easy = false;
                            medium = false;
                            continous = false;
                        }
                        if (difermenu == 3)
                        {
                            continous = true;
                            easy = false;
                            medium = false;
                            hard = false;
                        }

                        if (e.key.code == Keyboard::Escape)
                        {
                            gameplay = 0;
                        }
                    }

                    if (e.key.code == Keyboard::Escape)
                    {
                        gameplay = 0;
                    }
                }
            }
            else if (gameplay == 0)
            {
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Escape)
                    {
                        window.close();
                    }
                }
            }

            else if (gameplay == 6)
            {
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Down)
                    {
                        endmenu++;
                        if (endmenu > 2)
                        {
                            endmenu = 0;
                        }
                    }

                    if (e.key.code == Keyboard::Up)
                    {
                        endmenu--;
                        if (endmenu < 0)
                        {
                            endmenu = 2;
                        }
                    }

                    if (e.key.code == Keyboard::Enter)
                    {
                        if (endmenu == 0)
                        {
                            gameplay = 0;
                            lemda = true;
                        }

                        if (endmenu == 1)
                        {
                            gameplay = 4;
                        }

                        if (endmenu == 2)
                        {
                            window.close();
                        }
                    }

                    if (e.key.code == Keyboard::Escape)
                    {
                        gameplay = 0;
                    }
                }
            }
            //////////////////////////////////////////player selection

            else if (gameplay == 7)
            {
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Down)
                    {
                        selection++;
                        if (selection > 1) // Fixed to match the actual array size
                        {
                            selection = 0;
                        }
                    }

                    if (e.key.code == Keyboard::Up)
                    {
                        selection--;
                        if (selection < 0)
                        {
                            selection = 1;
                        }
                    }
                    if (selection == 0)
                    {
                        if (e.key.code == Keyboard::Enter)
                        {
                            gameplay = 1;
                            gameTimer.restart();
                            x2speed = -1;
                        }
                    }

                    if (selection == 1)
                    {
                        if (e.key.code == Keyboard::Enter)
                        {
                            gameplay = 12;
                            gameTimer.restart();
                            x2speed = -1;
                        }
                    }

                    if (e.key.code == Keyboard::Escape)
                    {
                        gameplay = 0;
                    }
                }
            }

            //////////////////////////////////////score board////////////////////////////////////////

            else if (gameplay == 4)
            {
                window.clear();
                window.draw(input1);
                window.display();

                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Escape)
                    {
                        gameplay = 0;
                    }
                }
                continue;
            }

            else if (gameplay == 1 || gameplay == 12)
            {
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Escape)
                    {
                        gameplay = 0;
                    }
                }
            }
        }

        window.clear();

        if (gameplay == 0)
        {
            if (lemda)
            {
                iscore = 0;
                i2score = 0;
                lemda = false;
            }

            for (int i = 0; i < 4; i++)
            {
                if (i == menuindex)
                    npc[i].setFillColor(sf::Color::Yellow);
                else
                    npc[i].setFillColor(sf::Color::White);

                npc[i].setString(labels[i]);

                window.draw(npc[i]);
            }
        }

        else if (gameplay == 2)
        {
            for (int i = 0; i < 4; i++)
            {
                if (i == difermenu)
                    dif[i].setFillColor(sf::Color::Yellow);
                else
                    dif[i].setFillColor(sf::Color::White);

                dif[i].setString(mer[i]);

                window.draw(dif[i]);
            }
        }

        else if (gameplay == 6)
        {
            for (int i = 0; i < 4; i++)
            {
                if (i == endmenu)
                    npc1[i].setFillColor(sf::Color::Yellow);
                else
                    npc1[i].setFillColor(sf::Color::White);

                npc1[i].setString(labels1[i]);

                window.draw(npc1[i]);
            }
        }

        else if (gameplay == 7)
        {
            for (int i = 0; i < 2; i++)
            {
                if (i == selection)
                    modes2[i].setFillColor(sf::Color::Yellow);
                else
                    modes2[i].setFillColor(sf::Color::White);

                modes2[i].setString(lmodes2[i]);

                window.draw(modes2[i]);
            }
        }

        /////////////////////////////////GAME START///////////////////////////////////////////////////////

        else if (gameplay == 1)
        {
            score.setString("p1: " + std::to_string(iscore));
            scorep2.setString("p2: " + std::to_string(i2score));

            powerforplayer2.setString("Power 2: " + std::to_string(powerholder1));
            powerUpText1.setString("Power 1: " + std::to_string(powerholder2));
            moveCounter.setString("Move : " + std::to_string(move/60));
            moveCounter1.setString("Move : " + std::to_string(move1/60));
            

            if (powerholder1 > 0)
            {
                /* code */
                if (!enemystop)
                {

                    if (Keyboard::isKeyPressed(Keyboard::Space))
                    {
                        powerholder1--;
                        enemystop = true;
                        oponnet2 = true;
                        powereredtimer1 = 180;
                    }
                }
            }
            if (enemystop)
            {
                powereredtimer1--;
                if (powereredtimer1 <= 0)
                {
                    enemystop = false;
                    oponnet2 = false;
                }
            }

            // Power up for player 2
            if (powerholder2 > 0)
            {
                /* code */
                if (!enemystop1)
                {

                    if (Keyboard::isKeyPressed(Keyboard::X))
                    {
                        powerholder2--;
                        enemystop1 = true;
                        oponnet1 = true;
                        powereredtimer2 = 180;
                    }
                }
            }
            if (enemystop1)
            {
                powereredtimer2--;
                if (powereredtimer2 <= 0)
                {
                    enemystop1 = false;
                    oponnet1 = false;
                }
            }


            // Player 1 controls
            if (!oponnet1)
            {
                if (Keyboard::isKeyPressed(Keyboard::Left))
                {
                    dx = -1;
                    dy = 0;
                    move++;
                }
                if (Keyboard::isKeyPressed(Keyboard::Right))
                {
                    dx = 1;
                    dy = 0;
                    move++;
                }
                if (Keyboard::isKeyPressed(Keyboard::Up))
                {
                    dx = 0;
                    dy = -1;
                    move++;
                }
                if (Keyboard::isKeyPressed(Keyboard::Down))
                {
                    dx = 0;
                    dy = 1;
                    move++;
                }
            }

            // Player 2 controls
            if (!oponnet2)
            {
                if (Keyboard::isKeyPressed(Keyboard::A))
                {
                    dx2 = -1;
                    dy2 = 0;
                    move1++;
                }
                if (Keyboard::isKeyPressed(Keyboard::D))
                {
                    dx2 = 1;
                    dy2 = 0;
                    move1++;
                }
                if (Keyboard::isKeyPressed(Keyboard::W))
                {
                    dx2 = 0;
                    dy2 = -1;
                    move1++;
                }
                if (Keyboard::isKeyPressed(Keyboard::S))
                {
                    dx2 = 0;
                    dy2 = 1;
                    move1++;
                }
            }
            
            
            if (!Game)
                continue;

            if (timer > delay)
            {
                x += dx;
                y += dy;

                if (x < 0)
                    x = 0;
                if (x > N - 1)
                    x = N - 1;
                if (y < 0)
                    y = 0;
                if (y > M - 1)
                    y = M - 1;

                if (grid[y][x] == 2)
                    Game = false;

                if (x == x2 && y == y2)
                {
                    Game = false;
                }

                if (grid[y][x] == 0)
                {
                    grid[y][x] = 2;
                    immdediate += 1;
                }

                x2 += dx2;
                y2 += dy2;

                if (x2 < 0)
                    x2 = 0;
                if (x2 > N - 1)
                    x2 = N - 1;
                if (y2 < 0)
                    y2 = 0;
                if (y2 > M - 1)
                    y2 = M - 1;

                if (grid[y2][x2] == 3)
                    Game = false;
                if (grid[y2][x2] == 0)
                {
                    grid[y2][x2] = 3;
                    immdediate2 += 1;
                }

                if (grid[y2][x2] == 2)
                    Game = false;

                if (grid[y][x] == 3 && grid[y2][x2] == 0)
                    Game = false;

                if (grid[y2][x2] == 2 && grid[y][x] == 0)
                    Game = false;

                if (x == x2 && y == y2)
                {
                    Game = false;
                }

                timer = 0;
            }
            if (easy)
            {
                enemyCount = 2;
            }
            else if (medium)
            {
                enemyCount = 4;
            }
            else if (hard)
            {
                enemyCount = 6;
            }

            if (continous && !initialized)
            {
                enemyCount = 2;
                initialized = true;
            }

            if (continous)
            {
                if (!initialized)
                {
                    enemyCount = 2;
                    initialized = true;
                    lastEnemyIncreaseTime = secs;
                }

                if (secs - lastEnemyIncreaseTime >= 20)
                {
                    enemyCount += 2;
                    lastEnemyIncreaseTime = secs;
                }
            }
            else
            {
                initialized = false;
            }

            if (!enemystop && !enemystop1)
            {
                for (int i = 0; i < enemyCount; i++)
                    a[i].move(secs, i, enemyCount);
            }

           // if (grid[y][x] == 0)  
             //   grid[y][x] = 4;     
                
             if (grid[y][x] == 1)
             {
                 dx = dy = 0;
                 for (int i = 0; i < enemyCount; i++)
                     drop(a[i].y / ts, a[i].x / ts);
            
                 for (int i = 0; i < M; i++)
                     for (int j = 0; j < N; j++)
                         if (grid[i][j] == -1)
                             grid[i][j] = 0;
                         else if (grid[i][j] == 2)
                             grid[i][j] = 1;
                 ///////////////////////////////////////////////////scoring
                if (immdediate >= threshold)
                {
                    occurence += 1;
                    if (occurence >= 5)
                    {
                        x4time1 = true;
                    }
                    else if (occurence >= 3)
                    {
                        threshold = 5;
                    }
                    x2time1 = true;
                }

                else
                {
                    x2time1 = false;
                    x4time1 = false;
                }
                if (x4time1)
{
    immdediate *= 4;
    iscore += immdediate;
}

else if (x2time1)
{
    immdediate *= 2;
    iscore += immdediate;
}
                else
                {
                    iscore += immdediate;
                }
                while (iscore >= next)
                {
                    powerholder1 += 1;
                    next += 30; // Next threshold is +30
                }
                immdediate = 0;
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
            
           //  if (grid[y2][x2] == 0)
            //{  grid[y2][x2] = 3;  }
            
            if (grid[y2][x2] == 1)
            {
                dx2 = dy2 = 0;
                for (int i = 0; i < enemyCount; i++)
                    drop(a[i].y / ts, a[i].x / ts);

                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == -1)
                            grid[i][j] = 0;
                        else if (grid[i][j] == 3)
                             grid[i][j] = 1;   //filling the area in between --- player2
                //////////////////////////////////////////////SCORE CALCULATE//////////////////////////////////////////////
                if (immdediate2 >= threshold1)
                {
                    occurence1 += 1;
                    if (occurence1 >= 5)
                    {
                        x4time2 = true;
                    }
                    else if (occurence1 >= 3)
                    {
                        threshold1 = 5;
                    }
                    x2time2 = true;
                }

                else
                {
                    x2time2 = false;
                    x4time2 = false;
                }
                if (x4time2)
                {
                    immdediate2 *= 4;
                    i2score += immdediate2;
                }
                else if (x2time2)
                {
                    immdediate2 *= 2;
                    i2score += immdediate2;
                }
                else
                {
                    i2score += immdediate2;
                }
                while (i2score >= next2)
                {
                    powerholder2 += 1;
                    next2 += 30; // Next threshold is +30
                }
                immdediate2 = 0;
            }
            //////////////////////////////////////////////////////////////SCORE CALCULATE=END
            for (int i = 0; i < enemyCount; i++)
            {
                if (grid[a[i].y / ts][a[i].x / ts] == 2)
                    Game = false;
                ///////////////////////PLAYER 2 ENEMY COLLISON
                if (grid[a[i].y / ts][a[i].x / ts] == 3)
                    Game = false;
            }

            /////////draw//////////
            window.clear();
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                {
                    if (grid[i][j] == 0)
                        continue;
                    if (grid[i][j] == 1)
                        sTile.setTextureRect(IntRect(0, 0, ts, ts));
                    if (grid[i][j] == 2)
                        sTile.setTextureRect(IntRect(54, 0, ts, ts));
                    if (grid[i][j] == 3)
                    {
                        sTile.setTextureRect(IntRect(72, 0, ts, ts));
                        sTile.setColor(Color::Red);
                    }
                    sTile.setPosition(j * ts, i * ts);
                    window.draw(sTile);
                }

            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x * ts, y * ts);
            sTile.setColor(Color::White);
            window.draw(sTile);

            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x2 * ts, y2 * ts);
            window.draw(sTile);

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++)
            {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }

            if (!Game)
            {

                window.draw(sGameover);
                if (iscore > i2score)
                {
                    window.draw(score);
                }
                else if (iscore < i2score)
                {
                    /* code */ window.draw(scorep2);
                }


                checker = false;
          
            for (int i = 0; i < 5; i++)
        {
            if (iscore > scores[i])
          {
              checker = true;
                break;
          }
        }

            checker1 = false;
          for (int i = 0; i < 5; i++)
        {
           if (i2score > scores[i])
          {
             checker1 = true;
                break;
          } 
        }

                if (checker)
                {
                    score.setFillColor(sf::Color::Yellow);
                    score.setPosition(280, 250);
                    score.setCharacterSize(80);
                    window.draw(score);
                }

              
                else if (checker1)
                {
                    scorep2.setFillColor(sf::Color::Red);
                    scorep2.setPosition(280, 250);
                    scorep2.setCharacterSize(80);
                    window.draw(scorep2);
                }


                window.display();
                sf::sleep(sf::seconds(2));
                
score.setFont(font);
score.setPosition(150, 250);
score.setCharacterSize(42);
score.setFillColor(sf::Color::Black);
score.setOutlineColor(Color::Yellow);
score.setOutlineThickness(2);

scorep2.setFont(font);
scorep2.setPosition(480, 250);
scorep2.setCharacterSize(42);
scorep2.setFillColor(sf::Color::Black);
scorep2.setOutlineColor(Color::Red);
scorep2.setOutlineThickness(2);


                std::ofstream savers("ScoreBoard.txt", std::ios::app);

                if (savers.is_open())
                {
                    if (iscore > i2score)
                    {
                        savers << iscore << std::endl;
                    }
                    else

                        savers << i2score << std::endl;

                    savers.close();
                }
                easy = false;
                hard = false;
                continous = false;
                medium = false;

                iscore = 0;
                immdediate2 = 0;
                immdediate = 0;
                i2score = 0;
                gameplay = 6;
                incrementer = 0;
                powerholder1 = 0;
                powerholder2 = 0;
            }
            
            ///// For Displaying ///////
            
            window.draw(powerforplayer2);
            window.draw(powerUpText1);

            window.draw(timerText);
            window.draw(score);
            window.draw(scorep2);
            window.draw(moveCounter);   
            window.draw(moveCounter1);   
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////single player
        else if (gameplay == 12)
        {
            score.setString("p1: " + std::to_string(iscore));
            powerforplayer2.setString("Power-ups: " + std::to_string(powerholder1));
            moveCounter.setString("Move : " + std::to_string(move/60));

            if (gameplay == 12)
            {
                incrementer1++;
            }

            int secs1 = incrementer1 / holder;
            timerText1.setString("Time: " + std::to_string(secs1) + "s");

            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                dx = -1;
                dy = 0;
                move++;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                dx = 1;
                dy = 0;
                move++;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                dx = 0;
                dy = -1;
                move++;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                dx = 0;
                dy = 1;
                move++;
            }

            if (!Game)
                continue;

            if (timer > delay)
            {
                x += dx;
                y += dy;

                if (x < 0)
                    x = 0;
                if (x > N - 1)
                    x = N - 1;
                if (y < 0)
                    y = 0;
                if (y > M - 1)
                    y = M - 1;

                if (grid[y][x] == 2)
                    Game = false;

                if (grid[y][x] == 0)
                {
                    grid[y][x] = 2;
                    immdediate += 1;
                }
                timer = 0;
            }
            /////////////////////ENEMY NUMBER DECIDER
            if (easy)
            {
                enemyCount = 2;
            }
            else if (medium)
            {
                enemyCount = 4;
            }
            else if (hard)
            {
                enemyCount = 6;
            }
            if (continous)
            {
                if (!initialized1)
                {
                    enemyCount = 2;
                    initialized1 = true;
                    lastEnemyIncreaseTime1 = secs1;
                }
                else if (secs1 - lastEnemyIncreaseTime1 >= 20)
                {
                    enemyCount += 2;
                    lastEnemyIncreaseTime1 = secs1;
                }
            }
            else
            {
                initialized1 = false;
            }

            if (powerholder1 > 0)
            {
                /* code */

                if (Keyboard::isKeyPressed(Keyboard::Space) && !enemystop)
                {
                    powerholder1--;
                    enemystop = true;
                    powereredtimer1 = 180;
                }
            }
            if (enemystop)
            {
                powereredtimer1--;
                if (powereredtimer1 <= 0)
                {
                    enemystop = false;
                }
            }
            else
            {
                for (int i = 0; i < enemyCount; i++)
                    a[i].move(secs1, i, enemyCount);
            }

            if (grid[y][x] == 1)
            {
                dx = dy = 0;
                for (int i = 0; i < enemyCount; i++)
                    drop(a[i].y / ts, a[i].x / ts);

                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == -1)
                            grid[i][j] = 0;
                        else if (grid[i][j] == 2)
                            grid[i][j] = 1;

                if (immdediate >= threshold)
                {
                    occurence += 1;
                    if (occurence >= 5)
                    {
                        x4time1 = true;
                    }
                    else if (occurence >= 3)
                    {
                        threshold = 5;
                    }
                    x2time1 = true;
                }

                else
                {
                    x2time1 = false;
                    x4time1 = false;
                }
                if (x2time1)
                {
                    immdediate *= 2;
                    iscore += immdediate;
                }
                else if (x4time1)
                {
                    immdediate *= 4;
                    iscore += immdediate;
                }
                else
                {
                    iscore += immdediate;
                }
                while (iscore >= next)
                {
                    powerholder1 += 1;
                    next += 30;
                }
                immdediate = 0;
            }

            for (int i = 0; i < enemyCount; i++)
            {
                if (grid[a[i].y / ts][a[i].x / ts] == 2)
                    Game = false;
            }

            /////////draw//////////
            window.clear();
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                {
                    if (grid[i][j] == 0)
                        continue;
                    if (grid[i][j] == 1)
                        sTile.setTextureRect(IntRect(0, 0, ts, ts));
                    if (grid[i][j] == 2)
                        sTile.setTextureRect(IntRect(54, 0, ts, ts));
                    if (grid[i][j] == 3)
                        sTile.setTextureRect(IntRect(72, 0, ts, ts));
                    sTile.setPosition(j * ts, i * ts);
                    window.draw(sTile);
                }

            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x * ts, y * ts);
            window.draw(sTile);

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++)
            {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }
            ///////////////////////////////// SINGLE PLAYER GAME END FUNCTIONALITY
            if (!Game)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (iscore>scores[i])
                    {
                        checker=true;
                    }
                    else
                    checker=false;
                    
                }

                if (checker)
                {
                    score.setFillColor(sf::Color::Yellow);
                    score.setPosition(280, 250);
                    score.setCharacterSize(80);

                }
                

                window.draw(sGameover);

                window.draw(score);
                window.display();

                sf::sleep(sf::seconds(2));

                std::ofstream savers("ScoreBoard.txt", std::ios::app);

                if (savers.is_open())
                {

                    savers << iscore << std::endl;

                    savers.close();
                }
                immdediate = 0;
                powerholder1 = 0;

                iscore = 0;
                incrementer1 = 0;
                gameplay = 6;

                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Escape)
                    {
                        gameplay = 6;
                    }
                }
            }
            window.draw(powerforplayer2);
            window.draw(timerText1);
            window.draw(score);
            window.draw(scorep2);
            window.draw(moveCounter);
        }

        window.display();
    }

    return 0;
}
