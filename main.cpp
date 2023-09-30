#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

typedef struct _Point
{
    int x;
    int y;
} Point;

void logRelativeSquarePosition(int, Point, const char *);
void logAbsolutePosition(const sf::Sprite &, int);
bool arePointsEqual(Point, Point);
Point rotatePoint(Point, Point);
Point rotatePoint(Point, Point, bool);
bool check();
void setUpShape(int);

Point a[4];
Point b[4];

const int M = 30;
const int N = 18;

int field[M][N] = {0};

const int figures[7][4] = {
    1, 3, 5, 7, // Incest (see:Rudi Guiliani)
    2, 4, 5, 7, // Z
    3, 4, 5, 6, // S
    3, 4, 5, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5  // O
};

int main(void)
{
    srand(time(0));
    int dx = 0;
    int dy = 0;
    bool rotateRight = false;
    bool rotateLeft = false;
    int colorNum = 3;
    bool initialised = false;
    bool keyPressed = false;
    float timer = 0.0f;
    float delay = 0.3f;

    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(N * 18, M * 18), "Tetris");

    sf::Texture t;

    t.loadFromFile("./images/tiles.png");

    sf::Sprite s(t);
    s.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event e;

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            if (e.type == sf::Event::KeyPressed)
            {
                switch (e.key.code)
                {
                case sf::Keyboard::Up:
                    rotateRight = true;
                    break;
                case sf::Keyboard::Down:
                    rotateLeft = true;
                    break;
                case sf::Keyboard::Left:
                    dx = -1;
                    break;
                case sf::Keyboard::Right:
                    dx = 1;
                    break;
                case sf::Keyboard::Space:
                    dy = 1;
                    break;
                default:
                    break;
                }
                keyPressed = true;
            }
        }
        /* ## <- Move -> ## */
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
            a[i].y += dy;
        }

        if (!check())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];

        /* ## Rotation ## */
        if (rotateRight || rotateLeft)
        {
            Point p = a[1]; // Center of Rotation
            // logRelativeSquarePosition(-1, p, "rotation center");
            for (int i = 0; i < 4; i++)
            {
                a[i] = rotatePoint(a[i], p, rotateRight);
            }
            if (!check())
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
        }

        /* Tick */
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                logRelativeSquarePosition(0, a[i], "A pre");
                logRelativeSquarePosition(0, b[i], "B pre");
                b[i] = a[i];
                a[i].y++;
                logRelativeSquarePosition(3, a[i], "A pre");
                logRelativeSquarePosition(3, b[i], "B pre");
            }
            if (!check())
            {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                setUpShape(colorNum);
            }
            timer = 0.0f;
        }

        if (!initialised)
        {
            initialised = true;
            setUpShape(colorNum);
        }
        dx = 0;
        dy = 0;
        rotateRight = false;
        rotateLeft = false;

        window.clear(sf::Color::White);

        for (int y = 0; y < M; y++)
        {
            for (int x = 0; x < N; x++)
            {
                if (field[y][x] == 0)
                    continue;
                s.setPosition(x * 18.f, y * 18.f);
                window.draw(s);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            s.setPosition(a[i].x * 18.f, a[i].y * 18.f);
            if (keyPressed)
                logAbsolutePosition(s, i);
            window.draw(s);
        }
        keyPressed = false;

        window.display();
    }

    return 0;
}

bool check()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return false;
        else if (field[a[i].y][a[i].x])
            return false;
    }

    return true;
}

Point rotatePoint(Point p, Point around)
{
    return rotatePoint(p, around, true);
}

Point rotatePoint(Point p, Point around, bool right)
{
    int factor = (right) ? 1 : -1;
    Point pointRotated;

    pointRotated.x = -1 * factor * (p.y - around.y);
    pointRotated.y = factor * (p.x - around.x);
    pointRotated.x += around.x;
    pointRotated.y += around.y;

    return pointRotated;
}

void setUpShape(int n)
{
    for (int i = 0; i < 4; i++)
    {
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
        logRelativeSquarePosition(i, a[i], "initial");
    }
}

void logRelativeSquarePosition(int row, Point pPoint, const char *msg)
{
    if (row == 0)
        std::cout << "-START----------" << msg << "-------------" << std::endl;
    std::cout << "a[" << row << "] = (" << pPoint.x << " / " << pPoint.y << ")" << std::endl;
    // std::cout << "a[" << row << "].y = " << pPoint.y << std::endl;
    if (row == 3)
        std::cout << "-END----------" << msg << "-------------" << std::endl;
}

void logAbsolutePosition(const sf::Sprite &pSprite, int row)
{
    if (row == 0)
        std::cout << "-Start----------" << std::endl;
    std::cout << " S(" << pSprite.getPosition().x << "/" << pSprite.getPosition().y << ")" << std::endl;
    if (row == 3)
        std::cout << "-End----------" << std::endl;
}

bool arePointsEqual(Point pointA, Point pointB)
{
    bool isEqual = true;
    if (pointA.x != pointB.x)
        isEqual = false;
    if (pointA.y != pointB.y)
        isEqual = false;
    return isEqual;
}