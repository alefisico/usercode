#! /usr/bin/env python

#############################################
## Imports
############################################

'''
   Usage: %prog <file>
'''

import os
import re
import os.path
import subprocess 
import sys
import tempfile
from ROOT import *
import cStringIO 

gROOT.SetStyle("Plain")
gStyle.SetOptStat(1111111)
gStyle.SetHistFillColor(kBlue)

###############################################################
def usage():
###############################################################

    if False:
        print "This is the usage function"

    print '\n'
    print 'Usage: '+sys.argv[0]+' <file1> '
    print 'e.g.:  '+sys.argv[0]+' outputTiming1.root \n'



#################################################
def texfile(rootfile, fname):
	''' Function to create a .tex file'''
################################################

	texpreamble = ['\documentclass[10pt,a5paper,landscape]{book}\n',
			'\usepackage{graphicx}\n',
			'\usepackage[a5paper,vmargin={5mm,2mm},hmargin={5mm,5mm}]{geometry}\n',
			'\usepackage[linktocpage]{hyperref}\n',
			'\hypersetup{backref, colorlinks=true}\n',
			'\\title{ \\textbf{\Huge{HLT Timing Summary}} \\footnote{\large{Documentation at \url{https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHLTTimingSummary}}}}\n',
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
def get_plot(file):
	''' Function to create the plot and save it as png file '''
###################################################################
	file = TFile(file,'read')
	for k in file.GetListOfKeys():
		h = k.ReadObj()
		allnames = h.GetName()
		name = ''.join(allnames.split('_'))
		histo = file.Get(allnames)

		can = TCanvas('can', '', 1000,800)
		can.cd()
		histo.UseCurrentStyle()
		histo.Draw()
		can.SetBorderMode(0)
		can.SetFillColor(kWhite)
		can.SaveAs(name+'temp.png')
		del can
	 


###########################################################
def finalpdf(output):
	''' Compile tex file to pdf '''
##########################################################
	args = ['pdflatex', output,]
	
	process = subprocess.call(args, 
			#stdout = subprocess.PIPE,
			stderr = subprocess.PIPE,
			stdin  = subprocess.PIPE)

#	pdfout = process.communicate()
#	return pdfout



###############################################################
def main():
###############################################################
        #check the number of parameter
	if len(sys.argv) < 1:
                usage()
                return 1

        print "Python script that creates the Timing Summary pdf"
        print "For more info, please contact with"
        print "Alejandro Gomez"
        print "alejandro.gomez@cern.ch\n"

        file = sys.argv[1]
	fname = file.strip('.root')
#	test = get_plot(file)         
	test = texfile(file,fname)
	pdf = finalpdf(fname+'.tex')
	pdf1= finalpdf(fname+'.tex')

#######################################################
if __name__ =='__main__':
#######################################################
        sys.exit(main())

