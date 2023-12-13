#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <array>
#include <algorithm>

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
bool positionOfShapeIsValid(const std::array<Point, 4> &);
void handleCompletedRows(sf::RenderWindow &window);
void setUpShape(sf::Sprite &, int, std::array<Point, 4> &);


const int M = 30;
const int N = 18;
const float offsetX = 35.f;
const float offsetY = 12.f;

int field[M][N] = {0};

const int figures[8][4] = {
    0, 0, 0, 0, // no shape for index zero
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 4, 5, 6, // S
    3, 4, 5, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5  // O
};

int main(void)
{
    std::array<Point, 4> a = {};
    std::array<Point, 4> b = {};
    srand((unsigned int)time(0));
    int dx = 0;
    int dy = 0;
    bool rotateRight = false;
    bool rotateLeft = false;
    int colorNum = 1;
    bool initialised = false;
    float timer = 0.0f;
    float delay = 0.3f;

    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode((N + 4) * 18, (M + 1) * 18), "Tetris");

    sf::Texture tTiles;
    sf::Texture tFrame;
    sf::Texture tBackground;

    tTiles.loadFromFile("./images/tiles.png");
    tFrame.loadFromFile("./images/frame2.png");
    tBackground.loadFromFile("./images/background2.png");

    sf::Sprite sTiles(tTiles);
    sf::Sprite sFrame(tFrame);
    sf::IntRect rectFrame = sFrame.getTextureRect();
    sf::Vector2u rectSize = window.getSize();

    std::cout << " width = " << rectFrame.width << " height = " << rectFrame.height << std::endl;
    std::cout << " width = " << rectSize.x << " height = " << rectSize.y << std::endl;
    sf::Sprite sBackground(tBackground);

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
                    // dy = 1;
                    delay = 0.05f;
                    break;
                default:
                    break;
                }
            }
        }
        /* ## <- Move -> ## */
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
            a[i].y += dy;
        }

        if (!positionOfShapeIsValid(a))
        {
            for (int i = 0; i < 4; i++)
            {
                a[i] = b[i];
            }
        }

        /* ## Rotation ## */
        if (rotateRight || rotateLeft)
        {
            Point p = a[1]; // Center of Rotation
            // logRelativeSquarePosition(-1, p, "rotation center");
            for (int i = 0; i < 4; i++)
            {
                a[i] = rotatePoint(a[i], p, rotateRight);
            }
            if (!positionOfShapeIsValid(a))
            {
                for (int i = 0; i < 4; i++)
                {
                    a[i] = b[i];
                }
            }
        }

        /* Tick */
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                // logRelativeSquarePosition(0, a[i], "A pre");
                // logRelativeSquarePosition(0, b[i], "B pre");
                b[i] = a[i];
                a[i].y++;
                // logRelativeSquarePosition(3, a[i], "A pre");
                // logRelativeSquarePosition(3, b[i], "B pre");
            }

            if (!positionOfShapeIsValid(a))
            {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                setUpShape(sTiles, colorNum, a);
            }
            timer = 0.0f;
        }

        handleCompletedRows(window);

        if (!initialised)
        {
            initialised = true;
            setUpShape(sTiles, colorNum, a);
        }
        dx = 0;
        dy = 0;
        rotateRight = false;
        rotateLeft = false;
        delay = 0.3f;

        window.clear(sf::Color::White);
        window.draw(sBackground);
        for (int y = 0; y < M; y++)
        {
            for (int x = 0; x < N; x++)
            {
                if (field[y][x] == 0)
                {
                    continue;
                }

                sTiles.setTextureRect(sf::IntRect(field[y][x] * 18, 0, 18, 18));
                sTiles.setPosition((float)x * 18.f, (float)y * 18.f);
                sTiles.move(offsetX, offsetY);
                window.draw(sTiles);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            sTiles.setPosition((float)a[i].x * 18.f, (float)a[i].y * 18.f);
            sTiles.move(offsetX, offsetY);

            sTiles.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));
            window.draw(sTiles);
        }

        window.draw(sFrame);
        window.display();
    }

    return 0;
}

void handleCompletedRows(sf::RenderWindow &window)
{
    int k = M - 1;

    for (int i = M - 1; i > 0; i--)
    {
        int counter = 0;
        for (int j = 0; j < N; j++)
        {
            if (field[i][j])
                counter++;
            field[k][j] = field[i][j];
        }
        if (counter < N)
            k--;
    }
}

bool positionOfShapeIsValid(const std::array<Point, 4> &arrPosition)
{
    for (int i = 0; i < 4; i++)
    {
        if (arrPosition[i].x < 0 || arrPosition[i].x >= N || arrPosition[i].y >= M || field[arrPosition[i].y][arrPosition[i].x])
            return false;
        // else if (field[arrPosition[i].y][arrPosition[i].x])
        //     return false;
    }

    return true;
}

/**
 * Rotates a point around another point.
 *
 * @param p The point to be rotated.
 * @param around The point around which the rotation occurs.
 *
 * @return The rotated point.
 *
 * @throws None.
 */
Point rotatePoint(Point p, Point around)
{
    return rotatePoint(p, around, true);
}

/**
 * Rotates a point around another point either to the right or left.
 *
 * @param p The point to be rotated.
 * @param around The point to rotate around.
 * @param right A boolean indicating whether to rotate to the right (true) or left (false).
 *
 * @return The rotated point.
 *
 * @throws None.
 */
Point rotatePoint(Point p, Point around, bool right)
{
    int factor = right ? 1 : -1;
    Point pointRotated;

    pointRotated.x = -1 * factor * (p.y - around.y);
    pointRotated.y = factor * (p.x - around.x);
    pointRotated.x += around.x;
    pointRotated.y += around.y;

    return pointRotated;
}

/**
 * Sets up the shape for the given input.
 *
 * @param n The index of the shape in the figures array.
 *
 * @throws ErrorType If the index is out of bounds.
 */
void setUpShape(sf::Sprite &pS, int n, std::array<Point, 4> &a)
{
    pS.setTextureRect(sf::IntRect(n * 18, 0, 18, 18));
    for (int i = 0; i < 4; i++)
    {
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
        // logRelativeSquarePosition(i, a[i], "initial");
    }
}

/**
 * Logs the relative square position.
 *
 * @param row the row number
 * @param pPoint the Point object
 * @param msg the message to be logged
 */
void logRelativeSquarePosition(int row, Point pPoint, const char *msg)
{
    if (row == 0)
        std::cout << "-START----------" << msg << "-------------" << std::endl;
    std::cout << "a[" << row << "] = (" << pPoint.x << " / " << pPoint.y << ")" << std::endl;

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