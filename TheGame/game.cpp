#include "game.h"
#include <algorithm>
#include <fstream>
#include <vector>

float ballSpeed = 2.f;
float paddleSpeed = 2.6f;

// utworzenie okna gry
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "The Game");

// ----------------------------------------   CLASS BALL    ---------------------------------------- //

Ball::Ball(float mX, float mY)
{
	speed.x = ballSpeed;
	speed.y = -ballSpeed;
	shape.setPosition(mX, mY);
	shape.setRadius(ballRadius);
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(ballRadius, ballRadius);
}

void Ball::update()
{
	shape.move(speed);

	if (left() < 0)
		speed.x = ballSpeed;
	else if (right() > windowWidth)
		speed.x = -ballSpeed;

	if (top() < 0)
		speed.y = ballSpeed;
	else if (bottom() > windowHeight)
		speed.y = -ballSpeed;
}

float Ball::x() const
{
	return shape.getPosition().x;
}

float Ball::y() const
{
	return shape.getPosition().y;
}

float Ball::left() const
{
	return x() - shape.getRadius();
}

float Ball::right() const
{
	return x() + shape.getRadius();
}

float Ball::top() const
{
	return y() - shape.getRadius();
}

float Ball::bottom() const
{ 
	return y() + shape.getRadius();
}

sf::CircleShape Ball::getShape() const
{
	return shape;
}

sf::Vector2f& Ball::getSpeed()
{
	return speed;
}

// ----------------------------------------   CLASS RECTANGLE    ---------------------------------------- //

float Rectangle::x() const
{
	return shape.getPosition().x;
}

float Rectangle::y() const
{
	return shape.getPosition().y;
}

float Rectangle::left() const
{
	return x() - shape.getSize().x / 2.f;
}

float Rectangle::right() const
{
	return x() + shape.getSize().x / 2.f;
}

float Rectangle::top() const
{
	return y() - shape.getSize().y / 2.f;
}
float Rectangle::bottom() const
{
	return y() + shape.getSize().y / 2.f;
}

sf::RectangleShape& Rectangle::getShape()
{
	return shape;
}

// ----------------------------------------   CLASS PADDLE    ---------------------------------------- //

Paddle::Paddle(float mX, float mY)
{
	getShape().setPosition(mX, mY);
	getShape().setSize({ paddleWidth, paddleHeight });
	getShape().setFillColor(sf::Color::Red);
	getShape().setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
}

void Paddle::update()
{
	getShape().move(speed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
		speed.x = -paddleSpeed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < windowWidth)
		speed.x = paddleSpeed;
	else
		speed.x = 0;
}

// ----------------------------------------   CLASS BRICK    ---------------------------------------- //

Brick::Brick(float mX, float mY)
{
	getShape().setPosition(mX, mY);
	getShape().setSize({ blockWidth, blockHeight });
	getShape().setFillColor(sf::Color::Yellow);
	getShape().setOrigin(blockWidth / 2.f, blockHeight / 2.f);
}

void Brick::setDestroyed(bool state)
{
	destroyed = state;
}

bool Brick::isDestroyed() const
{
	return destroyed;
}

// ----------------------------------------   CLASS GAME    ---------------------------------------- //

Game::Game()
{
	state = END;
	font.loadFromFile("desib.ttf");
	window.setFramerateLimit(limit); // ustawienie limitu klatek
	score = 0;
	level = 1;
	state = MENU;
}

void Game::runGame()
{
	while (state != END)
	{
		switch (state)
		{
		case GameState::MENU:
			menu();
			break;
		case GameState::SCORES:
			scores();
			break;
		case GameState::GAME:
			newGame();
			break;
		case GameState::GAME_OVER:
			gameOver();
			break;
		}
	}
}

