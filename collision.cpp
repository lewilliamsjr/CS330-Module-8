#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>
using namespace std;
const float DEG2RAD = 3.14159 / 180;
float paddlePos = 0;
void processInput(GLFWwindow* window);
enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
enum ONOFF { ON, OFF };
float rr = rand() / 10000;
float gg = rand() / 10000;
float bb = rand() / 10000;
class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	BRICKTYPE brick_type;
	ONOFF onoff;
	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue =
			bb;
		onoff = ON;
	};
	void drawBrick()
	{
		if (onoff == ON)
		{
			double halfside = width / 2;
			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);
			glVertex2d(x + halfside, y + halfside);
			glVertex2d(x + halfside, y - halfside);
			glVertex2d(x - halfside, y - halfside);
			glVertex2d(x - halfside, y + halfside);
			glEnd();
		}
	}
};
class Circle
	
{
public:
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.02;
	int direction; // 1=up 2=right 3=down 4=left 5 = up right 6 = up left 7 = down right 8 = down left
		Circle(double xx, double yy, double rr, int dir, float rad, float r, float g,
			float b)
	{
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
	}
	void CheckCollision(Brick* brk)
	{
		if (brk->brick_type == REFLECTIVE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk -> y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				x = x + 0.03;
				y = y + 0.04;
				speed += 0.01;
				if (speed > 0.15)
					speed = 0.01;
			}
		}
		else if (brk->brick_type == DESTRUCTABLE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk -> y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				x = x + 0.03;
				y = y + 0.04;
				if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y >
					brk->y - brk->width && y <= brk->y + brk->width))
				{
					brk->onoff = OFF;
					speed -= 0.01;
				}
			}
		}
	}
	void CheckPaddleColl(Brick* brk)
	{
		
			if (((x > paddlePos - brk->width && x <= paddlePos + brk->width) ||
				(x > brk->x - brk->width && paddlePos <= brk->x + brk->width)) &&
				(y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				x = x + 0.03;
				y = y + 0.04;
				speed -= 0.001;
				if (speed > 0.15)
					speed = 0.01;
			}
	}
	void CircleCollision(Circle* cir)
	{
		if ((x > cir->x - cir->radius && x <= cir->x + cir->radius) && (y > cir->y
			- cir->radius && y <= cir->y + cir->radius))
		{
			double r, g, b;
			r = rand() / 10000;
			g = rand() / 10000;
			b = rand() / 10000;
			Circle C(0, 0, 03, 6, 03, r, g, b);
		}
	}
	int GetRandomDirection()
	{
		return (rand() % 8) + 1;
	}
	void MoveOneStep()
	{
		if (direction == 1 || direction == 5 || direction == 6) // up
		{
			if (y > -1 + radius)
			{
				y -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
		if (direction == 2 || direction == 5 || direction == 7) // right
		{
			if (x < 1 - radius)
			{
				x += speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
		if (direction == 3 || direction == 7 || direction == 8) // down
		{
			
				if (y < 1 - radius) {
					y += speed;
				}
				else
				{
					direction = GetRandomDirection();
				}
		}
		if (direction == 4 || direction == 6 || direction == 8) // left
		{
			if (x > -1 + radius) {
				x -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
	}
	void DrawCircle()
	{
		glColor3f(red, green, blue);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
		}
		glEnd();
	}
};
vector<Circle> world;
int main(void) {
	srand(time(NULL));
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(480, 480, "Random World of Circles",
		NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	Brick brick(REFLECTIVE, 0.5, -0.33, 0.1, 1, 1, 0);
	Brick brick2(DESTRUCTABLE, -0.5, 0.33, 0.1, 0, 1, 0);
	Brick brick3(DESTRUCTABLE, -0.5, -0.33, 0.1, 0, 1, 1);
	Brick brick4(REFLECTIVE, 0, 0, 0.2, 1, 0.5, 0.5);
	Brick paddlebrick(REFLECTIVE, paddlePos, -0.8, 0.2, rr, gg, bb);
	
		while (!glfwWindowShouldClose(window)) {
			//Setup View
			float ratio;
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			ratio = width / (float)height;
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);
			processInput(window);
			//Movement
			for (int i = 0; i < world.size(); i++)
			{
				world[i].CheckCollision(&brick);
				world[i].CheckCollision(&brick2);
				world[i].CheckCollision(&brick3);
				world[i].CheckCollision(&brick4);
				world[i].CheckPaddleColl(&paddlebrick);
				world[i].CircleCollision(&world[i]);
				world[i].MoveOneStep();
				world[i].DrawCircle();
			}
			brick.drawBrick();
			brick2.drawBrick();
			brick3.drawBrick();
			brick4.drawBrick();
			glPushMatrix(); //save the current matrix
			glTranslatef(paddlePos, 0, 0); //move to the desired location
			glColor3d(rr, gg, bb);
			paddlebrick.drawBrick();
			glPopMatrix(); //restore the old matrix
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		paddlePos -= 0.04;
	if (paddlePos < -0.9)
		paddlePos = -0.9;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		paddlePos += 0.04;
	if (paddlePos > 0.9)
		paddlePos = 0.9;
	
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			double r, g, b;
			r = rand() / 10000;
			g = rand() / 10000;
			b = rand() / 10000;
			Circle B(0, 0, 02, 2, 0.05, r, g, b);
			world.push_back(B);
		}
}