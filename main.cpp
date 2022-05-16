#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <math.h>

using namespace sf;
using namespace std;

int ScreenSize[2], alertlimitval = 200, alertshowlimit = alertlimitval, lineopt = 6;
const int qofbukashki = 2000, rassst = 100;
RenderWindow window; 
vector<RectangleShape> preps;

float dist(Vector2f a, Vector2f b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }

void DrawLine(Vector2f a, Vector2f b, Color da = Color(255, 255, 0, 128)) {
	VertexArray curve(LineStrip, 0);
	curve.append(Vertex(a, da));
	curve.append(Vertex(b, da));
	if (window.hasFocus()) window.draw(curve);
}

struct Bukashka {
	Vector2f pos;
	Vector2f puti;
	float angletomove;
	float randspeed;
	bool kudaidem;
	Bukashka() { }
	void Init() {
		bool norm;
		do
		{
			pos.x = rand() % ScreenSize[0];
			pos.y = rand() % ScreenSize[1];
			norm = false;
			for (int i = 0; i < preps.size(); i++)
				if (preps[i].getGlobalBounds().contains(pos)) norm = true;
		} while (norm);
		puti.x = 30000;
		puti.y = 30000;
		angletomove = rand() % 360;
		kudaidem = rand() % 2;
		randspeed = 0.6 + (rand() % 60) * 0.01;
	}
	void Update(float TimesGone) {
		if (pos.x <= 0 || pos.y <= 0 || pos.x >= ScreenSize[0] ||
			pos.y >= ScreenSize[1]) angletomove = int(angletomove + 180) % 360;
		for (int i = 0; i < preps.size(); i++)
			if (preps[i].getGlobalBounds().contains(pos))
			{
				if (preps[i].getGlobalBounds().contains(Vector2f(
					pos.x + cos(angletomove * 0.01745) * 150 * 0.08,
					pos.y + sin(angletomove * 0.01745) * 150 * 0.08)))
				{
					angletomove = int(angletomove + 180) % 360;
					while (preps[i].getGlobalBounds().contains(pos))
					{
						pos.x += cos(angletomove * 0.01745) * TimesGone * 0.08;
						pos.y += sin(angletomove * 0.01745) * TimesGone * 0.08;
					}
				}
			}
		angletomove += ((rand() % 5) - 2) * TimesGone * float(rand() % 5) * 0.001;
		pos.x += cos(angletomove * 0.01745) * TimesGone * 0.08 * randspeed;
		pos.y += sin(angletomove * 0.01745) * TimesGone * 0.08 * randspeed;
		puti.x += TimesGone * 0.15; puti.y += TimesGone * 0.15;

	}
};

Bukashka bukashki[qofbukashki];

void GetDesktopResolution()
{
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	ScreenSize[0] = GetSystemMetrics(SM_CXSCREEN);
	ScreenSize[1] = GetSystemMetrics(SM_CYSCREEN);
}

void alert(int whoisalert, float put, bool kuda)
{
	for (int i = 0; i < qofbukashki; i++)
		if (i != whoisalert
			&& dist(bukashki[whoisalert].pos, bukashki[i].pos) < rassst)
		{
			bool may = true;
			for (int g = 0; g < preps.size(); g++)
				for (int f = 0; f < 4; f++)
				if (preps[g].getGlobalBounds().contains(
					Vector2f(bukashki[whoisalert].pos.x + f * ((bukashki[i].pos.x - bukashki[whoisalert].pos.x) / 4),
						bukashki[whoisalert].pos.y + f * ((bukashki[i].pos.y - bukashki[whoisalert].pos.y) / 4)))) may = false;
			if (may)
			{
				bool lineee = false;
				if (kuda) if (bukashki[i].puti.x > put) { bukashki[i].puti.x = put; lineee = true; }
				if (!kuda) if (bukashki[i].puti.y > put) { bukashki[i].puti.y = put; lineee = true; }
				if (lineee && bukashki[i].kudaidem == kuda) {
					if (alertshowlimit < alertlimitval) if (rand() % lineopt == 0) DrawLine(bukashki[whoisalert].pos, bukashki[i].pos, Color(0, 255, 255, 196));
					bukashki[i].angletomove = 180 + atan2(bukashki[i].pos.y - bukashki[whoisalert].pos.y,
						bukashki[i].pos.x - bukashki[whoisalert].pos.x) * 57.2957;
				}
				else
					if (alertshowlimit < alertlimitval) if (rand() % lineopt == 0) DrawLine(bukashki[whoisalert].pos, bukashki[i].pos);
			}
		}
}

