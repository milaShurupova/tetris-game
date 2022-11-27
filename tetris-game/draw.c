#include <windows.h>
#include "entity.h"
#include "draw.h"

extern int playField[PLAY_FIELD_HEIGHT_IN_BLOCKS][PLAY_FIELD_WIDTH_IN_BLOCKS];

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

DrawCoordinates drawCoordinates;

void initializateDrawCoordinates() 
{
	drawCoordinates.xBackground = PLAY_FIELD_WIDTH_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS * 2 + SUPPORT_FIELD_WIDTH_IN_PIXEL;
	drawCoordinates.yBackground = PLAY_FIELD_HEIGHT_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS * 2;
	drawCoordinates.xScoreField = PLAY_FIELD_WIDTH_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS * 3;
	drawCoordinates.xFieldBoundary = (BLOCK_SIZE_IN_PIXEL * PLAY_FIELD_WIDTH_IN_BLOCKS) + INDENT_IN_PIXELS;
	drawCoordinates.yFieldBoundary = (BLOCK_SIZE_IN_PIXEL * PLAY_FIELD_HEIGHT_IN_BLOCKS) + INDENT_IN_PIXELS;
	drawCoordinates.xBuffer = PLAY_FIELD_WIDTH_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS * 2 + SUPPORT_FIELD_WIDTH_IN_PIXEL;
	drawCoordinates.yBuffer = PLAY_FIELD_HEIGHT_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS * 2;
}


void drawBackground(HDC hdc)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(128, 128, 128));
	HBRUSH hBrushBackground = CreateSolidBrush(RGB(128, 128, 128));
	HBRUSH holdBrush = SelectObject(hdc, hBrushBackground);
	Rectangle(hdc, 0, 0, drawCoordinates.xBackground, drawCoordinates.yBackground);
	SelectObject(hdc, holdBrush);
	DeleteObject(hBrushBackground);
}


void drawScoreField(HDC hdc)
{
	static TCHAR buf[128];

	wsprintf(buf, L"Move right: push right arrow");
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(128, 128, 128));
	TextOut(hdc, drawCoordinates.xScoreField, 120, buf, lstrlen(buf));

	wsprintf(buf, L"Move left: push left arrow");
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(128, 128, 128));
	TextOut(hdc, drawCoordinates.xScoreField, 160, buf, lstrlen(buf));

	wsprintf(buf, L"Move down: push down arrow");
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(128, 128, 128));
	TextOut(hdc, drawCoordinates.xScoreField, 200, buf, lstrlen(buf));

	wsprintf(buf, L"Rotate clockwise: push up arrow");
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(128, 128, 128));
	TextOut(hdc, drawCoordinates.xScoreField, 240, buf, lstrlen(buf));

	wsprintf(buf, L"Rotate counter clockwise: push space");
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(128, 128, 128));
	TextOut(hdc, drawCoordinates.xScoreField, 280, buf, lstrlen(buf));

	wsprintf(buf, L"Exit game: push escape");
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(128, 128, 128));
	TextOut(hdc, drawCoordinates.xScoreField, 320, buf, lstrlen(buf));
}


void drawFieldBoundary(HDC hdc)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(255, 192, 203));
	MoveToEx(hdc, INDENT_IN_PIXELS, INDENT_IN_PIXELS, NULL);
	LineTo(hdc, INDENT_IN_PIXELS, drawCoordinates.yFieldBoundary);
	LineTo(hdc, drawCoordinates.xFieldBoundary, drawCoordinates.yFieldBoundary);
	LineTo(hdc, drawCoordinates.xFieldBoundary, INDENT_IN_PIXELS);
	LineTo(hdc, INDENT_IN_PIXELS, INDENT_IN_PIXELS);
}


void drawBlock(HDC hdc, int i, int j)
{
	int x1, x2, y1, y2;

	x1 = INDENT_IN_PIXELS + (i * BLOCK_SIZE_IN_PIXEL);
	x2 = x1 + BLOCK_SIZE_IN_PIXEL;
	y1 = INDENT_IN_PIXELS + (j * BLOCK_SIZE_IN_PIXEL);
	y2 = y1 + BLOCK_SIZE_IN_PIXEL;

	Rectangle(hdc, x1, y1, x2, y2);
}


void drawField(HDC hdc)
{
	int x, y;
	HBRUSH hBrushMoving = CreateSolidBrush(RGB(255, 105, 180));
	HBRUSH hBrushStaying = CreateSolidBrush(RGB(135, 206, 250));
	HBRUSH hBrushBoundary = CreateSolidBrush(RGB(255, 192, 203));
	HBRUSH holdBrush = SelectObject(hdc, hBrushBoundary);

	for (y = 0; y < PLAY_FIELD_HEIGHT_IN_BLOCKS; y++)
	{
		for (x = 0; x < PLAY_FIELD_WIDTH_IN_BLOCKS; x++)
		{
			if (playField[y][x] == MOVING_BLOCK) {
				SelectObject(hdc, hBrushMoving);
				drawBlock(hdc, x, y);
			}
			else if (playField[y][x] == STAYING_BLOCK) {
				SelectObject(hdc, hBrushStaying);
				drawBlock(hdc, x, y);
			}
			else if (playField[y][x] == BOUNDARY_BLOCK) {
				SelectObject(hdc, hBrushBoundary);
				drawBlock(hdc, x, y);
			}
		}
	}

	SelectObject(hdc, holdBrush);
	DeleteObject(hBrushMoving);
	DeleteObject(hBrushStaying);
	DeleteObject(hBrushBoundary);
}


void drawWithBuffer(HDC hdc)
{
	HDC bdc = CreateCompatibleDC(hdc);
	HBITMAP bufferBitmap = CreateCompatibleBitmap(hdc, drawCoordinates.xBuffer, drawCoordinates.yBuffer);
	SelectObject(bdc, bufferBitmap);

	drawBackground(bdc);
	drawFieldBoundary(bdc);
	drawField(bdc);
	drawScoreField(bdc);
	BitBlt(hdc, 0, 0, drawCoordinates.xBuffer, drawCoordinates.yBuffer, bdc, 0, 0, SRCCOPY);

	DeleteDC(bdc);
	DeleteObject(bufferBitmap);
}