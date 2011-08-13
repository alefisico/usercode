#! /usr/bin/env python

#############################################
## Imports
############################################

'''
   Usage: %prog <file>
'''

import os, glob
import re
import os.path
import subprocess 
import sys, getopt
import tempfile
from ROOT import *
import cStringIO 

gROOT.SetStyle("Plain")
gStyle.SetOptStat(1111111)
gStyle.SetHistFillColor(kBlue)

###############################################################
def usage():
###############################################################

#    if False:
    print "This is the usage function"

    print '\n'
    print 'Usage: '+sys.argv[0]+' <file1> '
    print 'e.g.:  '+sys.argv[0]+' outputTiming1.root \n'


###############################################################
def maininfo(infile, outfile):
##############################################################
        texpreamble = ['\documentclass[10pt,a5paper,landscape]{report}\n',
                        '\usepackage{graphicx}\n',
                        '\usepackage[a5paper,vmargin={5mm,2mm},hmargin={5mm,5mm}]{geometry}\n',
                        '\usepackage[linktocpage]{hyperref}\n',
                        '\hypersetup{backref, colorlinks=true}\n',
                        '\\title{ \\textbf{\Huge{HLT Timing Summary}} \\footnote{\large{Documentation at \url{https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHLTTimingSummary}}} \\\\ Main Info }\n',
                        '\\author{\Large{CMS Experiment}}\n',
                        '\date{\\today}\n',
                        '\\begin{document}\n',
                        '\maketitle\n',
                        '\\newpage\n',
                        '\centering \\section*{Total time per event} \includegraphics[scale=0.45]{totalTimeEventtemp.png}\n',
                        '\\newpage \section*{Total Time for Event per accepted event} \includegraphics[scale=0.45]{acceptedTotalTimeEventtemp.png}\n',
                        '\\newpage \section*{Total Time for Event per rejected event} \includegraphics[scale=0.45]{rejectedTotalTimeEventtemp.png}\n',
                        '\\newpage \section*{Total CPU Time for Event} \includegraphics[scale=0.45]{totalCPUTimeEventtemp.png}\n',
                        '\\newpage \section*{Total CPU Time for Event per accepted event} \includegraphics[scale=0.45]{acceptedTotalCPUTimeEventtemp.png}\n',
                        '\\newpage \section*{Total CPU Time for Event per rejected event} \includegraphics[scale=0.45]{rejectedTotalCPUTimeEventtemp.png}\n',
                        '\\newpage \section*{Total time for all modules per event} \includegraphics[scale=0.45]{totalTimetemp.png}\n',
                        '\\newpage \section*{Total Time for all modules per accepted event} \includegraphics[scale=0.45]{acceptedTotalTimetemp.png}\n',
                        '\\newpage \section*{Total Time for all modules per rejected event} \includegraphics[scale=0.45]{rejectedTotalTimetemp.png}\n'
			'\end{document}']

        texfile = open(outfile+'.tex', 'w')
        texfile.writelines(texpreamble)
        texfile.close()
	get_plot1(infile,'totalTimeEvent')
	get_plot1(infile,'acceptedTotalTimeEvent')
	get_plot1(infile,'rejectedTotalTimeEvent')
	get_plot1(infile,'totalCPUTimeEvent')
	get_plot1(infile,'acceptedTotalCPUTimeEvent')
	get_plot1(infile,'rejectedTotalCPUTimeEvent')
	get_plot1(infile,'totalTime')
	get_plot1(infile,'acceptedTotalTime')
	get_plot1(infile,'rejectedTotalTime')



#################################################
def texfile(rootfile, fname):
	''' Function to create a .tex file'''
