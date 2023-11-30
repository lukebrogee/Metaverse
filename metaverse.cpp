#include "metaverse.h"
#include "display.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <vector>


//counts the number of squares around the occupied square has neighbors(meaning if they are true)
int count_neighbors(const metaverse_t& board, int row, int column) {

    int count{ 0 };

    

    for (int i = -1; i <= 1; i++) {
        for (int x = -1; x <= 1; x++) {

            if (((row + i) >= 0 && (column + x) >= 0)
                && ((row + i) < board.size() && ((column + x) < board.size()))) {
                if (board[row + i][column + x]) {
                    count++;
                }
            }

        }
    }
    if ((count > 0) && (board[row][column])) {
        count--;
    }

    return count;
}






//determines whether each section on the board will keep its citizenship or not
bool occupied_in_next_tick(bool currently_occupied, int neighbor_count) {
    //If the location is occupied and it has either two or three occupied neighbors, the location will continue to be occupied during the next generation.
    //If the location is not occupied and it has exactly three occupied neighbors, the location will become occupied during the next generation.

    if (currently_occupied) {
        if (neighbor_count == 2 || neighbor_count == 3) {
            return true;
        }
    }
    else if (!currently_occupied) {
        if (neighbor_count == 3) {
            return true;
        }
    }

    return false;
}



//changes the board accordingly with how many neighbors it has and if its already occupied
metaverse_t tick(const metaverse_t& board) {

     metaverse_t newboard;
     int new_size = board.size();


    for (int i = 0; i < board.size(); i++) {

        for (int x = 0; x < board.size(); x++) {
            
            resize_metaverse(board.size(), newboard);

            if (occupied_in_next_tick(board[i][x], count_neighbors(board, i, x))) {
                newboard[i][x] = true;
            }
            else {
                newboard[i][x] = false;
            }
        }
    }
    return metaverse_t{newboard};
}






//resizes the board by the number of rows, since the board is a cube it doesnt matter the column
bool resize_metaverse(int rows, metaverse_t& board) {

    board.resize(rows, std::vector<bool>(rows));

    return true;
}






//reads one line at a time and converts the char values of '1' and '0' to true and false
bool citizenship_row_to_metaverse_row(const std::string& input_row, int row,
    metaverse_t& board) {


    for (int i = 0; i < input_row.size(); i++) {
        if (input_row[i] == '1') {
            board[row][i] = true;
        }
        else {
            board[row][i] = false;
        }
    }



    return input_row.size() == board.size();
}






//reads the first line of the file to gather the size of the board and generations
bool read_metaverse_configuration_line_from_file(std::ifstream& metaverse_file,
    int& size, int& generations) {
    char comma{};

    if (metaverse_file >> size >> comma >> generations) {
        return true;
    }



    return false;
}







//initializes the metaverse by checking if all of the other functions are true
bool initialize_metaverse_from_file(std::ifstream& metaverse_file,
    metaverse_t& metaverse,
    int& generations) {
    int size{};
    int actual_row{};

    generations = 0;


    if (!read_metaverse_configuration_line_from_file(metaverse_file, size,
        generations)) {



        return false;
    }

    if (!resize_metaverse(size, metaverse)) {
        return false;
    }

    metaverse_file >> std::ws;
    while (!metaverse_file.eof()) {
        std::string line{};
        if (!std::getline(metaverse_file, line)) {
            return false;
        }
        if (!citizenship_row_to_metaverse_row(line, actual_row, metaverse)) {
            return false;
        }
        actual_row++;
        metaverse_file >> std::ws;
    }
    return actual_row == size;
}





//displays the metaverse the number of times generations is
void model_metaverse(const metaverse_t& starting_metaverse, int generations) {


    metaverse_t new_board = starting_metaverse;
    for (int i = 0; i < generations; i++) {
        
        display_metaverse(std::cout, new_board);
        new_board = tick(new_board);
    }
}

