#include <iostream>
#include <vector>

#define HUMAN 1
#define AI 2

struct Game_info {
    char ai_symb;
    char hum_symb;
    int cur_player;
    int steps;
};

void print_board(std::vector<std::vector<char>>& board) {
    for (int i = 0; i < 3; ++i) {
        std::cout << "\t\t ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " ";
            if (j != 2) {
                std::cout << "| ";
            }
        }
        if (i != 2) {
            std::cout << "\n\t\t-----------\n"; 
        }
    }
    std::cout << "\n\n";
} 

void print_ai_board(std::vector<std::vector<std::vector<char>>>& board) {
    std::cout << " " << board[0][0][0] << " | " << board[0][0][1] << " | " << board[0][0][2] << "\t"
        << " " << board[1][0][0] << " | " << board[1][0][1] << " | " << board[1][0][2] << "\t"
        << " " << board[2][0][0] << " | " << board[2][0][1] << " | " << board[2][0][2] << "\n";
	std::cout << "-----------\t-----------\t-----------\n"; 
    std::cout << " " << board[0][1][0] << " | " << board[0][1][1] << " | " << board[0][1][2] << "\t"
        << " " << board[1][1][0] << " | " << board[1][1][1] << " | " << board[1][1][2] << "\t"
        <<" " << board[2][1][0] << " | " << board[2][1][1] << " | " << board[2][1][2] << "\n";
    std::cout << "-----------\t-----------\t-----------\n"; 
    std::cout << " " << board[0][2][0] << " | " << board[0][2][1] << " | " << board[0][2][2] << "\t"
    << " " << board[1][2][0] << " | " << board[1][2][1] << " | " << board[1][2][2] << "\t"
    << " " << board[2][2][0] << " | " << board[2][2][1] << " | " << board[2][2][2] << "\n\n";
}

void print_congrats(int turn) { 
	if (turn == HUMAN) {
        std::cout << "\t    VICTORY OF HUMANITY\n\n"; 
    } else {
        std::cout << "\t   MACHINES RULE THE WORLD\n\n"; 
    }
} 

std::vector<int> get_available_moves(std::vector<std::vector<char>>& board) {
    std::vector<int> available_moves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                available_moves.push_back(i * 3 + j);
            }
        }
    }
    return available_moves;
}

bool check_win(std::vector<std::vector<char>>& board) { 
    for (int i = 0; i < 3; ++i) { 
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            return true; 
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            return true; 
        }
	}
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        return true; 
    }
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        return true; 
    }
    return false;
}

int minimax(std::vector<std::vector<char>>& board, int depth, Game_info& info, bool min_flag = false) {
	int cur_score = 0, res_score = 0;
	if (check_win(board)) {
        return min_flag ? -1 : 1;
	} else {
        if (depth == 9) {
            return 0;
        } else {
            auto available_moves = get_available_moves(board);
			if (min_flag) {
				res_score = INT_MIN;
                for (int i = 0; i < available_moves.size(); ++i) {
                    int row = available_moves[i] / 3, column = available_moves[i] % 3;
                    board[row][column] = info.ai_symb;
                    cur_score = minimax(board, depth + 1, info);
                    board[row][column] = ' ';
                    res_score = std::max(cur_score, res_score);
                }
			} else {
				res_score = INT_MAX;
                for (int i = 0; i < available_moves.size(); ++i) {
                    int row = available_moves[i] / 3, column = available_moves[i] % 3;
                    board[row][column] = info.hum_symb;
                    cur_score = minimax(board, depth + 1, info, true);
                    board[row][column] = ' ';
                    res_score = std::min(cur_score, res_score);
                }
			}
            return res_score;
        }
	}
    return 0;
}

int ai_random_choice(std::vector<std::vector<char>>& board, Game_info& info) {
    while (true) {
        int pos = rand() % 9;
        if (board[pos / 3][pos % 3] == ' ') {
            return pos;
        } else {
            continue;
        }
    }
    return -1;
}

