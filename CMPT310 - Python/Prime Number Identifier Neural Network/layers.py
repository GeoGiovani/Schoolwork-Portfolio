import numpy as np



class BaseLayer:
    def __init__(self):
        self._input_data = None
        self._input_error_gradient = None
        self._output_error_gradient = None
        self.weights = None

    def compute_activation(self, x):
        raise NotImplementedError

    def compute_gradient(self):
        raise NotImplementedError

    def update_weights(self, learning_rate):
        raise NotImplementedError

    def set_output_error_gradient(self, dError):
        self._output_error_gradient = dError

    def get_input_error_gradient(self):
        return self._input_error_gradient

    def get_input_data(self):
        return self._input_data

    def get_weights(self):
        return self.weights

    def set_weights(self, weights_dict):
        self.weights = weights_dict
        if self.weights is not None:
            for k, v in self.weights.items():
                setattr(self, k, v)


class DenseLayer(BaseLayer):
    """
    This layer implements the following function:
    o = x.w + b
    """
    def __init__(self, in_size, out_size, w_mean=0.0, w_std=1.0):
        """
        Creates a linear layer and initialzes the weights by sampling from a 
        normal distribution with mean of w_mean and standard deviation of w_std

        :param in_size:  number of input neurons
        :param out_size:  number of output neurons
        :param w_mean: mean
        :param w_std: standard deviation
        """
        super().__init__()
        # Create and initialize weights (w)
        # self.w is a (in_size x out_size) matrix
        self.w = np.random.normal(loc=w_mean, scale=w_std, size=(in_size, out_size))
        # Create derivative of the weights
        self.dw = None
        # Create and initialize biases (b)
        self.b = np.zeros((1, out_size), dtype=np.float32)
        # Create derivative of the biases
        self.db = None

        self.weights = {"w": self.w, "b": self.b}

    def compute_activation(self, x):
        """
        Compute the output of this layer and return it
        """
        self._input_data = x

        #Calculate the value of output neuron i: oi = (xi * wi) + bi
        output = np.add(np.dot(self._input_data, self.w), self.b) 

        return output

    def compute_gradient(self):
        """
        Computes the gradient of Error with respect to the parameters weights 
        (self.w) aand biases (self.b) of this layer and stores them in self.dw` 
        and self.db, respectively. Also computes the gradient of error with 
        respect to the input and stores it in self._input_error_gradient.

        The function definition is
            o = x.w + b
        Gradient of the error (E) function with respect to o is dE/do. By the 
        chain rule of derivative we have:
            - Derivative of error (E) with respect to the weights (w) is
                 dE/dw = dE/do . do/dx = dE/do . x
            - Derivative of error (E) with respect to the biases (b) is
                 dE/db = dE/do . do/db = dE/do . 1
            - Derivative of error (E) with respect to the input (x) is
                 dE/dx = dE/do . do/dx = dE/do . w

        The first two derivatives (dE/dw and dE/db) are computed and saved in 
        `self.dw` and `self.db, respectively, and the last one (dE/dx) is 
        stored in `self._input_error_gradient`
        """
        
        #Calculate dE/dw
        self.dw = np.dot(np.transpose(self._input_data), 
                                      self._output_error_gradient)

        #Calculate dE/db
        ones_row = np.ones((1, self._output_error_gradient.shape[0]))
        self.db = np.dot(ones_row, self._output_error_gradient)

        #Calculate dE/dx
        self._input_error_gradient = np.dot(self._output_error_gradient, 
                                            np.transpose(self.w))

    def update_weights(self, learning_rate):
        """
        Performs one step of Stochastic Gradient Descent(SGD).
        Updates the weights and biases (w and b) using the computed gradients 
        (dw and db) and the learning rate.
        """

        self.w = self.w - np.dot(learning_rate, self.dw)
        self.b = self.b - np.dot(learning_rate, self.db)

        self.weights = {"w": self.w, "b": self.b}


class SigmoidLayer(BaseLayer):
    """
    This Layer implements sigmoid function:
        f(x) = 1 / (1 + exp(-x))
    For numerical stability it's better to use the above definition when 
    x >= 0, otherwise the below definition:
        f(x) = exp(x) / (exp(x) + 1)
    """

    def __init__(self):
        super().__init__()

    @staticmethod
    def stable_sigmoid_func(x):
        out = np.where(x >= 0,
                       1. / (1. + np.exp(-x)),
                       np.exp(x) / (np.exp(x) + 1.))
        return out

    def compute_activation(self, x):
        """
        Computes sigmoid of x and returns it
        """
        self._input_data = x
        activation = SigmoidLayer.stable_sigmoid_func(self._input_data)
        return activation

    def compute_gradient(self):
        """
        Computes derivative of sigmoid function with respect to input and 
        stores it in `self._input_error_gradient`.

        The function definition is:
            o = sigmoid(x) =  1 / (1 + exp(-x))
        The derivative of function(o) with respect to the input (x) is:
            do/dx = sigmoid(x).(1 - sigmoid(x)) = o . (1 - o)

        Gradient of the error (E) function with respect to o is dE/do. By the 
        chain rule of derivative we have:
            - Derivative of error (E) with respect to the input (x) is
                 dE/dx = dE/do . do/dx = dE/do . o . (1 - o)
        """
        dE = self._output_error_gradient
        o = SigmoidLayer.stable_sigmoid_func(self._input_data)
        self._input_error_gradient = dE * o * (1 - o)

    def update_weights(self, learning_rate):
        pass


class L2LossLayer(BaseLayer):

    def __init__(self):
        super().__init__()
        self._target = None

    def compute_activation(self, predictions, target=None):
        self._input_data = predictions
        self._target = target.reshape(self._input_data.shape)
        loss = np.mean(np.power(self._input_data - target, 2))
        return loss

    def compute_gradient(self):
        self._input_error_gradient = (self._input_data - self._target)

    def update_weights(self, learning_rate):
        pass