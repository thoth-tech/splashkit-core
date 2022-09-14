#include "game_learning.h"
#include "machine_learning.h"
#include "matrix_2d.h"
using namespace std;

namespace splashkit_lib
{
	void InputFormat::convert_int(std::vector<int> input, std::vector<bool> *out, int index)
	{
		for (int i = 0; i < sizes[index]; i++)
		{
			if (input[i] != 0)
			{
				// indexes_out is The starting index for the given data in bool form
				// max_vals is the maximum value in the input integer matrix
				// indexes_in is The starting index for the given data in int form

				// indexes_in[indexes_in[index] + i] tells us which integer we are looking at
				// indexes_out[index] tells us where the bool representation of the integer range starts
				// i * max_vals[index] tells us where the bool representation of this specific starts inside the integer range
				// input[indexes_in[index] + i] - 1 we want to set the input-th bool to true to get a unique bool representation for each possible integer
				// the -1 is there since we can represent 0 as all zeroes
				(*out)[indexes_out[index] + i * max_vals[index] + input[indexes_in[index] + i] - 1] = true;

				// e.g. we might receive input [0, 1, 2] (max_vals=2)
				// we would want to return [00, 10, 01] where each integer has a unique bool representation
				// or more literally [...,0,1,1,0,0,1,...] since we are only modifing a section of out (0 is false, 1 is true)
			}
		}
	}

	int InputFormat::add_type(Type type, int size, int max_val)
	{
		if (size < 1)
			LOG(WARNING) << "Size must be greater than 0!";
		if (max_val <= 1)
			LOG(WARNING) << "Max Value must be greater than 1!";
		format.push_back(type);				  // How to interpret the data
		sizes.push_back(size);				  // How many values to read
		max_vals.push_back(max_val - 1);	  // The range of the given values, with a domain of [0, max_val) exclusive
		indexes_in.push_back(input_width);	  // The starting index for the given data in int form
		indexes_out.push_back(output_width);  // The starting index for the given data in bool form
		input_width += size;				  // The total width of the inputted data
		output_width += size * (max_val - 1); // The total width of the outputted data
		return format.size() - 1;
	}

	std::vector<bool> InputFormat::convert_input(std::vector<int> input)
	{
		std::vector<bool> out(output_width, false);
		for (int i = 0; i < format.size(); i++)
		{
			convert_int(input, &out, i); // only integer inputs supported currently
		}
		return out;
	}

	bool InputFormat::operator==(const InputFormat &other) const
	{
		return input_width == other.input_width && output_width == other.output_width && format == other.format && sizes == other.sizes && max_vals == other.max_vals && indexes_in == other.indexes_in && indexes_out == other.indexes_out;
	}

	int OutputFormat::add_type(Type type, int data)
	{
		format.push_back(type);
		f_data.push_back(data);
		indexes.push_back(output_width);
		switch (type)
		{
		case Type::Number:
			output_width++;
			break;
		case Type::Position:
		case Type::Category:
			output_width += data;
			break;
		}
		return format.size() - 1;
	}

	OutputValue::OutputValue(int width)
	{
		value = std::vector<float>(width, 0.5f);
	}

	OutputValue::OutputValue(matrix_2d m)
	{
		value = std::vector<float>(m.y);
		for (int i = 0; i < m.y; i++)
			value[i] = m.elements[0][i];
	}

	// TODO: if random take weighted random?
	int OutputValue::get_max_position(OutputFormat format, int index, const std::vector<int> filter, bool random)
	{
		if (format.get_type(index) != OutputFormat::Type::Position && format.get_type(index) != OutputFormat::Type::Category)
			LOG(WARNING) << "Format at index is not of type Position!";
		index = format.get_output_index(index); // Grab the starting index for the given position data
		if (random)
		{
			int out = filter[rnd(filter.size())]; // take a random value from the list of possible solutions
			// update reward_table
			to_update(out);
			return out;
		}
		int initial_index = rnd(filter.size()); // Set the initial index to a random position, this prevents the AI from always playing the first move when all equal
		float max = value[index + filter[initial_index]];
		int max_pos = filter[initial_index];
		for (int i = 0; i < filter.size(); i++)
		{
			if (value[index + filter[i]] > max)
			{
				max = value[index + filter[i]];
				max_pos = filter[i];
			}
		}
		to_update(max_pos);
		return max_pos;
	}

