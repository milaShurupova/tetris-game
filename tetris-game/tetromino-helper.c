#include "entity.h"
#include "tetromino-helper.h"

int playField[PLAY_FIELD_HEIGHT_IN_BLOCKS][PLAY_FIELD_WIDTH_IN_BLOCKS];


void forFunction(int y1, int y2, int x1, int x2, Tetromino* t, void (*innerFunction)(Tetromino*, int, int))
{
	int x, y;
	for (y = y1; y < y2; y++)
	{
		for (x = x1; x < x2; x++)
		{
			innerFunction(t, x, y);
		}
	}
}


void innerStopTetromino(Tetromino* t, int x, int y) 
{

	if (playField[t->y + y][t->x + x] == MOVING_BLOCK)
	{
		playField[t->y + y][t->x + x] = STAYING_BLOCK;
	}
}

void stopTetromino(Tetromino* t)
{
	forFunction(0, TETROMINO_HEIGHT, 0, TETROMINO_WIDTH, t, innerStopTetromino);
}


void innerPlaceTetromino(Tetromino* t, int x, int y) {

	if (t->tetromino[y][x] == MOVING_BLOCK)
	{
		playField[t->y + y][t->x + x] = MOVING_BLOCK;
	}
}

BOOL placeTetromino(Tetromino* t)
{
	if (noCollisionCheck(t, t->x, t->y) == FALSE)
	{
		return FALSE;
	}

	forFunction(0, TETROMINO_HEIGHT, 0, TETROMINO_WIDTH, t, innerPlaceTetromino);

	return TRUE;
}


void innerRemoveTetromino(Tetromino* t, int x, int y)
{
	if (playField[t->y + y][t->x + x] == MOVING_BLOCK)
	{
		playField[t->y + y][t->x + x] = EMPTY_BLOCK;
	}
}

void removeTetromino(Tetromino* t)
{
	forFunction(0, TETROMINO_HEIGHT, 0, TETROMINO_WIDTH, t, innerRemoveTetromino);
}


BOOL noCollisionCheck(Tetromino* t, int x, int y)
{
	int i, j;

	for (i = 0; i < TETROMINO_HEIGHT; i++)
	{
		for (j = 0; j < TETROMINO_WIDTH; j++)
		{

			if (t->tetromino[i][j] == 0)
			{
				continue;
			}

			int next_x = x + j;
			int next_y = y + i;

			if (playField[next_y][next_x] != EMPTY_BLOCK)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


void shiftLine(int n)
{
	int i, j;
	for (i = n; i > 0; i--)
	{
		for (j = 1; j < (PLAY_FIELD_WIDTH_IN_BLOCKS - 1); j++)
		{
			playField[i][j] = playField[i - 1][j];
		}
	}

	for (j = 1; j < (PLAY_FIELD_WIDTH_IN_BLOCKS - 1); j++)
	{
		playField[0][j] = EMPTY_BLOCK;
	}
}

BOOL isFullLine(int n)
{
	int i;
	for (i = 1; i < (PLAY_FIELD_WIDTH_IN_BLOCKS - 1); i++)
	{
		if (playField[n][i] == EMPTY_BLOCK)
		{
			return FALSE;
		}
	}
	return TRUE;
}

int eraseLines()
{
	int result = 0;
	int i;

	for (i = 1; i < (PLAY_FIELD_HEIGHT_IN_BLOCKS - 1); i++)
	{
		if (isFullLine(i) == TRUE)
		{
			shiftLine(i);
			result++;
		}
	}
	return result;
}