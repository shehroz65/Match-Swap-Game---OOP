#include <windows.h>
#include "util.h"
#include <iostream>
#include <string>
#include <cmath> // for basic math functions such as cos, sin, sqrt
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;



class Menu
{
private:
public:
	stringstream NAME;

	void PauseMenu()
	{
		DrawString(280, 170, "Resume Game", colors[GOLDEN_ROD]);
		DrawString(280, 210, "Save File", colors[GOLDEN_ROD]);
		DrawString(280, 250, "Load File", colors[GOLDEN_ROD]);
		DrawString(280, 290, "How to Play!", colors[GOLDEN_ROD]);
		DrawString(280, 330, "Quit", colors[GOLDEN_ROD]);


	}
	void InputNameScreen()
	{
		DrawString(200, 250, "Please enter name (press Enter once done)", colors[GOLDEN_ROD]);
		DrawRectangle(200, 300, 500, 30, colors[WHITE]);
		DrawString(400, 370, "Done!", colors[GREEN]);

	}
};

class Player
{
private:
	string name;
	int highscore;
public:

	Player()
	{
		highscore = 0;
	}

	int NameXCoordinate = 205;
	int NameYCoordinate = 325;
	string name2;

	void setname(string n)
	{
		name = n;
	}
	string getname()
	{
		return name;
	}
	void sethighscore(int hs)
	{
		highscore = hs;
	}
	int gethighscore()
	{
		return highscore;
	}

};


void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 0.0);

	glVertex3f(0.1, 0.1, 0.0);
	glVertex3f(0.9, 0.1, 0.0);
	glVertex3f(0.9, 0.9, 0.0);
	glVertex3f(0.1, 0.9, 0.0);

	glEnd();

	glFlush();
}
void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	// set the screen size to given width and height.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DrawCircle(float cx, float cy, float r, int num_segments) {
	float theta = 2 * 3.1415926 / float(num_segments);
	float tangetial_factor = tanf(theta); // calculate the tangential factor

	float radial_factor = cosf(theta); // calculate the radial factor

	float x = r; // we start at angle = 0

	float y = 0;

	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++) {
		glVertex2f(x + cx, y + cy); // output vertex

		// calculate the tangential vector
		// remember, the radial vector is (x, y)
		// to get the tangential vector we flip those coordinates and negate one of them

		float tx = -y;
		float ty = x;

		// add the tangential vector

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		// correct using the radial factor

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}




void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT
		/* GLUT_KEY_LEFT is constant and contains ASCII for left arrow key */) {
		// what to do when left key is pressed...
	}
	else if (key ==
		GLUT_KEY_RIGHT /* GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key */
		) {

	}
	else if (key ==
		GLUT_KEY_UP /* GLUT_KEY_UP is constant and contains ASCII for up arrow key */
		) {

	}
	else if (key ==
		GLUT_KEY_DOWN /* GLUT_KEY_DOWN is constant and contains ASCII for down arrow key */
		) {

	}
	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call this function */
	glutPostRedisplay();
}

class Score
{
private:
	int obtainedscore;
	int totalscore;

public:
	stringstream ss;
	Score()
	{
		obtainedscore = 0;
	}
	void settotalscore(int ss)
	{
		totalscore = ss;
	}
	int gettotalscore()
	{
		return totalscore;
	}
	void setobtainedscore(int ss)
	{
		obtainedscore = ss;

	}
	int getobtainedscore()
	{
		return obtainedscore;
	}
};

class Board {
private:
	int mXin, mYin, mXout, mYout;
	int boardStartX = 160, boardStartY = 120;

public:
	int randC;
	int hint = 5;
	Menu m;
	int gamestate = 0;
	const int size = 8;
	bool mainmode = false;
	int timer = 30;
	fstream f;
	Score s;
	Player p;
	int seconds = 0;

	void setmXin(int x)
	{
		mXin = x;
	}
	int getmXin()
	{
		return mXin;
	}
	void setmYin(int x)
	{
		mYin = x;
	}
	int getmYin()
	{
		return mYin;
	}
	void setmXout(int x)
	{
		mXout = x;
	}
	int getmXout()
	{
		return mXout;
	}
	void setmYout(int x)
	{
		mYout = x;
	}
	int getmYout()
	{
		return mYout;
	}



	int gems[8][8];


	void checkandRemovePatterns(int i, int j)
	{
		if (gems[i][j] == 1 && (i + 1) < size && gems[i + 1][j] == 1 && (i + 2) < size && gems[i + 2][j] == 1)
		{
			while (gems[i][j] == 1)
			{
				if ((i > size - 1) || (j > size - 1))
					gems[i][j] = 0;

				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 2 && (i + 1) < size && gems[i + 1][j] == 2 && (i + 2) < size && gems[i + 2][j] == 2)
		{
			while (gems[i][j] == 2)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 3 && (i + 1) < size && gems[i + 1][j] == 3 && (i + 2) < size && gems[i + 2][j] == 3)
		{
			while (gems[i][j] == 3)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 4 && (i + 1) < size && gems[i + 1][j] == 4 && (i + 2) < size && gems[i + 2][j] == 4)
		{
			while (gems[i][j] == 4)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 5 && (i + 1) < size && gems[i + 1][j] == 5 && (i + 2) < size && gems[i + 2][j] == 5)
		{
			while (gems[i][j] == 5)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 6 && (i + 1) < size && gems[i + 1][j] == 6 && (i + 2) < size && gems[i + 2][j] == 6)
		{
			while (gems[i][j] == 6)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 7 && (i + 1) < size && gems[i + 1][j] == 7 && (i + 2) < size && gems[i + 2][j] == 7)
		{
			while (gems[i][j] == 7)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 1 && (j + 1) < size && gems[i][j + 1] == 0 && (j + 2) < size && gems[i][j + 2] == 1)
		{
			while (gems[i][j] == 7)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 2 && (j + 1) < size && gems[i][j + 1] == 2 && (j + 2) < size && gems[i][j + 2] == 2)
		{
			while (gems[i][j] == 2)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 3 && (j + 1) < size && gems[i][j + 1] == 3 && (j + 2) < size && gems[i][j + 2] == 3)
		{
			while (gems[i][j] == 3)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 4 && (j + 1) < size && gems[i][j + 1] == 4 && (j + 2) < size && gems[i][j + 2] == 4)
		{
			while (gems[i][j] == 4)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 5 && (j + 1) < size && gems[i][j + 1] == 5 && (j + 2) < size && gems[i][j + 2] == 5)
		{
			while (gems[i][j] == 5)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 6 && (j + 1) < size && gems[i][j + 1] == 6 && (j + 2) < size && gems[i][j + 2] == 6)
		{
			while (gems[i][j] == 6)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
		if (gems[i][j] == 7 && (j + 1) < size && gems[i][j + 1] == 7 && (j + 2) < size && gems[i][j + 2] == 7)
		{
			while (gems[i][j] == 7)
			{
				gems[i][j] = (rand() % 7) + 1;
			}
		}
	}

	void DisplayMainMenu()
	{
		DrawString(50, 80, "Normal Mode", colors[GOLDEN_ROD]);
		DrawString(50, 130, "Time Trial Mode", colors[GOLDEN_ROD]);
		DrawString(50, 180, "Load Game", colors[GOLDEN_ROD]);
		DrawString(50, 230, "Music On/Off", colors[GOLDEN_ROD]);
		DrawString(50, 280, "Options", colors[GOLDEN_ROD]);
		DrawString(50, 330, "Quit", colors[GOLDEN_ROD]);



	}
	bool checkWithinRange(int i, int j)
	{
		if (i >= 0 && i < size && j >= 0 && j < size)
			return true;
		else
			return false;
	}

	bool HintSystem(int& x, int& y)
	{
		cout << "I am in hint " << endl;
		x = -1; y = -1;
		bool cond = false;
		for (int i = 0; i < size; i++)
		{
			if (cond == true)
				break;

			for (int j = 0; j < size; j++)
			{
				if (cond == true)
					break;
				if (gems[i][j] == gems[i + 1][j] && checkWithinRange(i + 1, j) == true)
				{
					if (gems[i - 2][j] == gems[i][j] && checkWithinRange(i - 2, j) == true)
					{
						x = i - 2; y = j;  cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i - 1][j - 1] == gems[i][j] && checkWithinRange(i - 1, j - 1) == true)
					{
						x = i - 1; y = j - 1; cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i - 1][j + 1] == gems[i][j] && checkWithinRange(i - 1, j - 1) == true)
					{
						x = i - 1; y = j + 1;  cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i + 2][j - 1] == gems[i][j] && checkWithinRange(i + 2, j - 1) == true)
					{
						x = i + 2; y = j - 1; cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i + 2][j + 1] == gems[i][j] && checkWithinRange(i + 2, j + 1) == true)
					{
						x = i + 2; y = j + 1;  cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i + 3][j] == gems[i][j] && checkWithinRange(i + 3, j) == true)
					{
						x = i + 3; y = j; cond = true;
						gems[x][y] += 7;
						break;
					}


				}

