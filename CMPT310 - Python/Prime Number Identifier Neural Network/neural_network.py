import numpy as np
import pickle


class NeuralNet:
    def __init__(self):
        self._layers = []
        self.loss = None

    def build(self, *args, **kwargs):
        raise NotImplementedError

    def build_loss(self, *args, **kwargs):
        raise NotImplementedError

    def compute_activations(self, x, target):
        """
        Iterates over all the layers (self._layers) starting from the first 
        layer and computes the activations of all layers by passing the output 
        of one to the output of the next one. 
        """

        #Using outputs from previous layer, compute current layer's activations
        output = x
        for layer in self._layers:
            output = layer.compute_activation(output)
        
        #Obtain the loss from the L2LossLayer
        loss = self.loss.compute_activation(output, target)

        return output, loss

    def compute_gradients(self):
        """
        Computes the gradient of all weights with respect to the error. For 
        each layer L, stores the gradient in L._input_error_gradient, 
        where L._error_input_gradient is the gradient of loss with respect to 
        the input of layer L. Then passes that to the previous layer. In other 
        words, sets the previous layer's output gradient by the current layer's 
        input gradient.

        """

        #Compute gradient of loss
        self.loss.compute_gradient()  

        #Maintain previous error gradient value
        prev_gradient = self.loss.get_input_error_gradient()

        #Back propagate by iterating over layers backwards and passing error
        #gradients from the previous layer to the current layer
        for layer in reversed(self._layers):
            layer.set_output_error_gradient(prev_gradient)    #Set output grad
            layer.compute_gradient()                          #Compute gradient
            prev_gradient = layer.get_input_error_gradient()  #Update prev

    def update_weights(self, learning_rate):
        """
        Updates the weights given a specific update function (e.g. SGD).
        Iterates over all layers and update their weights.

        """
        for layer in self._layers: layer.update_weights(learning_rate)

    def save_weights(self, path):
        weights = []
        for layer in self._layers:
            weights.append(layer.get_weights())
        with open(path, "wb") as f:
            pickle.dump(weights, f)

    def load_weights(self, path):
        with open(path, "rb") as f:
            weights = pickle.load(f)
            for i, layer in enumerate(self._layers):
                layer.set_weights(weights[i])