	float OutputValue::process_output_number(OutputFormat format, int index, bool random)
	{
		if (format.get_type(index) != OutputFormat::Type::Number)
			LOG(WARNING) << "Format at index is not of type Number!";
		to_update(format.get_output_index(index));														 // Tells the AI that the number was retrieved and should be updated during reward/punishment
		return (random ? rnd() : value[format.get_output_index(index)]) * format.get_output_info(index); // Scale the number to the previously given range [0, f_data[index]]
	}

	void OutputValue::update(float learning_rate, float discount_factor, float reward, OutputValue *next_move)
	{
		for (int i = 0; i < indexes.size(); i++)
		{
			if (next_move == nullptr)
			{
				value[indexes[i]] += learning_rate * reward; // Next move is game over with this reward. i.e. instant win/instant loss
				continue;
			}

			float max_future;
			if (indexes.size() != next_move->indexes.size())
			{
				max_future = next_move->value[indexes[i]]; // Fall-back to this to ensure index is in bounds
			}
			else
			{
				max_future = next_move->value[next_move->indexes[i]];
			}

			// QLearning method, using the next move as future reward
			// since we use temporal difference learning we update these in reverse order and subsequently take into account all future steps
			value[indexes[i]] += learning_rate * (reward + discount_factor * max_future - value[indexes[i]]);
		}
	}

	string OutputValue::to_string() const
	{
		std::stringstream ss;
		ss << "[ ";
		for (float v : value)
		{
			ss << v << " ";
		}
		ss << "]";
		return ss.str();
	}

	RewardTable::RewardTable(int width)
	{
		this->width = width;
	}

	OutputValue &RewardTable::get_value(std::vector<bool> &key)
	{
		// log(INFO, "RewardTable::get_value");
		if (reward_table.find(key) == reward_table.end()) // key not found
		{
			// log(INFO, "RewardTable::get_value - creating new q_value");
			reward_table[key] = OutputValue(width);
		}
		return reward_table[key];
	}

	OutputValue &RewardTable::get_value(Game *game)
	{
		vector<bool> game_state = game->get_input_format().convert_input(game->get_input());
		return get_value(game_state);
	}

	void Agent::train(Game *game, int player_count, int iterations)
	{
		int bar_length = 50;
		int bar_units = iterations / bar_length; // calculates how many iterations account for each bar
		int progress_units = iterations / 100;

		cout << "Training " << name() << " for " << iterations << " iterations" << endl;
		std::vector<SelfPlay*> agents;
		for (int i = 0; i < player_count; i++)
		{
			agents.push_back(create_self_play()); // generate the agents to play the game
		}

		// Initialise progress bar
		cout << "  0 % [" << string(bar_length, '_') << "]" << std::flush;

		// Main loop
		for (int i = 0; i < iterations; i++)
		{
			while (!game->is_finished())
			{
				int move = agents[game->get_current_player()]->get_move(game);
				game->make_move(move);
			}

			// Reward agents
			std::vector<float> scores = game->score();
			for (int i = 0; i < scores.size(); i++)
				agents[i]->reward(scores[i]);
			game->reset();

			// Draw progress bar
			if (i % progress_units == 0)
			{
				int percentage = i / progress_units;
				cout << "\r  " << percentage << "%  [" << string((i / bar_units), '#') << std::flush;
			}
		}
		cout << "\r  100%" << endl;

		for (SelfPlay *agent : agents)
			delete agent;

		total_iterations += iterations;
	}

	class QAgent::SelfPlay : public splashkit_lib::SelfPlay
	{
	private:
		RewardTable *reward_table;
		std::vector<OutputValue *> move_history; // Store the q_values used for the last game
	public:
		float learning_rate; // step size
		float discount_factor; // discount factor for future rewards
		float epsilon; // chance of random move

		SelfPlay(RewardTable *reward_table, float learning_rate, float discount_factor, float epsilon)
		{
			this->reward_table = reward_table;
			this->learning_rate = learning_rate;
			this->discount_factor = discount_factor;
			this->epsilon = epsilon;
		}

		/**
		 * @brief Get the move this agent wishes to play.
		 *
		 * @param game
		 * @return int
		 */
		int get_move(Game *game)
		{
			std::vector<bool> input = game->get_input_format().convert_input(game->get_input()); // convert input to boolean for neural networks / ease of hashing
			OutputValue &output = reward_table->get_value(input);								 // get the q_value for the current state
			move_history.push_back(&output);													 // remember that we took this move for reward if we win or lose
			return game->convert_output(output, rnd() < epsilon);								 // convert the q_value into a move, if epsilon take a random move (e.g. take the highest value (best) move)
		}

