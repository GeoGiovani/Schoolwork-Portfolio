import util



def depthFirstSearch(problem):
    """
    Q1.1
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print ( problem.getStartState() )
    You will get (5,5)

    print (problem.isGoalState(problem.getStartState()) )
    You will get True

    print ( problem.getSuccessors(problem.getStartState()) )
    You will get [((x1,y1),'South',1),((x2,y2),'West',1)]
    """
    "*** YOUR CODE HERE ***"

    explored = set()       #Stores a history of expanded states 
    parents = {}           #Dictionary of parent states
    fringe = util.Stack()  #Fringe will be a LIFO stack for DFS
    
    startState = (problem.getStartState(), [], 0)  #Format the start state
    fringe.push(startState)  
 
    #Perform search by expanding the deepest nodes of the search tree first
    while (fringe.isEmpty() == False):
      state = fringe.pop()
      if state[0] in explored: continue   #Skip previously expanded states
      else: explored.add(state[0])        #Add new states to expanded history

      if (problem.isGoalState(state[0])): #Goal check
        return makeList(parents, state)

      for successor in problem.getSuccessors(state[0]):  #Expand new successors
        if (successor[0] not in explored):
          parents[successor] = state  #Maintain dictionary of parent states
          fringe.push(successor)

    return []


def breadthFirstSearch(problem):
    """
    Q1.2
    Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"

    explored = set()       #Stores a history of expanded states 
    parents = {}           #Dictionary of parent states
    fringe = util.Queue()  #Fringe will be a FIFO Queue for BFS

    startState = (problem.getStartState(), [], 0)  #Format the start state
    explored.add(startState[0])
    fringe.push(startState)

    #Perform search by expanding nodes based on their level in the search tree
    while (fringe.isEmpty() == False):
      state = fringe.pop()
      if (problem.isGoalState(state[0])):  #Goal check
        return makeList(parents, state)
      
      for successor in problem.getSuccessors(state[0]):  #Expand new successors
        if (successor[0] not in explored):
          explored.add(successor[0])  #Add new states to expanded history
          parents[successor] = state  #Maintain dictionary of parent states
          fringe.push(successor) 
    
    return []
      

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0


def aStarSearch(problem, heuristic=nullHeuristic):
    """
    Q1.3
    Search the node that has the lowest combined cost and heuristic first."""
    """Call heuristic(s,problem) to get h(s) value."""
    "*** YOUR CODE HERE ***"
    
    explored = set()               #Stores a history of expanded states 
    parents = {}                   #Dictionary of parent states
    fringe = util.PriorityQueue()  #Fringe will be a Priority Queue for A*
    
    startState = (problem.getStartState(), [], 0)  #Format the start state
    fringe.push( (startState, 0), 0)               #Store cost of path in tuple

    #Perform search by expanding nodes based on the sum of their current 
    #path cost and estimated cost to the goal, as determined by the heuristic
    while (fringe.isEmpty() == False):
      state = fringe.pop()
      current = state[0]
      if current[0] in explored: continue   #Skip previously expanded states 
      else: explored.add(current[0])        #Add new states to expanded history
      
      if (problem.isGoalState(current[0])): #Goal check
        return makeList(parents, current)

      for successor in problem.getSuccessors(current[0]): #Expand new successors
        if (successor[0] not in explored):
          parents[successor] = current  #Maintain dictionary of parent states
          fringe.push( (successor, state[1] + successor[2]), 
          heuristic(successor[0], problem) + state[1] + successor[2]) 

    return []


#Utilizes the dictionary of parent states to reconstructs a path with actions
#to a goal state
def makeList(parents, goal):
  path = []
  while (goal[1] != [] and goal in parents):
    path.insert(0, goal[1])
    goal = parents[goal]

  return path

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch