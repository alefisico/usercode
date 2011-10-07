#! /usr/bin/env python
#################################
# Python script to plot nice histos  
#################################
import sys,getopt
import operator
sys.argv.append('-b')
from ROOT import *
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
				#print 'id',p.pdgId(), p.status(), 'status'
				if (abs( int(p.pdgId()) ) == int(PDG[particle])) and (p.status() in range(21,30)):
						nparticles += 1
#						print 'nparticles', nparticles
						hpt.Fill(float(p.pt())) 
						heta.Fill(float(p.eta())) 
						hmass.Fill(float(p.mass()))
			hnparticles.Fill(nparticles)

		hmass.Draw()
		c1.SaveAs(outfile+'_'+particle+'_mass.png')
	        hpt.Draw()
	        c1.SaveAs(outfile+'_'+particle+'_pt.png')
	        heta.Draw()
	        c1.SaveAs(outfile+'_'+particle+'_eta.png')
	        hnparticles.Draw()
	        c1.SaveAs(outfile+'_'+particle+'_nparticles.png')

#######################################
def deltaR(infile,outfile):
######################################
	events = Events (infile)
	handleGen = Handle ("vector<reco::GenParticle>")
	label = "genParticles"

	hdeltaR = TH1F('deltaR','deltaR',50,0,2.5)

	for event in events:
#		print event
		L1 = []
		Q1 = []
		event.getByLabel (label, handleGen)
		gens = handleGen.product()
		
		for p in gens:
#			print 'particle', p.pdgId(), 'status', p.status()
			if p.status() > 0:
				if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13)) and (abs(p.mother().pdgId()) == 24):
					L1.append(p.p4())
			if p.status() in range(21,30):
				if ((abs(p.pdgId()) in range(1,7)) or (p.pdgId() == 21)) and ((abs(p.mother().pdgId()) == 24) or (abs(p.mother().pdgId()) == 6)) :
					Q1.append(p.p4())
#		print L1
#		print Q1
                for pLepton in L1:
                        deltaR = []
                        for pQuark in Q1:
                                deltaR.append(Math.VectorUtil.DeltaR(pLepton, pQuark))
                        hdeltaR.Fill(min(deltaR))
#
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

        hBR = TH1F('h1','Branching Ratio',3,0,3)
	hBR.SetStats(0)
	
	nevent = 0
	hadronically = 0
	dilepton = 0
	leptonJets = 0
        for event in events:
#		print event
		nevent += 1
		leptons = 0
		leptonsTau = 0
                event.getByLabel (label, handleGen)
                gens = handleGen.product()

                for p in gens:
			if p.mother():
				#print e, p.status(), p.pdgId(), mother
				if p.status() > 0 and (abs(p.mother().pdgId()) == 24):
					if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)):
						leptonsTau +=1
						if not (abs(p.pdgId()) == 15) : leptons += 1
#		print e, leptons
		if leptonsTau == 0 : hadronically += 1
		elif leptons == 1: leptonJets += 1
		elif leptons == 2: dilepton += 1
		
	#print e, hadronically, dilepton, leptonJets
	hBR.Fill('hadronically', ((hadronically*100)/nevent))
	hBR.Fill('lepton + Jets', (leptonJets*100/nevent))
 	hBR.Fill('dilepton', (dilepton*100/nevent))
	hBR.Draw()
	c1.SetGridx()
	c1.SetGridy()
	c1.SaveAs(outfile+'_BR.png')


#######################################
def accepEfficiency(infile,outfile):
#######################################
        events = Events (infile)
        handleGen = Handle ("vector<reco::GenParticle>")
        label = "genParticles"

        hAE = TH1F('h1','Acceptance Efficiency',4,0,4)
        hAE.SetYTitle('percentage')
        hAE.SetStats(0)


	e = 0
	accepMuons = 0
	accepElectrons = 0
	accepJets = 0
	total = 0
	for event in events:
		e += 1
		muons = 0
		electrons = 0
		jets = 0
