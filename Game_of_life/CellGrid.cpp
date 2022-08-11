#include "CellGrid.h"

#define h sf::VideoMode::getDesktopMode().height
#define w sf::VideoMode::getDesktopMode().width

CellGrid::CellGrid(int dim)
{
	m_lin = h / (dim+0.5);
	m_col = w / (dim+0.5);

	m_frame = new bool* [m_lin];
	for (int i = 0; i < m_lin; i++)
		m_frame[i] = new bool[m_col];


	for (int i = 0; i < m_lin; i++)
		for (int j = 0; j < m_col; j++)
			m_frame[i][j] = 0;

	m_next_frame = new bool* [m_lin];
	for (int i = 0; i < m_lin; i++)
		m_next_frame[i] = new bool[m_col];


}

CellGrid::~CellGrid()
{
	for (int i = 0; i < m_lin; i++)
	{
		delete[] m_frame[i];
		delete[] m_next_frame[i];
	}
	delete[]m_frame;
	delete[]m_next_frame;

}




void CellGrid::InitRand()
{
	srand(time(NULL));

	for (int i = 0; i < m_lin; i++)
		for (int j = 0; j < m_col; j++)
		{

			m_frame[i][j] = rand() % 2;


		}
	NextFrame();

}


void CellGrid::Init()
{


	for (int i = 0; i < m_lin; i++)
		for (int j = 0; j < m_col; j++)
		{

			m_frame[i][j] = 0;


		}
	NextFrame();

}


void CellGrid::AddCell(int l, int c)
{
	if ((l >= 0 && l < m_lin) && (c >= 0 && c < m_col))  // incadrare in fereastra
		m_frame[l][c] = true;

}

void CellGrid::DelCell(int l, int c)
{
	if ((l >= 0 && l < m_lin) && (c >= 0 && c < m_col))  // incadrare in fereastra
		m_frame[l][c] = false;

}


int CellGrid::CountNeighbours(int pos_x, int pos_y)
{
	int neighbours = 0;
	int y = 0, x = 0;
	for (int i = pos_y - 1; i <= pos_y + 1; i++)
		for (int j = pos_x - 1; j <= pos_x + 1; j++)
		{   //  warp space  ( miscare pacman)
			if (!(i == pos_y && j == pos_x))
			{
				y = i; x = j;
				if (y < 0)   y += m_lin;  // sus => jos
				if (y >= m_lin) y = 0;    // jos => sus   
				if (x < 0)   x += m_col;  // stanga => dreapta
				if (x >= m_col) x = 0;    // dreapta => stanga

				neighbours += m_frame[y][x];
			}
		}
	return neighbours;


}


void CellGrid::NextFrame()
{
	//reguli  
	for (int i = 0; i < m_lin; i++)
		for (int j = 0; j < m_col; j++)
		{


			m_next_frame[i][j] = m_frame[i][j];

			if (CountNeighbours(j, i) == 3)  m_next_frame[i][j] = true;
			//Any dead cell with exm_framely three live neighbors becomes a live cell, as if by reproduction.


			if (CountNeighbours(j, i) > 3)   m_next_frame[i][j] = false;
			//Any live cell with fewer than two live neighbors dies, as if caused by under population.

			if (CountNeighbours(j, i) < 2)   m_next_frame[i][j] = false;
			//Any live cell with more than three live neighbors dies, as if by overpopulation.


		}
	std::swap(m_frame, m_next_frame);  // interschimbare intre matricea m_frameuala si matricea de la pasul m_next_frameator

}

void::CellGrid::Display(sf::RenderWindow& window, sf::RectangleShape& cell, int& dim)
{
	//display 


	for (int i = 0; i < m_lin; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			if (!m_frame[i][j]) continue;
			else {
				cell.setPosition(j * (dim + 0.5), i * (dim + 0.5));


				if (CountNeighbours(i, j) >= 2)
					cell.setFillColor(sf::Color::Cyan);
				else
					cell.setFillColor(sf::Color::White);

				window.draw(cell);
			}





		}
	}



	window.display();
	window.clear();
}



