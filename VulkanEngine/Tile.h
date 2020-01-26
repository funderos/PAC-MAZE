#ifndef TILE_H
#define TILE_H

class Tile
{
private:
	virtual int getOpenWays();
public:
	bool left = false;
	bool up = false;
	bool down = false;
	bool right = false;
	int type = 0;

	virtual std::string getMeshName();
	virtual float getMeshRotation();

	///Constructor
	Tile(bool left, bool up, bool down, bool right, int type) { 
		this->left = left;
		this->up = up;
		this->right = right;
		this->down = down;
		this->type = type;
	};
	///Destructor
	~Tile() {};

};

#endif