#		print event
		event.getByLabel (label,handleGen)
		gens = handleGen.product()

		for p in gens:
			if p.status() > 0:
				if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1) and (abs(p.mother().pdgId()) == 24):
					muons += 1
				elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5):
					electrons += 1
			if (p.status() in range(21,30)) and ((abs(p.mother().pdgId()) == 24) or (abs(p.mother().pdgId()) == 6)):
				if (abs(p.pdgId()) in range (1,7)) and (abs(p.eta()) < 2.4):
					jets += 1
		if muons: accepMuons += 1
		if electrons: accepElectrons += 1
		if jets: accepJets += 1
		if (muons and electrons and jets): total +=1

	
#	print e, accepMuons, accepElectrons, accepJets, total

        hAE.Fill('|eta| muons < 2.1', ((accepMuons*100)/e))
        hAE.Fill('|eta| electrons < 2.5', (accepElectrons*100/e))
        hAE.Fill('|eta| jets < 2.4', (accepJets*100/e))
        hAE.Fill('combined', (total*100/e))
        hAE.Draw()
        c1.SetGridx()
        c1.SetGridy()
        c1.SaveAs(outfile+'_accepEfficiency.png')

#######################################
def acceptance(infile,outfile):
#######################################
        events = Events (infile)
        handleGen = Handle ("vector<reco::GenParticle>")
        label = "genParticles"
                
        hAE = TH1F('h1','Acceptance',12,0,12)
        hAE.SetYTitle('percentage')
        hAE.SetStats(0)

	hpt = TH1F('partons pt for all jets', 'partons pt for all jets',100,0,300)
	heta = TH1F('partons eta for all jets','partons eta for all jets',100,-3,3)
        hdeltaR = TH1F('deltaR for all jets','deltaR for all jets',100,0,3)

	llpt = TH1F('leptons pt for lepton + jets', 'leptons pt for lepton + jets',100,0,100)
	lleta = TH1F('leptons eta for lepton + jets','leptons eta for lepton + jets',100,-10,10)
	lppt = TH1F('partons pt for lepton + jets', 'partons pt for lepton + jets',100,0,300)
	lpeta = TH1F('partons eta for lepton + jets','partons eta for lepton + jets',100,-3,3)
        ldeltaR = TH1F('deltaR for lepton + jets','deltaR for lepton + jets',100,0,3)

        dlpt = TH1F('leptons pt for dilepton', 'leptons pt for dilepton',100,0,100)
        dleta = TH1F('leptons eta for dilepton','leptons eta for dilepton',100,-10,10)
        dppt = TH1F('partons pt for dilepton', 'partons pt for dilepton',100,0,300)
        dpeta = TH1F('partons eta for dilepton','partons eta for dilepton',100,-3,3)
        tdeltaR = TH1F('deltaR for dilepton','deltaR for dilepton',100,0,3)

        nevent = 0
	hadronically = 0
	haccepJets = 0
	leptonJets = 0
        laccepMuons = 0
        laccepElectrons = 0
        laccepJets = 0
        ltotal = 0
	dilepton = 0
        daccepMuons = 0
        daccepElectrons = 0
        daccepJets = 0          
        dtotal = 0
        for event in events:
                nevent += 1
		leptonsTau = 0
		leptons = 0
		hjets = 0
		lmuons = 0
		lelectrons = 0
		ljets = 0
		dmuons = 0
		delectrons = 0
		djets = 0

#               print event
                event.getByLabel (label,handleGen)
                gens = handleGen.product()

                for p in gens:
                        if p.mother():# and  (p.status() == 23):
                                if (p.status() > 0) and (abs(p.mother().pdgId()) == 24):
                                        if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)):# and (abs(p.mother().pdgId()) == 24):
                                                leptonsTau +=1
                                                if not (abs(p.pdgId()) == 15) : leptons += 1