int find_best_ai_move(std::vector<std::vector<char>>& board, Game_info& info) {
	int cur_score = 0, res_score = INT_MIN, res_pos = -1;
    if (!info.steps) {
        return rand() % 9;
    }
    auto available_moves = get_available_moves(board);
    for (int i = 0; i < available_moves.size(); ++i) {
        int row = available_moves[i] / 3, column = available_moves[i] % 3;
        board[row][column] = info.ai_symb;
        cur_score = minimax(board, info.steps + 1, info, false);
        board[row][column] = ' ';
        if (cur_score > res_score) {
            res_score = cur_score;
            res_pos = available_moves[i];
        }
    }
	return res_pos;
}

void play(Game_info& info) { 
    std::vector<std::vector<std::vector<char>>> board(3,
        std::vector<std::vector<char>>(3, std::vector<char>(3, ' ')));
	int row = 0, column = 0, pos;
    char pos_str;
    info.steps = 0;
    while (true) {
        if (info.cur_player == HUMAN) {
            for (int k = 0; k < 3; ++k) {
                // auto available_moves = get_available_moves(board[k]);
                // std::cout << "Available moves for Field " << k + 1 << ": ";
                // for (auto& elem : available_moves) {
                //     std::cout << elem + 1 << " ";
                // }
                // std::cout << "\n";
                while (true) {
                    std::cout << "\nChoose the position for FIELD " << k + 1 << " (from 1 to 9): ";
                    std::cin >> pos_str;
                    pos = pos_str - '0';
                    --pos;
                    if (pos < 0 || pos > 8) {
                        std::cout << "Invalid position. Try again\n";
                        continue;
                    }
                    row = pos / 3, column = pos % 3;
                    if (board[k][row][column] != ' ') {
                        std::cout << "This position isn't available. Choose another one\n";
                    } else {
                        board[k][row][column] = info.hum_symb;
                        if (check_win(board[k])) {
                            std::cout << "\n\t\t   FINAL\n\n";
                            print_board(board[k]);
                            print_congrats(info.cur_player); 
                            return ;
                        }
                        break;
                    }
                }
            }
            info.cur_player = AI;
            ++info.steps;
        } else {
            for (int k = 0; k < 3; ++k) {
                pos = find_best_ai_move(board[k], info);
                // pos = ai_random_choice(board[k], info);
                row = pos / 3, column = pos % 3;
                board[k][row][column] = info.ai_symb;
                if (check_win(board[k])) {
                    std::cout << "\n\t\t   FINAL\n\n";
                    print_board(board[k]);
                    print_congrats(info.cur_player); 
                    return ;
                }
            }
            info.cur_player = HUMAN;
            ++info.steps;
        }
        if (info.steps == 9) {
            std::cout << "\n\t\t   FINAL\n\n";
            print_ai_board(board);
            std::cout << "\t\t   DRAW\n";
            return ;
        }
        std::cout << "\n\t\t   STEP " << info.steps << "\n\n";
        print_ai_board(board);
    }
} 

int main() { 
	std::cout << "\n -------------------------------------------- \n";
	std::cout << "|\t\t TIC_TAC_TOE\t\t     |"; 
	std::cout << "\n -------------------------------------------- \n\n";
    Game_info info;
    char next_round = 'y';
    while (next_round == 'y') {
        char symb, turn;
        std::cout << "Choose X or O: ";
        std::cin >> symb;
        if (symb == 'X') {
            info.hum_symb = 'X';
            info.ai_symb = 'O';
        } else if (symb == 'O') {
            info.hum_symb = 'O';
            info.ai_symb = 'X';
        } else {
            std::cout << "Invalid symbol\n";
            break;
        }
	 	std::cout << "Choose the order of the move (1 or 2): ";
	 	std::cin >> turn;
		if (turn == '1') {
            info.cur_player = HUMAN;
        } else if (turn == '2') {
            info.cur_player = AI;
        } else {
            std::cout << "Invalid number\n";
            break;
        }
        play(info);
		std::cout << "Continue (y/n): ";
       	std::cin >> next_round;
    }
	return (0); 
} 