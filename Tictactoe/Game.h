#pragma once
#include<graphics.h>
#include<TCHAR.H>
#include<time.h>
#include<easyx.h>
#include<iostream>

class Player;
class Piece_Board;

class Game
{
public:


	Player* A;
	Player* B;
	Piece_Board* m_Board;
	bool is_running;

};