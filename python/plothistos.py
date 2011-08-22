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
		hmass.SetXTitle('Mass [GeV]')
		hmass1 = TH1F(particle+particle+'bar mass',particle+particle+'bar mass',100,140,220)
		hmass1.SetXTitle('Mass [GeV]')
		hnparticles = TH1F('number of '+particle+' and '+particle+'bar','number of '+particle+' and '+particle+'bar',5,0,4)
		hpt = TH1F(particle+' pt', particle+' pt',100,50,250)
		heta = TH1F(particle+' eta',particle+' eta',100,-10,10)

#		entry = 0
		for event in events:
			#entry += 1
			nparticles = 0
#			print "Event ", count
			event.getByLabel (label, handleGen)
			gens = handleGen.product()
    
			for p in gens:
#				print p.pdgId()
				if int(p.pdgId()) == int(PDG[particle]):
#					print p.mass()
					hmass.Fill(float(p.mass()))
	
#				print 'id',p.pdgId(), p.status(), 'status'
				if abs( int(p.pdgId()) ) == int(PDG[particle]):
					if p.status() == 3:
						nparticles += 1
#						print 'nparticles', nparticles
						hpt.Fill(float(p.pt())) 
						heta.Fill(float(p.eta())) 
						hmass1.Fill(float(p.mass()))
			hnparticles.Fill(nparticles)

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
def deltaR(infile):
######################################
	events = Events (infile)
	handleGen = Handle ("vector<reco::GenParticle>")
	label = "genParticles"

	leptons = [11, 13, 15, -11, -13, -15]
	quarks = [1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6]

	hdeltaR = TH1F('deltaR','deltaR',100,0,2)

	L = {}
	Q = {}
	dR = {}	
	count = 0
	count2 = 0 
	for event in events:
#		print event
		event.getByLabel (label, handleGen)
		gens = handleGen.product()
		
		for p in gens:
			if p.pdgId() in leptons:
				if p.status() == 1:			# for final particles
					count += 1
					L[count] = {}
					L[count][int(p.pdgId())] = p.p4()
			if p.pdgId() in quarks: 	# for particles before parton showering
				if p.status() == 3: 
					count2 += 1
					Q[count2] = {}
					Q[count2][int(p.pdgId())] = p.p4()
		for i in L.keys():
			dR[i] = {}
			for lepton, ivalue in L[i].iteritems():
				for j in Q.keys():
					dR[i][j] = {}
					for quark, jvalue in Q[j].iteritems():
						deltaR = Math.VectorUtil.DeltaR(ivalue, jvalue)
						if (0 < deltaR < 50):
							dR[i][j] = deltaR
						elif deltaR > 50:
							dR[i][j] = 50
						else:
							dR[i][j] = 1000

		for k in dR.keys():
			for x in dR[k].keys():
				myMin = min(dR[k].values())
			hdeltaR.Fill(myMin)

	c1 = TCanvas('c1','c1',800,600)
	hdeltaR.Draw()
	c1.SetLogy()
	c1.SaveAs('deltaRTotal.png')

#######################################
def getBR(infile):
#######################################
        events = Events (infile)
        handleGen = Handle ("vector<reco::GenParticle>")
        label = "genParticles"

        leptons = [11, 13, 15, -11, -13, -15]
        quarks = [1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6]

        hBR = TH1F('hBR','Branching Ratio',100,0,250)
	hBR.SetXTitle('Mass (GeV)')
	hBR.SetYTitle('BR')
	hBR.SetStats(0)
	
	e = 0
        for event in events:
		print event
		e += 1
		hadronically = 0
		leptonically = 0
                event.getByLabel (label, handleGen)
                gens = handleGen.product()

                for p in gens:
			id = p.pdgId()
			st = p.status()
			nmom = p.numberOfMothers()
			if nmom == 1:
				if st == 3:
#				for i in nmom:
#					mom = p.mother(i)
					print id, st, nmom
					if id in leptons:
						leptonically += 1
					if not id in leptons:
						hadronically +=1
	print e, hadronically, leptonically


#######################################
def main(argv):
#######################################
	infile = None
	try: 
		opts, args = getopt.getopt(argv, 'hbi:p:dr', ['help','input=', 'particle='])
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
			get_info(infile, particle)
		elif opt == '-d':	
			deltaR(infile)
		elif opt == '-r':
			getBR(infile)
		else:
			sys.exit(2)

if __name__ == '__main__':
	main(sys.argv[1:])
