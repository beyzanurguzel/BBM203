#include "AsteroidDash.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <regex>


// A reference to the 2D coded space grid
  // 0 for space grid, 1 for player, 2 for asteroid, 3 for ammo, 4 for live, 5 for projectile
  vector<vector<int>> codedGrid;



// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name); // initialize leader entries
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here
    std::ifstream file(input_file);
    if (!file) {
        std::cerr << "Could not load space grid file : " << input_file << std::endl;
        exit (1);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::vector<int> row;
        std::vector<int> rowCoded;
        int value;

        while (line_stream >> value) {
            row.push_back(value);
            rowCoded.push_back(value);
        }
        space_grid.push_back(row);
        codedGrid.push_back(rowCoded);
    }
    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here
    std::ifstream file(player_file_name);
    if (!file) {
        std::cerr << "Could not load player file : " << player_file_name << std::endl;
        exit (1);
    }

    std::vector<std::vector<bool>> playerShape;
    std::string line;
    std::getline(file, line);

    int startRow = line[0] - '0';
    int startCol = line[2] - '0';

    while (std::getline(file, line)) {
        std::vector<bool> row;
        std::istringstream line_stream(line);
        int value;

        while (line_stream >> value) {
            if (value == 0 || value == 1) {
                bool boolValue = (value == 1); // '1' -> true, '0' -> false
                row.push_back(boolValue);
            }
        }
        playerShape.push_back(row);
    }
    Player* playerObject = new Player(playerShape, startRow, startCol, player_name, 10, 3);
    player = playerObject;
    file.close();
}

bool areEqual(const std::vector<std::vector<bool>>& vec1, const std::vector<std::vector<bool>>& vec2) {
    // check sizes
    if (vec1.size() != vec2.size()) return false;

    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i].size() != vec2[i].size()) return false;

        // compare values
        for (size_t j = 0; j < vec1[i].size(); ++j) {
            if (vec1[i][j] != vec2[i][j]) return false;
        }
    }

    return true;  // If they equal, return true
}

