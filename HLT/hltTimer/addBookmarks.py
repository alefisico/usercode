#! /usr/bin/env python

#############################################
## Imports
############################################

import os
import re
import os.path
import subprocess 
import sys
import tempfile
from ROOT import *
import cStringIO 

############################################
## Inputs
###########################################

inputtxt = "outfile_v2-bookmark.txt"
inputroot = 'outputTiming.root'


############################################
## Functions
############################################

def managerootfile(rootfile):
	'''
	Manage root file
	'''
	file = TFile(inputroot,'read')

def splitmenus(txtfile):
	'''
	Split the summary.txt file
	'''
	infile = open(txtfile, 'r')
	for line in infile.readlines():
		linesplit = line.split('^')
			#for item in linesplit:
			#	generalmenu = linesplit[0]
			#	pgNum = linesplit[-1]
			#	print generalmenu, pgNum
#			for i in range(00001,len(infile.readlines()):
#					if pgNum = 


	else:
		print 'Summary file doesnt exist'

def texfile(txtfile, outputtex):
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
			'\\newpage\n'
			]
	chapters = ['Total time per event',
			'Average module time',
			'Average module running time',
			'Average module (in path) time',
			'Average module (in path) running time',
			'Avergae path time',
			'Average incremental path time',
			'Failing module (by path)',
			'Per event module time',
			'Per event module running time',
			'Per event module (in path) running time',
			'Per event path time',
			'Per event incremental path time'
			]
	infile = open(txtfile, 'r')
	texfile = open(outputtex+'.tex', 'w')
	texfile.writelines(texpreamble)
	for line_number, line in enumerate(infile, start = 2):
		linesplit = line.split('^')
		for i,u in enumerate(chapters[1:]):
			if linesplit[0] == chapters[i]: texfile.write('\chapter{{{0}}}\n\\newpage\n'.format(linesplit[0])) 
		if len(linesplit) == 3: None
		if len(linesplit) >= 4: texfile.write(('\section{{{0}}}\n'.format(linesplit[1])).replace('_','\_'))
		texfile.write('\\begin{figure*}[h!]\n')
		texfile.write('\centering\n')
		texfile.write('\includegraphics[scale=0.8]{pg_%04d.pdf}\n' %line_number)
		texfile.write('\end{figure*}\n')
		texfile.write('\\newpage\n')

	texfile.write('\end{document}')
	texfile.close()

def finalpdf(output):
	'''
	Compile tex file to pdf
	'''
	args = ['pdflatex',
			output,
			]
	
	process = subprocess.call(args, 
			#stdout = subprocess.PIPE,
			stderr = subprocess.PIPE,
			stdin  = subprocess.PIPE)

	pdfout = process.communicate()
	return pdfout

         
#splitmenus(inputtxt)
#splitpdf(inputpdf)
#name = 'hola'
#texfile(inputtxt, name)
#finalpdf(name)
