#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include <string>

//rozmiar okna
constexpr unsigned int windowWidth = 1024, windowHeight = 768;
//pilka
constexpr float ballRadius = 13.f;
extern float ballSpeed;
//paletka
constexpr float paddleWidth = 148.f;
constexpr float paddleHeight = 22.f;
extern float paddleSpeed;
//cegie³ki
constexpr float blockWidth = 84.f;
constexpr float blockHeight = 28.f;
constexpr int countBlocksX = 10, countBlocksY = 4;

constexpr int limit = 100;

class Ball
{
	sf::CircleShape shape;
	sf::Vector2f speed;

public:
	Ball(float mX, float mY);
	void update();
	float x() const;
	float y() const;
	float left() const;
	float right() const;
	float top() const;
	float bottom() const;
	sf::CircleShape getShape() const;
	sf::Vector2f& getSpeed();
};

class Rectangle
{
	sf::RectangleShape shape;

public:
	float x() const;
	float y() const;
	float left() const;
	float right() const;
	float top() const;
	float bottom() const;
	sf::RectangleShape& getShape();
};

class Paddle : public Rectangle
{
	sf::Vector2f speed;

public:
	Paddle(float mX, float mY);
	void update();
};

class Brick : public Rectangle
{
	bool destroyed = false;

public:
	Brick(float mX, float mY);
	void setDestroyed(bool state);
	bool isDestroyed() const;
};

class Game
{
	sf::Font font;
	unsigned int score;
	unsigned int level;
	void menu();
	void scores();
	void newGame();
	void gameOver();

protected:
	enum GameState { MENU, SCORES, GAME, GAME_OVER, END };
	GameState state;

public:
	Game();
	void runGame();	
};

// sprawdzenie czy dwa obiekty nak³adaj¹ siê na siebie (kolizja)
template <class T1, class T2>
bool isOverlapping(T1& mA, T2& mB)
{
	return mA.right() >= mB.left() && mA.left() <= mB.right() && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
};

void testCollision(Paddle& mPaddle, Ball& mBall);
void testCollision(Brick& mBrick, Ball& mBall);