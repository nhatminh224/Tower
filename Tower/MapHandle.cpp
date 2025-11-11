#include "MapHandle.h"

vector<vector<Vector2f>> MapHandle::towerButtonData;

// MAP 1
void MapHandle::setCmap1(cmap& map, int a, int b, int C) {
	if (a >= 14 && a <= 16 && b >= 6 && b <= 9) {
		for (int i = 14; i <= 16; ++i) {
			for (int j = 6; j <= 9; ++j) {
				map.getMap()[i][j].setC(C);
			}
		}
	}

	else if (a >= 22 && a <= 24 && b >= 12 && b <= 15) {
		for (int i = 22; i <= 24; ++i) {
			for (int j = 12; j <= 15; ++j) {
				map.getMap()[i][j].setC(C);
			}
		}
	}

	else if (a >= 15 && a <= 18 && b >= 26 && b <= 28) {
		for (int i = 15; i <= 18; ++i) {
			for (int j = 26; j <= 28; ++j) {
				map.getMap()[i][j].setC(C);
			}
		}
	}

	else if (a >= 4 && a <= 6 && b >= 30 && b <= 33) {
		for (int i = 4; i <= 6; ++i) {
			for (int j = 30; j <= 33; ++j) {
				map.getMap()[i][j].setC(C);
			}
		}
	}

	else if (a >= 12 && a <= 14 && b >= 35 && b <= 38) {
		for (int i = 12; i <= 14; ++i) {
			for (int j = 35; j <= 38; ++j) {
				map.getMap()[i][j].setC(C);
			}
		}
	}
}

pair<int, int> MapHandle::getTowerdes1(int a, int b)
{
	if (a >= 14 && a <= 16 && b >= 6 && b <= 9)
		return { 16, 7 };
	else if (a >= 22 && a <= 24 && b >= 12 && b <= 15)
		return { 24, 13 };
	else if (a >= 15 && a <= 18 && b >= 26 && b <= 28)
		return { 17, 26 };
	else if (a >= 4 && a <= 6 && b >= 30 && b <= 33)
		return { 6, 31 };
	else if (a >= 12 && a <= 14 && b >= 35 && b <= 38)
		return { 14, 36 };

	return { -1, -1 };
}

void MapHandle::setIconsmap1(Sprite* icon)
{
	pair<int, int> positions[5] = {
		{16, 7},
		{24, 13},
		{17, 26},
		{6, 31},
		{14, 36}
	};

	for (int i = 0; i < 5; ++i) {
		float x = positions[i].second * cpoint::TILE_SIZE;
		float y = positions[i].first * cpoint::TILE_SIZE;
		icon[i].setPosition(x, y);
		if (i == 2)
			icon[i].setOrigin(icon[i].getTexture()->getSize().x - 107.0f, icon[i].getTexture()->getSize().y * 0.5f);
		else
			icon[i].setOrigin(icon[i].getTexture()->getSize().x * 0.07f, icon[i].getTexture()->getSize().y * 0.7f);
	}
}

int MapHandle::findBlockmap1(int a, int b)
{
	std::pair<int, int> positions[5] = {
		{16, 7},
		{24, 13},
		{17, 26},
		{6, 31},
		{14, 36}
	};

	for (int i = 0; i < 5; ++i) {
		if (a == positions[i].first && b == positions[i].second)
			return i;
	}

	return -1;
}