#		print nevent, leptons
		if leptonsTau == 0 :
			hadronically += 1
			phighpt = []
                        for p in gens:
                                if p.status() == 23 and (1 <= abs(p.pdgId()) <= 5) and (abs(p.eta()) < 2.4):
                                        hjets += 1
        #                               hpt.Fill(float(p.pt()))
                                        phighpt.append(p.pt())
                                        heta.Fill(float(p.eta()))
                        hpt.Fill(max(highpt))
                        if hjets: 
                                haccepJets += 1
                                Q1 = []
                                for p in gens:
                                        if p.status() == 23:
                                                if (1 <= abs(p.pdgId()) <= 5): Q1.append(p.p4())
                                for pQuark in Q1:
                                        deltaR = []
                                        for qQuark in Q1:
                                                dR = (Math.VectorUtil.DeltaR(pQuark, qQuark))
                                                if dR != 0: deltaR.append(dR)
#                               print deltaR
                                hdeltaR.Fill(min(deltaR))


		if leptons == 1: 
			leptonJets += 1
			llhighpt = []
			lphighpt = []
			for p in gens:
				if (p.status() == 23) and (abs(p.mother().pdgId()) == 24):
					if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1): lmuons += 1
					elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5): lelectrons += 1
				if (p.status() == 23) and ((abs(p.mother().pdgId()) == 24) or (abs(p.mother().pdgId()) == 6)):
					if (abs(p.eta()) < 2.4) and (1 <= abs(p.pdgId()) <= 5) :
						ljets += 1
						lphighpt.append(p.pt())
               		               	        lpeta.Fill(float(p.eta()))
#			llpt.Fill(max(llhighpt))
			if lphighpt: lppt.Fill(max(lphighpt))
                        if lmuons: laccepMuons += 1
                        if lelectrons: laccepElectrons += 1
			if lmuons or lelectrons:
				llpt.Fill(p.pt())
				lleta.Fill(float(p.eta())) 
                        if ljets: laccepJets += 1
                        if ((lmuons or lelectrons) and ljets): 
				ltotal +=1
				L1 = []
				Q1 = []
		
				for p in gens:
#					print 'particle', p.pdgId(), 'status', p.status()
					if p.status() == 23:
						if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13)) and (abs(p.mother().pdgId()) == 24):
							L1.append(p.p4())
						if ((abs(p.pdgId()) in range(1,7)) or (abs(p.pdgId()) == 21)) and ((abs(p.mother().pdgId()) == 24) or (abs(p.mother().pdgId()) == 6)) :
							Q1.append(p.p4())
                		for pLepton in L1:
		                        deltaR = []
                		        for pQuark in Q1:
                                		deltaR.append(Math.VectorUtil.DeltaR(pLepton, pQuark))
	                        ldeltaR.Fill(min(deltaR))

		elif leptons == 2:
			dilepton += 1
			dlhighpt = []
			for p in gens:
				if (p.status() == 23) and (abs(p.mother().pdgId()) == 24):
					if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1): 
						dmuons += 1
						dlhighpt.append(p.pt())
						dleta.Fill(float(p.eta()))
					elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5): 
						delectrons += 1
						dlhighpt.append(p.pt())
						dleta.Fill(float(p.eta()))
				if (p.status() == 23) and (abs(p.pdgId()) == 6) and (abs(p.eta()) < 2.4):
						djets += 1
						dppt.Fill(p.pt())
               		               	        dpeta.Fill(float(p.eta()))
