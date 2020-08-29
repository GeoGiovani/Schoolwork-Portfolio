import sys, getopt
import copy
import random
import time
import numpy as np



# Finds a satisfying assignment to a SAT instance,
# using the DPLL algorithm.
# Input: a SAT instance and verbosity flag
# Output: print "UNSAT" or
#    "SAT"
#    list of true literals (if verbosity == True)
def solve_dpll(instance, verbosity):

    clauses = instance.clauses
    variables = instance.VARS
    model = DPLL(clauses, variables, set())  #Run DPLL

    if(model):
        print("SAT")
        if(verbosity):
            literals = [v for v in model if v > 0]
            print(literals)
    else:
        print("UNSAT")


def DPLL(clauses, symbols, model):

#==============================================================================
# Check Model
#==============================================================================

    #Check model for satisfiability
    result = modelCheck(clauses, model)
    if(result == "SAT"): return True
    elif(result == "UNSAT"): return False
    else: splitLiteral = result #Randomly unsatisfied literal used to split

#==============================================================================
# Perform Pure Literal Elimination
#==============================================================================

    #Find pure literals and add them as unit clauses
    pureLiterals = findPureLiterals(clauses, model)
    if(pureLiterals):
        for literal in pureLiterals:
            clause = []
            clause.append(literal)
            clauses.append(clause)  
        
#==============================================================================
# Perform Unit Propagation
#==============================================================================

    # Finds all unit literals and adds them to model
    unitLiterals = findUnitLiterals(clauses, model)
    if (unitLiterals == "UNSAT"): return False
    elif (unitLiterals): 
        model = model.union(unitLiterals) #Add unit literals to model
        for literal in unitLiterals:      #Remove unit literals from symbols
            if literal == splitLiteral: splitLiteral = None
            if abs(literal) in symbols: symbols.remove(abs(literal))

        propClauses = unitPropagation(clauses, unitLiterals)  #Propagate
        if (propClauses == "UNSAT"): return False             #Check if UNSAT
        ret = DPLL(propClauses, symbols, model)   #Split
        if (ret): return model
        else: return False

#==============================================================================
# Randomly Split
#==============================================================================

    #Split using selected literal if exists
    if not splitLiteral:
        #Else split using a randomly determined rare literal
        splitLiteral = findRareLiteral(clauses)
    if abs(splitLiteral) in symbols:      
        symbols.remove(abs(splitLiteral))             #Remove from symbols

    for value in [splitLiteral, 0 - splitLiteral]:    
        model.add(value)                              #Add splitLit to model
        trimmedClauses = trimClauses(clauses, value)  #Propagate
        if (trimmedClauses == "UNSAT"):               #Clauses became UNSAT
            model.remove(value)
            continue

        ret = DPLL(trimmedClauses, symbols, model)   #Recurse
        if not ret: model.remove(value)              #Model is UNSAT                               
        elif (ret == True): return model             #Model is SAT
        else: return ret                    #Model became SAT during unit prop
        
    return False


#Finds and returns a set of all pure literals, if present
def findPureLiterals(clauses, model):

    #Extract all literals
    literals = set()
    for clause in clauses:
        for literal in clause:
            literals.add(literal)

    #Extract and return pure literals
    pureLiterals = set()
    for literal in literals:
        if (0 - literal) in literals: continue
        else: pureLiterals.add(literal)
    return pureLiterals


#Returns a set containing unit literals
def findUnitLiterals(clauses, model):

    unitLiterals = set()
    for clause in clauses:
        if (len(clause) == 1):
            #If literal and -literal both occur, problem is UNSAT
            if (0 - clause[0]) in unitLiterals or (0 - clause[0]) in model:
                return "UNSAT"
            unitLiterals.add(clause[0])
    return(unitLiterals)


#Performs unit propagation using unit literal(s) and returns resulting clauses
def unitPropagation(clauses, unitLiterals):
    
    propagatedClauses = []
    for clause in clauses: 
        #Remove singleton clauses
        if (len(clause) <= 1): continue  

        newClause = []
        for literal in clause:
            #Remove satisfied clauses
            if literal in unitLiterals:  
                clause = []
                break
            #Remove negated unit literals from clauses
            if (0 - literal) in unitLiterals: continue
            else: newClause.append(literal)
        if len(clause): 
            if len(newClause): propagatedClauses.append(newClause)
            else: return "UNSAT"
    return propagatedClauses


#Removes clauses that become True from newly assigned literal (value)  
def trimClauses(clauses, value):

    trimmedClauses = []
    for clause in clauses:
        #Remove negated unit literals from clauses
        newClause = []
        satisfied = False
        for literal in clause:
            if literal == value: 
                satisfied = True
                break
            elif (0 - literal) == value: continue
            else: newClause.append(literal)
        if len(newClause) == 0 and not satisfied: return "UNSAT"
        if not satisfied: trimmedClauses.append(newClause)
    return trimmedClauses


#Checks model for satisfiability
def modelCheck(clauses, model):

    for clause in clauses:
        satisfied = len(clause)
        #Check if literals in model
        for literal in clause:
            if literal in model:
                satisfied = -1
                break
            elif (0 - literal) in model: satisfied -= 1
        #Examine cases
        if (satisfied == -1): continue
        elif (satisfied == 0): return "UNSAT"   
        elif (satisfied != 0): return clause[0]   
    return "SAT"


#Finds and returns a literal that occurs rarely in clauses  
def findRareLiteral(clauses):

    #Dictionary keeps track of literals (keys) and number of occurrences (vals)
    literals = dict()
    for clause in clauses:
        for literal in clause:
            if literal in literals.keys(): 
                literals[literal] += 1
                count = literals[literal]
            else: literals[literal] = 0

    #Find the smallest number of occurrences
    for literal in literals:
        if literals[literal] < count: 
            count = literals[literal]

    #Randomly select a literal occurring less than lowest count * 1.5 times
    tiedLiterals = set()
    for literal in literals:
        if (literals[literal] <= count * 1.5):
            tiedLiterals.add(literal)

    return tiedLiterals.pop()  #Here .pop is random because set is unordered