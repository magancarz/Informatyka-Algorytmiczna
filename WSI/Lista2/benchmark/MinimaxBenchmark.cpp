#include "MinimaxBenchmark.h"

#include <chrono>
#include <functional>
#include <vector>
#include <fstream>
#include <random>

#include "BenchmarkPlayer.h"


void MinimaxBenchmark::benchmark() {
	int max_win_count = 0;

	const auto values = {1, 10, 100};//, 1000, 10000};
	const int count_of_steps = 3;

	std::vector<BenchmarkResult> benchmark_results;
	benchmark_results.reserve(std::pow(count_of_steps, 3));

	for (const auto& player_win_modifier : values) {
		for (const auto& opponent_win_modifier : values) {
			for (const auto& opponent_near_win_block_modifier : values) {
				for (const auto& player_near_win_block_modifier : values) {
					auto player1 = BenchmarkPlayer(4, 1, player_win_modifier, player_win_modifier, opponent_win_modifier, opponent_win_modifier, player_near_win_block_modifier, opponent_near_win_block_modifier, nullptr);
					uint64_t win_count = 0;

					const auto start = std::chrono::steady_clock::now();
					
					for (const auto& second_player_win_modifier : values) {
						for (const auto& second_opponent_win_modifier : values) {
							for (const auto& second_opponent_near_win_block_modifier : values) {
								for (const auto& second_player_near_win_block_modifier : values) {
									auto player2 = BenchmarkPlayer(4, 2, second_player_win_modifier, second_player_win_modifier, second_opponent_win_modifier, second_opponent_win_modifier, second_player_near_win_block_modifier, second_opponent_near_win_block_modifier, nullptr);
									int result = runConfiguration(player1, player2);
									if (result == 1) {
										++win_count;
									}
								}
							}
						}
					}

					if (win_count > max_win_count) {
						max_win_count = win_count;
						std::cout << "Current winner (" << win_count << " wins):\n" << "depth = " << player1.depth << std::endl
									<< "player_win_modifier = " << player1.player_win_modifier << std::endl
									<< "player_lose_modifier = " << player1.player_lose_modifier << std::endl
									<< "opponent_win_modifier = " << player1.opponent_win_modifier << std::endl
									<< "opponent_lose_modifier = " << player1.opponent_lose_modifier << std::endl
									<< "player_near_win_block_modifier = " << player1.player_near_win_block_modifier << std::endl
									<< "opponent_near_win_block_modifier = " << player1.opponent_near_win_block_modifier << std::endl;
					}

					const auto end = std::chrono::steady_clock::now();
					std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " s." << std::endl;
					benchmark_results.push_back(BenchmarkResult{
						win_count,
						player1.depth,
						player1.player_win_modifier, player1.player_lose_modifier,
						player1.opponent_win_modifier,
						player1.opponent_lose_modifier,
						player1.player_near_win_block_modifier,
						player1.opponent_near_win_block_modifier});
				}
			}
		}
	}

	std::cout << std::endl;
	saveResultsToFile(benchmark_results);
}

void MinimaxBenchmark::battleBenchmark() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,3);
	std::uniform_int_distribution<std::mt19937::result_type> depth_dist(1,10);

	const std::vector<int> values = {1, 10, 100, 1000, 10000};//, 10000};
	auto player1 = BenchmarkPlayer(depth_dist(rng), 1, values[dist(rng)], values[dist(rng)], values[dist(rng)], values[dist(rng)], values[dist(rng)], values[dist(rng)], nullptr);
	int no_of_iterations = 100;

	for (int i = 0; i < no_of_iterations; ++i) {
		std::cout << "Iteration no. " << i << std::endl;
		player1 = testPlayerWithEveryConfiguration(player1);
	}

	std::cout << "Current winner:\n" << "depth = " << player1.depth << std::endl
			<< "player_win_modifier = " << player1.player_win_modifier << std::endl
			<< "player_lose_modifier = " << player1.player_lose_modifier << std::endl
			<< "opponent_win_modifier = " << player1.opponent_win_modifier << std::endl
			<< "opponent_lose_modifier = " << player1.opponent_lose_modifier << std::endl
			<< "player_near_win_block_modifier = " << player1.player_near_win_block_modifier << std::endl
			<< "opponent_near_win_block_modifier = " << player1.opponent_near_win_block_modifier << std::endl;
}

