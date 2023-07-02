#include <iostream>
#include <simple2d.h>
#include <cmath>
#include <ctime>
#include <string>
#include <fstream>
#include "json.hpp" 
#undef main
using namespace nlohmann;
////////////////////////////////
struct Projectile 
{
    double x, y;      // position
    double vx, vy;    // velocity
    double ax, ay;    // acceleration
};
Projectile projectile;

struct Line
{
    double x1 = 55, y1 = 549;
    int x2, y2;
};
Line line;

struct Ball
{
    double x, y;
    int xIncreamentor = 3;
    int yIncreamentor = 3;
    int hit = 0;
};
Ball ball1, ball2;

struct Player
{
    char playerName [20];
    int playerScore = 0;
    bool winner = false;
};
Player player1, player2;

/////////////////////////////
S2D_Window* window;
S2D_Sprite* spr;
S2D_Image* arrow;
S2D_Text* welcome;
S2D_Text* howToPlay;
S2D_Text* startGame;
S2D_Text* p1Name;
S2D_Text* p1NameInBall;
S2D_Text* p2Name;
S2D_Text* p2NameInBall;
S2D_Text* turntxt;
S2D_Text* scoreToBeat;
S2D_Text* winner;
S2D_Text* pressEsq;
S2D_Text* gameSpeed;
S2D_Music* backGroundMusic;
S2D_Sound* pop;
S2D_Sound* fail;
S2D_Sound* shot;
//////////////////////////
bool gameStarted = false;
bool helpPressed = false;
bool shoot = false;
bool touchTheWall = false;
bool printWinner = false;
std::string name1, name2;
int turn = 1;
int score;
int speed;
int musicVolume;
//////////Help////////////
void Help()
{
  puts(" |-------------------------------|");
  puts(" |      Pop The Balloon Game     |");
  puts(" |-------------------------------|");
  puts(" |                               |");
  puts(" |   The game is so simple       |");
  puts(" |  Every player should hit the  |");
  puts(" |  Other players ball to get    |");
  puts(" |          +5 points            |");
  puts(" |   If the player hits his own  |");
  puts(" |    Ball will pe punished by   |");
  puts(" |          -3 points            |");
  puts(" |                               |");
  puts(" |   The winner is who first     |");
  puts(" |  Reaches the assinged score   |");
  puts(" |      Have a good game         |");
  puts(" |                               |");
  puts(" |-------------------------------|");
}
//////////Save////////////
void Save()
{
    std::ofstream jsonInputFileStream("./savefiles/SaveFile.json");
    json jsonData;

    jsonData["ball1"] = { {"ball1.x" ,ball1.x} , {"ball1.y" ,ball1.y} , {"ball1.hit",ball1.hit}
    ,{"ball1.xIncreamentor" , ball1.xIncreamentor}  ,{"ball1.yIncreamentor" , ball1.yIncreamentor} };
    jsonData["ball2"] = { {"ball2.x" ,ball2.x} , {"ball2.y" ,ball2.y} , {"ball2.hit",ball2.hit}
    ,{"ball2.xIncreamentor" , ball2.xIncreamentor}  ,{"ball2.yIncreamentor" , ball2.yIncreamentor} };
    
    jsonData["player1"] = { {"player1.playerName",player1.playerName}
    ,{"player1.playerScore",player1.playerScore},{"player1.winner",player1.winner} };
    jsonData["player2"] = { {"player2.playerName",player2.playerName}
    ,{"player2.playerScore",player2.playerScore},{"player2.winner",player2.winner} };

    jsonData["projectile"] = { {"projectile.x",projectile.x},{"projectile.y",projectile.y}
    ,{"projectile.vx",projectile.vx},{"projectile.vy",projectile.vy},{"projectile.ax",projectile.ax},{"projectile.ay",projectile.ay} };
    jsonData["line"] = { {"line.x1",line.x1} ,{"line.y1",line.y1},{"line.x2",line.x2},{"line.y2",line.y2} };
    jsonData["shoot"] = shoot;
    jsonData["touchTheWall"] = touchTheWall;
    jsonData["printWinner"] = printWinner;
    jsonData["turn"] = turn;
    jsonData["score"] = score;
    jsonData["speed"] = speed;
    jsonData["musicVolume"] = musicVolume;

    jsonInputFileStream << std::setw(4) << jsonData;
}
//////////Load////////////
void Load()
{
    std::ifstream jsonOutputFileStream("./savefiles/SaveFile.json");
    json jsonData = json::parse(jsonOutputFileStream);

    ball1.x = jsonData["ball1"]["ball1.x"].get<int>();
    ball1.y = jsonData["ball1"]["ball1.y"].get<int>();
    ball1.hit = jsonData["ball1"]["ball1.hit"].get<int>();
    ball1.xIncreamentor = jsonData["ball1"]["ball1.xIncreamentor"].get<int>();
    ball1.yIncreamentor = jsonData["ball1"]["ball1.yIncreamentor"].get<int>();

    ball2.x = jsonData["ball2"]["ball2.x"].get<int>();
    ball2.y = jsonData["ball2"]["ball2.y"].get<int>();
    ball2.hit = jsonData["ball2"]["ball2.hit"].get<int>();
    ball2.xIncreamentor = jsonData["ball2"]["ball2.xIncreamentor"].get<int>();
    ball2.yIncreamentor = jsonData["ball2"]["ball2.yIncreamentor"].get<int>();

    std::string playerName;
    playerName = jsonData["player1"]["player1.playerName"].get<std::string>();
    for (int i = 0; i < playerName.size(); i++)
    {
        player1.playerName[i] = playerName[i];
    }
    player1.playerName[playerName.size()] = '\0';
    player1.playerScore = jsonData["player1"]["player1.playerScore"].get<int>();
    player1.winner = jsonData["player1"]["player1.winner"].get<bool>();

    playerName = jsonData["player2"]["player2.playerName"].get<std::string>();
    for (int i = 0; i < playerName.size(); i++)
    {
        player2.playerName[i] = playerName[i];
    }
    player2.playerName[playerName.size()] = '\0';
    player2.playerScore = jsonData["player2"]["player2.playerScore"].get<int>();
    player2.winner = jsonData["player2"]["player2.winner"].get<bool>();

    projectile.x = jsonData["projectile"]["projectile.x"].get<int>();
    projectile.y = jsonData["projectile"]["projectile.y"].get<int>();
    projectile.vx = jsonData["projectile"]["projectile.vx"].get<int>();
    projectile.vy = jsonData["projectile"]["projectile.vy"].get<int>();
    projectile.ax = jsonData["projectile"]["projectile.ax"].get<int>();
    projectile.ay = jsonData["projectile"]["projectile.ay"].get<int>();

    line.x1 = jsonData["line"]["line.x1"].get<int>();
    line.y1 = jsonData["line"]["line.y1"].get<int>();
    line.x2 = jsonData["line"]["line.x2"].get<int>();
    line.y2 = jsonData["line"]["line.y2"].get<int>();

    shoot = jsonData["shoot"].get<bool>();
    touchTheWall = jsonData["touchTheWall"].get<bool>();
    turn = jsonData["turn"].get<int>();
    score = jsonData["score"].get<int>();
    speed = jsonData["speed"].get<int>();
    musicVolume = jsonData["musicVolume"].get<int>();
}
////////////Getting inputs in the game///////
void on_key(S2D_Event e)
{
    switch (e.type) 
    {
    case S2D_KEY_DOWN:

        if (line.x1 <= 10)
        {
            line.x1 = 10;
            line.y1 = 504;
        }

        else if (line.y1 >= 589)
        {
            line.x1 = 95;
            line.y1 = 589;
        }
                
        if (strcmp(e.key, "Up") == 0 && (line.x1 >= 10 && line.y1 <= 589))
        {
            line.x1 -= 1;
            line.y1 -= 1;
        }

        else if (strcmp(e.key, "Down") == 0 && (line.x1 >= 10 && line.y1 <= 589))
        {
            line.x1 += 1;
            line.y1 += 1;
        }

        if (strcmp(e.key, "Space") == 0 && shoot == false)
        {
            ///calculate position
            projectile.x = line.x1;
            projectile.y = line.y1;
            ///calculate speed
            double arctan = atan((599 - line.y1) / line.x1);
            projectile.vx = cos(arctan) * 50.0;
            projectile.vy = -sin(arctan) * 50.0;
            ///calculate acceleration
            projectile.ax = 0;
            projectile.ay = 2;
            ///set shoot status true
            shoot = true;
            turn = 1 - turn;
            ball1.hit = 2;
            ball2.hit = 2;
            //////////
            S2D_PlaySound(shot);
        }

        if (strcmp(e.key, "S") == 0)
            if(gameStarted)
                Save();
        if (strcmp(e.key, "H") == 0)
        {
            helpPressed = true;
            Help();
        }
        if (strcmp(e.key, "N") == 0)
            gameStarted = true;

        if (strcmp(e.key, "Escape") == 0) 
            S2D_Close(window);
        break;
        
    case S2D_KEY_HELD:
        if (line.x1 <= 10)
        {
            line.x1 = 10;
            line.y1 = 504;
        }
        else if (line.y1 >= 589)
        {
            line.x1 = 95;
            line.y1 = 589;
        }

        if (strcmp(e.key, "Up") == 0 && (line.x1 >= 10 && line.y1 <= 589))
        {
            line.x1 -= 1;
            line.y1 -= 1;
        }
        else if (strcmp(e.key, "Down") == 0 && (line.x1 >= 10 && line.y1 <= 589))
        {
            line.x1 += 1;
            line.y1 += 1;
        }
                
        break;
        
    case S2D_KEY_UP:
        break;
    }
}
///////////////////Update////////////////////
void update()    
{  
    if(gameStarted)
    {
        if (player1.winner != true && player2.winner != true)
        {
            //check if balls hit the walls then change the direction
            if (ball1.x + 50 >= 799 || ball1.x - 50 <= 200)
                ball1.xIncreamentor = -ball1.xIncreamentor;

            if (ball1.y + 50 >= 599 || ball1.y - 50 <= 0)
                ball1.yIncreamentor = -ball1.yIncreamentor;

            if (ball2.x + 50 >= 799 || ball2.x - 50 <= 200)
                ball2.xIncreamentor = -ball2.xIncreamentor;

            if (ball2.y + 50 >= 599 || ball2.y - 50 <= 0)
                ball2.yIncreamentor = -ball2.yIncreamentor;

            //move ball1
            ball1.x += ball1.xIncreamentor;
            ball1.y += ball1.yIncreamentor;
            //move ball2
            ball2.x += ball2.xIncreamentor;
            ball2.y += ball2.yIncreamentor;

            /*  if ((ball1.x + 50 >= ball2.x - 50)
                  && (ball1.x + 50 <= ball2.x + 50)
                  && (ball1.y + 50 >= ball2.y - 50)
                  && (ball1.y + 50) <= ball2.y - 50)
              {
                  ball1.xIncreamentor = -ball1.xIncreamentor;
                  ball2.xIncreamentor = -ball2.xIncreamentor;
              }*/
            if ((projectile.x >= ball1.x - 50)
                && (projectile.y <= ball1.y + 50)
                && (projectile.y >= ball1.y - 50)
                && (projectile.x <= ball1.x + 50))
            {
                ball1.hit--;
            }
            if ((projectile.x >= ball2.x - 50)
                && (projectile.y <= ball2.y + 50)
                && (projectile.y >= ball2.y - 50)
                && (projectile.x <= ball2.x + 50))
            {
                ball2.hit--;
            }

            if (1 - turn == 1)
            {
                if (ball1.hit == 1)
                {
                    S2D_PlaySound(fail);
                    player1.playerScore -= 3;
                    ball1.hit--;
                    ball2.hit -= 2;
                }
                else if (ball2.hit == 1)
                {
                    S2D_PlaySound(pop);
                    player1.playerScore += 5;
                    ball2.hit--;
                    ball1.hit -= 2;
                }
            }
            else if (1 - turn == 0)
            {
                if (ball1.hit == 1)
                {
                    S2D_PlaySound(pop);
                    player2.playerScore += 5;
                    ball1.hit--;
                    ball2.hit -= 2;
                }
                else if (ball2.hit == 1)
                {
                    S2D_PlaySound(fail);
                    player2.playerScore -= 3;
                    ball2.hit--;
                    ball1.hit -= 2;
                }
            }

            if (player1.playerScore >= score)
            {
                player1.winner = true;
                printWinner;
            }
            else if (player2.playerScore >= score)
            {
                player2.winner = true;
                printWinner = true;
            }
            if (projectile.x > 799 || projectile.y > 599 || projectile.y < 0)
            {
                shoot = false;
            }

            if (shoot == true)
            {
                // Update position
                projectile.x += projectile.vx;
                projectile.y += projectile.vy;
                // Update velocity
                projectile.vx += projectile.ax;
                projectile.vy += projectile.ay;
            }
        }
        else
            printWinner = true;
    }
}
////////////////////////Render///////////////
void render()
{
    S2D_DrawQuad(
        0, 0, 1, 1, 1, 0.8,
        window->width, 0, 1, 1, 1, 0.8,
        window->width, window->height, 1, 1, 1, 0.8,
        0, window->height, 1, 1, 1, 0.8
    );

    if (gameStarted)
    {
        if (shoot)
        {
            S2D_DrawCircle(projectile.x, projectile.y, 10, 100, 1, 0, 0, 1);
        }


        S2D_SetText(p1Name, "%s->%s: %d", "P1", player1.playerName, player1.playerScore);
        S2D_SetText(p2Name, "%s->%s: %d", "P2", player2.playerName, player2.playerScore);
        S2D_SetText(scoreToBeat, "%s: %d", "Win score", score);
        S2D_SetText(gameSpeed, "%s: %d", "Speed", speed);
        if (turn == 1)
            S2D_SetText(turntxt, "%s: %s", "Turn", player1.playerName);
        else
            S2D_SetText(turntxt, "%s: %s", "Turn", player2.playerName);

        p1NameInBall->x = ball1.x - 35;
        p1NameInBall->y = ball1.y - 15;

        p2NameInBall->x = ball2.x - 35;
        p2NameInBall->y = ball2.y - 15;

        S2D_DrawText(p1NameInBall);
        S2D_DrawText(p2NameInBall);
        S2D_DrawText(p1Name);
        S2D_DrawText(p2Name);
        S2D_DrawText(scoreToBeat);
        if (!shoot)
            S2D_DrawText(turntxt);
        S2D_DrawText(gameSpeed);
        if (window->frames % 1 == 0)
        {
            S2D_DrawCircle(ball1.x, ball1.y, 50, 100, 0.39, 0.78, 0.52, 0.5);
            S2D_DrawCircle(ball2.x, ball2.y, 50, 100, 0.76, 0.92, 0.45, 0.5);

        }
        S2D_DrawLine(0, 599, line.x1, line.y1,
            2,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 0, 0, 1);

        S2D_DrawTriangle(
            line.x1 - 4, line.y1 - 4, 0, 0, 1, 1,
            line.x1 + 8, line.y1 - 8, 0, 0, 1, 1,
            line.x1 + 4, line.y1 + 4, 0, 0, 1, 1
        );

        //S2D_DrawSprite(spr);

        if (printWinner)
        {
            if (player1.winner)
            {
                S2D_SetText(winner, "%s %s", player1.playerName, "Win!!! ");
                S2D_DrawText(winner);
                S2D_DrawText(pressEsq);
            }

            else
            {
                S2D_SetText(winner, "%s %s", player2.playerName, "Win!!!");
                S2D_DrawText(winner);
                S2D_DrawText(pressEsq);
            }
        }
    }
    else
    {
        if (!helpPressed)
        {
            S2D_DrawText(welcome);
            S2D_DrawText(howToPlay);
            S2D_DrawText(startGame);
        }
        else
        {
            S2D_SetText(howToPlay, "%s", "Check the Console :)");
            S2D_DrawText(howToPlay);
            S2D_DrawText(startGame);
        }
    }
    
}
/////////////////////Main////////////////////
int main() {
    
    window = S2D_CreateWindow("Pop The Ballon", 800, 600, update, render, 0); //Create window
    /////////Getting inputs///////////
    int choice;
    std::cout << "1-Create a newgame\n2-Load last save\n->";
    std::cin >> choice;
    if(choice == 1)
    {
        std::cout << "Enter the first players name: \n->";
        std::cin >> player1.playerName;
        std::cout << "Enter the second players name: \n->";
        std::cin >> player2.playerName;
        std::cout << "Enter the score to beat: \n->";
        std::cin >> score;
        std::cout << "Enter the speed from 1-3: \n->";
        std::cin >> speed;
        std::cout << "Enter the music volume: \n->";
        std::cin >> musicVolume;
        ball1.xIncreamentor = ball1.xIncreamentor * speed;
        ball1.yIncreamentor = ball1.yIncreamentor * speed;
        ball2.xIncreamentor = ball2.xIncreamentor * speed;
        ball2.yIncreamentor = ball2.yIncreamentor * speed;
        
        //////////setup balls////////////////
        srand(time(nullptr));

        ball1.x = rand() % 799;
        if (ball1.x - 50 <= 200)
            ball1.x += 200;
        else if (ball1.x + 50 >= 799)
            ball1.x -= 100;

        ball1.y = rand() % 599;
        if (ball1.y + 50 >= 599)
            ball1.y -= 100;
        else if (ball1.y - 50 <= 0)
            ball1.y += 100;

        ball2.x = rand() % 800;
        if (ball2.x - 50 <= 200)
            ball2.x += 200;
        else if (ball2.x + 50 >= 799)
            ball2.x -= 100;

        ball2.y = rand() % 600;
        if (ball2.y + 50 >= 599)
            ball2.y -= 100;
        else if (ball2.y - 50 <= 0)
            ball2.y += 100;

    }
    else if (choice == 2)
    {
        Load();
    }
    //////////Setuping window////////////
    window->on_key = on_key;
    window->icon = "media/icon.png";
    //////////Adding Shooter(sprite)/////
    spr = S2D_CreateSprite("./media/arrow.png");
    S2D_ClipSprite(spr, 100, 100, 50, 50);
    arrow = S2D_CreateImage("./media/arrow.png");


    //////////Adding texts///////////////
    welcome = S2D_CreateText("./media/vera.ttf", "Welcome", 40); //Welcome message
    welcome->x = 10;
    welcome->y = 250;
    welcome->color.r = 0.0;
    welcome->color.g = 0.0;
    welcome->color.b = 0.0;
    welcome->color.a = 0.5;
    //////////////////////
    howToPlay = S2D_CreateText("./media/vera.ttf", "Press 'H' for how to play", 40); //How to play message
    howToPlay->x = 10;
    howToPlay->y = 300;
    howToPlay->color.r = 0.0;
    howToPlay->color.g = 0.0;
    howToPlay->color.b = 0.0;
    howToPlay->color.a = 0.5;
    //////////////////////
    startGame = S2D_CreateText("./media/vera.ttf", "Press 'N' to start the game", 40); //Start the game message
    startGame->x = 10;
    startGame->y = 350;
    startGame->color.r = 0.0;
    startGame->color.g = 0.0;
    startGame->color.b = 0.0;
    startGame->color.a = 0.5;
    //////////////////////
    p1Name = S2D_CreateText("./media/vera.ttf", player1.playerName, 25); //Player 1 name
    p1Name->x = 10;
    p1Name->y = 20;
    p1Name->color.r = 0.0;
    p1Name->color.g = 0.0;
    p1Name->color.b = 0.0;
    p1Name->color.a = 0.5;
    //////////////////////
    p2Name = S2D_CreateText("./media/vera.ttf", player2.playerName, 25); //Player 2 name
    p2Name->x = 10;
    p2Name->y = 60;
    p2Name->color.r = 0.0;
    p2Name->color.g = 0.0;
    p2Name->color.b = 0.0;
    p2Name->color.a = 0.5;
    /////////////////////
    p1NameInBall = S2D_CreateText("./media/vera.ttf", "Player1", 20); //The text in ball 1
    p1NameInBall->color.r = 0.0;
    p1NameInBall->color.g = 0.0;
    p1NameInBall->color.b = 0.0;
    p1NameInBall->color.a = 0.5;
    /////////////////////
    p2NameInBall = S2D_CreateText("./media/vera.ttf", "Player2", 20); //The text in ball 2
    p2NameInBall->color.r = 0.0;
    p2NameInBall->color.g = 0.0;
    p2NameInBall->color.b = 0.0;
    p2NameInBall->color.a = 0.5;
    /////////////////////
    scoreToBeat = S2D_CreateText("./media/vera.ttf", "Win score", 25); //Score to win
    scoreToBeat->x = 10;
    scoreToBeat->y = 100;
    scoreToBeat->color.r = 0.0;
    scoreToBeat->color.g = 0.0;
    scoreToBeat->color.b = 0.0;
    scoreToBeat->color.a = 0.5;
    ////////////////////
    turntxt = S2D_CreateText("./media/vera.ttf", "Turn: ", 25); //Turn
    turntxt->x = 10;
    turntxt->y = 140;
    turntxt->color.r = 0.0;
    turntxt->color.g = 0.0;
    turntxt->color.b = 0.0;
    turntxt->color.a = 0.5;
    ////////////////////
    gameSpeed = S2D_CreateText("./media/vera.ttf", "Speed: ", 25); //Game speed
    gameSpeed->x = 10;
    gameSpeed->y = 180;
    gameSpeed->color.r = 0.0;
    gameSpeed->color.g = 0.0;
    gameSpeed->color.b = 0.0;
    gameSpeed->color.a = 0.5;
    ////////////////////
    winner = S2D_CreateText("./media/vera.ttf", "Youwin", 50); //Winner message
    winner->x = 10;
    winner->y = 220;
    winner->color.r = 0.0;
    winner->color.g = 0.0;
    winner->color.b = 0.0;
    winner->color.a = 0.5;
    ///////////////////
    pressEsq = S2D_CreateText("./media/vera.ttf", "Press Esq to exit", 50); //Press to quit message
    pressEsq->x = 10;
    pressEsq->y = 270;
    pressEsq->color.r = 0.0;
    pressEsq->color.g = 0.0;
    pressEsq->color.b = 0.0;
    pressEsq->color.a = 0.5;
    ///////Adding background music///////
    backGroundMusic = S2D_CreateMusic("./media/bgMusic.mp3");
    S2D_PlayMusic(backGroundMusic,true);
    S2D_SetMusicVolume(musicVolume);
    //////////Adding sound effects///////
    pop = S2D_CreateSound("./media/pop.wav");
    fail = S2D_CreateSound("./media/failShot.mp3");
    shot = S2D_CreateSound("./media/shot.mp3");




    S2D_Show(window); //Show the created window
    ///Freeing the dynamicly alocated objects///
    S2D_FreeSprite(spr);
    S2D_FreeImage(arrow);
    S2D_FreeText(welcome);
    S2D_FreeText(howToPlay);
    S2D_FreeText(startGame);
    S2D_FreeText(p1Name);
    S2D_FreeText(p2Name);
    S2D_FreeText(p1NameInBall);
    S2D_FreeText(p2NameInBall);
    S2D_FreeText(scoreToBeat);
    S2D_FreeText(turntxt);
    S2D_FreeText(gameSpeed);
    S2D_FreeText(winner);
    S2D_FreeText(pressEsq);
    S2D_FreeMusic(backGroundMusic);
    S2D_FreeSound(pop);
    S2D_FreeSound(fail);
    S2D_FreeSound(shot);
    S2D_FreeWindow(window);
    return 0;
}
