
#include "main.h"
#include "snake.h"
#include "max7219.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

extern UART_HandleTypeDef huart1;

uint8_t SnakeMatrix[8] = {0};
Point Snake[MAX_LENGTH];
Point Fruit;
uint8_t SnakeLength = 3;

Direction SnakeDirection = LEFT;


uint8_t LedMask(uint8_t x) {
    return (1 << x);
}

void DrawPoint(Point p) {
    SnakeMatrix[p.y] |= LedMask(p.x);
    MAX7219_SendData(p.y + 1, SnakeMatrix[p.y]);
}

void ClearPoint(Point p) {
    SnakeMatrix[p.y] &= ~(LedMask(p.x));
    MAX7219_SendData(p.y + 1, SnakeMatrix[p.y]);
}

void Snake_Init(void) {
    Snake[0].x = 3; Snake[0].y = 3; // head
    Snake[1].x = 2; Snake[1].y = 3;
    Snake[2].x = 1; Snake[2].y = 3; // tail

}

bool IsOnSnake(uint8_t x, uint8_t y)
{
    for (int i = 0; i < SnakeLength; i++){
        if (Snake[i].x == x && Snake[i].y == y)
        	return true;
    }
    return false;
}

void GenerateFruit(void)
{
    do {
        Fruit.x = rand() % 8;        	   // Generate a value between 0 and 7 for fruit
        Fruit.y = rand() % 8;
    }while (IsOnSnake(Fruit.x, Fruit.y));  // If this coordinate is on a part of the snake, reproduce it.
}

void DrawFruit(void)
{
    SnakeMatrix[Fruit.y] |= (1 << Fruit.x);
    MAX7219_SendData(Fruit.y + 1, SnakeMatrix[Fruit.y]);
}


void ResetGame(void)
{
    // Clean the LEDs
    for (int i = 0; i < 8; i++) {
        SnakeMatrix[i] = 0x00;
        MAX7219_SendData(i + 1, 0x00);
    }

    // Snake starting position
    SnakeLength = 3;
    Snake[0].x = 3; Snake[0].y = 3;
    Snake[1].x = 2; Snake[1].y = 3;
    Snake[2].x = 1; Snake[2].y = 3;

    SnakeDirection = LEFT;

    // Draw the snake
    for (int i = 0; i < SnakeLength; i++) {
        DrawPoint(Snake[i]);
    }

    // Create and draw a new fruit
    GenerateFruit();
    DrawFruit();
}


void GameOver(void)
{
    // Turn on all LEDs
    for (int i = 0; i < 8; i++) {
        SnakeMatrix[i] = 0xFF;
        MAX7219_SendData(i + 1, SnakeMatrix[i]);
    }
    HAL_Delay(500);

    // Random Fade Effect
    int ledsRemaining = 64;
    Point leds[64];
    int count = 0;

    // Fill in all LED coordinates
    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
            leds[count].x = x;
            leds[count].y = y;
            count++;
        }
    }

    // Turn off the LEDs in random order
    while (ledsRemaining > 0) {
        // Select one at random from the remaining LEDs
        int randomIndex = rand() % ledsRemaining;
        Point p = leds[randomIndex];

        // Turn off the selected LED
        ClearPoint(p);

        // Remove the extinguished LED from the list
        leds[randomIndex] = leds[ledsRemaining - 1];
        ledsRemaining--;

        HAL_Delay(30);
    }

    HAL_Delay(1000);

    // Reset the game
    ResetGame();
}


void SetDirection(char cmd) {
    switch (cmd) {
        case 'w': if (SnakeDirection != DOWN)  SnakeDirection = UP;    break;
        case 's': if (SnakeDirection != UP)    SnakeDirection = DOWN;  break;
        case 'a': if (SnakeDirection != RIGHT) SnakeDirection = LEFT;  break;
        case 'd': if (SnakeDirection != LEFT)  SnakeDirection = RIGHT; break;
    }
}


bool IsCollision(Point head)
{
    // It checks whether the snake's head collides with any of its own body segments.
    for (int i = 1; i < SnakeLength; i++)                   // except head
    {
        if (Snake[i].x == head.x && Snake[i].y == head.y)
            return true; 									// If there is a collision
    }
    return false; 											// If there is no collision
}


void MoveSnake(void)
{

    Point nextHead = Snake[0];

    // Calculate the new head coordinates based on the snake's specified direction.
    switch (SnakeDirection) {
        case UP:    nextHead.y = (nextHead.y == 0) ? 7 : nextHead.y - 1; break;
        case DOWN:  nextHead.y = (nextHead.y == 7) ? 0 : nextHead.y + 1; break;
        case RIGHT: nextHead.x = (nextHead.x == 0) ? 7 : nextHead.x - 1; break;
        case LEFT: 	nextHead.x = (nextHead.x == 7) ? 0 : nextHead.x + 1; break;
    }

    // Collision control with its own body
    if (IsCollision(nextHead)) {
        GameOver();
        return;
    }

    // If the snake eats fruit,
    if (nextHead.x == Fruit.x && nextHead.y == Fruit.y)
    {
        // Shift the body segments
        for (int i = SnakeLength; i > 0; i--)
            Snake[i] = Snake[i - 1];

        // New Head
        Snake[0] = nextHead;

        // Max limit check before extension
        if (SnakeLength < MAX_LENGTH)
            SnakeLength++;

        // Draw new head
        DrawPoint(Snake[0]);

        // Create and draw a new fruit
        GenerateFruit();
        DrawFruit();
    }
    else
    {
        // Delete the tail
        ClearPoint(Snake[SnakeLength - 1]);

        // Shift the body segments
        for (int i = SnakeLength - 1; i > 0; i--)
            Snake[i] = Snake[i - 1];

        // New Head
        Snake[0] = nextHead;
        // Draw new head
        DrawPoint(Snake[0]);
    }
}

