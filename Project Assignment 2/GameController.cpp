#include "GameController.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here
    std::ifstream file(commands_file);
    if (!file) {
        std::cerr << "Could not load commands file : " << commands_file << std::endl;
        exit (1);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        if(line == "PRINT_GRID") {
            // Print the state of the space grid after the necessary updates in the current game tick.
            game->update_space_grid();
            game->print_space_grid();
            game->game_time++;

        } else if (line == "MOVE_UP") {
            // Move the player spacecraft one space up, if possible.
            game->player->move_up();
            game->update_space_grid();
            game->print_space_grid();
            game->game_time++;

        }else if (line == "MOVE_DOWN") {
            // Move the player spacecraft one space down, if possible.
            int height = game->space_grid.size();
            game->player->move_down(height);
            game->update_space_grid();
            game->print_space_grid();
            game->game_time++;

        } else if(line == "MOVE_RIGHT") {
            //  Move the player spacecraft one space to the right, if possible.
            int width = game->space_grid[0].size();
            game->player->move_right(width);
            game->update_space_grid();
            game->print_space_grid();
            game->game_time++;

        } else if(line == "MOVE_LEFT") {
            // Move the player spacecraft one space to the left, if possible.
            game->player->move_left();
            game->update_space_grid();
            game->print_space_grid();
            game->game_time++;

        }else if (line == "SHOOT") {
            // Shoot a projectile.
            game->shoot();
            game->update_space_grid();
            game->print_space_grid();
            game->game_time++;

        } else if (line == "NOP"){
            // No-operation in the next tick: do nothing.
            game->update_space_grid();
            game->print_space_grid();
            game->game_time++;

        } else {
            // Unknown commands are handled with printing an error message, but processing continues with the next command.
            std::cout << "Unknown command: " << line << std::endl;
            game->print_space_grid();
            game->game_time++;

        }
    }
    file.close();
    game->game_over = true;
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
    if (game) {
        delete game;
        game = nullptr; // Set pointer to nullptr for safety
    }
}
