#pragma once
#include "Drawable.h"
#include <vector>
#include <curses.h>

class DisplayManager {
public:
	void drawAll(std::vector<Drawable*> &elements) {
		clear();
		for (const auto& x : elements) {
			x->draw();
		}
	}

	void draw(Drawable* element) {
		element->draw();
	}
};