BenchmarkPlayer MinimaxBenchmark::testPlayerWithEveryConfiguration(BenchmarkPlayer& player) {

	const std::vector<int> values = {1, 10, 100, 1000};
	const std::vector<int> depth_values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int win_count = 0;
	float win_count_limit = std::pow(values.size(), 6) * 10 * 0.75f;

	for (const auto& depth : depth_values) {
		for (const auto& player_win_modifier : values) {
			for (const auto& player_lose_modifier : values) {
				for (const auto& opponent_win_modifier : values) {
					for (const auto& opponent_lose_modifier : values) {
						for (const auto& player_near_win_block_modifier : values) {
							for (const auto& opponent_near_win_block_modifier : values) {
								auto second_player = BenchmarkPlayer(depth, 2, player_win_modifier, player_lose_modifier, opponent_win_modifier, opponent_lose_modifier, player_near_win_block_modifier, opponent_near_win_block_modifier, nullptr);

								if (player == second_player) continue;
								if (runConfiguration(player, second_player) == 2) {
									if (runConfiguration(second_player, player) == 1) {
										if (win_count >= win_count_limit) {
											std::cout << "Player with over 75% wins:\n" << "depth = " << player.depth << std::endl
														<< "player_win_modifier = " << player.player_win_modifier << std::endl
														<< "player_lose_modifier = " << player.player_lose_modifier << std::endl
														<< "opponent_win_modifier = " << player.opponent_win_modifier << std::endl
														<< "opponent_lose_modifier = " << player.opponent_lose_modifier << std::endl
														<< "player_near_win_block_modifier = " << player.player_near_win_block_modifier << std::endl
														<< "opponent_near_win_block_modifier = " << player.opponent_near_win_block_modifier << std::endl;
											std::cout << "---------------\n";
										}

										return second_player;
									}
								}

								if (runConfiguration(second_player, player) == 2) {
									++win_count;
								}

								
							}
						}
					}
				}
			}
		}
	}
	
	std::cout << "Player with 100% wins:\n" << "depth = " << player.depth << std::endl
				<< "player_win_modifier = " << player.player_win_modifier << std::endl
				<< "player_lose_modifier = " << player.player_lose_modifier << std::endl
				<< "opponent_win_modifier = " << player.opponent_win_modifier << std::endl
				<< "opponent_lose_modifier = " << player.opponent_lose_modifier << std::endl
				<< "player_near_win_block_modifier = " << player.player_near_win_block_modifier << std::endl
				<< "opponent_near_win_block_modifier = " << player.opponent_near_win_block_modifier << std::endl;
	std::cout << "---------------\n";

	return player;
}

void MinimaxBenchmark::geneticBenchmark() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> values_dist(0, 3);
    std::uniform_int_distribution<std::mt19937::result_type> depth_dist(1, 10);
	
	const std::vector<int> values = {1, 10, 100, 1000};
	const int no_of_iterations = 20;

	std::vector<BenchmarkPlayer> players(8);
	for (int i = 0; i < players.size(); ++i) {
		players[i] = BenchmarkPlayer(depth_dist(rng), 1, values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], nullptr);
	}

	for (int i = 0; i < no_of_iterations; ++i) {
		std::cout << "Iteration no. " << i << std::endl;
		auto iteration_result = iterateGenetic(players);
		players = iteration_result;
	}

	saveResultsToFile(players);
}

std::vector<BenchmarkPlayer> MinimaxBenchmark::iterateGenetic(std::vector<BenchmarkPlayer>& players) {

	std::vector<BenchmarkResult> iteration_results(players.size());

	for (int i = 0; i < iteration_results.size(); ++i) {
		auto benchmark_result = testPlayerWithEveryPlayerInGroup(players[i], players);
		iteration_results[i] = benchmark_result;
	}

	std::sort(iteration_results.begin(), iteration_results.end(),
		[&](const BenchmarkResult& first, const BenchmarkResult& second) { return first.win_count > second.win_count; });

	for (int i = iteration_results.size() / 2; i < iteration_results.size(); ++i) {
		players[i] = mixTwoPlayersStatistics(iteration_results[i - iteration_results.size() / 2], iteration_results[i - iteration_results.size() / 2 + 1]);
	}

	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> values_dist(0, 3);
    std::uniform_int_distribution<std::mt19937::result_type> depth_dist(1, 10);
	
	const std::vector<int> values = {1, 10, 100, 1000};

	players[iteration_results.size() - 2] = BenchmarkPlayer(depth_dist(rng), 1, values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], nullptr);
	players[iteration_results.size() - 1] = BenchmarkPlayer(depth_dist(rng), 1, values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], values[values_dist(rng)], nullptr);

	return players;
}

BenchmarkResult MinimaxBenchmark::testPlayerWithEveryPlayerInGroup(BenchmarkPlayer& player, std::vector<BenchmarkPlayer>& players) {

	unsigned long long win_count = 0;

	for (auto& second_player : players) {
		if (runConfiguration(player, second_player) == 1) ++win_count;
	}

	return {win_count, player.depth, player.player_win_modifier, player.player_lose_modifier, player.opponent_win_modifier, player.opponent_lose_modifier, player.player_near_win_block_modifier, player.opponent_near_win_block_modifier};
}