				else if (gems[i][j + 1] == gems[i][j] && checkWithinRange(i, j + 1) == true)
				{
					if (gems[i][j - 2] == gems[i][j] && checkWithinRange(i, j - 2) == true)
					{
						x = i; y = j - 2; cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i - 1][j - 1] == gems[i][j] && checkWithinRange(i - 1, j - 1) == true)
					{
						x = i - 1; y = j - 1; cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i + 1][j - 1] == gems[i][j] && checkWithinRange(i + 1, j - 1) == true)
					{
						x = i + 1; y = j - 1; cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i][j + 3] == gems[i][j] && checkWithinRange(i, j + 3) == true)
					{
						x = i; y = j + 3; cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i - 1][j + 2] == gems[i][j] && checkWithinRange(i - 1, j + 2) == true)
					{
						x = i - 1; y = j + 2; cond = true;
						gems[x][y] += 7;
						break;
					}
					else if (gems[i + 1][j + 2] == gems[i][j] && checkWithinRange(i + 1, j + 2) == true)
					{
						x = i + 1; y = j + 2; cond = true;
						gems[x][y] += 7;
						break;
					}
				}
				//else if (gems[i][j] == gems[])
			}
		}
		return cond;
	}

	bool CheckForValidSwap(int i, int j)
	{
		bool cond = false;
		int sumleft = 0, sumright = 0, sumup = 0, sumdown = 0;
		int	k = i - 1;

		//checks sum on left

		while (gems[k][j] == gems[i][j] && k >= 0)
		{
			sumleft++;
			k--;
		}

		k = i + 1;

		//checks sum on right
		while (gems[k][j] == gems[i][j] && k < size)
		{
			sumright++;
			k++;
		}

		k = j - 1;

		//checks sum on top
		while (gems[i][k] == gems[i][j] && k >= 0)
		{
			sumup++;
			k--;
		}

		k = j + 1;

		//checks sum on bottom
		while (gems[i][k] == gems[i][j] && k < size)
		{
			sumdown++;
			k++;
		}

		if (sumleft + sumright + 1 >= 3)
		{
			if (sumleft + sumright + 1 == 3 && gems[i][j] != 0)
			{
				s.setobtainedscore(s.getobtainedscore() + 10);
				if (s.getobtainedscore() > p.gethighscore())
				{
					p.sethighscore(s.getobtainedscore());
					if (s.getobtainedscore() >= 600)
						gamestate = 12;
				}
				if (gamestate == 2)
				{
					timer += 3;
				}
			}
			else if (sumleft + sumright + 1 == 4 && gems[i][j] != 0)
			{
				s.setobtainedscore(s.getobtainedscore() + 15);
				if (s.getobtainedscore() > p.gethighscore())
				{
					p.sethighscore(s.getobtainedscore());
					if (s.getobtainedscore() >= 600)
						gamestate = 12;
				}
				if (gamestate == 2)
				{
					timer += 5;
				}
			}
			else if (sumleft + sumright + 1 == 5 && gems[i][j] != 0)
			{
				s.setobtainedscore(s.getobtainedscore() + 20);
				if (s.getobtainedscore() > p.gethighscore())
				{
					p.sethighscore(s.getobtainedscore());
					if (s.getobtainedscore() >= 600)
						gamestate = 12;
				}
				if (gamestate == 2)
				{
					timer += 7;
				}
			}
			for (k = i - sumleft; k <= i + sumright; k++)
			{
				gems[k][j] = 0;
			}
			cond = true;
		}
		if (sumup + sumdown + 1 >= 3)
		{
			if (sumup + sumdown + 1 == 3 && gems[i][j] != 0)
			{
				s.setobtainedscore(s.getobtainedscore() + 10);
				if (s.getobtainedscore() > p.gethighscore())
				{
					p.sethighscore(s.getobtainedscore());
					if (s.getobtainedscore() >= 600)
						gamestate = 12;
				}
				if (gamestate == 2)
				{
					timer += 3;
				}
			}
			else if (sumup + sumdown + 1 == 4 && gems[i][j] != 0)
			{
				s.setobtainedscore(s.getobtainedscore() + 15);
				if (s.getobtainedscore() > p.gethighscore())
				{
					p.sethighscore(s.getobtainedscore());
					if (s.getobtainedscore() >= 600)
						gamestate = 12;
				}
				if (gamestate == 2)
				{
					timer += 5;
				}
			}
			else if (sumup + sumdown + 1 == 5 && gems[i][j] != 0)
			{
				s.setobtainedscore(s.getobtainedscore() + 20);
				if (s.getobtainedscore() > p.gethighscore())
				{
					p.sethighscore(s.getobtainedscore());
					if (s.getobtainedscore() >= 600)
						gamestate = 12;
				}
				if (gamestate == 2)
				{
					timer += 7;
				}
			}
			for (k = j - sumup; k <= j + sumdown; k++)
			{
				gems[i][k] = 0;
			}
			cond = true;

		}
		return cond;
	}

	bool CheckGridForPatternInternal()
	{
		bool cond = false;
		int sumleft = 0, sumright = 0, sumup = 0, sumdown = 0;
		int k;

		for (int i = 0; i < size; i++) //row loop
		{
			sumleft = 0, sumright = 0, sumup = 0, sumdown = 0;
			for (int j = 0; j < size; j++) //column loop
			{
				if (CheckForValidSwap(i, j) == true)
					cond = true;
			}
		}

		return cond;
	}
	void CheckGridForPattern()
	{
		bool cond2 = true;
		while (cond2 == true)
		{
			cond2 = CheckGridForPatternInternal();
			checkEntireGrid();
		}
	}

	void dropdowncolumn(int x, int y)
	{
		int k = y - 1;
		while (gems[x][k] == 0 && k >= 0)
		{
			k--;
		}
		if (k == 0)
		{
			int l = y;
			while (l >= 0)
			{
				gems[x][l] = rand() % 7 + 1;
				//checkandRemovePatterns(x, l);
				l--;
			}

		}
		else
		{
			int l = y;
			while (k >= 0)
			{
				gems[x][l] = gems[x][k];
				gems[x][k] = 0;
				l--; k--;
			}
			l = 0;
			while (gems[x][l] == 0)
			{
				gems[x][l] = rand() % 7 + 1;
				checkandRemovePatterns(x, l);
				l++;
			}
		}

	}
	bool checkEntireGridInternal()
	{
		bool cond = false;
		for (int i = 0; i < size; i++)
		{
			for (int j = size - 1; j >= 0; j--)
			{
				if (gems[i][j] == 0)
				{
					cond = true;
					dropdowncolumn(i, j);
				}
			}
		}
		return cond;
	}
	void checkEntireGrid()
	{
		bool cond = true;
		//while (cond == true)
		{
			cond = checkEntireGridInternal();
		}

	}

	Board() {
		gamestate = 0;
		srand(time(NULL));

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {

				randC = (rand() % 7) + 1;

				gems[i][j] = randC;
			}
		}
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				checkandRemovePatterns(i, j);
			}
		}
	}

	void DisplayBoard() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				int gem = gems[i][j];
				if (gem > 7)
				{
					DrawLine(boardStartX + i * 50, (j * 50 + boardStartY) + 42, (boardStartX + i * 50) + 40, (j * 50 + boardStartY) + 42, 2, colors[WHITE]);
					gem = gem - 7;
				}
				if (gem == 0)
					DrawSquare(boardStartX + i * 50, ((j * 50 + boardStartY)), 40,
						colors[BLACK]);
				if (gem == 1)
					DrawSquare(boardStartX + i * 50, ((j * 50 + boardStartY)), 40,
						colors[RED]);
				if (gem == 2)
					DrawSquare(boardStartX + i * 50, ((j * 50 + boardStartY)), 40,
						colors[BLUE]);
				if (gem == 3)
					DrawCircle(boardStartX + 20 + i * 50, (j * 50 + boardStartY) + 20, 20, colors[BLUE]);
			
				if (gem == 4)
					DrawCircle(boardStartX + 20 + i * 50, (j * 50 + boardStartY) + 20, 20, colors[GREEN]);

				if (gem == 5)
					DrawTriangle(boardStartX + i * 50, ((j * 50 + boardStartY)), boardStartX + i * 50 + 35, ((j * 50 + boardStartY)), boardStartX + i * 50 + 17, ((j * 50 + boardStartY)) + 35, colors[FIREBRICK]);

				if (gem == 6)
					DrawTriangle(boardStartX + i * 50, ((j * 50 + boardStartY)), boardStartX + i * 50 + 35, ((j * 50 + boardStartY)), boardStartX + i * 50 + 17, ((j * 50 + boardStartY)) + 35, colors[MEDIUM_AQUA_MARINE]);

				if (gem == 7)
					DrawTriangle(boardStartX + i * 50, ((j * 50 + boardStartY)), boardStartX + i * 50 + 35, ((j * 50 + boardStartY)), boardStartX + i * 50 + 17, ((j * 50 + boardStartY)) + 35, colors[MEDIUM_VIOLET_RED]);

				
			}
		}
	}

	void returngem(int mX, int mY, int& iX, int& iY) {

		iX = iY = -1;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				/* if (XX >= gems[i][j].minX && XX <= gems[i][j].maxX)
				 {
				 if (YY >= gems[i][j].minY && YY <= gems[i][j].maxY)
				 {
				 cond = true;
				 return &gems[i][j];
				 }
				 }
				 */

				if ((mX >= boardStartX + (i * 50)) && (mX <= i * 50 + 40 + boardStartX))
					if ((mY >= boardStartY + (j * 50)) && (mY <= j * 50 + 40 + boardStartY)) {
						iY = j;
						iX = i;
						cout << "In element " << iX << " " << iY << endl;
					}
			}
		}
	}

	void swap(int x1, int y1, int x2, int y2) {
		int temp;
		temp = gems[x1][y1];
		gems[x1][y1] = gems[x2][y2];
		gems[x2][y2] = temp;

	}

	int GetDragDirection(int mXin, int mYin, int mXout, int mYout)
	{
		int x, y;
		int r = 0;
		x = mXin - mXout;
		y = mYin - mYout;
		cout << "abs" << abs(mXin - mXout) << endl;
		if ((abs(mXin - mXout)) >= abs(mYin - mYout)) {
			if (abs(mXin - mXout) > 50 / 2) {
				cout << "Horizontal-";
				if (x > 0) {
					cout << "Left?";
					return 1;
				}
				if (x < 0) {
					cout << "Right?";
					return 2;
				}
			}

		}
		else {
			if (abs(mYin - mYout) > 50 / 2) {
				cout << "Vertical-";
				if (y > 0) {
					cout << "Top?";
					return 3;
				}
				if (y < 0) {
					cout << "Bottom?";
					return 4;
				}
			}
		}

	}

	void SaveGame()
	{
		if (gamestate == 1)
		{
			string name;
			int hs;
			f.open("SaveFile.txt", ios::in);
			f >> name;
			f >> hs;
			f.close();

			if (s.getobtainedscore() > hs)
			{
				f.open("SaveFile.txt", ios::out);
				f << name << endl << s.getobtainedscore();
				f.close();

			}
		}
		if (gamestate == 2)
		{
			string name;
			int hs;
			f.open("SaveFileTimeTrial.txt", ios::in);
			f >> name;
			f >> hs;
			f.close();

			if (s.getobtainedscore() > hs)
			{
				f.open("SaveFile.txt", ios::out);
				f << name << endl << s.getobtainedscore();
				f.close();

			}
		}

	}

};

