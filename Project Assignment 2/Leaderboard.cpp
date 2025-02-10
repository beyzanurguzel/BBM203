#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not load leader board file : " << filename << std::endl;
        std::ofstream new_file(filename);
        if (!new_file) {
            std::cerr << "Error creating the leaderboard file: " << filename << std::endl;
            return;
        }
        new_file.close();
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        unsigned long score;
        time_t lastPlayed;
        std::string playerName;

        // Parse the line into score, timestamp, and player name
        if (line_stream >> score >> lastPlayed && std::getline(line_stream >> std::ws, playerName)) {
            // Create a new LeaderboardEntry object
            LeaderboardEntry *entry = new LeaderboardEntry(score, lastPlayed, playerName);

           insert(entry);
        }
    }
    file.close();
}

std::string timestamp_to_string(time_t timestamp) {
    // Convert the timestamp to a tm struct
    std::tm *time_info = std::localtime(&timestamp);

    // Format the time as a string
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", time_info);

    return std::string(buffer);
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
    LeaderboardEntry *temp = head_leaderboard_entry;
    std::ofstream file(filename, std::ios::out | std::ios::trunc); // it deletes the contents of the txt
    int order = 1;

    for (int i = 0; i < 10; i++) {
        if (temp == nullptr) {
            break;
        } else {
            std::string formatted_time = timestamp_to_string(temp->last_played);
            file << temp->score << " " << temp->last_played << " " << temp->player_name << std::endl;
        }
        order++;
        temp = temp->next;
    }
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here
    std::cout << "Leaderboard\n-----------" << std::endl;

    LeaderboardEntry *temp = head_leaderboard_entry;
    int order = 1;
    while (temp != nullptr) {
        std::string formatted_time = timestamp_to_string(temp->last_played);
        std::cout << order << ". " << temp->player_name << " " << temp->score << " " << formatted_time << std::endl;
        order++;
        temp = temp->next;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // TODO: Your code here
    if (head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
        return;
    }
    LeaderboardEntry *temp = head_leaderboard_entry;
    LeaderboardEntry *prev = nullptr;
    while (temp != nullptr) {
        if (temp->score < new_entry->score) {
            if (prev == nullptr) {
                new_entry->next = head_leaderboard_entry;
                head_leaderboard_entry = new_entry;
            } else {
                prev->next = new_entry;
                new_entry->next = temp;
            }
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    prev->next = new_entry;
    new_entry->next = nullptr;

}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
    LeaderboardEntry *temp = head_leaderboard_entry;
    while (temp != nullptr) {
        LeaderboardEntry* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }
    head_leaderboard_entry = nullptr;
}
