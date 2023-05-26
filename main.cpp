#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>

using namespace sf;
using namespace std;

int N = 32, M = 25;
int ts = 25;

int score = 0;
int dir = 2, num = 4;
int applesEaten = 0;

bool game = true;
bool messageDisplayed = false;

struct Snake {
    int x, y;
} s[600];

struct Fruct {
    int x, y;
} f;

void gameOver() {
    game = false;
}

void Tick() {
    for (int i = num; i > 0; i--) {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    // W A S D
    if (Keyboard::isKeyPressed(Keyboard::W) && dir != 0)
        dir = 3; // W
    else if (Keyboard::isKeyPressed(Keyboard::A) && dir != 2)
        dir = 1; // A
    else if (Keyboard::isKeyPressed(Keyboard::S) && dir != 3)
        dir = 0; // S
    else if (Keyboard::isKeyPressed(Keyboard::D) && dir != 1)
        dir = 2; // D

    if (dir == 0)
        s[0].y += 1; // Down
    else if (dir == 1)
        s[0].x -= 1; // Left
    else if (dir == 2)
        s[0].x += 1; // Right
    else if (dir == 3)
        s[0].y -= 1; // Up

    if (s[0].x > N || s[0].x < 0 || s[0].y > M || s[0].y < 0) {
        gameOver();
        return;
    }

    //spawn jabłka
    if (s[0].x == f.x && s[0].y == f.y) {
        num++;
        applesEaten++;
        score += (dir == 0) ? 3 : (dir == 1) ? 4 : (dir == 2) ? 2 : 1; // Подсчет очков
        f.x = rand() % N;
        f.y = rand() % M;
    }

    //skończenie gry przez wjebanie w siebie
    for (int i = 1; i < num; i++) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) {
            gameOver();
            return;
        }
    }
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(ts * N, ts * M), "Snake!");


    Texture t;
    t.loadFromFile("C:/Snake/Paint/tiles.png");
    Sprite tiles(t);

    Texture sn;
    sn.loadFromFile("C:/Snake/Paint/snake.png");
    Sprite snake(sn);

    Texture ap;
    ap.loadFromFile("C:/Snake/Paint/apple.png");
    Sprite apple(ap);

    Texture go;
    go.loadFromFile("C:/Snake/Paint/gameover.png");
    Sprite gameover(go);

    gameover.setPosition(30, 450);

    f.x = 10;
    f.y = 10;

    Clock clock;
    float timer = 0, delay = 0.1;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (timer > delay && game) {
            timer = 0;
            Tick();
        }

        window.clear();

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) {
                tiles.setPosition(i * ts, j * ts);
                window.draw(tiles);
            }

        for (int i = 0; i < num; i++) {
            if (i != 0)
                snake.setTextureRect(IntRect(0, 0, ts, ts));
            else
                snake.setTextureRect(IntRect(dir * ts, ts, ts, ts));

            if (!game && i == 1)
                snake.setTextureRect(IntRect(dir * ts, ts * 2, ts, ts));

            snake.setPosition(s[i].x * ts, s[i].y * ts);

            window.draw(snake);
        }

        apple.setPosition(f.x * ts, f.y * ts);
        window.draw(apple);

        if (!game) {
            window.draw(gameover);
        }

        window.display();
    }

    return 0;
}
