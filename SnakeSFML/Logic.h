#pragma once
#pragma warning(disable : 4996)

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <conio.h>
#include <Windows.h>
#include <vector>
using namespace sf;

class SnakePart
{
	RectangleShape part;
	Vector2f pos;
public:
	SnakePart(Vector2f start, Vector2f size = { 20,20 }, Color c = Color::Green) { this->pos = start; part.setSize(size); part.setPosition(start); part.setFillColor(c);  }
	Vector2f getPosition() { return pos; }
	void setPosition(Vector2f where) { part.setPosition(pos); pos = where; }
	RectangleShape& getRect() { return part; }
};
class Snake
{
	std::vector<SnakePart> snake;
public:
	Snake(Vector2f f = { 0,0 }) { snake.push_back(f); }
	void addSnakePart()
	{
		snake.push_back(SnakePart(snake[snake.size()-1].getPosition()));
	}
	std::vector<SnakePart>& getParts()
	{
		return snake;
	}
	void reset()
	{
		snake.clear();
		snake.push_back(Vector2f(0,0));
	}
};

class Fruit
{
	RectangleShape fruit;
	Vector2f pos;
public:
	Fruit(Vector2f where = { 80,80 }, Vector2f size = { 20,20 }, Color c = Color::Red) :pos(where) { fruit.setPosition(where); fruit.setSize(size); fruit.setFillColor(c); Texture* t=new Texture; t->loadFromFile("apple.png"); fruit.setTexture(t); }
	RectangleShape& getRect() { return fruit; }
	Vector2f getPosition() { return pos; }
	void setPosition(Vector2f where) {
		pos=where;
		fruit.setPosition(where);
	}
};

class Application
{
	Snake snake;
	Fruit fruit;
	UINT width;
	UINT height;
	UINT fruitEaten = 0;
	bool gameover = 0;
	void drawSnake(RenderWindow&window)
	{
		for (auto& i : snake.getParts())
		{
			window.draw(i.getRect());
		}
	}
	void drawField(RenderWindow&window)
	{
		RectangleShape block;
		block.setFillColor(Color::Black);
		block.setOutlineColor(Color::White);
		block.setOutlineThickness(1);
		block.setSize({20,20});
		for (size_t i = 0; i < height/20; i++)
		{
			for (size_t j = 0; j < width/20; j++)
			{
				block.setPosition(j*20,i*20);
				window.draw(block);
			}
		}
	}
	void inputControl(Keyboard::Key& prev)
	{
		if (Keyboard::isKeyPressed(Keyboard::Up)&&prev!=Keyboard::Down)
		{
			prev = Keyboard::Up;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)&& prev != Keyboard::Up)
		{
			prev = Keyboard::Down;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left) && prev != Keyboard::Right)
		{
			prev = Keyboard::Left;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) && prev != Keyboard::Left)
		{
			prev = Keyboard::Right;
		}
		for (size_t i = snake.getParts().size() - 1; i > 0; i--)
		{
			snake.getParts()[i].setPosition({ snake.getParts()[i - 1].getPosition().x, snake.getParts()[i - 1].getPosition().y });
		}
		switch (prev)
		{
		case Keyboard::Key::Up:	  snake.getParts()[0].setPosition({snake.getParts()[0].getPosition().x, snake.getParts()[0].getPosition().y - snake.getParts()[0].getRect().getSize().y });break;
		case Keyboard::Key::Down: snake.getParts()[0].setPosition({ snake.getParts()[0].getPosition().x, snake.getParts()[0].getPosition().y + snake.getParts()[0].getRect().getSize().y}); break;
		case Keyboard::Key::Left: snake.getParts()[0].setPosition({ snake.getParts()[0].getPosition().x- snake.getParts()[0].getRect().getSize().x, snake.getParts()[0].getPosition().y }); break;
		case Keyboard::Key::Right:snake.getParts()[0].setPosition({ snake.getParts()[0].getPosition().x+ snake.getParts()[0].getRect().getSize().x, snake.getParts()[0].getPosition().y }); break;
		}
	}
	void logic()
	{
		srand(time(0));
		if (snake.getParts()[0].getPosition() == fruit.getPosition()) 
		{
			snake.addSnakePart();
			bool f;
			do
			{
				f = true;
				Vector2f pos(rand() % width, rand() % height);
				fruit.setPosition(pos);
				for (size_t i = 0; i < snake.getParts().size(); i++)
				{
					if (snake.getParts()[i].getPosition() == pos||(int)pos.x%20!=0||(int)pos.y%20!=0)
						f = false;
				}
			} while (!f);
			fruitEaten++;
		}
		if (snake.getParts()[0].getPosition().x > width || snake.getParts()[0].getPosition().y > height || snake.getParts()[0].getPosition().x < 0 || snake.getParts()[0].getPosition().y < 0)
		{
			gameover = 1;
		}
		for (size_t i = 1; i < snake.getParts().size(); i++)
		{
			if (snake.getParts()[0].getPosition() == snake.getParts()[i].getPosition()&&snake.getParts().size()>2)
				gameover = 1;
		}
	}
public:
	Application(UINT width, UINT height) :width(width), height(height) {}
	void Play()
	{
		srand(time(0));
		RenderWindow window(VideoMode(width,height),"Snake");
		Keyboard::Key prev=Keyboard::Unknown;
		while (window.isOpen())
		{
			if (gameover)
			{
				window.clear();
				Text t;
				t.setString("Gameover!\n Your score: "+std::to_string(fruitEaten));
				t.setPosition(width/2-130,height/2);
				t.setColor(Color::Green);
				t.setCharacterSize(60);
				Font font;
				font.loadFromFile("caviar-dreams.ttf");
				t.setFont(font);
				window.draw(t);
				window.display();
				Sleep(2000);
				window.close();
			}
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
				}
			}
			window.clear(Color::Black);
			window.setTitle("Snake" + std::to_string(fruitEaten));
			drawField(window);
			drawSnake(window);
			window.draw(fruit.getRect());
			inputControl(prev);
			logic();
			window.display();
			Sleep(50);
		}
	}
};