void CellGrid::Game(int dim)
{
	sf::Music menu_music;
	menu_music.openFromFile("../extras/menu_music.wav");
	menu_music.setLoop(true);
	menu_music.setVolume(20);
	menu_music.play();


	sf::SoundBuffer sb;
	sb.loadFromFile("../extras/menu_select.wav");
	sf::Sound select_sound;
	select_sound.setBuffer(sb);
	select_sound.setVolume(30);

	bool exit = 0;
	sf::RenderWindow window2(sf::VideoMode(w, h), "Game of life - Main Menu", sf::Style::Fullscreen);
	Menu menu(w, h);
	while (window2.isOpen())
	{
		sf::Event event;


		while (window2.pollEvent(event))
		{
			menu.draw(window2);
			window2.display();
			window2.clear();
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp(), select_sound.play();

					break;

				case sf::Keyboard::Down:
					menu.MoveDown(), select_sound.play();

					break;
				case sf::Keyboard::Escape:
					window2.close(), exit = 1; break;

				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:

						Init(), window2.close();
						break;
					case 1:
						InitRand(), window2.close();
						break;
					case 2:
						window2.close(), exit = 1;
						break;

					}

					break;
				}
				menu.draw(window2);
				window2.display();
				window2.clear();



			}

		}




		if (!window2.isOpen() && exit != 1)
		{
			sf::Music music;
			music.openFromFile("../extras/game_music.wav");
			music.setLoop(true);
			music.setVolume(20);
			menu_music.stop();
			music.play();

			sf::RenderWindow window(sf::VideoMode(w, h), "Game of life", sf::Style::Fullscreen);
			sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(w, h));
			view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);   // camera
			sf::View view_init = view;  // original de referinta 
			sf::View view_check = view; // auxiliar
			window.setView(view);
			sf::RectangleShape cell(sf::Vector2f((float)dim, (float)dim));           //celula
			sf::Sprite fundal;
			sf::Texture texture;
			texture.loadFromFile("../extras/bg.jpg");   //fundal
			fundal.setTexture(texture);
			fundal.setOrigin(0, 0);


			bool run = true;
			while (window.isOpen())
			{

				sf::Event event;


				while (window.pollEvent(event))
				{

					if (event.type == sf::Event::Closed)
					{

						window.close();

					}
				}





				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{ // adaugare manuala de celule  
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
					sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
					int px = worldPos.x / (dim + 0.5);
					int py = worldPos.y / (dim + 0.5);
					AddCell(py, px);


				}

				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{ // stergere manuala de celule  

					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
					sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
					int px = worldPos.x / (dim + 0.5);
					int py = worldPos.y / (dim + 0.5);
					DelCell(py, px);

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				{
					view.zoom(0.99f);
					window.setView(view);
					//zoom in
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				{
					sf::View view_check = view;

					view_check.zoom(1.01f);
					if (view_check.getCenter().y + view_check.getSize().y / 2.0 <= view_init.getCenter().y + view_init.getSize().y / 2.0
						&& view_check.getCenter().y - view_check.getSize().y / 2.0 >= view_init.getCenter().y - view_init.getSize().y / 2.0
						&& view_check.getCenter().x + view_check.getSize().x / 2.0 <= view_init.getCenter().x + view_init.getSize().x / 2.0
						&& view_check.getCenter().x - view_check.getSize().x / 2.0 >= view_init.getCenter().x - view_init.getSize().x / 2.0
						)
						// zoom out
					{
						view = view_check;
						window.setView(view);
					}

				}


				switch (event.type)
				{
				case sf::Event::KeyReleased:
					switch (event.key.code)
					{
					case sf::Keyboard::G:
						run = true;  //start
						break;

					case sf::Keyboard::Space:   //pauza
						run = false;
						break;
					case sf::Keyboard::Escape:
						window.close();             //exit
						break;
					case sf::Keyboard::R:
						view = view_init, window.setView(view);   // reset camera
						break;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				{

					view_check = view;
					view_check.move(0.f, -4.f);
					if (view_check.getCenter().y - view_check.getSize().y / 2.0 >= view_init.getCenter().y - view_init.getSize().y / 2.0)
					{
						view = view_check;
						window.setView(view);
					}

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				{
					view_check = view;
					view_check.move(-4.f, 0.f);
					if (view_check.getCenter().x - view_check.getSize().x / 2.0 >= view_init.getCenter().x - view_init.getSize().x / 2.0)
					{
						view = view_check;
						window.setView(view);
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					view_check = view;
					view_check.move(0.f, 4.f);
					if (view_check.getCenter().y + view_check.getSize().y / 2.0 <= view_init.getCenter().y + view_init.getSize().y / 2.0)
					{
						view = view_check;
						window.setView(view);
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					view_check = view;
					view_check.move(4.f, 0.f);
					if (view_check.getCenter().x + view_check.getSize().x / 2.0 <= view_init.getCenter().x + view_init.getSize().x / 2.0)
					{
						view = view_check;
						window.setView(view);
					}
				}


				if (run)
					NextFrame();


				window.draw(fundal);
				Display(window, cell, dim);



			}

		}
	}
}