#			llpt.Fill(max(llhighpt))
			if dlhighpt: dlpt.Fill(max(dlhighpt))
                        if dmuons: daccepMuons += 1
                        if delectrons: daccepElectrons += 1
                        if djets: daccepJets += 1
                        if (dmuons and delectrons and djets): 
				dtotal +=1
				L1 = []
				Q1 = []
		
				for p in gens:
					if p.status() == 23:
						if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13)) and (abs(p.mother().pdgId()) == 24):
							L1.append(p.p4())
						if ((abs(p.pdgId()) in range(1,7)) or (abs(p.pdgId()) == 21)) and ((abs(p.mother().pdgId()) == 24) or (abs(p.mother().pdgId()) == 6)) :
							Q1.append(p.p4())
                		for pLepton in L1:
		                        deltaR = []
	               		        for pQuark in Q1:
                                		deltaR.append(Math.VectorUtil.DeltaR(pLepton, pQuark))
	                        tdeltaR.Fill(min(deltaR))

	
        hAE.Fill('alljets |eta| jets < 2.4', (haccepJets*100/hadronically))
        hAE.Fill('alljets without cuts', (hadronically*100/hadronically))
        hAE.Fill('leptonJet |eta| muons < 2.1', ((laccepMuons*100)/leptonJets))
        hAE.Fill('leptonJet |eta| electrons < 2.5', (laccepElectrons*100/leptonJets))
        hAE.Fill('leptonJet |eta| jets < 2.4', (laccepJets*100/leptonJets))
        hAE.Fill('leptonJet combined', (ltotal*100/leptonJets))
        hAE.Fill('leptonJet without cuts', (leptonJets*100/leptonJets))
        hAE.Fill('dilepton |eta| muons < 2.1', ((daccepMuons*100)/dilepton))
        hAE.Fill('dilepton |eta| electrons < 2.5', (daccepElectrons*100/dilepton))
        hAE.Fill('dilepton |eta| jets < 2.4', (daccepJets*100/dilepton))
        hAE.Fill('diletpon combined', (dtotal*100/dilepton))
        hAE.Fill('dilepton without cuts', (dilepton*100/dilepton))
	can1 = TCanvas('can1', '', 800, 600)
	can1.cd()
        hAE.Draw()
        can1.SetGridx()
        can1.SetGridy()
	can1.SetBottomMargin(0.2)
        can1.SaveAs(outfile+'_acceptance.png')
	del can1

	c1 = TCanvas('c1', '', 800, 600)
	heta.Draw()
	c1.SaveAs(outfile+'_alljets_eta.png')
        hpt.Draw()
        c1.SaveAs(outfile+'_alljets_pt.png')
        hdeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_alljets_delta.png')
        llpt.Draw()
        c1.SaveAs(outfile+'_leptonjets_lepton_pt.png')
        lleta.Draw()
        c1.SaveAs(outfile+'_leptonjets_lepton_eta.png')
	lppt.Draw()
        c1.SaveAs(outfile+'_leptonjets_parton_pt.png')
        lpeta.Draw()
        c1.SaveAs(outfile+'_leptonjets_parton_eta.png')
        ldeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_leptonjets_deltaR.png')
        dlpt.Draw()
        c1.SaveAs(outfile+'_dilepton_lepton_pt.png')
        dleta.Draw()
        c1.SaveAs(outfile+'_dilepton_lepton_eta.png')
        dppt.Draw()
        c1.SaveAs(outfile+'_dilepton_parton_pt.png')
        dpeta.Draw()
        c1.SaveAs(outfile+'_dilepton_parton_eta.png')
        tdeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_dilepton_deltaR.png')



#######################################
def main(argv):
#######################################
	infile = None
	outfile = None
	try: 
		opts, args = getopt.getopt(argv, 'hbi:p:drae', ['help','input=', 'particle='])
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
#		elif opt in ('-o', '--output'):
			outfile = infile.replace('.root','')
		elif opt in ('-p', '--particle'):
			particle = arg
			get_info(infile, particle, outfile)
		elif opt == '-d':	
			deltaR(infile, outfile)
			gSystem.Exit(0)
		elif opt == '-r':
			getBR(infile, outfile)
		elif opt == '-a':
			acceptance(infile, outfile)
		elif opt == '-e':
			accepEfficiency(infile, outfile)
		else:
			sys.exit(2)

if __name__ == '__main__':
	main(sys.argv[1:])