void rotate(CelestialObject* celestial_object, bool isRotationRight) {
        vector<vector<bool>> shape = celestial_object->shape;
        int howManyShapes = 3;
        std::vector<std::vector<bool>> copyShape;
        for (const auto& row : shape) {
            copyShape.push_back(row);  // make a copy shape
        }
        // rotate 3 times
        while(howManyShapes != 0) {
            vector<vector<bool>> newShape;
            size_t newCol =  copyShape.size();
            size_t newRow = copyShape[0].size();
            for(size_t i = 0; i < newRow; ++i) {
                vector<bool> rotationsRow;
                for (size_t j = 0; j < newCol; ++j) {
                    if (isRotationRight) {
                        rotationsRow.push_back(copyShape[newCol - j - 1][i]);
                    } else {
                        rotationsRow.push_back(copyShape[j][newRow - 1 - i]);
                    }
                }
                newShape.push_back(rotationsRow);
            }

            if (areEqual(celestial_object->shape, newShape) and howManyShapes == 3) {
                // it means it is the right shape of the original shape.
                // if they are same, then all rotations must be the same, so there is no need to create other rotations node
                // let right and left rotation pointers point itself
                celestial_object->right_rotation = celestial_object;
                celestial_object->left_rotation = celestial_object;
                celestial_object->left_rotation->next_celestial_object = celestial_object->next_celestial_object;
                celestial_object->right_rotation->next_celestial_object = celestial_object->next_celestial_object;
                // break the loop and continue
                break;
            }
            if (howManyShapes == 3) {
                auto* newCelestial = new CelestialObject(celestial_object);
                newCelestial->shape = newShape;
                celestial_object->right_rotation = newCelestial;
                newCelestial->left_rotation = celestial_object;
                newCelestial->next_celestial_object = celestial_object->next_celestial_object;
                copyShape = newShape; // right rotation --------------------------
            }
            if (areEqual(celestial_object->shape, newShape) and howManyShapes == 2) {
                // it means it is the down rotation of the original shape
                // if they are the same, then right and left rotations of the original shape are the same
                // set rotation pointers and break the loop
                celestial_object->right_rotation->right_rotation = celestial_object;
                celestial_object->left_rotation = celestial_object->right_rotation;
                break;
            }
            if (howManyShapes == 2) {
                auto* newCelestial = new CelestialObject(celestial_object);
                newCelestial->shape = newShape;
                newCelestial->next_celestial_object = celestial_object->next_celestial_object;

                celestial_object->right_rotation->right_rotation = newCelestial;
                newCelestial->left_rotation = celestial_object->right_rotation;
                copyShape = newShape;
            }
            if (howManyShapes == 1) {
                // if we enter this situation, it means the all rotations are different
                // we created 3 nodes by far, so we should create the left rotation of original celestial object
                auto* newCelestial = new CelestialObject(celestial_object);
                newCelestial->shape = newShape;
                newCelestial->next_celestial_object = celestial_object->next_celestial_object;
                celestial_object->right_rotation->right_rotation->right_rotation = newCelestial;
                newCelestial->left_rotation = celestial_object->right_rotation->right_rotation;
                newCelestial->right_rotation = celestial_object;
                celestial_object->left_rotation = newCelestial;
            }
            // do nothing, let loop continues
            howManyShapes--;
        }

}
// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here
    std::ifstream file(input_file);
    if (!file) {
        std::cerr << "Could not load celestial objects file : " << input_file << std::endl;
        exit (1);
    }
    std::string line;
    int rowNumber;
    int timeStep;
    std::string effect;
    ObjectType objectType;
    CelestialObject *temp = celestial_objects_list_head;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        if (line[0] == '[' or line[0] == '{') {
            vector<vector<bool>> shape;
            char startChar =(line[0] == '[') ? '[' : '{';
            std::vector<bool> row;

            for (size_t i = 1; i < line.size(); ++i) {
                if (line[i] == '0' || line[i] == '1') {
                    bool boolValue = (line[i] == '1');
                    row.push_back(boolValue);
                }
            }
            shape.push_back(row);
            while (std::getline(file, line) && line[0] != 's') {
                std::vector<bool> row1;
                for (size_t i = 0; i < line.size(); ++i) {
                    if (line[i] == '0' || line[i] == '1') {
                        bool boolValue = (line[i] == '1'); // '1' -> true, '0' -> false
                        row1.push_back(boolValue);
                    }
                }
                shape.push_back(row1);
            }

            std::smatch match;
            std::regex number_regex("(\\d+)");

            if (std::regex_search(line, match, number_regex)) {
                rowNumber = std::stoi(match.str(0));  // takes the first number that found and converts it to integer
            }
            std::getline(file, line);
            if (std::regex_search(line, match, number_regex)) {
                timeStep = std::stoi(match.str(0)); // takes the first number that found and converts it to integer
            }

            if (startChar == '[') {
                objectType = ASTEROID;
            }

            if (startChar == '{') {
                std::getline(file, line);
                effect = line.substr(2, line.size() - 2);
                objectType = effect == "ammo" ? AMMO : LIFE_UP;
            }
            auto* celestial_object = new CelestialObject(shape, objectType, rowNumber, timeStep);
            rotate(celestial_object, true);

            if(celestial_objects_list_head == nullptr) {
                celestial_objects_list_head = celestial_object;
                temp = celestial_object;
            }else {
                temp->next_celestial_object = celestial_object;
                temp = temp->next_celestial_object;
            }
        }
    }
    file.close();
}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
    std::cout << "Tick: " << game_time << std::endl;
    std::cout << "Lives: " << player->lives << std::endl;
    std::cout << "Ammo: " << player->current_ammo << std::endl;
    std::cout << "Score: " << 0 << std::endl;
    std::cout << "High Score: " << 0 << 0 << std::endl;
    for (const auto& row : space_grid) {
        for (int value : row) {
            if (value == true) {
                std::cout << occupiedCellChar;
            } else {
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


struct Projectile {
    int row;
    int col;
    Projectile *next = nullptr;

    Projectile(int r, int c) : row(r), col(c) {}
};
Projectile *projectile_head = nullptr;
Projectile *tempProjectile = nullptr;

void deleteProjectile(int row, int col) {
    Projectile *tempDelete = projectile_head;
    Projectile *prevDelete = nullptr;
    while (tempDelete != nullptr) {
        if (tempDelete->row == row && tempDelete->col == col) {
            if (prevDelete == nullptr) {
                prevDelete = tempDelete;
                projectile_head = projectile_head->next;
                delete prevDelete;
                break;
            } else {
                prevDelete->next = tempDelete->next;
                delete tempDelete;
                break;
            }
        } else {
            prevDelete = tempDelete;
            tempDelete = tempDelete->next;
        }
    }
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {

    // TODO: Your code here
    // update space grşd and coded grid
    for (size_t i = 0; i < space_grid.size(); i++) {
        for (size_t j = 0; j < space_grid[0].size(); j++) {
            space_grid[i][j] = 0;
            codedGrid[i][j] = 0;
        }
    }

    // place player
    size_t playerHeight = player->spacecraft_shape.size();
    size_t playerWidth = player->spacecraft_shape[0].size();
    int playerRow = player->position_row;
    int playerCol = player->position_col;

    for (int i = 0; i < playerHeight; i++) {
        for (int j = 0; j < playerWidth; j++) {
            if (player->spacecraft_shape[i][j] == 1) {
                codedGrid[i + playerRow][j + playerCol] = 1;
            }
        }
    }

    // place projectiles
    Projectile *tempP = projectile_head;
    Projectile *prevP = nullptr;
    while (tempP != nullptr) {
        if (tempP->col == space_grid[0].size() - 1) {
            Projectile *projectileDelete = tempP;
            tempP = tempP->next;
            if (prevP == nullptr) {
                // Deleting the head projectile
                projectile_head = tempP;
            } else {
                // Skip the deleted node
                prevP->next = tempP;
            }
            delete projectileDelete;
        } else {
            tempP->col++;
            codedGrid[tempP->row][tempP->col] = 5; // 5 represents projectile in coded grid
            prevP = tempP;
            tempP = tempP->next;
        }
    }

    // check if any collisons for celestials with projectiles or player
    CelestialObject *temp = celestial_objects_list_head;
    CelestialObject *previous = nullptr;
    while (temp != nullptr && temp->time_of_appearance <= game_time) {

        int timeSpend = game_time - temp->time_of_appearance;
        int startCol = space_grid[0].size() - timeSpend - 1;
        if (startCol == - player->spacecraft_shape[0].size()) {
            CelestialObject *neededtoDelete = temp;
            if (previous == nullptr) {
                celestial_objects_list_head = temp->next_celestial_object;
            } else {
                previous->next_celestial_object = temp->next_celestial_object;
            }
            temp = temp->next_celestial_object;
            delete neededtoDelete;
            continue;
        }

        int startRow = temp->starting_row;
        bool breakLoop = false;
        while (breakLoop != true) {
            for (int r = 0; temp != nullptr && r < temp->shape.size(); r++) {
                for (int c = 0; temp != nullptr &&  c < temp->shape[0].size(); c++) {
                   if (breakLoop) break;
                    // if celestial's cell is 1, then we can consider if we move to the left this cell
                    if (temp->shape[r][c] == 1) {
                        if (startCol + c <= 0 || startCol + c > space_grid[0].size() - 1) {
                            continue;
                        }
                        if (codedGrid[startRow + r][startCol + c] == 1 || codedGrid[startRow + r][startCol + c + 1] == 1) {
                            // it means we hit the player (1 represents player)
                            if (previous == nullptr) {
                                if (temp->object_type == LIFE_UP) {
                                    player->lives++;
                                } else if (temp->object_type == AMMO) {
                                    player->current_ammo = 10;
                                } else {
                                    player->lives--;
                                    if (player->lives == 0) {
                                        game_over = true;
                                    }
                                }
                                CelestialObject *deleteObject = temp;
                                celestial_objects_list_head = celestial_objects_list_head->next_celestial_object;
                                delete deleteObject;
                                temp = celestial_objects_list_head;
                                breakLoop = true;
                            } else {
                                if (temp->object_type == LIFE_UP) {
                                    player->lives++;
                                } else if (temp->object_type == AMMO) {
                                    player->current_ammo = 10; // make full ammo--------------
                                } else {
                                    player->lives--;
                                    if (player->lives == 0) {
                                        game_over = true; // ---------------------------------
                                    }
                                }
                                previous->next_celestial_object = temp->next_celestial_object;
                                CelestialObject *deleteObject = temp;
                                temp = temp->next_celestial_object;
                                delete deleteObject;
                                breakLoop = true;
                            }
                        }else if(codedGrid[startRow + r][startCol + c] == 5 || codedGrid[startRow + r][startCol + c + 1] == 5) {
                            // it means we hit a projectile
                            if (temp->object_type == ASTEROID) {

                                int hittedCol;
                                if (codedGrid[startRow + r][startCol + c] == 5) {
                                    hittedCol = startCol + c ;
                                } else {
                                    hittedCol = startCol + c + 1 ;
                                }
                                // remove projectile
                                deleteProjectile(startRow + r, hittedCol);
                                // remove hitted cell of celestial
                                temp->shape[r][c] = false;
                                   breakLoop = true;
                            }
                        }
                    }
                }
                if (breakLoop) break;
            }
            breakLoop = true;
        }
        previous = temp;
        if (temp == nullptr) {
            break;
        }
        temp = temp->next_celestial_object;
    }

    // update coded grid
    for (size_t i = 0; i < space_grid.size(); i++) {
        for (size_t j = 0; j < space_grid[0].size(); j++) {
            codedGrid[i][j] = 0;
        }
    }

    // place player
    size_t playerHeight2 = player->spacecraft_shape.size();
    size_t playerWidth2 = player->spacecraft_shape[0].size();
    int playerRow2 = player->position_row;
    int playerCol2 = player->position_col;

    for (int i = 0; i < playerHeight2; i++) {
        for (int j = 0; j < playerWidth2; j++) {
            if (player->spacecraft_shape[i][j] == 1) {
                codedGrid[i + playerRow2][j + playerCol2] = 1;
            }
        }
    }
    // place projetiles
    Projectile *tempPr = projectile_head;
    while (tempPr != nullptr) {
        codedGrid[tempPr->row][tempPr->col] = 5;
        tempPr = tempPr->next;
    }
    // place celestials
    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr && current->time_of_appearance <= game_time) {
        int startcol = space_grid[0].size() - (game_time - current->time_of_appearance) - 1;
        int startrow = current->starting_row;
        for (int i = 0; i < current->shape.size(); i++) {
            for (int j = 0; j < current->shape[0].size(); j++) {
                if (current->shape[i][j] == 1) {
                    if (startcol + j < space_grid[0].size() && startcol + j >= 0) {
                        codedGrid[startrow + i][startcol + j] = 2;
                    }
                }
            }
        }
        current = current->next_celestial_object;
    }
    // update space grid with all changes
    for (int r = 0; r < space_grid.size(); r++) {
        for (int c = 0; c < space_grid[0].size(); c++) {
            if (codedGrid[r][c] != 0) {
                space_grid[r][c] = 1;
            }
        }
    }
}


// Corresponds to the SHOOT command. It should shoot if the player has enough ammo. It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here
    if (player->current_ammo > 0) {
        //shoot
        int projectileRow = (int) player->position_row + (player->spacecraft_shape.size() / 2);
        int projectileCol = player->position_col + player->spacecraft_shape[0].size() - 1;
        player->current_ammo--;
        auto* projectile = new Projectile(projectileRow, projectileCol);
        if (projectile_head == nullptr) {
            projectile_head = projectile;
            tempProjectile = projectile;
        } else {
            tempProjectile->next = projectile;
            tempProjectile = tempProjectile->next;
        }
    }
}


// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here

    // Delete celestial objects
    CelestialObject* currentCelestial = celestial_objects_list_head;
    while (currentCelestial != nullptr) {
        CelestialObject* to_delete = currentCelestial;

        // Handle linked rotations if needed
        CelestialObject* rotation = currentCelestial->right_rotation;
        while (rotation != nullptr && rotation != currentCelestial) {
            CelestialObject* to_delete_rotation = rotation;
            rotation = rotation->right_rotation;
            delete to_delete_rotation;
        }

        currentCelestial = currentCelestial->next_celestial_object;
        delete to_delete;
    }

    // Delete the player
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }

    // Delete projectiles
    Projectile* currentProjectile = projectile_head;
    while (currentProjectile != nullptr) {
        Projectile* to_delete = currentProjectile;
        currentProjectile = currentProjectile->next;
        delete to_delete;
    }

    // Clear the space grid and codedGrid (optional if dynamic memory isn't involved)
    space_grid.clear();
    codedGrid.clear();
}

