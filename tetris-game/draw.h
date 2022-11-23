#pragma once

typedef struct _DrawCoordinates
{
	int xBackground;
	int yBackground;
	int xScoreField;
	int xFieldBoundary;
	int yFieldBoundary;
	int xBuffer;
	int yBuffer;
} DrawCoordinates;

void initializateDrawCoordinates();
void drawBackground(HDC hdc);
void drawScoreField(HDC hdc);
void drawFieldBoundary(HDC hdc);
void drawField(HDC hdc);
void drawWithBuffer(HDC hdc);