#include "GameController.h"
#include <fstream>
#include <sstream>
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
    fstream file(commands_file);
    string line;

    game->update_space_grid();


    while (getline(file, line)) {


        if (line == "PRINT_GRID") {
            game->print_space_grid();

        } else if (line == "SHOOT") {
            game->shoot();
        } else if (line == "MOVE_DOWN") {

            game->player->move_down(game->space_grid.size());
        } else if (line == "MOVE_UP") {
            game->player->move_up();

        } else if (line == "MOVE_LEFT") {

            game->player->move_left();
        } else if (line == "MOVE_RIGHT") {

            game->player->move_right(game->space_grid[0].size());
        } else if (line == "NOP") {
           // game->print_space_grid();

        } else {
            cout << "Unknown command: " << line << endl;
        }
        game->game_time++;
        game->update_space_grid();
        if(game->scoreincrease) {
            game->current_score++;
        }




        if (game->player->lives <= 0) {
            game->game_over = true;
            for (auto &row : game->player->spacecraft_shape) {
                fill(row.begin(), row.end(), 0);
            }
            game->update_space_grid();
            //game->current_score++;

            cout << "GAME OVER!" << endl;
            break;
        }
       // game->print_space_grid();





    }
    if (!game->game_over) {

        game->game_time--;
        game->game_over = true;
        cout << "GAME FINISHED! No more commands!" << endl;
    }
    game->print_space_grid();
    LeaderboardEntry* new_entry = new LeaderboardEntry(game->current_score, game->game_time, game->player->player_name);
    game->leaderboard.insert(new_entry);

    game->leaderboard.write_to_file("leaderboard.txt");

    game->leaderboard.print_leaderboard();

}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
    delete game;
    game==nullptr;
}
