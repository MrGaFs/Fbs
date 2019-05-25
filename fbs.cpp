#include<bits/stdc++.h>
#include<Windows.h>
#include <chrono>
#define _CRT_SECURE_NO_WAENINGS
#define pb push_back
#define pf push_front
#define ff first
#define ss second
#define mp make_pair
#define dance ios_base::sync_with_stdio(false);cout.tie(NULL);cin.tie(NULL);
#define repi(n) for(ll i = 0;i<(n);++i)
#define repj(n) for(ll j = 0;j<(n);++j)
#define repk(n) for(ll k = 0;k<(n);++k)
#define repi1(n) for(ll i = 1;i<(n);++i)
#define repj1(n) for(ll j = 1;j<(n);++j)
#define all(v) ((v).begin()), ((v).end())
#define allr(v) ((v).rbegin()), ((v).rend())
#define endl "\n"
#define sz(v)	((int)((v).size()))
#define py printf("Yes\n")
#define pn printf("No\n")
using namespace std;

int ScreenWidth = 120;
int ScreenHeight = 40;
int MapWidth = 16;
int MapHeight = 16;

float PlayerX = 14.7f;
float PlayerY = 5.09f;
float PlayerA = 0.0f;
float FOV = 3.14159f / 4.0f;
float Depth = 16.0f;
float Speed = 5.0f;

int main()
{
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Create Map of world space # = wall block, . = space
	wstring map;
	map += L"################";
	map += L"#...............";
	map += L"#.......########";
	map += L"#..............#";
	map += L"#......##......#";
	map += L"#......##......#";
	map += L"#..............#";
	map += L"###............#";
	map += L"##.............#";
	map += L"#......####..###";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#..............#";
	map += L"#......#########";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (1)
	{

		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float ElapsedTime = elapsedTime.count();



		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			PlayerA -= (Speed * 0.75f) * ElapsedTime;


		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			PlayerA += (Speed * 0.75f) * ElapsedTime;


		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			PlayerX += sinf(PlayerA) * Speed * ElapsedTime;;
			PlayerY += cosf(PlayerA) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * MapWidth + (int)PlayerY] == '#')
			{
				PlayerX -= sinf(PlayerA) * Speed * ElapsedTime;;
				PlayerY -= cosf(PlayerA) * Speed * ElapsedTime;;
			}
		}



		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			PlayerX -= sinf(PlayerA) * Speed * ElapsedTime;;
			PlayerY -= cosf(PlayerA) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * MapWidth + (int)PlayerY] == '#')
			{
				PlayerX += sinf(PlayerA) * Speed * ElapsedTime;;
				PlayerY += cosf(PlayerA) * Speed * ElapsedTime;;
			}
		}

		for (int x = 0; x < ScreenWidth; x++)
		{


			float RayAngle = (PlayerA - FOV / 2.0f) + ((float)x / (float)ScreenWidth) * FOV;



			float StepSize = 0.1f;

			float DistanceToWall = 0.0f;


			bool HitWall = false;

			bool Boundary = false;


			float EyeX = sinf(RayAngle);

			float EyeY = cosf(RayAngle);

			while (!HitWall && DistanceToWall < Depth)
			{
				DistanceToWall += StepSize;
				int TestX = (int)(PlayerX + EyeX * DistanceToWall);
				int TestY = (int)(PlayerY + EyeY * DistanceToWall);



				if (TestX < 0 || TestX >= MapWidth || TestY < 0 || TestY >= MapHeight)
				{
					HitWall = true;

					DistanceToWall = Depth;
				}
				else
				{


					if (map.c_str()[TestX * MapWidth + TestY] == '#')
					{
						// Ray has hit wall
						HitWall = true;

						vector<pair<float, float>> p;



						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{


								float vy = (float)TestY + ty - PlayerY;
								float vx = (float)TestX + tx - PlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (EyeX * vx / d) + (EyeY * vy / d);
								p.pb(mp(d, dot));
							}



						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.ff < right.ff; });



						float Bound = 0.01;
						if (acos(p.at(0).ss) < Bound) Boundary = true;
						if (acos(p.at(1).ss) < Bound) Boundary = true;
						if (acos(p.at(2).ss) < Bound) Boundary = true;
					}
				}
			}



			int Ceiling = (float)(ScreenHeight / 2.0) - ScreenHeight / ((float)DistanceToWall);
			int Floor = ScreenHeight - Ceiling;



			short Shade = ' ';
			if (DistanceToWall <= Depth / 4.0f)			Shade = 0x2588;

			else if (DistanceToWall < Depth / 3.0f)		Shade = 0x2593;
			else if (DistanceToWall < Depth / 2.0f)		Shade = 0x2592;
			else if (DistanceToWall < Depth)				Shade = 0x2591;
			else											Shade = ' ';

			if (Boundary)		Shade = ' ';


			for (int y = 0; y < ScreenHeight; y++)
			{


				if (y <= Ceiling)
					screen[y * ScreenWidth + x] = ' ';
				else if (y > Ceiling && y <= Floor)
					screen[y * ScreenWidth + x] = Shade;
				else

				{


					float b = 1.0f - (((float)y - ScreenHeight / 2.0f) / ((float)ScreenHeight / 2.0f));
					if (b < 0.25)		Shade = '#';
					else if (b < 0.5)	Shade = 'x';
					else if (b < 0.75)	Shade = '.';
					else if (b < 0.9)	Shade = '-';
					else				Shade = ' ';
					screen[y * ScreenWidth + x] = Shade;
				}
			}
		}


		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", PlayerX, PlayerY, PlayerA, 1.0f / ElapsedTime);



		for (int nx = 0; nx < MapWidth; nx++)
			for (int ny = 0; ny < MapWidth; ny++)
			{
				screen[(ny + 1) * ScreenWidth + nx] = map[ny * MapWidth + nx];
			}
		screen[((int)PlayerX + 1) * ScreenWidth + (int)PlayerY] = 'P';



		screen[ScreenWidth * ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}
