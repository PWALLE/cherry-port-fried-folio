###############################################################################
# Constants.py
#
# Constants for the entire program. Mainly toggles for survey type
# and utility functions.
#
# @author: Paul Elliott
# @date: 10/29/14
###############################################################################
import sys

# CONSTANTS #
DEBUG = False

CUR_DIR = './'
DATA_DIR = CUR_DIR + 'junk/'
DFILE_EXT = '.dat'

ORDER = 0
ORDER_LEN = 4
SELECT = 1

Q_MAP = { # for printing purposes
        0 : 'ORDER',
        1 : 'SELECT'
        }

NUM_QUESTIONS = 24

QUESTION_MAP = {
        0 : ORDER, 1 : SELECT, 2 : SELECT, 3 : SELECT,
        4 : SELECT, 5 : SELECT, 6 : SELECT, 7 : SELECT,
        8 : SELECT, 9 : SELECT, 10 : ORDER, 11 : SELECT,
        12 : SELECT, 13 : SELECT, 14 : SELECT, 15 : SELECT,
        16 : SELECT, 17 : SELECT, 18 : SELECT, 19 : SELECT,
        20 : SELECT, 21 : SELECT, 22 : SELECT, 23 : SELECT
        } # each survey, 24 questions of above types

POSSIBLE_VALUES = ['1','2','3','4']
NULL_VALUE = '-1'

BACK = 'go back'
SUCCESS = 'success'
FAIL = 'fail'

# Utility Functions #

#Checks if user input is valid, raises errors otherwise
#Input: user_input string
#           '<command line input>'
#       qtype <ORDER|SELECT>
def CheckUserInput(user_input, qtype):
    if user_input == NULL_VALUE:
        return
    if qtype == ORDER:
        if len(user_input) != 4:
            raise ValueError 
        if set(user_input) != set(POSSIBLE_VALUES):
            raise ValueError
    else:
        if len(user_input) != 1:
            raise IndexError
        if user_input not in POSSIBLE_VALUES:
            raise ValueError

#(2) Converts between Python list index nums [0..len-1]
#and human survey question nums [1..len]
#Input: pynum int | hnum int
#Output: pynum int | hnum int
def PynumToHnum(pynum):
    return pynum + 1
def HnumToPynum(hnum):
    return hnum - 1

#Pulls a survey number from filename
#Input: fn string
#Output: survey_number int
def GetNumFromFN(fn):
    return int(fn.rstrip(DFILE_EXT))

### PRINT FUNCTIONS ###
def PrintWelcome():
    out('')
    out('##################################################')
    out('###           Welcome, Tina                      #')
    out('##################################################')
    x = inp()
    out('')

def PrintProcessingWelcome():
    out('#      Now processing surveys                    #')
    x = inp()
    out('#                                                #')
    out('#      Enter survey data like these examples:    #')
    out('#        Q.1 (ORDER):                            #')
    out('#      1234<enter>                               #')
    out('#        Q.2 (SELECT):                           #')
    out('#      3<enter>                                  #')
    x = inp()
    out('#                                                #')
    out("#      To throw out data, type '-1', hit 'enter' #")
    out('#        Q.1 (ORDER):                            #')
    out('#      -1<enter>                                 #')
    out('#        Q.2 (SELECT):                           #')
    out('#      -1<enter>                                 #')
    x = inp()
    out('#                                                #')
    out("#      To go back 1 question, type 'b':          #")
    out('#        Q.2 (SELECT):                           #')
    out('#      b<enter>                                  #')
    out('#        Q.1 (ORDER):                            #')
    out('#          [currently: 1234]                     #')
    out('#                                                #')
    x = inp()
    out('')

def PrintProcessingGoodbye(next):
    out("###    Finished processing surveys up to %03d    #" %(next-1))
    x = inp()
    out('')

def PrintGoodbye():
    out('')
    out('##################################################')
    out('###           Goodbye, Tina                      #')
    out('##################################################')
    devnull = inp()
    out('')

def out(msg):
    print(msg)

def inp(msg="Hit 'enter' to continue"):
    try:
        return input(msg)
    except SyntaxError:
        return None
