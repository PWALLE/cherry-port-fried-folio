# id3.py
#
# This script runs the id3 decision tree algorithm. 
# It takes 3 command line arguments:
#   training_file : the filename of an existing training file
#   test_file     : the filename of an existing test file
#   model_file    : a filename for the outputted model
# And should be called via:
#   python id3.py <training_file> <test_file> <model_file>
#
# @author: Paul Elliott
# @date: 1/28/13

import math,os,argparse,logging,sys

### Some Constants ###
log_format = '%(asctime)s - %(levelname)7s : %(message)s'
LEAF = 'Leaf'
NODE = 'Node'

###########################################################################
# Data Structures
###########################################################################

# A node in the decision tree data structure.
#   attr  : attribute to test
#   left  : pointer to the left subtree, where attribute = 0
#   right : pointer to the right subtree, where attribute = 1
class Node():
    def __init__(self, attr, left_tree, right_tree):
        self.attr = attr
        self.left = left_tree
        self.right = right_tree

    def getType(self):
        return NODE

# A leaf in the decision tree data structure.
#   class_val : the class value (Y) of the particular attribute path to that leaf
class Leaf():
    def __init__(self, class_val):
        self.class_val = class_val
    
    def getType(self):
        return LEAF


###########################################################################
# Utility Methods
###########################################################################

#Parses a dataset csv file into a list of tuple entries.
# Input:
#   dataset_filename : the name of the csv file to parse
# Output:
#   data_set : a list of tuple entries, in the form
#       [ ({ 'attr1':0|1, 'attr2':0|1, ... },class_val) ]
def parseDataFile(dataset_filename):
    logging.debug("Parsing: {}".format(dataset_filename))
    data_set = []
    with open(dataset_filename) as data_file:
        attributes = data_file.readline().split(',')[:-1]
        for line in data_file:
            vals = line.split(',')
            attrs, class_val = map(int, vals[:-1]), int(vals[-1].strip())
            data_set.append((dict(zip(attributes,attrs)), class_val))
    return data_set

#Creates the model file from the decision tree
# Input:
#   tree : root node of the decision tree data structure
#   model_file : filename of eventual model file
def createModelFile(tree, model_file):
    logging.debug('Creating model file')
    model_file = open(model_file,'w')
    printModel(tree, 0, model_file)
    model_file.close()

    #Remove spurious newline at beginning of model file
    with open(model_file.name,'r') as model_file:
        lines = model_file.readlines()
    with open(model_file.name,'w') as model_file:
        for line in lines[1:]:
        	logging.debug(line)
        	model_file.write(line)
        

#Recursively print the decision tree to the model file
# Input:
#   tree_element : a node/leaf in the decision tree
#   depth : current recursive depth
#   model_file : file object we're printing to
def printModel(tree_element, depth, model_file):
    if tree_element.getType() == LEAF:
        model_file.write(tree_element.class_val)
    else:
        for i in range(0,2):
            model_file.write("\n")
            model_file.write('| ' * depth)
            model_file.write("%s = %d : " %(tree_element.attr, i))
            if i == 0: 
                printModel(tree_element.left,depth+1,model_file)
            else: 
                printModel(tree_element.right,depth+1,model_file)

#Check accuracy of model (tree) against test set
# Input:
#   tree : the root node of the decision tree
#   test_set : list of entries in the form
#       [ ({ 'attr1':0|1, 'attr2':0|1, ... },class_val) ]
def checkAccuracy(tree, test_set):
    logging.debug('Checking accuracy of model on test data')
    total_entries = float(len(test_set))
    num_correct = 0
    for attributes,class_val in test_set:
        test_tree = tree
        #Recurse until hit leaf, check leaf label against actual label
        while test_tree.getType() != LEAF:
            if attributes[test_tree.attr] == 0:
                test_tree = test_tree.left
            else:
                test_tree = test_tree.right
        if test_tree.class_val == str(class_val):
            num_correct += 1
    return num_correct/total_entries

def countTreeElementsAndLeafs(tree):
    element = 0
    leaf = 0
    element = recurseAndCountElement(tree)
    leaf = recurseAndCountLeaf(tree)
    logging.debug("Number elements: {}, number leaves: {}".format(element,leaf))

def recurseAndCountLeaf(tree_element):
    if tree_element.getType() == LEAF : 
        return 1
    return recurseAndCountElement(tree_element.left) + recurseAndCountElement(tree_element.right)

