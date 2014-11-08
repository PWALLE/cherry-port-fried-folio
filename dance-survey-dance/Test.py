###############################################################################
# Test.py
#
# Just some unit test functions here....
#
# @author: Paul Elliott
# @date: 10/29/14
###############################################################################
import Constants
import Survey as S

#Creates a test survey with pre-filled answers
#Input: next int
#           <number of next survey to fill>
#Output: test_survey Survey
#           <pre-filled test Survey>
def CreateTestSurvey(next):
    answers = [ 1234,1,2,3,
            4,1,2,3,
            4,1,1234,1,
            2,3,4,1,
            2,3,4,1,
            2,3,4,1 ]
    test_survey = S.Survey(next)
    for i in range(0,24):
        test_survey.responses[i].AddData(str(answers[i]))
    return test_survey
