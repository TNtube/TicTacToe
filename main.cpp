#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <random>
#include <sstream>
#include <array>


void display(std::map<std::string, std::string> & game){
    const std::array<std::string, 3> lines {"A", "B", "C"};
    const std::array<std::string, 3> columns {"1", "2", "3"};
    const std::string sep = "   +---+---+---+";
    std::string coord;
    std::cout << "   ";

    for(int i = 0; i<3; i++){
        std::cout << "  " << columns[i] << " ";
    }

    std::cout << std::endl;

    for(int i = 0; i < 3; i++){
        std::cout << sep << std::endl;
        std::cout << " " << lines[i];
        for (int j = 0; j < 3; ++j) {
            std::cout << " | " << game[lines[i] + columns[j]];
        }
        std::cout << " |"<< std::endl;
    }

    std::cout << sep << std::endl;
}


bool count3Marks(std::map<std::string, std::string> & game,
                 const std::string & symbol, const std::array<std::string, 3> & l){
    int marksCount {};
    for(const auto & element: l){
        if(game[element] == symbol){
            marksCount++;
        }
    }

    return marksCount == 3;
}


bool endGame(std::map<std::string, std::string> & game, std::string & choice){
    const std::string symbol = game[choice];

    const std::array<std::string, 3> lines {"A", "B", "C"};
    const std::array<std::string, 3> columns {"1", "2", "3"};

    const char l = choice[0];
    const char c = choice[1];

    std::array<std::string, 3> line {};
    for (int i = 0; i < 3; i++) {
        line[i] = std::string(1, l).append(columns[i]);
    }

    std::array<std::string, 3> col {};
    for (int i = 0; i < 3; i++) {
        col[i] = std::string(lines[i]).append(1, c);
    }

    return (count3Marks(game, symbol, line) ||
            count3Marks(game, symbol, col) ||
            count3Marks(game, symbol, {"A1","B2","C3"}) || // Diag1
            count3Marks(game, symbol, {"A3","B2","C1"})); // Diag2

}


int main() {

    std::map<std::string, std::string> game;

    std::vector<std::string> locations;

    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            std::stringstream ss {};
            ss << "ABC"[i] << "123"[j];
            game[ss.str()] = " ";
            locations.push_back(ss.str());
        }
    }

    std::cout << "Game of Tic Tac Toe." << std::endl;

    std::string choice;
    std::random_device rd;
    std::mt19937 gen(rd());

    while(!locations.empty()){

        display(game);

        while(true){
            std::cout << "[Player] Choose a case : " << std::endl;
            std::cin >> choice;
            if(std::find(locations.begin(), locations.end(), choice) != locations.end()){
                break;
            }
            std::cout << "Not a valid case, retry" << std::endl;
        }


        game[choice] = "X";

        display(game);

        locations.erase(std::remove(locations.begin(), locations.end(), choice), locations.end());

        if(endGame(game, choice)){
            std::cout << "Congratulation, you are the winner !" << std::endl;
            break;
        }

        if(locations.empty()){
            std::cout << "Draw, try it an other time." << std::endl;
            break;
        }

        std::cout << "[IA] I'm thinking..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::uniform_int_distribution<> randint(0, (int)locations.size() - 1);
        const int index = randint(gen);
        std::cout << "[IA] I play at " << locations[index] << std::endl;
        game[locations[index]] = "O";


        if(endGame(game, locations[index])){
            display(game);
            std::cout << "You loose, try it an other time" << std::endl;
            break;
        }

        locations.erase(locations.begin() + index);
    }
    std::cout << "Press any key to continue..." << std::endl;

    std::cin.ignore();
    std::cin.get();

    return 0;
}
