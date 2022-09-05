#ifndef machine_learning_h
#define machine_learning_h

#include <iostream>
#include <memory>

namespace splashkit_lib
{
	matrix_2d to_categorical(const matrix_2d &m);

	enum ActivationFunction
	{
		ReLu, // Rectified Linear Unit
		Sigmoid,
		Softmax, // Softmax
	};

	struct _ActivationFunction
	{
		/**
		 * @brief The enum identifier for this activation function. 
		 * This is used to distinguish between different activation functions for saving/debugging purposes.
		 */
		static const ActivationFunction type;

		/**
		 * @brief Apply the activation function to the given input.
		 * 
		 * @param input The layer output to apply the activation function to.
		 * @return matrix_2d 
		 */
		virtual matrix_2d apply(const matrix_2d &input) { throw std::logic_error("not implemented"); };

		/**
		 * @brief The backward pass of the activation function.
		 * 
		 * @param input The input that was passed to the activation function.
		 * @return matrix_2d 
		 */
		virtual matrix_2d backward(const matrix_2d &input, const matrix_2d &output, const matrix_2d &delta) { throw std::logic_error("not implemented"); };
	};

	enum LossFunction
	{
		MSE, // Residual Sum of Squares
	};

	struct _LossFunction
	{
		/**
		 * @brief The enum identifier for this loss function.
		 * This is used to distinguish between different loss functions for saving/debugging purposes.
		 */
		static const LossFunction type;

		/**
		 * @brief Apply the loss function to the given output and target output.
		 * 
		 * @param output The output of the model.
		 * @param target_output The target output.
		 * @return double 
		 */
		virtual double loss(const matrix_2d &output, const matrix_2d &target_output) { throw std::logic_error("not implemented"); };

		/**
		 * @brief The backward pass of the loss function.
		 * Calculates dE/dO where E is the loss and O is the output.
		 * 
		 * @param output The output of the model.
		 * @param target_output The target output.
		 * @return matrix_2d 
		 */
		virtual matrix_2d backward(const matrix_2d &output, const matrix_2d &target_output) { throw std::logic_error("not implemented"); };
	};

	/**
	 * @brief Hidden layer for Machine Learning models
	 * 
	 */
	struct Layer
	{
		/**
		 * @brief The name of the layer, used for debugging.
		 */
		std::string name;
		
		/**
		 * @brief The expected shape to be inputted into this layer.
		 */
		size_t input_size;
		/**
		 * @brief The expected shape to be outputted from this layer.
		 */
		size_t output_size;

		/**
		 * @brief The activation function to use for this layer.
		 */
		std::shared_ptr<_ActivationFunction> activation_function;

		/**
		 * @brief The learning_rate to use for this layer. This value is initialised when the layer is added to a Model.
		 */
		double learning_rate;

		/**
		 * @brief Feed-forward function
		 * 
		 * @param input 
		 * @return matrix_2d 
		 */
		virtual matrix_2d forward(const matrix_2d &input) { throw std::logic_error("not implemented"); };
		/**
		 * @brief Back-propagation function
		 * 
		 * @param input The data that was inputted into the layer from the previous later
		 * @param before_activation The output of the layer before the activation function was applied
		 * @param output The data that was outputted into the next layer after activation function was applied
		 * @param delta The delta/derivative of the previous layer during back propagation.
		 * @return matrix_2d 
		 */
		virtual matrix_2d backward(const matrix_2d &input, const matrix_2d &before_activation, const matrix_2d &output, matrix_2d &delta) { throw std::logic_error("not implemented"); };

		/**
		 * @brief Updates the weights of the layer after back propagation.
		 * 
		 * @param input The data that was inputted into the layer from the previous later
		 * @param delta The delta calculated from back propagation.
		 */
		virtual void update_weights(const matrix_2d &input, const matrix_2d &delta) { throw std::logic_error("not implemented"); };
	};

	/**
	 * @brief Fully connected layer, slowest type, but can be used for any purpose.
	 * 
	 */
	class Dense : public Layer
	{
		matrix_2d weights;
		matrix_2d biases;
	public:
		Dense(size_t input_size, size_t output_size, ActivationFunction activation_function);
		matrix_2d get_weights() { return weights; };
		matrix_2d forward(const matrix_2d &input) override;
		matrix_2d backward(const matrix_2d &input, const matrix_2d &before_activation, const matrix_2d &output, matrix_2d &delta) override;
		void update_weights(const matrix_2d &input, const matrix_2d &delta) override;
	};

	/**
	 * @brief A Machine Learning model
	 * 
	 */
	class Model
	{
	private:
		std::shared_ptr<_LossFunction> error_function;

		double learning_rate;

		vector<std::shared_ptr<Layer>> layers;
	public:
		Model(LossFunction error_function, double learning_rate=0.01);

		/**
		 * @brief Add a layer to the back of the model
		 * 
		 * @param layer 
		 */
		void add_layer(Layer *layer);

		/**
		 * @brief Feed-forward the input through the model
		 * 
		 * Will produce a result for each row in the input matrix.
		 * 
		 * @param input The data to feed-forward through the model
		 * @return matrix_2d The model output
		 */
		matrix_2d predict(const matrix_2d &input);

		/**
		 * @brief Feed-forward the input through the model and store all the intermediate results for back propagation.
		 * 
		 * @param input The data to feed-forward through the model
		 * @param input_index The index indicating the start of this batch for input.
		 * @param batch_size The size of the batch to use for gradient averaging.
		 * @return vector<vector<matrix_2d>> The model output and all the intermediate results for each batch.
		 */
		vector<vector<matrix_2d>> forward(const matrix_2d &input, int input_index=0, int batch_size=1);

		/**
		 * @brief Back propagate the loss through the model and return the gradient of the loss with respect to the weights for each layer.
		 * 
		 * @param outputs The outputs from forward propagation
		 * @param target_output The target output of the model
		 * @param index The index indicating the start of this batch for target_output.
		 * @return vector<matrix_2d> The average delta gradients for each layer averaged across the batch.
		 */
		vector<matrix_2d> backward(vector<double> &losses, const vector<vector<matrix_2d>> &outputs, const matrix_2d &target_output, int index=0);

		/**
		 * @brief Update the weights of the model using the gradients from back propagation and the intermediate outputs from forward propagation.
		 * 
		 * @param avg_deltas The gradients from back propagation for the given batch.
		 * @param outputs The outputs from forward propagation for the given batch.
		 */
		void update_weights(const vector<matrix_2d> &avg_deltas, const vector<vector<matrix_2d>> &outputs);

		/**
		 * @brief Runs forward and backward propagation and updates the weights of the model.
		 * 
		 * @param input The input to the model
		 * @param target_output The target output of the model
		 * @param batch_size The size of the batch to use for gradient averaging.
		 * @return vector<double> The loss over time during training.
		 */
		vector<double> train(const matrix_2d &input, const matrix_2d &target_output, int batch_size=1);

		/**
		 * @brief Save a trained model to disk so that it can be loaded later using the model.load() function.
		 * 
		 * @param filename 
		 */
		void save(const string &filename);

		/**
		 * @brief Load a model that was saved to disk using the model.save() function.
		 * 
		 * @param filename 
		 */
		void load(const string &filename);
	};
}

#endif