Board b1;


void GameDisplay() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0 /* Red Component */, 0, // 148.0/255/*Green Component*/,
		0.0 /* Blue Component */, 0 /* Alpha component */);
	// Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); // Update the colors

	if (b1.gamestate == 0)
	{
		b1.DisplayMainMenu();
		DrawString(350, 60, "Candy Crusher by Shehroz Khan - 19i 1751", colors[RED]);
		b1.m.NAME.str("");
	}
	if (b1.gamestate == 1 || b1.gamestate == 6 || b1.gamestate == 2)
	{

		if (b1.gamestate != 6)
		{
			DrawLine(125, 600, 725, 600, 50, colors[YELLOW_GREEN]);
			if (b1.gamestate == 1)
				DrawLine(125, 600, 125 + b1.s.getobtainedscore(), 600, 50, colors[DARK_GREEN]);

			DrawString(620, 300, "Hint: ", colors[DARK_GREEN]); //for Hint Text
		}
		b1.s.ss << b1.hint;


		DrawString(680, 300, b1.s.ss.str(), colors[DARK_OLIVE_GREEN]); //for number of Hints
		b1.s.ss.str("");
		b1.s.ss << b1.s.getobtainedscore();

		DrawString(430, 30, "Obtained Score:", colors[GOLDEN_ROD]);
		DrawString(430, 60, "Required Score: 600", colors[GOLDEN_ROD]);

		DrawString(609, 30, b1.s.ss.str(), colors[GOLDEN_ROD]); //for getting obtained score.

		b1.s.ss.str("");


		b1.DisplayBoard();
		//}
		if (b1.gamestate == 2)
		{
			if (b1.timer <= 0)
			{
				b1.gamestate = 10;
			}
		}

	}
	if (b1.gamestate == 5)
	{
		b1.m.PauseMenu();
	}
	if ((b1.gamestate == 6))
	{
		static int iii, jjj;
		if (b1.seconds == 0)
		{
			iii = jjj = 0;
			b1.HintSystem(iii, jjj);
			b1.seconds = 1;
		}
		if ((b1.seconds > 3))
		{
			cout << "State Changed" << endl;
			{
				if (b1.mainmode == true)
					b1.gamestate = 1;
				else
					b1.gamestate = 2;
			}

			b1.gems[iii][jjj] -= 7;
			if (b1.gamestate == 2)
				b1.timer += 3;
		}


		//b1.DisplayBoard();


	}


	if (b1.gamestate == 7)
	{
		b1.m.InputNameScreen();
		DrawString(b1.p.NameXCoordinate, b1.p.NameYCoordinate, b1.p.name2, colors[BLACK]);
	}
	if (b1.gamestate == 2)
	{
		stringstream time;
		time << b1.timer;
		DrawString(155, 30, "Time Left: " + time.str(), colors[GOLDEN_ROD]);

	}
	if (b1.gamestate == 10)
	{
		DrawString(150, 150, "GAME FINISHED!", colors[WHITE]);
	}
	if (b1.gamestate == 12)
	{
		DrawString(150, 150, "GAME FINISHED!", colors[WHITE]);
	}

	glutSwapBuffers(); // do not modify this line..

}

