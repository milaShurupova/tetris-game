#pragma once
#include "main.h"

void innerStopTetromino(Tetromino t, int x, int y);
void innerPlaceTetromino(Tetromino t, int x, int y);
void innerRemoveTetromino(Tetromino t, int x, int y);
//void innerCreateTetromino(Tetromino t, int x, int y, int type);
void forFunction(int y1, int y2, int x1, int x2, Tetromino t, void (*innerFunction)(Tetromino, int, int));
BOOL createTetromino(int x, int y, int type);
BOOL downTetromino();
BOOL moveTetromino(Tetromino t, MoveType type);
BOOL rotateTetromino(Tetromino t, RotateType type);