void Game::menu()
{
	sf::Text title("The Game", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, 20);

	const int n = 3;
	sf::Text text[n];
	std::string str[] = { "Play", "Scores", "Exit" };
	for (int i = 0; i < n; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(65);

		text[i].setString(str[i]);
		text[i].setPosition(windowWidth / 2 - text[i].getGlobalBounds().width / 2, 250.f + i * 120.f);
	}

	while (state == MENU)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				state = END;
			}
			else if (text[0].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = GAME;
			}
			else if (text[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = SCORES;
			}
			else if (text[2].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = END;
			}
		}
		for (int i = 0; i < n; i++)
			if (text[i].getGlobalBounds().contains(mouse))
				text[i].setFillColor(sf::Color::Cyan);
			else text[i].setFillColor(sf::Color::White);

			window.clear();
			window.draw(title);
			for (int i = 0; i < n; i++)
				window.draw(text[i]);
			window.display();
	}
}

void Game::scores()
{
	sf::Text title("Best scores:", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, 20);

	sf::Text back("Back", font, 40);
	back.setPosition(windowWidth - back.getGlobalBounds().width - 60.f, windowHeight - back.getGlobalBounds().height - 60.f);

	const int n = 10;
	sf::Text text[n];
	std::string str[n];
	std::ifstream input;
	input.open("scores.txt");
	if (input)
	{
		for (int i = 0; i < n; i++)
		{
			input >> str[i];
			str[i] = std::to_string(i+1) + ". " + str[i];
		}
	}
	input.close();
	
	for (int i = 0; i < n; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(40);

		text[i].setString(str[i]);
		text[i].setPosition(windowWidth / 2 - text[i].getGlobalBounds().width / 2, 150.f + i * 50.f);
	}

	while (state == SCORES)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				state = END;
			}
			else if (back.getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = MENU;
			}
		}
		if (back.getGlobalBounds().contains(mouse))
			back.setFillColor(sf::Color::Cyan);
		else back.setFillColor(sf::Color::White);

		window.clear();
		window.draw(title);
		window.draw(back);
		for (int i = 0; i < n; i++)
			window.draw(text[i]);
		window.display();
	}
}

void Game::newGame()
{
	Ball ball((windowWidth - ballRadius) / 2, (windowHeight - ballRadius) / 2); // utworzenie obiektu pi³ki wraz z jej pozycj¹ pocz¹tkow¹
	Paddle paddle(windowWidth / 2, windowHeight - 50); // utworzenie obiektu paletki wraz z jej pozycj¹ pocz¹tkow¹
	// utworzenie wektora zawieraj¹cego cegie³ki
	std::vector<Brick> bricks;
	for (int iX = 0; iX < countBlocksX; ++iX)
		for (int iY = 0; iY < countBlocksY; ++iY)
			bricks.emplace_back((iX + 1) * (blockWidth + 4) + 25, (iY + 2) * (blockHeight + 4)); // stwórz cegie³kê i do³¹cz j¹ na koniec wektora

	// punktacja + poziom trudnoœci
	score = 0;
	level = 1;

	const int n = 2;
	sf::Text text[n];
	std::string str[] = { "Level: " + std::to_string(level), "Score: " + std::to_string(score) };
	for (int i = 0; i < n; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(60);

		text[i].setString(str[i]);
		text[i].setPosition(30.f, (windowHeight - 200) + i * 60.f);
	}

	sf::Event event;

	while (state == GAME)
	{
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
			{
				state = END;
			}
		}

		ball.update();
		paddle.update();
		testCollision(paddle, ball);

		if (ball.bottom() > paddle.bottom() + 20.f)
			state = GAME_OVER;

		for (auto& brick : bricks)
		{
			testCollision(brick, ball);
			if (brick.isDestroyed())
			{
				score += 10;
				text[1].setString("Score: " + std::to_string(score));
			}
		}
		bricks.erase(remove_if(begin(bricks), end(bricks), [](const Brick& mBrick) { return mBrick.isDestroyed(); }),
			end(bricks));

		if (bricks.empty())
		{
			for (int iX = 0; iX < countBlocksX; ++iX)
				for (int iY = 0; iY < countBlocksY; ++iY)
					//stwórz cegie³kê i dodaj j¹ do wektora
					bricks.emplace_back((iX + 1) * (blockWidth + 4) + 25, (iY + 2) * (blockHeight + 4));
			ballSpeed *= 1.5f;
			paddleSpeed *= 1.5f;
			level++;
			text[0].setString("Level: " + std::to_string(level));
		}

		window.clear();
		for (int i = 0; i < n; i++)
			window.draw(text[i]);
		window.draw(ball.getShape());
		window.draw(paddle.getShape());
		for (auto& brick : bricks)
			window.draw(brick.getShape());
		window.display();
	}
}