/* This function is called (automatically) whenever your mouse moves witin inside the game window
 * You will have to add the necessary code here for finding the direction of shooting
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 */

void MousePressedAndMoved(int x, int y) {
	//if (x ==)
	glutPostRedisplay();
}

void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}


void Reshape(int w, int h) {
	// adjusts the pixel rectangle for drawing to be the entire new window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	// matrix specifies the projection transformation
	glMatrixMode(GL_PROJECTION);
	// load the identity of matrix by clearing it.
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	// matrix specifies the modelview transformation
	glMatrixMode(GL_MODELVIEW);
	// again  load the identity of matrix
	glLoadIdentity();
	// gluLookAt() this function is used to specify the eye.
	// it is used to specify the coordinates to view objects from a specific position
	gluLookAt(-0.3, 0.5, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void MouseClicked(int button, int state, int x, int y) {
	cout << "in MouseClicked " << endl;
	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == 0) {
			b1.setmXin(x);
			b1.setmYin(y);
			cout << "I am in state 0 " << x << " " << y << endl;
		}
		if (state == 1) {

			if (b1.gamestate == 0)
			{
				if (x >= 50 && x <= 180 && y >= 55 && y <= 80)
				{
					b1.gamestate = 7;
					b1.mainmode = true;
				}
				else if (x >= 50 && x <= 220 && y >= 105 && y <= 130)
				{
					b1.gamestate = 7;
				}
				else if (x >= 50 && x <= 180 && y >= 155 && y <= 230)
				{
					b1.gamestate = 3;
				}
				else if (x >= 50 && x <= 180 && y >= 205 && y <= 280)
				{
					b1.gamestate = 4;
				}
				else if (x >= 50 && x <= 92 && y >= 314 && y <= 330)
				{
					exit(1);
				}

			}

			if (b1.gamestate == 1 || b1.gamestate == 2)
			{
				cout << "un cliked" << endl;
				if (x >= 620 && x <= 670 && y >= 280 && y <= 300)
				{
					if (b1.hint == 0)
					{
						cout << "No more hints available " << endl;
					}
					else
					{
						b1.hint--;
						b1.gamestate = 6;
						b1.seconds = 0;
						// for milliseconds, use using ms = std::chrono::duration<double, std::milli


						//std::cout << "It took " << duration.count() << "s" << std::endl;
					}
				}

			}
			if (b1.gamestate == 1 || b1.gamestate == 2)
			{

				cout << "I am in state 1 " << x << " " << y << endl;
				b1.setmXout(x);
				b1.setmYout( y);
				//cout << "From " << mXin << " " << mYin << "Mouse UP " << x << " " << y << endl;
				int dir = b1.GetDragDirection(b1.getmXin(), b1.getmYin(), b1.getmXout(), b1.getmYout());
				int i, j;
				b1.returngem(b1.getmXin(), b1.getmYin(), i, j);
				if (dir == 1)
				{ //left
					b1.swap(i, j, i - 1, j);
					//b1.DisplayBoard();
					if (b1.CheckForValidSwap(i - 1, j) == false)
					{
						b1.swap(i, j, i - 1, j);

					}
					else
					{
						b1.CheckGridForPattern();
					}

					b1.DisplayBoard();
				}
				else if (dir == 2)
				{ //right
					b1.swap(i, j, i + 1, j);
					//b1.DisplayBoard();
					if (b1.CheckForValidSwap(i + 1, j) == false)
					{
						b1.swap(i, j, i + 1, j);

					}
					else
					{
						b1.CheckGridForPattern();
					}
					b1.DisplayBoard();

				}
				else if (dir == 3)
				{ //top
					b1.swap(i, j, i, j - 1);
					b1.DisplayBoard();
					if (b1.CheckForValidSwap(i, j - 1) == false)
					{
						b1.swap(i, j, i, j - 1);
					}
					else
					{
						b1.CheckGridForPattern();
					}
					b1.DisplayBoard();

				}
				else if (dir == 4)
				{ //bottom
					b1.swap(i, j, i, j + 1);
					b1.DisplayBoard();
					if (b1.CheckForValidSwap(i, j + 1) == false)
					{
						b1.swap(i, j, i, j + 1);

					}
					else
					{
						b1.CheckGridForPattern();
					}
					b1.DisplayBoard();
				}
			}


			if (b1.gamestate == 5) //for checking Pause Menu (NOT MAIN MENU)
			{
				if (x >= 280 && x <= 424 && y >= 150 && y <= 170)
				{
					if (b1.mainmode == true)
						b1.gamestate = 1;
					else
						b1.gamestate = 2;
				}
				//b1.gamestate = 1;
				if (x >= 280 && x <= 370 && y >= 190 && y <= 210)
				{
					b1.SaveGame();
					if (b1.mainmode == true)
					{
						b1.gamestate = 1;
					}
					else
						b1.gamestate = 2;
				}

			}
			if (b1.gamestate == 7)
			{
				if (x >= 400 && x <= 460 && y >= 350 && y <= 375)
				{
					if (b1.mainmode == true)
						b1.gamestate = 1;
					else
					{
						b1.gamestate = 2;
						b1.timer = 15;
					}

				}


			}

		}
		b1.DisplayBoard();
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
		cout << "Right Button Pressed" << endl;
	}
	glutPostRedisplay();
	cout << "out MouseClicked " << endl;
}

