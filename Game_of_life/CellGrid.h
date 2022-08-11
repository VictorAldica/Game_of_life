#pragma once
#include <ctime>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <cmath>
#include "Menu.h"



class CellGrid
{
private:
    int m_lin, m_col;
    bool** m_frame, ** m_next_frame;
    
public:
    CellGrid(int);
    ~CellGrid();
    
    void Init();
    void InitRand();
    void AddCell(int,int);
    void DelCell(int, int);
    void NextFrame();
    int CountNeighbours(int, int);
    void Game(int);
    void Display(sf::RenderWindow& window, sf::RectangleShape& cell,int &);
   

};