		/**
		 * @brief Rewards all the moves taken in the game. If we won the game we give a reward so that we try to take that move again.
		 *
		 * @param score The total score of this agent
		 */
		void reward(float score)
		{
			move_history[move_history.size() - 1]->update(learning_rate, discount_factor, score, nullptr); // Special case for first instance
			for (int i = move_history.size() - 2; i >= 0; i--)
			{
				move_history[i]->update(learning_rate, discount_factor, score, move_history[i + 1]);
				move_history[i + 1]->reset();
			}
			move_history[0]->reset();
			move_history.clear();
		}
	};

	QAgent::QAgent(OutputFormat &out_format, float learning_rate, float discount_factor, float epsilon)
	{
		this->learning_rate = learning_rate;
		this->discount_factor = discount_factor;
		this->epsilon = epsilon;
		reward_table = new RewardTable(out_format.get_width());
	}

	int QAgent::get_move(Game *game)
	{
		std::vector<bool> input = game->get_input_format().convert_input(game->get_input());
		OutputValue &output = reward_table->get_value(input);
		return game->convert_output(output, false);
	}

	SelfPlay *QAgent::create_self_play()
	{
		return new QAgent::SelfPlay(reward_table, learning_rate, discount_factor, epsilon);
	}

	class DenseAgent::SelfPlay : public splashkit_lib::SelfPlay
	{
	private:
		Model *model;
		std::vector<OutputValue> move_history; // Store the moves played for the last game
		std::vector<std::vector<matrix_2d>> forward_history; // Forward propagation history
		int current_move = 0;

		float learning_rate; // step size
		float epsilon; // chance of picking a random non-optimal move (exploration vs exploitation)
		float epsilon_decay; // how much to decay epsilon/reduce by each iteration as a percentage
		float epsilon_min; // minimum value for epsilon (dont decay past this)

		float average_score = 0.0; // average reward that the agent gets
		int current_epoch = 0;
	public:
		SelfPlay(Model *model, float learning_rate, float epsilon, float epsilon_decay, float epsilon_min)
		{
			this->model = model;
			this->learning_rate = learning_rate;
			this->epsilon = epsilon;
			this->epsilon_decay = epsilon_decay;
			this->epsilon_min = epsilon_min;
			forward_history.reserve(32);
		}
		
		/**
		 * @brief Get the move this agent wishes to play.
		 *
		 * @param game
		 * @return int
		 */
		int get_move(Game *game) override
		{
			std::vector<bool> input = game->get_input_format().convert_input(game->get_input()); // convert input to boolean for neural networks / ease of hashing
			
			matrix_2d input_matrix(1, input.size());
			for (int i = 0; i < input.size(); i++)
				input_matrix[0][i] = input[i] ? 1.0 : -1.0; // convert boolean to double for neural network

			forward_history.resize(current_move + 1); // ensure space for forward propagation
			matrix_2d output_matrix = model->forward(forward_history, input_matrix, 0, current_move++);

			move_history.push_back(OutputValue(output_matrix));

			return game->convert_output(move_history.back(), rnd() < epsilon);
		}

		/**
		 * @brief Rewards all the moves taken in the game. If we won the game we give a reward so that we try to take that move again.
		 *
		 * @param score The total score of this agent
		 */
		void reward(float score) override
		{
			// cumulative moving average
			// calculate the agent's average score for the past 50 games
			current_epoch++;
			int cma_len = current_epoch < 50 ? current_epoch : 50;
			average_score = (average_score * (cma_len - 1) + score) / cma_len;

			matrix_2d target_output(current_move, model->get_output_size());
			for (int i = 0; i < current_move; i++)
			{
				for (int j = 0; j < model->get_output_size(); j++)
					target_output[i][j] = move_history[i][j]; // initialise target output to the played move (i.e. y = yhat)

				vector<int> used_outputs = move_history[i].get_indexes();
				for (int j : used_outputs)
				{
					// TODO: This part of the code seems to be causing the agent to learn to output the same for every input?
					// TODO: This algorithm is bad, since average_score converges to 0 and therefore target_output will always be the same (e.g. draw and loss are punished equally)
					// if we received an above average reward than we want to reinforce that
					// if we received a below average reward than we want to discourage that
					if (target_output.elements[i][j] > 0.5 && score < average_score ||
						target_output.elements[i][j] < 0.5 && score > average_score)
					{
						// Reduce the target
						target_output.elements[i][j] -= abs(score - average_score) * 0.3;
						target_output.elements[i][j] = max(target_output.elements[i][j], 0.0);
					}
					else
					{
						// Increase the target
						target_output.elements[i][j] += abs(score - average_score) * 0.3;
						target_output.elements[i][j] = min(target_output.elements[i][j], 1.0);
					}
				}

				move_history[i].reset();
			}
			
			vector<double> losses;
			auto avg_deltas = model->backward(losses, forward_history, target_output, 0);
			model->update_weights(avg_deltas, forward_history);
			epsilon = max(epsilon_min, epsilon * epsilon_decay); // decay epsilon

			// reset
			current_move = 0;
			move_history.clear();
			forward_history.clear();
		}
	};