def recurseAndCountElement(tree_element):
    if tree_element.getType() == LEAF: 
        return 1
    return 1 + recurseAndCountElement(tree_element.left) + recurseAndCountElement(tree_element.right)

###########################################################################
# id3 Algorithm Methods
###########################################################################

#Recursively grows the decision tree by splitting on best attributes
# Input:
#   training_set : list of entries in the form
#       [ ({ 'attr1':0|1, 'attr2':0|1, ... },class_val) ]
# Output:
#   (the root Node of the resulting decision tree)
def growTree(training_set):
    logging.debug('Running id3 algorithm')
    attributes = generateAttributes(training_set)
    best_attr = chooseBestAttribute(attributes)
    if best_attr == 'LEAF' or chiSquared(attributes[best_attr]) < 6.635: 
        #Bottomed out (max info gain = 0 or chi^2 val < threshold), 
        #return Leaf of most common class val
        num_zeros = len([class_val 
                         for attributes,class_val in training_set 
                         if class_val==0])
        num_ones = len([class_val 
                        for attributes,class_val in training_set 
                        if class_val==1])
        if num_zeros >= num_ones: return Leaf('0')
        else: return Leaf('1')
    else:
        #Split training_set on best_attr and recurse on both halves
        training_set0, training_set1 = splitTrainingSet(best_attr,
                                                        training_set)
        return Node(best_attr,
                    growTree(training_set0),growTree(training_set1))

#Parses training_set entries into a dict of attributes
# Input:
#   training_set : a list of tuple entries in the form
#       [ ({ 'attr1':0|1, 'attr2':0|1, ... },class_val) ]
# Output:
#   attributes : a dict of attributes in the form 
#       { 'attr_id' : { 0 : {0 : |attr=0,c_v=0|, 1 : |attr=0,c_v=1|},
#                       1 : {0 : |attr=1,c_v=0|, 1 : |attr=1,c_v=1|} }
#       }
def generateAttributes(training_set):
    attributes = {}
    for attrs,class_val in training_set:
        for attr_id,attr_val in attrs.items():
            if attr_id not in attributes.keys():
                #Init all values = 0
                attributes[attr_id] = { 0: {0:0, 1:0}, 
                                        1: {0:0, 1:0} }
            #Increment
            attributes[attr_id][attr_val][class_val] += 1
    return attributes

#Selects best attribute based on information gain calculations
# Input:
#   attributes : a dict of attributes in the form 
#       { 'attr_id' : { 0 : {0 : |attr=0,c_v=0|, 1 : |attr=0,c_v=1|},
#                       1 : {0 : |attr=1,c_v=0|, 1 : |attr=1,c_v=1|} }
#       }
# Output:
#   (the attribute with the maximum information gain,
#    or 'LEAF' if max info gain = 0)
def chooseBestAttribute(attributes):
    I_vals = {}
    for attr,totals in attributes.items():
        #Total entries
        Y_total = float(totals[0][0] + totals[0][1] + 
                        totals[1][0] + totals[1][1])
        #Totals where attr=0, attr=1
        attr0_total = float(totals[0][0] + totals[0][1])
        attr1_total = float(totals[1][0] + totals[1][1])
        #Totals where Y=0,Y=1
        Y_zeros = float(totals[0][0] + totals[1][0])
        Y_ones = float(totals[0][1] + totals[1][1])
        #Probabilities of attr=0,attr=1
        P_attr0 = float(attr0_total/Y_total)
        P_attr1 = float(attr1_total/Y_total)
        #H(Y) = entropy of entire set of entries
        Y_entropy = calculateEntropy(Y_total,Y_zeros,Y_ones)
        #H(Y|attr=0)
        Y_attr0_entropy = calculateEntropy(attr0_total,
                                           totals[0][0],totals[0][1])
        #H(Y|attr=1)
        Y_attr1_entropy = calculateEntropy(attr1_total,
                                           totals[1][0],totals[1][1])
        #Calculate I(Y=class; X=attr) = H(Y) - 
        #                               sum_{X=0|1} [ P(X=x) * H(Y|X=x) ] 
        I_vals[attr] = Y_entropy - ((P_attr0 * Y_attr0_entropy) + 
                                    (P_attr1 * Y_attr1_entropy))
    #In the case where all information gain is 0, we've hit a leaf
    if max(I_vals.values()) == 0.0:
        return 'LEAF'
    #Otherwise, return attribute with highest information gain
    return max([(val,key) for key,val in I_vals.items()])[1]

