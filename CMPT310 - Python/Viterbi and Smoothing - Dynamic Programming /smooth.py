import sys
import os
import random
import math
import numpy as np
import operator



# Outputs a random integer, according to a multinomial
# distribution specified by probs.
def rand_multinomial(probs):
    # Make sure probs sum to 1
    assert(abs(sum(probs) - 1.0) < 1e-5)
    rand = random.random()
    for index, prob in enumerate(probs):
        if rand < prob:
            return index
        else:
            rand -= prob
    return 0

# Outputs a random key, according to a (key,prob)
# iterator. For a probability dictionary
# d = {"A": 0.9, "C": 0.1}
# call using rand_multinomial_iter(d.items())
def rand_multinomial_iter(iterator):
    rand = random.random()
    for key, prob in iterator:
        if rand < prob:
            return key
        else:
            rand -= prob
    return 0


class HMM():

    def __init__(self):
        self.num_states = 2
        self.prior      = np.array([0.5, 0.5])
        self.transition = np.array([[0.999, 0.001], [0.01, 0.99]])
        self.emission   = np.array([{"A": 0.291, "T": 0.291, 
                                     "C": 0.209, "G": 0.209},
                                    {"A": 0.169, "T": 0.169, 
                                     "C": 0.331, "G": 0.331}])

    # Generates a sequence of states and characters from
    # the HMM model.
    # - length: Length of output sequence
    def sample(self, length):
        sequence = []
        states = []
        rand = random.random()
        cur_state = rand_multinomial(self.prior)
        for i in range(length):
            states.append(cur_state)
            char = rand_multinomial_iter(self.emission[cur_state].items())
            sequence.append(char)
            cur_state = rand_multinomial(self.transition[cur_state])
        return sequence, states

    # Generates a emission sequence given a sequence of states
    def generate_sequence(self, states):
        sequence = []
        for state in states:
            char = rand_multinomial_iter(self.emission[state].items())
            sequence.append(char)
        return sequence

    # Outputs the most likely sequence of states given an emission sequence
    # - sequence: String with characters [A,C,T,G]
    # return: list of state indices, e.g. [0,0,0,1,1,0,0,...]
    def viterbi(self, sequence):

        seqLen = len(sequence)
        vitMatrix = np.zeros([2, seqLen])  #Holds argmax values
        prevMatrix = np.zeros([2, seqLen]) #Previous vals for constructing path

        #Initialize first entries
        vitMatrix[0][0] = (math.log(0.5) +
        math.log(self.emission[0][sequence[0]]))
        vitMatrix[1][0] = (math.log(0.5) +
        math.log(self.emission[1][sequence[0]]))

        for i in range(1, seqLen):
            base = sequence[i]
            # 0 <=> A/T rich, 1 <=> G/C rich
            for currentState in [0, 1]:
                for prevState in [0, 1]:

                    # p(base | currentState)
                    pBaseGivState = self.emission[currentState][base]
                    # p(currentState | prevState)
                    pCurrGivState = self.transition[prevState][currentState]

                    #Calculate probability
                    if (vitMatrix[currentState][i] == 0):
                        vitMatrix[currentState][i] = (math.log(pBaseGivState) +
                        math.log(pCurrGivState) + vitMatrix[prevState][i-1])
                    else:
                        newProb = (math.log(pBaseGivState) +
                        math.log(pCurrGivState) + vitMatrix[prevState][i-1])
                        #Assign max value based on previous state
                        if (vitMatrix[currentState][i] < newProb):
                            vitMatrix[currentState][i] = newProb
                            prevMatrix[currentState][i] = 1

        #Return most likely sequence
        return self.reconstructPath(vitMatrix, prevMatrix, seqLen)

    #Returns most likely sequence given. A helper function of viterbi()
    def reconstructPath(self, vitMatrix, prevMatrix, seqLen):
        path = [0] * seqLen 
        j = seqLen - 1
        #Determine last value of most likely sequence
        if (vitMatrix[0][j] > vitMatrix[1][j]): path[j] = 0
        else:                                   path[j] = 1
        #Reconstruct most likely sequence
        for i in reversed(range(j)):
            if (path[i + 1] == 1): path[i] = int(prevMatrix[1][i + 1])
            else:                  path[i] = int(prevMatrix[0][i + 1])

        return path

    def log_sum(self, factors):
        if abs(min(factors)) > abs(max(factors)):
            a = min(factors)
        else:
            a = max(factors)

        total = 0
        for x in factors:
            total += math.exp(x - a)
        return a + math.log(total)

    # - sequence: String with characters [A,C,T,G]
    # return: posterior distribution. shape should be (len(sequence), 2)
    # Please use log_sum() in posterior computations.
    def posterior(self, sequence):

        seqLen = len(sequence)
        fwdMatrix = np.zeros([seqLen, 2])
        bwdMatrix = np.zeros([seqLen, 2])
        postMatrix = np.zeros([seqLen, 2])

        #Initialize first entries of fwdMatrix
        fwdMatrix[0][0] = self.log_sum([ math.log(self.prior[0]) + 
                    math.log(self.emission[0][sequence[0]]) + 
                    math.log(self.transition[0][0]), math.log(self.prior[0]) + 
                    math.log(self.emission[0][sequence[0]]) + 
                    math.log(self.transition[1][0]) ])

        fwdMatrix[0][1] = self.log_sum([ math.log(self.prior[1]) + 
                    math.log(self.emission[1][sequence[0]]) + 
                    math.log(self.transition[0][1]), math.log(self.prior[1]) + 
                    math.log(self.emission[1][sequence[0]]) + 
                    math.log(self.transition[1][1]) ])

        #Initialize last entries of bwdMatrix
        bwdMatrix[seqLen-1][0] = self.log_sum([math.log(self.transition[0][0]), 
                    math.log(self.transition[0][1]) ])
                    
        bwdMatrix[seqLen-1][1] = self.log_sum([math.log(self.transition[1][0]), 
                    math.log(self.transition[1][1]) ])

        #Calculate forward matrix
        for i in range(1, seqLen):
            base = sequence[i]
            for curr in [0, 1]:
                #Next probability is the sum of previous 2 states probabilities
                prev1 = (math.log(self.transition[0][curr]) + 
                    fwdMatrix[i-1][0] + math.log(self.emission[curr][base]))
                prev2 = (math.log(self.transition[1][curr]) + 
                    fwdMatrix[i-1][1] + math.log(self.emission[curr][base]))
                #Sum probabilities
                fwdMatrix[i][curr] = self.log_sum([prev1, prev2])

        #Calculate backward matrix
        for i in range(seqLen-2, -1, -1):
            base = sequence[i + 1]
            for curr in [0, 1]:
                #Next probability is the sum of next 2 states probabilities
                next1 = (math.log(self.transition[curr][0]) + 
                    bwdMatrix[i+1][0] + math.log(self.emission[0][base]))
                next2 = (math.log(self.transition[curr][1]) + 
                    bwdMatrix[i+1][1] + math.log(self.emission[1][base]))
                #Sum probabilities
                bwdMatrix[i][curr] = self.log_sum([next1, next2])

        #Multiple matrices and normalize to get posterior probabilities
        alpha = 1/self.log_sum(fwdMatrix[:, -1])
        for i in range(seqLen):
            for j in [0,1]:
                postMatrix[i][j] = fwdMatrix[i][j] * bwdMatrix[i][j] * alpha

        return postMatrix

    # Output the most likely state for each symbol in an emmision sequence
    # - sequence: posterior probabilities received from posterior()
    # return: list of state indices, e.g. [0,0,0,1,1,0,0,...]
    def posterior_decode(self, sequence):
        nSamples  = len(sequence)
        post = self.posterior(sequence)
        best_path = np.zeros(nSamples)
        for t in range(nSamples):
            best_path[t], _ = max(enumerate(post[t]), key=operator.itemgetter(1))
        return list(best_path.astype(int))