################################################

	texpreamble = ['\documentclass[10pt,a5paper,landscape]{book}\n',
			'\usepackage{graphicx}\n',
			'\usepackage[a5paper,vmargin={5mm,2mm},hmargin={5mm,5mm}]{geometry}\n',
			'\usepackage[linktocpage]{hyperref}\n',
			'\hypersetup{backref, colorlinks=true}\n',
			'\\title{ \\textbf{\Huge{HLT Timing Summary}} \\footnote{\large{Documentation at \url{https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHLTTimingSummary}}} \\\\ Main Info }\n',
			'\\author{\Large{CMS Experiment}}\n',
			'\date{\\today}\n',
			'\\begin{document}\n',
			'\maketitle\n',
			'\\newpage\n',
			'\\tableofcontents\n',
			'\\newpage\n',
			'\\chapter{Total time per event}\n',
			'\\newpage \centering {Total Time for Event} \includegraphics[scale=0.45]{totalTimeEventtemp.png}\n',
			'\\newpage \section{Total Time for Event per accepted event} \centering \includegraphics[scale=0.45]{acceptedTotalTimeEventtemp.png}\n',
			'\\newpage \section{Total Time for Event per rejected event} \centering \includegraphics[scale=0.45]{rejectedTotalTimeEventtemp.png}\n',
			'\\newpage \section{Total CPU Time for Event} \centering \includegraphics[scale=0.45]{totalCPUTimeEventtemp.png}\n',
			'\\newpage \section{Total CPU Time for Event per accepted event} \centering \includegraphics[scale=0.45]{acceptedTotalCPUTimeEventtemp.png}\n',
			'\\newpage \section{Total CPU Time for Event per rejected event} \centering \includegraphics[scale=0.45]{rejectedTotalCPUTimeEventtemp.png}\n',
			'\\newpage \section{Total time for all modules per event} \centering \includegraphics[scale=0.45]{totalTimetemp.png}\n',
			'\\newpage \section{Total Time for all modules per accepted event} \centering \includegraphics[scale=0.45]{acceptedTotalTimetemp.png}\n',
			'\\newpage \section{Total Time for all modules per rejected event} \centering \includegraphics[scale=0.45]{rejectedTotalTimetemp.png}\n',
			'\\chapter{Average module time}\n',
			'\\newpage \centering \includegraphics[scale=0.45]{moduleTimeSummarytemp.png}\n',
			'\\chapter{Average module running time}\n',
			'\\newpage \centering \includegraphics[scale=0.45]{moduleScaledTimeSummarytemp.png}\n']
#pathSuccessFraction
#uniquePathSuccessFraction
#pathRejection
#pathRejectAll

	texfile = open(fname+'.tex', 'w')
	texfile.writelines(texpreamble)

	names = {}
	file = TFile(rootfile,'read')
	for k in file.GetListOfKeys():
		h = k.ReadObj() 
		allnames = h.GetName()
		mean = 1
		ishist = allnames.split("_").count("moduleInPathScaledTime")
		if ishist > 0:
			fullname = allnames.split("_")
			fullname.remove("moduleInPathScaledTime")
			pathname = ""
			for j in range(len(fullname)-1):
				pathname = pathname + fullname[j]
				if j is not (len(fullname)-2):
					pathname = pathname + "_"
			modname = fullname[len(fullname)-1]
			if not pathname in names:
				names[pathname] = {} 
			names[pathname][modname] = mean

	texfile.write('\\chapter{Average module (in path) time}\n')
	for path in names:
		#texfile.write('\\newpage \section{Average module time for path '+ path.replace('_','\_') +'} \centering \includegraphics[scale=0.45]{moduleInPathTimeSummary'+ path.replace('_','') +'temp.png}\n')	
		texfile.write('\\newpage \section{'+ path.replace('_','\_') +'} \centering \includegraphics[scale=0.45]{moduleInPathTimeSummary'+ path.replace('_','') +'temp.png}\n')	
	texfile.write('\\chapter{Average module (in path) running time}\n')
	for path in names:
		#texfile.write('\\newpage \section{Average module running time for path '+ path.replace('_','\_') +'} \centering \includegraphics[scale=0.45]{moduleInPathScaledTimeSummary'+ path.replace('_','') +'temp.png}\n')	
		texfile.write('\\newpage \section{'+ path.replace('_','\_') +'} \centering \includegraphics[scale=0.45]{moduleInPathScaledTimeSummary'+ path.replace('_','') +'temp.png}\n')	
	texfile.write('\\chapter{Average path time} \\newpage \centering \includegraphics[scale=0.45]{pathTimeSummarytemp.png}\n')
	texfile.write('\\chapter{Average incremental path time} \\newpage \centering \includegraphics[scale=0.45]{incPathTimeSummarytemp.png}\n')
