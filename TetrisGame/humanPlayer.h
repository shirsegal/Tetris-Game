#pragma once
#include "player.h"
#include "shape.h"

class HumanPlayer: public Player
{
	bool moveShape(GameConfig::eKeys key) override
	{
		return shape.move(key, getIndex());
	}

	void getRandShape(bool color) override
	{
		shape.getRandShape(color);
	}

public:
	HumanPlayer(int i) : Player(i) { }

	 ~HumanPlayer() {}
};