	DenseAgent::DenseAgent(InputFormat &in_format, OutputFormat &out_format, Type type)
	{
		int input = in_format.get_out_width();
		int output = out_format.get_width();
		switch (type)
		{
		case Type::Tiny:
			learning_rate = 0.01;
			model = new Model(MSE, learning_rate);
			model->add_layer(new Dense(input, output, Sigmoid));
			break;
		case Type::Small:
			learning_rate = 0.01;
			model = new Model(MSE, learning_rate);
			model->add_layer(new Dense(input, output*2, Sigmoid));
			model->add_layer(new Dense(output*2, output, Sigmoid));
			break;
		case Type::Medium:
			learning_rate = 0.1;
			model = new Model(MSE, learning_rate);
			model->add_layer(new Dense(input, output*4, Sigmoid));
			model->add_layer(new Dense(output*4, output*2, Sigmoid));
			model->add_layer(new Dense(output*2, output, Sigmoid));
			break;
		case Type::Large:
			learning_rate = 0.1;
			model = new Model(MSE, learning_rate);
			model->add_layer(new Dense(input, input/2, Sigmoid));
			model->add_layer(new Dense(input/2, output*4, Sigmoid));
			model->add_layer(new Dense(output*4, output*2, Sigmoid));
			model->add_layer(new Dense(output*2, output, Sigmoid));
			break;
		}
	}

	int DenseAgent::get_move(Game *game)
	{
		std::vector<bool> input = game->get_input_format().convert_input(game->get_input()); // convert input to boolean for neural networks / ease of hashing
			
		matrix_2d input_matrix(1, input.size());
		for (int i = 0; i < input.size(); i++)
			input_matrix[0][i] = input[i] ? 1.0 : -1.0;

		matrix_2d output_matrix = model->predict(input_matrix);
		OutputValue output(output_matrix); // convert to output value

		return game->convert_output(output, false);
	}

	splashkit_lib::SelfPlay *DenseAgent::create_self_play()
	{
		return new DenseAgent::SelfPlay(model, learning_rate, epsilon, epsilon_decay, epsilon_min);
	}

	MinimaxAgent::MinimaxAgent(InputFormat input_format)
	{
		this->input_format = input_format;
	}

	std::vector<float> MinimaxAgent::search_move(Game *game, int &best_move)
	{
		// TODO: Research Alpha Beta pruning for multiple players

		if (game->is_finished())
		{
			return game->score(); // TODO: Maybe AI gets stuck if scores are not normalised
		}

		int next_best_move = 0;
		int cur_player = game->get_current_player();
		std::vector<float> best_score(cur_player + 1, -999999); // TODO: Better default
		int temp;

		std::vector<int> all_moves = game->get_possible_moves();
		for (int move : all_moves)
		{
			Game *new_position = game->clone();
			new_position->make_move(move);

			std::vector<bool> new_game_state = input_format.convert_input(new_position->get_input());

			if (cache.find(new_game_state) == cache.end()) // TODO: produces sub-optimal outputs?
			{
				cache[new_game_state] = search_move(new_position, temp);
			}
			std::vector<float> score = cache[new_game_state];
			delete new_position;

			if (score[cur_player] > best_score[cur_player])
			{
				best_score = score;
				next_best_move = move;
			}
		}

		best_move = next_best_move;
		return best_score;
	}

	int MinimaxAgent::get_move(Game *game)
	{
		if (input_format != game->get_input_format())
		{
			throw invalid_argument("Input format of game and agent do not match");
		}

		int move = -1;
		search_move(game, move);
		return move;
	}
}
