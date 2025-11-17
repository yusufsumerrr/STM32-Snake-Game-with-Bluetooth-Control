
#ifndef INC_SNAKE_H_
#define INC_SNAKE_H_

#define MAX_LENGTH 64

#include "main.h"
#include <stdbool.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} Point;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;


uint8_t LedMask(uint8_t x);
void DrawPoint(Point p);
void ClearPoint(Point p);
void Snake_Init(void);
bool IsOnSnake(uint8_t x, uint8_t y);
void GenerateFruit(void);
void DrawFruit(void);
bool IsCollision(Point head);
void ResetGame(void);
void GameOver(void);
void SetDirection(char cmd);
void MoveSnake(void);
#endif /* INC_SNAKE_H_ */
