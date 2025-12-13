//
// Created by Marek on 13.12.2025.
//

#ifndef GAME_SNAKE_H
#define GAME_SNAKE_H
#include <utility>

struct Head {
	Head() {
		pos.first = 15; pos.second = 7;
		last_x = 15; last_y = 7;

	}
	std::pair<int, int> pos;
	int last_x, last_y;

};

struct Body {
	Body(const int _x, const int _y) {
		x = _x; y = _y;
		last_x = _x; last_y = _y;
		next = nullptr;
		prev = nullptr;
	}
	void print() const{
		std::cout << x << " " << y << " " << last_x << " " << last_y << std::endl;
	}
	int x;
	int y;
	int last_x;
	int last_y;
	Body* prev;
	Body* next;
};

class Snake {
public:
	Snake(): head(Head()) {
		for (int i = 0; i < 3; i++) {
			body.reserve(100);
			auto tmp = Body(head.pos.first - i - 1, head.pos.second);
			if (!body.empty()) {
				tmp.prev = &body.back();
				body.back().next = &tmp;


			}
			body.push_back(tmp);
		}
	}
	Head head;
	std::vector<Body> body;

	void extend() {
		auto tmp = Body(body.back().last_x, body.back().last_y);
		tmp.prev = &body.back();
		body.back().next = &tmp;
		body.push_back(tmp);
	}
};

#endif //GAME_SNAKE_H