#	texfile.write('\\chapter{Failing module (by path)}\n')
#	for path in names:
		#texfile.write('\\newpage \section{Failure fraction (\%) by module '+ path.replace('_','\_') +'} \centering \includegraphics[scale=0.45]{failedModule'+ path.replace('_','') +'temp.png}\n')	
#		texfile.write('\\newpage \section{'+ path.replace('_','\_') +'} \centering \includegraphics[scale=0.45]{failedModule'+ path.replace('_','') +'temp.png}\n')	
#	texfile.write('\\chapter{Per event module time}\n')
#	for path in names:
#	for modules in names[path]:
#		texfile.write('\\newpage \section{Time per event for module '+ modules+'} \centering \includegraphics[scale=0.45]{moduleTime'+modules+'temp.png}\n')	
			

	texfile.write('\\chapter{Per event module running time}\n')

	texfile.write('\\chapter{Per event module (in path) running time}\n')

	texfile.write('\\chapter{Per event path time}\n')

	texfile.write('\\chapter{Per event incremental path time}\n')


#			'\\newpage \section{} \centering \includegraphics[scale=0.45]{.png}\n',

	texfile.write('\end{document}')
	texfile.close()

###################################################################
def get_plot1(file,allnames):
	''' Function to create the plot and save it as png file '''
###################################################################
	file = TFile(file,'read')
#	for k in file.GetListOfKeys():
#		h = k.ReadObj()
#		allnames = h.GetName()
#		name = ''.join(allnames.split('_'))
	histo = file.Get(allnames)
	can = TCanvas('can', '', 1000,800)
	can.cd()
	histo.UseCurrentStyle()
	histo.Draw()
	can.SetBorderMode(0)
	can.SetFillColor(kWhite)
	can.SaveAs(allnames+'temp.png')
	del can
	 


###########################################################
def finalpdf(output):
	''' Compile tex file to pdf '''
##########################################################
	args = ['pdflatex', output, '-interaction=batchmode']
	
	process = subprocess.call(args, 
			stdout = subprocess.PIPE,
			stderr = subprocess.PIPE,
			stdin  = subprocess.PIPE)

#	pdfout = process.communicate()
#	return pdfout



###############################################################
def main(argv):
###############################################################
        print "\nPython script that creates the Timing Summary pdf"
        print "For more info, please contact Alejandro Gomez"
        print "email: alejandro.gomez@cern.ch\n"

	infile = None
	outfile = None
	call_maininfo = False
	try:
		opts, args = getopt.getopt(argv, 'hi:o:tb', ['help', 'input=', 'output='])
		if not opts:
			print 'No options supplied'
			usage()
	except getopt.GetoptError,e:
		print e
		usage()
		sys.exit(2)
	for opt, arg in opts:
		if opt in ('-h', '--help'):
			usage()
			sys.exit(2)
		elif opt == '-b':
			print 'Running in batch mode' 
		elif opt in ('-i', '--input'):
			infile = arg
		elif opt in ('-o', '--output'):
			outfile = arg
		elif opt == '-t':
			call_maininfo = True
#		elif opt == 'p':
#			global _paths
#			_paths = True
#		elif opt == 'm':
#			global _modules
#			_modules = True
		else:
			usage()
			sys.exit(2)
		 
	
	if call_maininfo:
		print 'Creating the Main Info Timing Summary pdf'
		maininfo(infile,outfile)
		pdf = finalpdf(outfile+'.tex')
		pdf1= finalpdf(outfile+'.tex')
		os.remove(outfile+'.aux')
		os.remove(outfile+'.log')
		os.remove(outfile+'.out')
		os.remove(outfile+'.tex')
		for filename in glob.glob('*.png'):
			os.remove(filename)
		print '{0}.pdf is done'.format(outfile)


#######################################################
if __name__ =='__main__':
#######################################################
	main(sys.argv[1:])
