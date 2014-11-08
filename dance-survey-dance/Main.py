###############################################################################
# Main.py
# 
# Driver program. Contains high-level methods and main program.
#
# This program allows a tired, underpaid and overworked graduate student
# to quickly store the results of a set of surveys into a file directory.
# See README.md for directions on how to actually use the program.
#
# @author: Paul Elliott
# @date: 11/8/14
###############################################################################
from Constants import *
import sys

### Utility ###

#Handles one instance of user input..modifies survey if valid, otherwise fails nicely
#Input: survey Survey
#           <the survey to modify>
#       qnum int
#           <the current question number being added/edited>
#Output: result String (in Constants.py)
#           'back'|'success'|'fail'
def HandleInput(survey, qnum):
    try:
        user_input = inp("")
        if user_input =='b':
            return BACK
        CheckUserInput(user_input, survey.responses[qnum].GetType())
        survey.responses[qnum].Reset()
        survey.responses[qnum].AddData(user_input)
        return SUCCESS
    except ValueError:
        out("  *needs to be all separate digits like '1234'..try again*")
        return FAIL
    except IndexError:
        out("  *needs to be 1 or 4 digit number..try again*")
        return FAIL
    except:
        out("ERROR: something weird happened")
        out("User input: %s" %user_input)
        import traceback
        traceback.print_exc()
        sys.exit(1)

### Primary ###

#Pulls the user's current progress from DATA_DIR (data directory, in Constants.py)
#Output: last int
#           <number of the last survey processed in current survey set>
def GetProgress():
    from os import listdir,mkdir
    try: # find data directory, or create it
        listdir(DATA_DIR)
    except:
        mkdir(DATA_DIR)
    finished_files = filter(lambda x: x.endswith(DFILE_EXT), listdir(DATA_DIR))
    finished = sorted([ GetNumFromFN(fn) for fn in finished_files ])

    if len(finished) == 0:
        return 0
    last = finished[-1]
    return last

#Program loop for inputing survey data from command line
#Writes to file in DATA_DIR
#Input: next int
#           <the number of the next survey to process>
def ProcessSurveys(next):
    PrintProcessingWelcome()
    quit = False
    while not quit:
        survey = ProcessOneSurvey(next)
        # review responses
        out("Printing responses:")
        survey.PrintResponses()
        # edit, if necessary
        user_cmd = inp("Do you need to edit? Type [Y/n] and hit 'enter':")
        if user_cmd != 'n': # lazily accept anything but 'n' for edit
            EditSurvey(survey)
            out("Finished editing.")
        # save
        out("Saving survey number %d" %survey.number)
        SaveSurveyToFile(survey)
        next += 1

        # prompt next survey
        user_cmd = inp("Next survey? Type [Y/n] and hit 'enter':")
        if user_cmd == 'n': # lazily accept anything but 'n' for continue
            quit = True #TODO nothing but y or n
    PrintProcessingGoodbye(next)

#Helper method for processing a single survey (get/validate input, store in Survey)
#Input: next int
#           (see above)
#Output: survey Survey
#           <Survey object>
def ProcessOneSurvey(next):
    import Test
    survey = Test.CreateTestSurvey(next) if DEBUG else Survey(next)
    next_question = NUM_QUESTIONS if DEBUG else 0 # the next empty question to fill
    qnum = 0 # the current question being added/edited by user

    # enter questions
    out("Please enter data from survey number (((%d)))" %next)
    devnull = inp("Hit 'enter' to begin")
    while (next_question < NUM_QUESTIONS):
        survey.responses[qnum].PrintSelf()
        result = HandleInput(survey, qnum)

        if result == BACK: # user error-handling..go back 1 question
            if (qnum - 1 < 0):
                out('  *cannot go back*')
            else:
                qnum -= 1
        elif result == SUCCESS: # move on/return to next empty question
            if qnum == next_question:
                next_question += 1
            qnum = next_question
        else: # errors handled..pass
            pass
    return survey

#Edit a filled out survey-- update answers from command line
#Input: survey Survey
#           <Survey object to edit>
def EditSurvey(survey):
    quit = False
    while not quit:
        user_input = inp("Type question number or 'q' to quit, hit 'enter':")
        if user_input == 'q':
            quit = True
        elif user_input not in [str(i) for i in range(1, NUM_QUESTIONS+1)]:
            out("  *Needs to be a number between 1-24, or 'q'*")
        else:
            qnum = HnumToPynum(int(user_input))
            result = FAIL
            # edit survey question
            while result != SUCCESS:
                survey.responses[qnum].PrintSelf()
                result = HandleInput(survey, qnum)
            # print updated survey
            out("Updated survey:")
            survey.PrintResponses()

#Saves survey data to a data file in current DATA_DIR
#Input: survey Survey
#           <the survey to save>
def SaveSurveyToFile(survey):
    with open("%s%d%s" %(DATA_DIR, survey.number, DFILE_EXT), 'w') as survey_file:
        for i in range(0, NUM_QUESTIONS):
            survey_file.write(survey.responses[i].GetOutputString() + "\n")

#Main program: 'Hit the button start the show'
if __name__ == "__main__":
    #TODO command line options and minimal ui
    from Survey import Survey
    PrintWelcome()
    # get next survey 
    next = GetProgress() + 1

    # process surveys
    ProcessSurveys(next)    

    # print goodbye
    PrintGoodbye()