void Game::gameOver()
{
	sf::Text title("Game Over", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setFillColor(sf::Color::Red);
	title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, 20.f);

	const int n = 4;
	sf::Text text[n];
	std::string str[] = { "Your score: " + std::to_string(score), "Play again", "Back to menu", "Exit" };
	for (int i = 0; i < n; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(65);

		text[i].setString(str[i]);
		text[i].setPosition(windowWidth / 2 - text[i].getGlobalBounds().width / 2, 250.f + i * 120.f);
	}

	std::vector<int> v;
	std::ifstream input;
	input.open("scores.txt");
	if (input)
	{
		std::string buffer;
		while (input >> buffer)
			v.push_back(std::stoi(buffer));
	}
	input.close();

	v.push_back(score);
	std::sort(v.begin(), v.end(), [](int a, int b) {
		return b < a;
	});
	v.resize(10);

	std::ofstream output;
	output.open("scores.txt");
	if (output)
	{
		for (auto& e : v)
			output << e << " ";
	}
	output.close();

	while (state == GAME_OVER)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				state = END;
			}
			else if (text[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = GAME;
			}
			else if (text[2].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = MENU;
			}
			else if (text[3].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = END;
			}
		}
		for (int i = 1; i < n; i++)
			if (text[i].getGlobalBounds().contains(mouse))
				text[i].setFillColor(sf::Color::Cyan);
			else text[i].setFillColor(sf::Color::White);

			window.clear();
			window.draw(title);
			for (int i = 0; i < n; i++)
				window.draw(text[i]);
			window.display();
	}
}


// -------------------------------------------------------------------------------------------------- //

void testCollision(Paddle& mPaddle, Ball& mBall)
{
	// jeœli kolizja pomiêdzy pi³eczk¹ a paletk¹ nie wystêpuje to opuœæ funkcjê
	if (!isOverlapping(mPaddle, mBall)) return;

	// jeœli powy¿szy warunek nie jest spe³niony to odbij pi³êczkê
	mBall.getSpeed().y = -ballSpeed;
	if (mBall.x() < mPaddle.x() && mPaddle.left() > 0 && mPaddle.right() < windowWidth)
		mBall.getSpeed().x = -ballSpeed;
	else
		mBall.getSpeed().x = ballSpeed;
}

void testCollision(Brick& mBrick, Ball& mBall)
{
	// jeœli kolizja pomiêdzy pi³eczk¹ a paletk¹ nie wystêpuje to opuœæ funkcjê
	if (!isOverlapping(mBrick, mBall)) return;

	// jeœli powy¿szy warunek nie jest spe³niony to ustaw flagê zniszczenia cegie³ki
	mBrick.setDestroyed(true);

	// wykrywanie sk¹d nadlecia³a pi³eczka przy uderzeniu w cegie³kê
	float overlapLeft = mBall.right() - mBrick.left();
	float overlapRight = mBrick.right() - mBall.left();
	float overlapTop = mBall.bottom() - mBrick.top() ;
	float overlapBottom = mBrick.bottom() - mBall.top();

	bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
	bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

	float minOverlapX = ballFromLeft ? overlapLeft : overlapRight;
	float minOverlapY = ballFromTop ? overlapTop : overlapBottom;

	// nadanie zwrotu pi³eczce po odbiciu od cegie³ki na podstawie wczeœniejszych obliczeñ
	if (abs(minOverlapX) < abs(minOverlapY))
		mBall.getSpeed().x = ballFromLeft ? -ballSpeed : ballSpeed;
	else
		mBall.getSpeed().y = ballFromTop ? -ballSpeed : ballSpeed;
}