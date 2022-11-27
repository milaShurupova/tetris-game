#include <windows.h>
#include <stdio.h>
#include "entity.h"
#include "game-helper.h"
#include "tetromino.h"

int playField[PLAY_FIELD_HEIGHT_IN_BLOCKS][PLAY_FIELD_WIDTH_IN_BLOCKS];

void updateScore(int* score, HWND hScore)
{
    wchar_t scoreString[15];
    swprintf_s(scoreString, 15, L"Score: %d", *score);
    SetWindowTextW(hScore, scoreString);
}

void initializeGame(Tetromino* currentTetromino, GameStatus* currentGameStatus, int* score, HWND hWnd)
{
    int i, j;
    *currentGameStatus = PLAYING;
    *score = 0;
    srand((unsigned)time(NULL));
    SetTimer(hWnd, SPEED, SPEED, NULL);

    for (i = 1; i < (PLAY_FIELD_HEIGHT_IN_BLOCKS - 1); i++)
    {
        for (j = 1; j < (PLAY_FIELD_WIDTH_IN_BLOCKS - 1); j++)
        {
            playField[i][j] = EMPTY_BLOCK;
        }
    }

    for (i = 0; i < PLAY_FIELD_HEIGHT_IN_BLOCKS; i++)
    {
        playField[i][0] = BOUNDARY_BLOCK;
        playField[i][PLAY_FIELD_WIDTH_IN_BLOCKS - 1] = BOUNDARY_BLOCK;
    }

    for (j = 0; j < PLAY_FIELD_WIDTH_IN_BLOCKS; j++)
    {
        playField[PLAY_FIELD_HEIGHT_IN_BLOCKS - 1][j] = BOUNDARY_BLOCK;
    }

    createTetromino(currentTetromino, (PLAY_FIELD_WIDTH_IN_BLOCKS / 2 - 2), 0, rand() % TETROMINO_TYPES);
}