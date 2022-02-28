#pragma once
#include "Graphics/Graphics.h"


class Display
{
public:
	Display();
	~Display();

	void draw();

	Graphics graphics;

private:
};

Display::Display()
{

}

Display::~Display()
{

}

void Display::draw()
{
	graphics.drawFrame();
}