#Calculates entropy
# Input:
#   total : total values (n+p)
#   zeros : total zero values (n)
#   ones : total one values (p)
# Output:
#   (entropy = H(var) = sum_{v=0|1} [ -1 * (P(var=v) * lg P(var=v)) ])
def calculateEntropy(total, zeros, ones):
    if total == 0.0:
        return 0.0
    #Probability of zero,one values
    P_zero = zeros/total
    P_one = ones/total
    return (-1 * (P_zero * lg(P_zero))) + (-1 * (P_one * lg(P_one)))

#Calculates lg(val) = log(val)/log(2)
def lg(val):
    #doesn't matter if this isn't correct..multing by zero
    if val == 0: return 0 
    else: return math.log(val) / math.log(2)

#Calculates the chi^2 value of splitting on best_attr
# Input:
#   ba_vals : dict of the form
#       { 0 : {0 : |a=0,c_v=0|, 1 : |a=0,c_v=1|}, 
#         1 : {0 : |a=0,c_v=0|, 1 : |a=0,c_v=1|} }
# Output:
#   (chi^2(best_attr) = sum_{i=0|1} ((pi-p'i)^2/p'i)+((ni-n'i)^2/n'i) )
def chiSquared(ba_vals):
    p0 = float(ba_vals[0][1])
    p1 = float(ba_vals[1][1])
    n0 = float(ba_vals[0][0])
    n1 = float(ba_vals[1][0])
    p = p0 + p1
    n = n0 + n1
    p_prime0 = p * ((p0 + n0)/(p + n))
    n_prime0 = n * ((p0 + n0)/(p + n))
    p_prime1 = p * ((p1 + n1)/(p + n))
    n_prime1 = n * ((p1 + n1)/(p + n))
    #chi^2(best_attr=a) = (((p0-p'0)^2/p'0) + ((n0-n'0)^2/n'0)) + 
    #                     (((p1-p'1)^2/p'1) + ((n1-n'1)^2/n'1))
    return (((pow((p0-p_prime0),2)/p_prime0) + 
             (pow((n0-n_prime0),2)/n_prime0)) +
            ((pow((p1-p_prime1),2)/p_prime1) + 
             (pow((n1-n_prime1),2)/n_prime1)))

#Splits the training_set on the attribute split_attr
# Input:
#   split_attr : attribute to split on
#   training_set : list of entries in the form
#       [ ({ 'attr1':0|1, 'attr2':0|1, ... },class_val) ]
# Output:
#   training_set0 : list of entries in the above form, with split_attr=0
#   training_set1 : list of entries in the above form, with split_attr=1
def splitTrainingSet(split_attr,training_set):
    training_set0, training_set1 = [],[]
    for attributes,class_val in training_set:
        if attributes[split_attr] == 0: 
            training_set0.append((attributes,class_val))
        else:
            training_set1.append((attributes,class_val))
    return training_set0, training_set1

###########################################################################
# Main Method
###########################################################################

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Run ID3 decision tree algorithm')
    
    parser.add_argument('training_file', help='csv file of training data')
    parser.add_argument('test_file', help='csv file of test data')
    parser.add_argument('model_file', help='output file for decision tree model')
    parser.add_argument('--verbose','-v', action='store_true', default=False, 
                        help='print verbose output to stdout')
    parser.add_argument('--log_file','-l', 
                        help='prints output to log_file, default: stdout')
    
    args = parser.parse_args()
    if args.verbose:
        log_level = logging.DEBUG
    else:
        log_level = logging.INFO
    if args.log_file:
        logging.basicConfig(format=log_format, filename=args.log_file,
                            level=log_level)
    else:
        logging.basicConfig(format=log_format, level=log_level)
        
    #Parse the training file into a set of entries
    training_set = parseDataFile(args.training_file)

    #Build the decision tree using the training set
    tree = growTree(training_set)

    #Create model file
    createModelFile(tree, args.model_file)

    #Parse the test file into a set of entries
    test_set = parseDataFile(args.test_file)

    accuracy = checkAccuracy(tree, test_set)
    logging.info("Accuracy was {} on dataset: {}".format(accuracy, args.test_file))
    if args.verbose:
        countTreeElementsAndLeafs(tree)
