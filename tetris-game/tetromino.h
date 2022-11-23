#pragma once

BOOL createTetromino(int x, int y, int type);
BOOL downTetromino();
BOOL moveTetromino(Tetromino t, MoveType type);
BOOL rotateTetromino(Tetromino t, RotateType type);
