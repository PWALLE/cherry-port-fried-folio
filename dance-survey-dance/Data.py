###############################################################################
# Data.py
# 
# Data processing driver, for running statistics on saved survey data.
# Contains mainly functions for reading in saved data and running statistics.
# Also a function to spit data to csv.
#
# @author: Paul Elliott
# @date: 11/8/14
###############################################################################
import os
from Constants import *
from Survey import Survey

POSSIBLE_INT_VALUES = [1, 2, 3, 4, -1]

#Creates a container to hold survey results
#Returns: results dict 
#           { <survey question response> : <num responses> }
def GenerateResponseResults():
    results = {}
    for qnum in range( 0,NUM_QUESTIONS ):
        results[qnum] = { 1:0, 2:0, 3:0, 4:0, -1:0 }
    return results

#Prints results for each question
#Input: results dict (see above)
def PrintResults(results):
    for qnum,qres in results.items():
        out("Question %s results:" %( str( PynumToHnum(qnum) ) ))
        total = sum( qres.values() )
        for i in range(1,5):
            out("  %d: %d (%f percent)"
                    %( i, qres[i], (float( qres[i] )/float( total ))*100 ))
        out("    throwout: %d (%f percent)"
                %( qres[-1], (float( qres[-1] )/float( total ))*100 ))

#Calculates and returns results from surveys
#Input: surveys list
#           [ Survey ]
#Returns: results dict (see above)
def GetResultsFromSurveys(surveys):
    results = GenerateResponseResults()
    for survey in surveys:
        for question in survey.responses:
            qnum,qtype = ( question.GetNum(), question.GetType() )
            qdata = question.GetDataString()
            if qtype == ORDER: # right now, just want the top answer
                qdata = qdata.split(',')[0]
            results[question.GetNum()][int(qdata)] += 1
    return results

#Create and return surveys from a data directory
#Input: dir string
#           '<absolute path to data directory>'
#Input: surveys list
#           [ Survey ]
def GenerateSurveysFromDataDir(dir):
    surveys = []
    import os
    for fn in os.listdir(dir):
        if DFILE_EXT not in fn:
            continue
        survey = Survey( GetNumFromFN(fn) )
        survey.LoadFromFile( dir+fn )
        surveys.append(survey)
    return surveys

#Quickly prints results of surveys in data directory (no Survey creation)
#Input: dir string
#           '<absolute path to data directory>'
def QuickPrint(dir):
    # init results[24] for each question
    results = GenerateResponseResults()

    # load data[num entries][input]
    data = []
    for fn in os.listdir(dir):
        if DFILE_EXT not in fn:
            continue
        fdata = []
        with open(dir+fn, 'r') as dfile:
            for line in dfile:
                fdata.append( line.rstrip() )
        data.append(fdata)

    # for each file, increment results for each q
    for fdata in data:
        for qnum in range( 0,NUM_QUESTIONS ):
            qtype,val = fdata[qnum].split('.')
            if qtype == Q_MAP[ORDER]:
                val = val.split(',')[0] # right now, only want the top answer
            results[qnum][int(val)] += 1
    PrintResults(results) # finally, print results

#Prints a dirty calculation of the difference between 2 survey sets
#Input: diffs list
#           [ (<question number>,<dirty difference>) ]
def PrintDirtyDiffs(diffs):
    import statistics
    # make dirty quartiles using difference medians
    dmedian = statistics.median( [diff[1] for diff in diffs] )
    lower = [ diff[1] for diff in diffs if diff[1] < dmedian ]
    higher = [ diff[1] for diff in diffs if diff[1] not in lower ]
    low_median,high_median = (statistics.median(lower), statistics.median(higher))
    lowest = [ low for low in lower if low < low_median ]
    highest = [ high for high in higher if high > high_median ]

    out('Here is Pauls dirty difference calculations for each question:')
    for qnum, diff in diffs:
        out("  Question %s: %f percent" %(PynumToHnum(qnum), diff*100))
    out('These questions had very similar or very different answers')
    for qnum, diff in diffs:
        if diff in lowest:
            out("  Answers for Q.%s were very similar (%f percent)" 
                    %(PynumToHnum(qnum), diff*100))
        elif diff in highest:
            out("  Answers for Q.%s were very different (%f percent)" 
                    %(PynumToHnum(qnum), diff*100))

#Calculates a dirty difference between survey sets using Cartesian distance
#Input: (2) data_set tuples
#           ( <absolute path to data directory>, <survey list>, <results dict> )
def CalculateDirtyDiffsPerQuestion(data_set1, data_set2):
    import math
    dir_1,results_1 = data_set1[0],data_set1[2]
    dir_2,results_2 = data_set2[0],data_set2[2]

    out("Calculating difference between %s and %s" %( dir_1, dir_2 ))
    diffs = []
    for i in range( 0,NUM_QUESTIONS ):
        qres_1, qres_2 = ( results_1[i], results_2[i] )
        qr1_total = sum( [ qres_1[a] for a in POSSIBLE_INT_VALUES ] ) * 1.0
        qr2_total = sum( [ qres_2[a] for a in POSSIBLE_INT_VALUES ] ) * 1.0
        qsum = 0.0
        for a in POSSIBLE_INT_VALUES:
            aval_ratio = ( qres_1[a]/qr1_total, qres_2[a]/qr2_total )
            qsum += math.pow( aval_ratio[0] - aval_ratio[1], 2 )
        diffs.append( (i, math.sqrt(qsum)) )
    PrintDirtyDiffs(diffs)

# TODO this will calculate crossover between selected questions per survey
def CalculateQuestionCrossover(selected_questions, data_set1, data_set2):
    pass

#Creates a CSV file of a set of surveys
#Input: dir string
#           'absolute path to survey data dir'
#       surveys list
#           [ Surveys ]
def OutputCSV(dir, surveys):
    csv_fn = dir.split('/')[1] + '.csv'
    out("Creating %s from %s directory" %(csv_fn, dir))
    with open(csv_fn, 'w') as csv_file:
        for i in range(1, NUM_QUESTIONS):
            csv_file.write("Q%d," %i)
        csv_file.write("Q%s\n" %NUM_QUESTIONS)
        for s in surveys:
            csv_file.write(s.GetCSVOneLine() + "\n")

if __name__=="__main__":
    #QuickPrint('./data1/')
    #QuickPrint('./data2/')
    #QuickPrint('./data3/')

    ### What's the difference between survey set 1 and 2? 10/29/14 ###
    data_sets = []
    for i in range(1,3):
        dir = CUR_DIR + "data%d/"%(i)
        # QuickPrint(dir)
        surveys = GenerateSurveysFromDataDir(dir)
        OutputCSV(dir, surveys)
        data_sets.append( (dir, surveys, GetResultsFromSurveys(surveys)) )
    # for (dir, s, results) in data_sets:
        # out(dir)
        # PrintResults(results)
    # CalculateDiffsPerQuestion(data_sets[0], data_sets[1])
    CalculateQuestionCrossover([2,3], data_sets[0], data_sets[1])
