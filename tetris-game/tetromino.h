#pragma once

BOOL createTetromino(Tetromino* currentTetromino, int x, int y, int type);
BOOL downTetromino(Tetromino* currentTetromino, GameStatus* currentGameStatus, int* score);
BOOL moveTetromino(Tetromino* t, MoveType type);
BOOL rotateTetromino(Tetromino* t, RotateType type);