// MAP 2
void MapHandle::setCmap2(cmap& map, int a, int b, int C) {
	if (a >= 14 && a <= 16 && b >= 6 && b <= 9) {
		for (int i = 14; i <= 16; ++i)
			for (int j = 6; j <= 9; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 16 && a <= 19 && b >= 19 && b <= 21) {
		for (int i = 16; i <= 19; ++i)
			for (int j = 19; j <= 21; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 22 && a <= 24 && b >= 9 && b <= 12) {
		for (int i = 22; i <= 24; ++i)
			for (int j = 9; j <= 12; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 5 && a <= 7 && b >= 23 && b <= 26) {
		for (int i = 5; i <= 7; ++i)
			for (int j = 23; j <= 26; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 16 && a <= 19 && b >= 27 && b <= 29) {
		for (int i = 16; i <= 19; ++i)
			for (int j = 27; j <= 29; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 14 && a <= 16 && b >= 36 && b <= 39) {
		for (int i = 14; i <= 16; ++i)
			for (int j = 36; j <= 39; ++j)
				map.getMap()[i][j].setC(C);
	}
}

pair<int, int> MapHandle::getTowerdes2(int a, int b) {
	if (a >= 14 && a <= 16 && b >= 6 && b <= 9)
		return { 16, 7 };
	else if (a >= 16 && a <= 19 && b >= 19 && b <= 21)
		return { 18, 19 };
	else if (a >= 22 && a <= 24 && b >= 9 && b <= 12)
		return { 24, 10 };
	else if (a >= 5 && a <= 7 && b >= 23 && b <= 26)
		return { 7, 24 };
	else if (a >= 16 && a <= 19 && b >= 27 && b <= 29)
		return { 18, 27 };
	else if (a >= 14 && a <= 16 && b >= 36 && b <= 39)
		return { 16, 37 };

	return { -1, -1 };
}

void MapHandle::setIconsmap2(Sprite* icon) {
	std::pair<int, int> positions[6] = {
		{16, 7},
		{18, 19},
		{24, 10},
		{7, 24},
		{18, 27},
		{16, 37}
	};

	for (int i = 0; i < 6; ++i) {
		float x = positions[i].second * cpoint::TILE_SIZE;
		float y = positions[i].first * cpoint::TILE_SIZE;
		icon[i].setPosition(x, y);
		if (i == 1 || i == 4)
			icon[i].setOrigin(icon[i].getTexture()->getSize().x - 107.0f, icon[i].getTexture()->getSize().y * 0.5f);
		else
			icon[i].setOrigin(icon[i].getTexture()->getSize().x * 0.07f, icon[i].getTexture()->getSize().y * 0.7f);
	}
}

int MapHandle::findBlockmap2(int a, int b) {
	std::pair<int, int> positions[6] = {
		{16, 7},
		{18, 19},
		{24, 10},
		{7, 24},
		{18, 27},
		{16, 37}
	};

	for (int i = 0; i < 6; ++i) {
		if (a == positions[i].first && b == positions[i].second)
			return i;
	}

	return -1;
}

// MAP 3
void MapHandle::setCmap3(cmap& map, int a, int b, int C) {
	if (a >= 14 && a <= 16 && b >= 5 && b <= 8) {
		for (int i = 14; i <= 16; ++i)
			for (int j = 5; j <= 8; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 22 && a <= 24 && b >= 16 && b <= 19) {
		for (int i = 22; i <= 24; ++i)
			for (int j = 16; j <= 19; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 2 && a <= 4 && b >= 25 && b <= 28) {
		for (int i = 2; i <= 4; ++i)
			for (int j = 25; j <= 28; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 2 && a <= 4 && b >= 37 && b <= 40) {
		for (int i = 2; i <= 4; ++i)
			for (int j = 37; j <= 40; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 15 && a <= 18 && b >= 39 && b <= 41) {
		for (int i = 15; i <= 18; ++i)
			for (int j = 39; j <= 41; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 8 && a <= 11 && b >= 11 && b <= 13) {
		for (int i = 8; i <= 11; ++i)
			for (int j = 11; j <= 13; ++j)
				map.getMap()[i][j].setC(C);
	}
}

pair<int, int> MapHandle::getTowerdes3(int a, int b) {
	if (a >= 14 && a <= 16 && b >= 5 && b <= 8)
		return { 16, 6 };
	else if (a >= 22 && a <= 24 && b >= 16 && b <= 19)
		return { 24, 17 };
	else if (a >= 2 && a <= 4 && b >= 25 && b <= 28)
		return { 4, 26 };
	else if (a >= 2 && a <= 4 && b >= 37 && b <= 40)
		return { 4, 38 };
	else if (a >= 15 && a <= 18 && b >= 39 && b <= 41)
		return { 17, 39 };
	else if (a >= 8 && a <= 11 && b >= 11 && b <= 13)
		return { 10, 11 };
	return { -1, -1 };
}

void MapHandle::setIconsmap3(Sprite* icon) {
	std::pair<int, int> positions[6] = {
		{16, 6},
		{24, 17},
		{4, 26},
		{4, 38},
		{17, 39},
		{10, 11}
	};

	for (int i = 0; i < 6; ++i) {
		float x = positions[i].second * cpoint::TILE_SIZE;
		float y = positions[i].first * cpoint::TILE_SIZE;
		icon[i].setPosition(x, y);
		if (i == 4 || i == 5)
			icon[i].setOrigin(icon[i].getTexture()->getSize().x - 107.0f, icon[i].getTexture()->getSize().y * 0.5f);
		else
			icon[i].setOrigin(icon[i].getTexture()->getSize().x * 0.07f, icon[i].getTexture()->getSize().y * 0.7f);
	}
}

int MapHandle::findBlockmap3(int a, int b) {
	std::pair<int, int> positions[6] = {
		{16, 6},
		{24, 17},
		{4, 26},
		{4, 38},
		{17, 39},
		{10, 11}
	};

	for (int i = 0; i < 6; ++i) {
		if (a == positions[i].first && b == positions[i].second)
			return i;
	}

	return -1;
}


// MAP 4
void MapHandle::setCmap4(cmap& map, int a, int b, int C) {
	if (a >= 9 && a <= 12 && b >= 9 && b <= 11) {
		for (int i = 9; i <= 12; ++i)
			for (int j = 9; j <= 11; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 20 && a <= 22 && b >= 7 && b <= 10) {
		for (int i = 20; i <= 22; ++i)
			for (int j = 7; j <= 10; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 11 && a <= 13 && b >= 18 && b <= 21) {
		for (int i = 11; i <= 13; ++i)
			for (int j = 18; j <= 21; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 18 && a <= 21 && b >= 20 && b <= 22) {
		for (int i = 18; i <= 21; ++i)
			for (int j = 20; j <= 22; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 16 && a <= 19 && b >= 35 && b <= 37) {
		for (int i = 16; i <= 19; ++i)
			for (int j = 35; j <= 37; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 6 && a <= 8 && b >= 35 && b <= 38) {
		for (int i = 6; i <= 8; ++i)
			for (int j = 35; j <= 38; ++j)
				map.getMap()[i][j].setC(C);
	}
	else if (a >= 14 && a <= 16 && b >= 41 && b <= 44) {
		for (int i = 14; i <= 16; ++i)
			for (int j = 41; j <= 44; ++j)
				map.getMap()[i][j].setC(C);
	}
}



pair<int, int> MapHandle::getTowerdes4(int a, int b) {
	if (a >= 9 && a <= 12 && b >= 9 && b <= 11)
		return { 11, 9 };
	else if (a >= 20 && a <= 22 && b >= 7 && b <= 10)
		return { 22, 8 };
	else if (a >= 11 && a <= 13 && b >= 18 && b <= 21)
		return { 13, 19 };
	else if (a >= 18 && a <= 21 && b >= 20 && b <= 22)
		return { 20, 20 };
	else if (a >= 16 && a <= 19 && b >= 35 && b <= 37)
		return { 18, 35 };
	else if (a >= 6 && a <= 8 && b >= 35 && b <= 38)
		return { 8, 36 };
	else if (a >= 14 && a <= 16 && b >= 41 && b <= 44)
		return { 16, 42 };

	return { -1, -1 };
}

void MapHandle::setIconsmap4(Sprite* icon) {
	std::pair<int, int> positions[7] = {
		{11, 9},
		{22, 8},
		{13, 19},
		{20, 20},
		{18, 35},
		{8, 36},
		{16, 42}
	};

	for (int i = 0; i < 7; ++i) {
		float x = positions[i].second * cpoint::TILE_SIZE;
		float y = positions[i].first * cpoint::TILE_SIZE;
		icon[i].setPosition(x, y);
		if (i == 0 || i == 3 || i == 4)
			icon[i].setOrigin(icon[i].getTexture()->getSize().x - 107.0f, icon[i].getTexture()->getSize().y * 0.5f);
		else
			icon[i].setOrigin(icon[i].getTexture()->getSize().x * 0.07f, icon[i].getTexture()->getSize().y * 0.7f);
	}
}

int MapHandle::findBlockmap4(int a, int b) {
	std::pair<int, int> positions[7] = {
		{11, 9},
		{22, 8},
		{13, 19},
		{20, 20},
		{18, 35},
		{8, 36},
		{16, 42}
	};

	for (int i = 0; i < 7; ++i) {
		if (a == positions[i].first && b == positions[i].second)
			return i;
	}

	return -1;
}


// Main Handle
void MapHandle::setCmap(int index, cmap& map, int a, int b, int C) {
	switch (index) {
	case 0:
		setCmap1(map, a, b, C);
		break;
	case 1:
		setCmap2(map, a, b, C);
		break;
	case 2:
		setCmap3(map, a, b, C);
		break;
	case 3:
		setCmap4(map, a, b, C);
		break;
	default:
		break;
	}
}

pair<int, int> MapHandle::getTowerdes(int index, int a, int b) {
	switch (index) {
	case 0:
		return getTowerdes1(a, b);
		break;
	case 1:
		return getTowerdes2(a, b);
		break;
	case 2:
		return getTowerdes3(a, b);
		break;
	case 3:
		return getTowerdes4(a, b);
		break;
	default:
		return { -1, -1 };
	}
}

void MapHandle::setIconsmap(int index, Sprite* icon) {
	switch (index) {
	case 0:
		setIconsmap1(icon);
		break;
	case 1:
		setIconsmap2(icon);
		break;
	case 2:
		setIconsmap3(icon);
		break;
	case 3:
		setIconsmap4(icon);
		break;
	default:
		break;
	}
}

int MapHandle::findBlockmap(int index, int a, int b) {
	switch (index) {
	case 0:
		return findBlockmap1(a, b);
	case 1:
		return findBlockmap2(a, b);
	case 2:
		return findBlockmap3(a, b);
	case 3:
		return findBlockmap4(a, b);
	default:
		return -1;
	}
}



void MapHandle::initTowerButtonData()
{
	towerButtonData.resize(4); // 4 maps

	// MAP 1
	towerButtonData[0] = {
		// Tower at (13, 8)
		{280.f, 480.f}, {160.f, 640.f}, {400.f, 640.f}, {200.f, 520.f},

		// Tower at (21, 14)
		{520.f, 760.f}, {400.f, 920.f}, {640.f, 920.f}, {440.f, 780.f},

		// Tower at (16, 27)
		{1040.f, 520.f}, {920.f, 680.f}, {1160.f, 680.f}, {960.f, 560.f},

		// Tower at (5, 31)
		{1240.f, 80.f}, {1120.f, 240.f}, {1360.f, 240.f}, {1160.f, 120.f},

		// Tower at (13, 36)
		{1440.f, 400.f}, {1320.f, 560.f}, {1560.f, 560.f}, {1360.f, 440.f}
	};

	// MAP 2
	towerButtonData[1] = {
		// Tower at (15, 8)
		{280.f, 480.f}, {160.f, 640.f}, {380.f, 640.f}, {200.f, 520.f},

		// Tower at (17, 21)
		{760.f, 560.f}, {640.f, 720.f}, {880.f, 720.f}, {680.f, 600.f},

		// Tower at (23, 10)
		{400.f, 760.f}, {280.f, 920.f}, {520.f, 920.f}, {320.f, 800.f},

		// Tower at (6, 24)
		{960.f, 120.f}, {840.f, 280.f}, {1080.f, 280.f}, {880.f, 160.f},

		// Tower at (17, 29)
		{1120.f, 560.f}, {1000.f, 720.f}, {1240.f, 720.f}, {1040.f, 600.f},

		// Tower at (15, 37)
		{1480.f, 480.f}, {1360.f, 640.f}, {1600.f, 640.f}, {1400.f, 520.f}
	};

	// MAP 3
	towerButtonData[2] = {
		// Tower at (15, 6)
		{240.f, 480.f}, {120.f, 640.f}, {360.f, 640.f}, {160.f, 520.f},

		// Tower at (23, 17)
		{680.f, 800.f}, {560.f, 960.f}, {800.f, 960.f}, { 600.f, 840.f},

		// Tower at (3, 26)
		{1040.f, 0.f}, {920.f, 160.f}, {1160.f, 160.f}, {960.f, 40.f},

		// Tower at (3, 38)
		{1520.f, 0.f}, {1400.f, 160.f}, {1640.f, 160.f}, { 1440.f, 40.f},

		// Tower at (16, 40)
		{1560.f, 520.f}, {1440.f, 680.f}, {1680.f, 680.f}, {1480.f, 560.f},

		// Tower at (9, 12)
		{480.f, 240.f}, {380.f, 400.f}, {600.f, 400.f}, {400.f, 280.f}
	};

	// MAP 4
	towerButtonData[3] = {
		// Tower at (10, 10)
		{400.f, 280.f}, {280.f, 440.f}, {520.f, 440.f}, {320.f, 320},

		// Tower at (21, 8)
		{320.f, 720.f}, {200.f, 880.f}, {440.f, 880.f}, {240.f, 760.f},

		// Tower at (12, 19)
		{760.f, 360.f}, {640.f, 520.f}, {880.f, 520.f}, {680.f, 400.f},

		// Tower at (19, 21)
		{840.f, 640.f}, {720.f, 800.f}, {960.f, 800.f}, {760.f, 680.f},

		// Tower at (17, 36)
		{1400.f, 560.f}, {1280.f, 720.f}, {1520.f, 720.f}, {1320.f, 600.f},

		// Tower at (7, 36)
		{1440.f, 160.f}, {1320.f, 320.f}, {1560.f, 320.f}, {1360.f, 200.f},

		// Tower at (15, 42)
		{1680.f, 480.f}, {1560.f, 640.f}, {1800.f, 640.f}, {1600.f, 520.f}
	};
}

vector<Vector2f> MapHandle::getTowerButtons(int levelIndex, int row, int col)
{
	pair<int, int> towerPos = getTowerdes(levelIndex, row, col);
	if (towerPos.first == -1) return {};

	int index = -1;

	if (levelIndex == 0) {
		if (towerPos == make_pair(16, 7)) index = 0;
		else if (towerPos == make_pair(24, 13)) index = 1;
		else if (towerPos == make_pair(17, 26)) index = 2;
		else if (towerPos == make_pair(6, 31)) index = 3;
		else if (towerPos == make_pair(14, 36)) index = 4;
	}

	else if (levelIndex == 1) {
		if (towerPos == make_pair(16, 7)) index = 0;
		else if (towerPos == make_pair(18, 19)) index = 1;
		else if (towerPos == make_pair(24, 10)) index = 2;
		else if (towerPos == make_pair(7, 24)) index = 3;
		else if (towerPos == make_pair(18, 27)) index = 4;
		else if (towerPos == make_pair(16, 37)) index = 5;
	}

	else if (levelIndex == 2) {
		if (towerPos == make_pair(16, 6)) index = 0;
		else if (towerPos == make_pair(24, 17)) index = 1;
		else if (towerPos == make_pair(4, 26)) index = 2;
		else if (towerPos == make_pair(4, 38)) index = 3;
		else if (towerPos == make_pair(17, 39)) index = 4;
		else if (towerPos == make_pair(10, 11)) index = 5;
	}

	else if (levelIndex == 3) {
		if (towerPos == make_pair(11, 9)) index = 0;
		else if (towerPos == make_pair(22, 8)) index = 1;
		else if (towerPos == make_pair(13, 19)) index = 2;
		else if (towerPos == make_pair(20, 20)) index = 3;
		else if (towerPos == make_pair(18, 35)) index = 4;
		else if (towerPos == make_pair(8, 36)) index = 5;
		else if (towerPos == make_pair(16, 42)) index = 6;
	}


	if (index == -1) return {};

	int start = index * 4;
	return {
		towerButtonData[levelIndex][start],
		towerButtonData[levelIndex][start + 1],
		towerButtonData[levelIndex][start + 2],
		towerButtonData[levelIndex][start + 3]
	};
}