void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27 /* Escape key ASCII */) {
		exit(1); // exit the program when escape key is pressed.
	}
	if (key == 'p' || key == 'P') // Key for Pausing the menu
	{
		if (b1.gamestate == 1 || b1.gamestate == 2)
		{
			b1.gamestate = 5;
		}
		/*
		if (b1.gamestate == 5)
		{
			b1.gamestate = 1;
		}
		*/
	}
	if (b1.gamestate == 7)
	{
		string ss = b1.p.name2;
		if (key == 8)
		{
			ss = ss.substr(0, ss.length() - 1);
		}
		else if (key >= 32)
			ss += key;
		if (key == 13)
		{
			if (b1.mainmode == true)
				b1.gamestate = 1;
			else
				b1.gamestate = 2;
		}
		b1.p.name2 = ss;
		//b1.p.NameXCoordinate = 3;
		cout << b1.p.name2 << endl;
		b1.p.sethighscore(0);

	}
	glutPostRedisplay();
}
void Timer(int m) {
	// implement your functionality here
	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(1000, Timer, 0);
	b1.seconds += 1;
	cout << b1.seconds << endl;
	b1.timer--;
}

void Resize(int w, int h)
{
	glutReshapeWindow(800, 800);
}

// *****************************************************************
int main(int argc, char* argv[]) {
	int width = 800, height = 800; // i have set my window size to be 800 x 600
	// InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	// we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("OOP Project"); // set the title of our game window

	SetCanvasSize(width, height); // set the number of pixels...
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// set shade model to Flat
	glShadeModel(GL_FLAT);
	glutReshapeFunc(Resize);
	// Register your functions to the library, you are telling the library names of function to call for different tasks.
	glutDisplayFunc(GameDisplay);
	// tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys);
	// tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys);
	// tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000, Timer, 0);
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when it deems necessary...
	glutMainLoop();
	return 1;
}