BenchmarkPlayer MinimaxBenchmark::mixTwoPlayersStatistics(BenchmarkResult& player1, BenchmarkResult& player2) {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> values_dist(0, 1);

	BenchmarkPlayer new_player;
	new_player.depth = values_dist(rng) ? player1.depth : player2.depth;
	new_player.player_win_modifier = values_dist(rng) ? player1.player_win_modifier : player2.player_win_modifier;
	new_player.player_lose_modifier = values_dist(rng) ? player1.player_lose_modifier : player2.player_lose_modifier;
	new_player.opponent_win_modifier = values_dist(rng) ? player1.opponent_win_modifier : player2.opponent_win_modifier;
	new_player.opponent_lose_modifier = values_dist(rng) ? player1.opponent_lose_modifier : player2.opponent_lose_modifier;
	new_player.player_near_win_block_modifier = values_dist(rng) ? player1.player_near_win_block_modifier : player2.player_near_win_block_modifier;
	new_player.opponent_near_win_block_modifier = values_dist(rng) ? player1.opponent_near_win_block_modifier : player2.opponent_near_win_block_modifier;
	new_player.board = nullptr;

	return new_player;
}

int MinimaxBenchmark::runConfiguration(BenchmarkPlayer& player1, BenchmarkPlayer& player2) {

	//std::cout << "Testing two players configuration:\n";

	int** board = new int*[5];
	for (int i = 0; i < 5; ++i) {
		board[i] = new int[5];
	}
	
	setBoard(board);
	//printBoard(board);

	int no_of_moves = 0;
	int result = 0;

	player1.currentPlayer = 1;
	player1.board = board;
	player2.currentPlayer = 2;
	player2.board = board;

	while (true) {
		auto move = player1.findBestMove();
		auto move_value = move.first * 10 + move.second;
		setMove(board, move_value, player1.currentPlayer);
		++no_of_moves;
		if (no_of_moves >= 25) break;

		//printBoard(board);
		if (no_of_moves >= 7 && winCheck(board, player1.currentPlayer)) {
			//std::cout << "Player 1 won!\n";
			result = 1;
			break;
		}

		if (no_of_moves >= 5 && loseCheck(board, player1.currentPlayer)) {
			//std::cout << "Player 1 lost!\n";
			result = 2;
			break;
		}

		move = player2.findBestMove();
		move_value = move.first * 10 + move.second;
		setMove(board, move_value, player2.currentPlayer);
		++no_of_moves;
		if (no_of_moves >= 25) break;

		//printBoard(board);
		if (no_of_moves >= 7 && winCheck(board, player2.currentPlayer)) {
			//std::cout << "Player 2 won!\n";
			result = 2;
			break;
		}

		if (no_of_moves >= 5 && loseCheck(board, player2.currentPlayer)) {
			//std::cout << "Player 2 lost!\n";
			result = 1;
			break;
		}
	}

	for (int i = 0; i < 5; ++i) {
		delete[] board[i];
	}
	delete[] board;

	return result;
}

void MinimaxBenchmark::saveResultsToFile(std::vector<BenchmarkResult>& benchmark_results) {
	std::cout << "-----------------------\n";
	std::cout << "Saving results to file.\n";

	std::sort(benchmark_results.begin(), benchmark_results.end(),
		[&](const BenchmarkResult& first, const BenchmarkResult& second) { return first.win_count > second.win_count; });

	std::ofstream output_file_stream("benchmark.txt");
	if (output_file_stream.is_open()) {
		for (const auto& benchmark_result : benchmark_results) {
			output_file_stream << "win count: " << benchmark_result.win_count << std::endl;
			output_file_stream << "depth: " << benchmark_result.depth << std::endl;
			output_file_stream << "player win modifier: " << benchmark_result.player_win_modifier << std::endl;
			output_file_stream << "player lose modifier: " << benchmark_result.player_lose_modifier << std::endl;
			output_file_stream << "opponent win modifier: " << benchmark_result.opponent_win_modifier << std::endl;
			output_file_stream << "opponent lose modifier: " << benchmark_result.opponent_lose_modifier << std::endl;
			output_file_stream << "player near win block modifier: " << benchmark_result.player_near_win_block_modifier << std::endl;
			output_file_stream << "opponent near win block modifier: " << benchmark_result.opponent_near_win_block_modifier << std::endl;
			output_file_stream << "-----------------------\n";
		}

		output_file_stream.close();
	}
}

void MinimaxBenchmark::saveResultsToFile(std::vector<BenchmarkPlayer>& benchmark_results) {
	std::cout << "-----------------------\n";
	std::cout << "Saving results to file.\n";
	
	std::ofstream output_file_stream("benchmark");
	if (output_file_stream.is_open()) {
		for (const auto& benchmark_result : benchmark_results) {
			output_file_stream << "depth: " << benchmark_result.depth << std::endl;
			output_file_stream << "player win modifier: " << benchmark_result.player_win_modifier << std::endl;
			output_file_stream << "player lose modifier: " << benchmark_result.player_lose_modifier << std::endl;
			output_file_stream << "opponent win modifier: " << benchmark_result.opponent_win_modifier << std::endl;
			output_file_stream << "opponent lose modifier: " << benchmark_result.opponent_lose_modifier << std::endl;
			output_file_stream << "player near win block modifier: " << benchmark_result.player_near_win_block_modifier << std::endl;
			output_file_stream << "opponent near win block modifier: " << benchmark_result.opponent_near_win_block_modifier << std::endl;
			output_file_stream << "-----------------------\n";
		}

		output_file_stream.close();
	}
}