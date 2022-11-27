#include "entity.h"
#include "tetromino-helper.h"
#include "tetromino.h"

int tetrominos[TETROMINO_TYPES][TETROMINO_HEIGHT][TETROMINO_WIDTH] =
{
	{
		{1,1,0,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0}
	},
	{
		{0,1,1,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{1,1,0,0},
		{0,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{1,1,0,0},
		{0,1,0,0},
		{0,1,0,0},
		{0,0,0,0}
	},
	{
		{1,1,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{0,0,0,0}
	},
	{
		{0,1,0,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
	}
};


BOOL createTetromino(Tetromino* currentTetromino, int x, int y, int type)
{
	int i, j;
	Tetromino t;

	t.x = x;
	t.y = y;
	t.type = type;


	for (i = 0; i < TETROMINO_HEIGHT; i++)
	{
		for (j = 0; j < TETROMINO_WIDTH; j++)
		{
			t.tetromino[i][j] = tetrominos[type][i][j];
		}
	}

	*currentTetromino = t;

	if (noCollisionCheck(&t, x, y) == FALSE)
	{
		return FALSE;
	}

	placeTetromino(currentTetromino);
	return TRUE;
}


BOOL moveTetromino(Tetromino* currentTetromino, MoveType type)
{
	Tetromino next_t = *currentTetromino;
	switch (type)
	{
	case MOVE_TO_LEFT:
		next_t.x = next_t.x - 1;
		break;
	case MOVE_TO_RIGHT:
		next_t.x = next_t.x + 1;
		break;
	case MOVE_TO_DOWN:
		next_t.y = next_t.y + 1;
		break;
	}

	removeTetromino(currentTetromino);

	if (placeTetromino(&next_t) == FALSE)
	{
		placeTetromino(currentTetromino);
		return FALSE;
	}

	*currentTetromino = next_t;
	return TRUE;
}


BOOL downTetromino(Tetromino* currentTetromino, GameStatus* currentGameStatus, int* score)
{
	if (moveTetromino(currentTetromino, MOVE_TO_DOWN) == FALSE)
	{
		stopTetromino(currentTetromino);
		*score += eraseLines();

		if (createTetromino(currentTetromino, PLAY_FIELD_WIDTH_IN_BLOCKS / 2, 0, rand() % TETROMINO_TYPES) == FALSE)
		{
			*currentGameStatus = GAME_OVER;
		}
		return FALSE;
	}
	return TRUE;
}


BOOL rotateTetromino(Tetromino* t, RotateType type)
{
	int i, j;
	Tetromino next_t = *t;

	if (type == CLOCKWISE)
	{
		for (i = 0; i < TETROMINO_HEIGHT; i++)
		{
			for (j = 0; j < TETROMINO_WIDTH; j++)
			{
				next_t.tetromino[i][j] = t->tetromino[TETROMINO_HEIGHT - 1 - j][i];
			}
		}
	}
	else
	{
		for (i = 0; i < TETROMINO_HEIGHT; i++)
		{
			for (j = 0; j < TETROMINO_WIDTH; j++)
			{
				next_t.tetromino[i][j] = t->tetromino[j][TETROMINO_WIDTH - 1 - i];
			}
		}
	}

	removeTetromino(t);

	if (placeTetromino(&next_t) == FALSE)
	{
		placeTetromino(t);
		return FALSE;
	}

	*t = next_t;
	return TRUE;
}