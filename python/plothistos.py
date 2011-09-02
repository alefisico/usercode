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
def get_info(infile,particle, outfile):
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
		hnparticles = TH1F('number of '+particle+' and '+particle+'bar','number of '+particle+' and '+particle+'bar',10,0,10)
		hpt = TH1F(particle+' pt', particle+' pt',100,0,500)
		heta = TH1F(particle+' eta',particle+' eta',100,-10,10)

#		entry = 0
		for event in events:
			#entry += 1
			nparticles = 0
		#	print "Event ", count
			event.getByLabel (label, handleGen)
			gens = handleGen.product()
    
			for p in gens:
#				print p.pdgId()
				if int(p.pdgId()) == int(PDG[particle]):
#					print p.mass()
					hmass.Fill(float(p.mass()))
	
				#print 'id',p.pdgId(), p.status(), 'status'
				if abs( int(p.pdgId()) ) == int(PDG[particle]):
					if p.status() == 3:
						nparticles += 1
#						print 'nparticles', nparticles
						hpt.Fill(float(p.pt())) 
						heta.Fill(float(p.eta())) 
						hmass1.Fill(float(p.mass()))
			hnparticles.Fill(nparticles)

		hmass.Draw()
		c1.SaveAs(outfile+'_'+particle+'_mass.png')
	        hpt.Draw()
	        c1.SaveAs(outfile+'_'+particle+'_pt.png')
	        heta.Draw()
	        c1.SaveAs(outfile+'_'+particle+'_eta.png')
	        hnparticles.Draw()
	        c1.SaveAs(outfile+'_'+particle+'_nparticles.png')
	        hmass1.Draw()
	        c1.SaveAs(outfile+'_'+particle+particle+'bar_mass.png')

#######################################
def deltaR(infile,outfile):
######################################
	events = Events (infile)
	handleGen = Handle ("vector<reco::GenParticle>")
	label = "genParticles"

	hdeltaR = TH1F('deltaR','deltaR',100,0,5)

	L = {}
	Q = {}
	dR = {}	
	nevent = 0
	for event in events:
#		print event
		nevent += 1
		count2 = 0 
		count = 0
		L[nevent] = {}
		Q[nevent] = {}
		event.getByLabel (label, handleGen)
		gens = handleGen.product()
		
		for p in gens:
#			print 'particle', p.pdgId(), 'status', p.status()
			if abs(p.pdgId()) in range(11,14):
				if p.status() == 3:
#					print 'name', p.pdgId(), p.status(), p.p4()
					count += 1
					L[nevent][count] = {}
					L[nevent][count][int(p.pdgId())] = p.p4()
			if (abs(p.pdgId()) in range(1,7)) or (p.pdgId() == 21) :
				if p.status() == 3: 
#					print 'name', p.pdgId(), p.status()
					count2 += 1
					Q[nevent][count2] = {}
					Q[nevent][count2][int(p.pdgId())] = p.p4()
		for e in L.keys():
			dR[e] = {}
			for i in L[e].keys():
				dR[e][i] = {}
				for lepton, ivalue in L[e][i].iteritems():
					for j in Q[e].keys():
						dR[e][i][j] = {}
#		print dR
						for quark, jvalue in Q[e][j].iteritems():
							deltaR = Math.VectorUtil.DeltaR(ivalue, jvalue)
							if (0 < deltaR < 100):
								dR[e][i][j] = deltaR
							elif deltaR > 100:
								dR[e][i][j] = 100
							else:
								del dR[e][i][j]
#		print dR
		dR = dict([(k,v) for k,v in dR.items() if len(v)>0])

		for ev in dR.keys():
			for k in dR[ev].keys():
				for x in dR[ev][k].keys():
					myMin = min(dR[ev][k].values())
				hdeltaR.Fill(myMin)

	c1 = TCanvas('c1','c1',800,600)
	hdeltaR.Draw()
	c1.SetLogy()
	c1.SaveAs(outfile+'_deltaR.png')

#######################################
def getBR(infile,outfile):
#######################################
        events = Events (infile)
        handleGen = Handle ("vector<reco::GenParticle>")
        label = "genParticles"

        #leptons = [11, 13, 15, -11, -13, -15]
        #quarks = [1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6]

        hBR = TH1F('h1','Branching Ratio',3,0,3)
#	hBR.SetYTitle('BR')
	hBR.SetStats(0)
	
	e = 0
	hadronically = 0
	dilepton = 0
	leptonJets = 0
        for event in events:
#		print event
		e += 1
		ntotal = 0
		leptons = 0
		quarks = 0
		others = 0
                event.getByLabel (label, handleGen)
                gens = handleGen.product()

                for p in gens:
			if p.status() == 3:
#				print e, p.status(), p.pdgId()
				ntotal += 1
				if abs(p.pdgId()) in range(11,14):
					leptons +=1
				elif abs(p.pdgId()) in range(1,7):
					quarks += 1
				else:
					others += 1
#		print e, ntotal, quarks, leptons, others
		if leptons == 0:
			hadronically += 1
		elif leptons == 4:
			dilepton += 1
		elif leptons == 2:
			leptonJets += 1
			
	print e, hadronically, dilepton, leptonJets
	hBR.Fill('hadronically', hadronically)
	hBR.Fill('dilepton', dilepton)
	hBR.Fill('lepton + Jets', leptonJets)
	hBR.Draw()
	c1.SetGridx()
	c1.SetGridy()
	c1.SaveAs(outfile+'_ttbarBR.png')


#######################################
def main(argv):
#######################################
	infile = None
	outfile = None
	try: 
		opts, args = getopt.getopt(argv, 'hbi:o:p:dr', ['help','input=', 'particle=', 'output='])
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
		elif opt in ('-o', '--output'):
			outfile = arg
		elif opt in ('-p', '--particle'):
			particle = arg
			get_info(infile, particle, outfile)
		elif opt == '-d':	
			deltaR(infile, outfile)
		elif opt == '-r':
			getBR(infile, outfile)
		else:
			sys.exit(2)

if __name__ == '__main__':
	main(sys.argv[1:])
