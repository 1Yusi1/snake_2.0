#pragma once
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

class Config {
public:
	Config() {
		try {
			std::ifstream file("config.json");
			data = json::parse(file);
		}
		catch (std::exception ex) {
			data = {
				{"border",{
					{"start_x", 5},
					{"end_x", 112},
					{"start_y", 4},
					{"end_y", 28}
				}},
				{"snakes", {
					{"snake_1", {
						{"head", {
							{"x", 10},
							{"y", 10}
						}},
						{"step", {
							{"x", 0},
							{"y", 1}
						}},
					}},
					{"snake_2", {
						{"head", {
							{"x", 10},
							{"y", 102}
						}},
						{"step", {
							{"x", 0},
							{"y", -1}
						}},
					}},
				}},
				{"apple"}

			};
		}
		
	}

	json& get() {
		return data;
	}

	void apply() {
		
	}

private:
	json data;
};