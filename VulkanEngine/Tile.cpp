#include "VEInclude.h"
#include "math.h"

int Tile::getOpenWays() {
	int i = 0;
	if (right)
		i++;
	if (left)
		i++;
	if (up)
		i++;
	if (down)
		i++;
	return i;
}

std::string Tile::getMeshName() {
	switch (type) {
	case 0: // playground
		switch (getOpenWays()) {
		case 2:
			if (right == left)
				return "StraightBlock";
			else
				return "CurveBlock";
		case 3:
			return "TBlock";
		case 4:
			return "CrossBlock";
		default:
			return "";
		}
	case 1: // wall
		return "Ceiling";
	case 2: // 
		return "";
	case 3: // teleport
		return "Straight";
	default:
		return "";
	}
}
float Tile::getMeshRotation() {
	switch (getOpenWays()) {
	case 2:
		if (left == right) {
			if (left)
				return M_PI / 2;
			else
				return 0;
		}
		else {
			if (up) {
				if (left)
					return 0;
				else
					return M_PI / 2;
			}
			else {
				if (left)
					return M_PI * 3 / 2;
				else
					return M_PI;
			}
		}
	case 3:
		if (right)
			return 0;
		if (down)
			return M_PI / 2;
		if (left)
			return M_PI;
		if (up)
			return M_PI * 3 / 2;
	default:
		return 0;
	}
}