###############################################################################
# Survey.py
# 
# Contains class Survey, which is a container for survey data
# and contains mainly function-specific get, set, and print methods.
# Internal class Question holds the actual data from survey responses.
#
# @author: Paul Elliott
# @date: 11/8/14
###############################################################################
from Constants import *

#Container and methods for a survey object
#   Contains subclass Question-- container for Survey question object
class Survey():

    #Question subclass, id'd by num:type (Constants.QUESTION_MAP)
    class Question():
        #Instantiate with num and type, empty = True and data nulled
        def __init__(self, n, t):
            self.num = n
            self.type = t
            self.empty = True
            if self.type == ORDER:
                self.data = []
            else:
                self.data = -1

        def Reset(self):
            self.empty = True
            if self.type == ORDER:
                self.data = []
            else:
                self.data = -1

        def GetNum(self):
            return self.num

        def GetType(self):
            return self.type

        def IsEmpty(self):
            return self.empty

        #Return data String (first answer of ORDER question)
        def GetTopAnswer(self):
            if self.type == ORDER:
                return self.GetDataString().split(',')[0]
            else:
                return str(self.data)

        #Return data as String (ORDER data is comma-delimited)
        def GetDataString(self):
            if self.type == ORDER:
                return ','.join([ str(self.data[i]) for i in range(0, ORDER_LEN) ])
            else:
                return str(self.data)

        #Parse user input into data int field
        #Sets empty to False
        #Input: user_input String
        #           '<command line user input>'
        def AddData(self, user_input):
            if user_input == NULL_VALUE:
                if self.type == ORDER: # null ORDER data is len(4) list of NULL_VALUE
                    self.data = [NULL_VALUE] * 4
                else:
                    self.data = NULL_VALUE
            elif self.type == ORDER: # user input is 4 digit string
                for i in range(0,ORDER_LEN):
                    self.data.append(int(user_input[i]))
            else:
                self.data = int(user_input)
            self.empty = False

        def PrintSelf(self):
            out('')
            out("  Q.%d (%s):" %(PynumToHnum(self.num),
                    Q_MAP[self.type]))
            if not self.empty:
                out("    [currently: %s]" 
                    %self.GetDataString())

        #Returns data String for datafile output (ORDER comma-delimited)
        #Output: o_string String
        #           '<question type>.<question data>'
        def GetOutputString(self):
           if self.type == ORDER: # comma-delimit
                data_str = ','.join([ str(self.data[i]) 
                    for i in range(0, ORDER_LEN) ])
           else:
                data_str = str(self.data)
           return '.'.join(( Q_MAP[self.type], data_str ))

    #Instantiate wth num and instantiate list of empty questions
    def __init__(self, num):
        self.number = num
        self.responses = []
        for i in range(0,NUM_QUESTIONS):
            self.responses.append(self.Question(i,QUESTION_MAP[i]))
   
    def PrintResponses(self):
        for i in range(0,NUM_QUESTIONS):
            out("Q %d (%s): %s" %(PynumToHnum(i), 
                    Q_MAP[self.responses[i].GetType()],
                    self.responses[i].GetDataString()))

    #Load survey data from an existing datafile
    #Input: fn String
    #           '<absolute path to input data file>'
    def LoadFromFile(self, fn):
        with open(fn, 'r') as datafile:
            for qnum, line in enumerate(datafile):
                qtype,val = line.rstrip().split('.')
                if qtype == Q_MAP[ORDER]:
                    val = val.split(',')
                try:
                    # hack to de_pad ORDER NULL_VALUE..will be padded on AddData
                    if val == ([NULL_VALUE] * 4):
                        val = NULL_VALUE
                    CheckUserInput(val, self.responses[qnum].GetType())
                    self.responses[qnum].AddData(val)
                except:
                    out('ERROR: Invalid data file format')
                    import traceback
                    traceback.print_exc()
                    sys.exit(1)

    #Generate a csv line of survey data
    #Output: csv_line String
    #           '<comma-separated question data, in order>'
    def GetCSVOneLine(self):
        csv_data = [ self.responses[i].GetTopAnswer() 
                for i in range(0, NUM_QUESTIONS) ]
        return ','.join(csv_data)

