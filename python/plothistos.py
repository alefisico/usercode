#! /usr/bin/env python
#################################
# Python script to plot nice histos  
#################################
from ROOT import *
import sys,getopt
from DataFormats.FWLite import Events, Handle

gROOT.SetStyle("Plain")
gStyle.SetOptStat(111111)
gStyle.SetHistFillColor(kBlue)

PDG =  {'d':1, 'dbar':-1, 'down':1,
	'u':2, 'ubar':-2, 'up':2,
	's':3, 'sbar':-3, 'strange':3,
	'c':4, 'cbar':-4, 'charm':4,
	'b':5, 'bbar':-5, 'bottom':5, 'beauty':5,
	't':6, 'tbar':-6, 'top':6,
	'electron':11, 'positron':-11,
	'nu_e':12, 'nu_ebar':-12,
	'muon':13, 'mu+':-13,
	'nu_mu':14, 'nu_mubar':-14,
	'tau':15, 'tau+':-15,
	'nu_tau':16, 'nu_taubar':-16,
	'gluon':21, 'photon':22, 'Z0':23, 
	'W+':24, 'W-':-24, 
	'Higgs0':25}

######################################
def get_info(infile,particle):
#####################################
	events = Events (infile)
	handleGen = Handle ("vector<reco::GenParticle>")
	label = "genParticles"

	if particle in PDG.keys():
		# For histos 
		hmass = TH1F( particle+' mass', particle+' mass',100,140,220)
		hmass1 = TH1F(particle+particle+'bar mass',particle+particle+'bar mass',100,140,220)
		hnparticles = TH1F('number of '+particle+' and '+particle+'bar','number of '+particle+' and '+particle+'bar',100,0,50)
		hpt = TH1F(particle+' pt', particle+' pt',100,50,250)
		heta = TH1F(particle+' eta',particle+' eta',100,-10,10)

		entry = 0
		for event in events:
			#entry += 1
			#print "Event ", count
			event.getByLabel (label, handleGen)
			gens = handleGen.product()
    
			for p in gens:
				#print p.pdgId()
				nparticles = 0
				if int(p.pdgId()) == int(PDG[particle]):
			#		print p.mass()
					hmass.Fill(float(p.mass()))
	
				if abs( int(p.pdgId()) ) == int(PDG[particle]):
					nparticles += 1
					hpt.Fill(float(p.pt())) 
					heta.Fill(float(p.eta())) 
					hnparticles.Fill(nparticles)
					hmass1.Fill(float(p.mass()))

		hmass.Draw()
		c1.SaveAs(particle+'_mass.png')
	        hpt.Draw()
	        c1.SaveAs(particle+'_pt.png')
	        heta.Draw()
	        c1.SaveAs(particle+'_eta.png')
	        hnparticles.Draw()
	        c1.SaveAs(particle+'_nparticles.png')
	        hmass1.Draw()
	        c1.SaveAs(particle+particle+'bar_mass.png')




#######################################
def main(argv):
#######################################
	print '\n Python script for nice histos\n'

	infile = None
	call_histos = False
	try: 
		opts, args = getopt.getopt(argv, 'hbi:p:', ['help','input=', 'particle='])
		if not opts:
			print 'No options supplied. Please insert input -i and pdgID -p'
	except getopt.GetoptError,e:
		print e
		sys.exit(2)

	for opt, arg in opts:
		if opt in ('-h', '--help'):
			sys.exit(2)
		elif opt == '-b':
			continue
		elif opt in ('-i', '--input'):
			infile = arg
		elif opt in ('-p', '--particle'):
			particle = arg
			call_histos = True
		else:
			sys.exit(2)

	if call_histos:
		get_info(infile, particle)

if __name__ == '__main__':
	main(sys.argv[1:])