int main()
{
	srand(time(NULL)); GetDesktopResolution();
	window.create(VideoMode(ScreenSize[0], ScreenSize[1]), "Programm", Style::Fullscreen, ContextSettings(0, 0, 8));
	Vector2f firstbase(100, 100), 
		secondbase(ScreenSize[0] - 100, ScreenSize[1] - 100);
	Clock clk; float cddd = 0; bool startmod = false;
	while (window.isOpen())
	{
		float TimesGone = clk.getElapsedTime().asMicroseconds() * 0.001;
		if (TimesGone > 200) TimesGone = 200; clk.restart();
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (window.hasFocus())
			{
				if (event.type == sf::Event::MouseButtonPressed)
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						preps.push_back(RectangleShape());
						preps[preps.size() - 1].setPosition(window.mapPixelToCoords(Mouse::getPosition(window)));
						preps[preps.size() - 1].setFillColor(Color(128, 128, 128));
					}
			}
		}
		if (window.hasFocus())
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{ 
				startmod = !startmod; 
				if (startmod) for (int i = 0; i < qofbukashki; i++) bukashki[i].Init();
				Sleep(300); 
			}
			if (Keyboard::isKeyPressed(Keyboard::Delete) && preps.size() > 0)
			{
				preps.erase(preps.begin());
			}
			if (Keyboard::isKeyPressed(Keyboard::F) && TimesGone != 0) cout << 1000.0 / TimesGone << endl;
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				preps[preps.size() - 1].setSize(Vector2f(
					window.mapPixelToCoords(Mouse::getPosition(window)).x - preps[preps.size() - 1].getPosition().x,
					window.mapPixelToCoords(Mouse::getPosition(window)).y - preps[preps.size() - 1].getPosition().y));
			}
		}
		window.clear();
		for (int i = 0; i < preps.size(); i++) window.draw(preps[i]);
		if (!startmod) TimesGone = 0;
		if (cddd < 50) cddd += TimesGone;
		else
		{
			cddd -= 50; alertshowlimit = 0;
		}
		for (int i = 0; i < qofbukashki; i++)
		{
			CircleShape sha(2);
			sha.setOrigin(2, 2);
			sha.setPosition(bukashki[i].pos);
			sha.setFillColor(Color(i % 128 + 128, 0, i % 128 + 128));
			if (window.hasFocus()) window.draw(sha);
			bukashki[i].Update(TimesGone);
			if (dist(bukashki[i].pos, firstbase) <= 15) {
				if (bukashki[i].kudaidem)
				{
					bukashki[i].angletomove = int(bukashki[i].angletomove + 180) % 360;
					bukashki[i].kudaidem = !bukashki[i].kudaidem;
				}
				bukashki[i].puti.x = 0; 
			}
			if (dist(bukashki[i].pos, secondbase) <= 15) {
				if (!bukashki[i].kudaidem)
				{
					bukashki[i].angletomove = int(bukashki[i].angletomove + 180) % 360;
					bukashki[i].kudaidem = !bukashki[i].kudaidem;
				}
				bukashki[i].puti.y = 0; 
			}
			if (rand() % 16 == 0)
			{
				if (rand() % 2) alert(i, bukashki[i].puti.x + rassst, true);
				else alert(i, bukashki[i].puti.y + rassst, false);
				alertshowlimit++;
			}
		}
		CircleShape base(15);
		base.setOrigin(15, 15);
		base.setPosition(firstbase); window.draw(base);
		base.setPosition(secondbase); window.draw(base);
		window.display();
	}
	return 0;
}