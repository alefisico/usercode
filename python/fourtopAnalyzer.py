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

	hdeltaR = TH1F('deltaR','deltaR',100,0,2)

	for event in events:
#		print event
		count2 = 0 
		count = 0
		L1 = {}
		Q1 = {}
		dR = {}	
		Min = {}
		event.getByLabel (label, handleGen)
		gens = handleGen.product()
		
		for p in gens:
#			print 'particle', p.pdgId(), 'status', p.status()
			if p.status() == 1:
				if (abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13):
#					print 'name', p.pdgId(), p.status(), p.p4()
					count += 1
					L1[count] = p.p4()
			if p.status() == 3: 
				if (abs(p.pdgId()) in range(1,7)) or (p.pdgId() == 21) :
#					print 'name', p.pdgId(), p.status()
					count2 += 1
					Q1[count2] = p.p4()
#		print L1
#		print Q1
		for lepton, pLepton in L1.iteritems():
			dR[lepton] = {}
			for quark, pQuark in Q1.iteritems():
				deltaR = Math.VectorUtil.DeltaR(pLepton, pQuark)
				dR[lepton][quark] = deltaR
#				print lepton, quark, deltaR
#		print dR
		for l in dR:
			myMin = min(dR[l].values())
			Min[l] = myMin
#			print l, myMin
#		print Min
		for i in Min:
			myMin2 = min(Min.values())
#		print myMin2
		hdeltaR.Fill(myMin2)
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

        hBR = TH1F('h1','Branching Ratio',5,0,5)
	hBR.SetStats(0)
	
	nevent = 0
	hadronically = 0
	leptonJets = 0
	dileptonJets = 0
	trileptonJets = 0
	fourleptonJets = 0
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
				if p.status() == 1:
					if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)) and ((abs(p.mother().pdgId()) == 11) or (abs(p.mother().pdgId()) == 13) or (abs(p.mother().pdgId()) == 15)): 
						leptonsTau +=1
						if not (abs(p.pdgId()) == 15) : leptons += 1
#		print e, leptons
		if leptonsTau == 0 : hadronically += 1
		elif leptons == 1: leptonJets += 1
		elif leptons == 2: dileptonJets += 1
		elif leptons == 3: trileptonJets += 1
		elif leptons == 4: fourleptonJets += 1
		
	#print e, hadronically, dilepton, leptonJets
	hBR.Fill('hadronically', ((hadronically*100)/nevent))
	hBR.Fill('lepton + Jets', (leptonJets*100/nevent))
 	hBR.Fill('dilepton + Jets', (dileptonJets*100/nevent))
 	hBR.Fill('3 lepton + Jets', (trileptonJets*100/nevent))
 	hBR.Fill('4 lepton + Jets', (fourleptonJets*100/nevent))
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
			if p.status() == 1:
				if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1):
					muons += 1
				elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5):
					electrons += 1
			if p.status() == 3:	
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
                
        hAE = TH1F('h1','Acceptance',22,0,22)
        hAE.SetYTitle('percentage')
        hAE.SetStats(0)

	hpt = TH1F('partons pt for all jets', 'partons pt for all jets',100,0,300)
	heta = TH1F('partons eta for all jets','partons eta for all jets',100,-3,3)

	llpt = TH1F('leptons pt for lepton + jets', 'leptons pt for lepton + jets',100,0,100)
	lleta = TH1F('leptons eta for lepton + jets','leptons eta for lepton + jets',100,-10,10)
	lppt = TH1F('partons pt for lepton + jets', 'partons pt for lepton + jets',100,0,300)
	lpeta = TH1F('partons eta for lepton + jets','partons eta for lepton + jets',100,-3,3)
	ldeltaR = TH1F('deltaR for lepton + jets','deltaR for lepton + jets',100,0,2)

        dlpt = TH1F('leptons pt for dilepton + jets', 'leptons pt for dilepton + jets',100,0,100)
        dleta = TH1F('leptons eta for dilepton + jets','leptons eta for dilepton + jets',100,-10,10)
        dppt = TH1F('partons pt for dilepton + jets', 'partons pt for dilepton + jets',100,0,300)
        dpeta = TH1F('partons eta for dilepton + jets','partons eta for dilepton + jets',100,-3,3)
	ddeltaR = TH1F('deltaR for dilepton + jets','deltaR for dilepton + jets',100,0,2)

        tlpt = TH1F('leptons pt for 3 lepton + jets', 'leptons pt for 3 lepton + jets',100,0,100)
        tleta = TH1F('leptons eta for 3 lepton + jets','leptons eta for 3 lepton + jets',100,-10,10)
        tppt = TH1F('partons pt for 3 lepton + jets', 'partons pt for 3 lepton + jets',100,0,300)
        tpeta = TH1F('partons eta for 3 lepton + jets','partons eta for 3 lepton + jets',100,-3,3)
	tdeltaR = TH1F('deltaR for 3 lepton + jets','deltaR for 3 lepton + jets',100,0,2)

        flpt = TH1F('leptons pt for 4 lepton + jets', 'leptons pt for 4 lepton + jets',100,0,100)
        fleta = TH1F('leptons eta for 4 lepton + jets','leptons eta for 4 lepton + jets',100,-10,10)
        fppt = TH1F('partons pt for 4 lepton + jets', 'partons pt for 4 lepton + jets',100,0,300)
        fpeta = TH1F('partons eta for 4 lepton + jets','partons eta for 4 lepton + jets',100,-3,3)
	fdeltaR = TH1F('deltaR for 4 lepton + jets','deltaR for 4 lepton + jets',100,0,2)

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
        tlepton = 0
        taccepMuons = 0
        taccepElectrons = 0
        taccepJets = 0
        ttotal = 0
        flepton = 0
        faccepMuons = 0
        faccepElectrons = 0
        faccepJets = 0
        ftotal = 0
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
                tmuons = 0
                telectrons = 0
                tjets = 0
                fmuons = 0
                felectrons = 0
                fjets = 0

#               print event
                event.getByLabel (label,handleGen)
                gens = handleGen.product()

                for p in gens:
			if p.mother():
				#print e, p.status(), p.pdgId(), mother
				if p.status() == 1:
					if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)) and ((abs(p.mother().pdgId()) == 11) or (abs(p.mother().pdgId()) == 13) or (abs(p.mother().pdgId()) == 15)):
						leptonsTau +=1
						if not (abs(p.pdgId()) == 15) : leptons += 1
#		print nevent, leptons
		if leptonsTau == 0 :
			hadronically += 1
			for p in gens:
				if p.status() == 3: 
					if (abs(p.pdgId()) in range (1,7)) and (abs(p.eta()) < 2.4):
						hjets += 1
						hpt.Fill(float(p.pt()))
						heta.Fill(float(p.eta()))
			if hjets: 
				haccepJets += 1
#				deltaR(infile,outfile+'_alljets')
			#print nevent, htotal

		if leptons == 1: 
			leptonJets += 1
			for p in gens:
				if p.status() == 1:
					if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1): lmuons += 1
					elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5): lelectrons += 1
					elif (abs(p.pdgId()) in range(11,15)):
						llpt.Fill(float(p.pt()))
						lleta.Fill(float(p.eta()))
				if p.status() == 3:
					if (abs(p.pdgId()) in range (1,7)) and (abs(p.eta()) < 2.4):
						ljets += 1
						lppt.Fill(float(p.pt()))
                                                lpeta.Fill(float(p.eta()))
                        if lmuons: laccepMuons += 1
                        if lelectrons: laccepElectrons += 1
                        if ljets: laccepJets += 1
                        if (lmuons and lelectrons and ljets): 
				ltotal +=1
				count2 = 0 
				count = 0
				L1 = {}
				Q1 = {}
				dR = {}	
				Min = {}
		
				for p in gens:
					if p.status() == 1:
						if (abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13):
							count += 1
							L1[count] = p.p4()
					if p.status() == 3: 
						if (abs(p.pdgId()) in range(1,7)) or (p.pdgId() == 21) :
							count2 += 1
							Q1[count2] = p.p4()
				for lepton, pLepton in L1.iteritems():
					dR[lepton] = {}
					for quark, pQuark in Q1.iteritems():
						deltaR = Math.VectorUtil.DeltaR(pLepton, pQuark)
						dR[lepton][quark] = deltaR
				for l in dR:
					myMin = min(dR[l].values())
					Min[l] = myMin
				for i in Min:
					myMin2 = min(Min.values())
				ldeltaR.Fill(myMin2)
#			print nevent, ltotal

		elif leptons == 2:
			dilepton += 1
                        for p in gens:
                                if p.status() == 1:
                                        if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1): dmuons += 1
                                        elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5): delectrons += 1
                                        elif (abs(p.pdgId()) in range(11,15)):
                                                dlpt.Fill(float(p.pt()))
                                                dleta.Fill(float(p.eta()))
                                if p.status() == 3:
                                        if (abs(p.pdgId()) in range (1,7)) and (abs(p.eta()) < 2.4): 
						djets += 1
                                                dppt.Fill(float(p.pt()))
                                                dpeta.Fill(float(p.eta()))
                        if dmuons: daccepMuons += 1
                        if delectrons: daccepElectrons += 1
                        if djets: daccepJets += 1
                        if (dmuons and delectrons and djets):
				dtotal +=1
				count2 = 0 
				count = 0
				L1 = {}
				Q1 = {}
				dR = {}	
				Min = {}
		
				for p in gens:
					if p.status() == 1:
						if (abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13):
							count += 1
							L1[count] = p.p4()
					if p.status() == 3: 
						if (abs(p.pdgId()) in range(1,7)) or (p.pdgId() == 21) :
							count2 += 1
							Q1[count2] = p.p4()
				for lepton, pLepton in L1.iteritems():
					dR[lepton] = {}
					for quark, pQuark in Q1.iteritems():
						deltaR = Math.VectorUtil.DeltaR(pLepton, pQuark)
						dR[lepton][quark] = deltaR
				for l in dR:
					myMin = min(dR[l].values())
					Min[l] = myMin
				for i in Min:
					myMin2 = min(Min.values())
				ddeltaR.Fill(myMin2)

                elif leptons == 3:
                        tlepton += 1
                        for p in gens:
                                if p.status() == 1:
                                        if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1): tmuons += 1
                                        elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5): telectrons += 1
                                        elif (abs(p.pdgId()) in range(11,15)):
                                                tlpt.Fill(float(p.pt()))
                                                tleta.Fill(float(p.eta()))
                                if p.status() == 3:
                                        if (abs(p.pdgId()) in range (1,7)) and (abs(p.eta()) < 2.4):
                                                tjets += 1
                                                tppt.Fill(float(p.pt()))
                                                tpeta.Fill(float(p.eta()))
                        if tmuons: taccepMuons += 1
                        if telectrons: taccepElectrons += 1
                        if tjets: taccepJets += 1
                        if (tmuons and telectrons and tjets): 
				ttotal +=1
				count2 = 0 
				count = 0
				L1 = {}
				Q1 = {}
				dR = {}	
				Min = {}
		
				for p in gens:
					if p.status() == 1:
						if (abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13):
							count += 1
							L1[count] = p.p4()
					if p.status() == 3: 
						if (abs(p.pdgId()) in range(1,7)) or (p.pdgId() == 21) :
							count2 += 1
							Q1[count2] = p.p4()
				for lepton, pLepton in L1.iteritems():
					dR[lepton] = {}
					for quark, pQuark in Q1.iteritems():
						deltaR = Math.VectorUtil.DeltaR(pLepton, pQuark)
						dR[lepton][quark] = deltaR
				for l in dR:
					myMin = min(dR[l].values())
					Min[l] = myMin
				for i in Min:
					myMin2 = min(Min.values())
				tdeltaR.Fill(myMin2)

                elif leptons == 4:
                        flepton += 1
                        for p in gens:
                                if p.status() == 1:
                                        if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1): fmuons += 1
                                        elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5): felectrons += 1
                                        elif (abs(p.pdgId()) in range(11,15)):
                                                flpt.Fill(float(p.pt()))
                                                fleta.Fill(float(p.eta()))
                                if p.status() == 3:
                                        if (abs(p.pdgId()) in range (1,7)) and (abs(p.eta()) < 2.4):
                                                fjets += 1
                                                fppt.Fill(float(p.pt()))
                                                fpeta.Fill(float(p.eta()))
                        if fmuons: faccepMuons += 1
                        if felectrons: faccepElectrons += 1
                        if fjets: faccepJets += 1
                        if (fmuons and felectrons and fjets):
				ftotal +=1
				count2 = 0 
				count = 0
				L1 = {}
				Q1 = {}
				dR = {}	
				Min = {}
		
				for p in gens:
					if p.status() == 1:
						if (abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13):
							count += 1
							L1[count] = p.p4()
					if p.status() == 3: 
						if (abs(p.pdgId()) in range(1,7)) or (p.pdgId() == 21) :
							count2 += 1
							Q1[count2] = p.p4()
				for lepton, pLepton in L1.iteritems():
					dR[lepton] = {}
					for quark, pQuark in Q1.iteritems():
						deltaR = Math.VectorUtil.DeltaR(pLepton, pQuark)
						dR[lepton][quark] = deltaR
				for l in dR:
					myMin = min(dR[l].values())
					Min[l] = myMin
				for i in Min:
					myMin2 = min(Min.values())
				fdeltaR.Fill(myMin2)

	
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
        hAE.Fill('3 lepton |eta| muons < 2.1', ((taccepMuons*100)/tlepton))
        hAE.Fill('3 lepton |eta| electrons < 2.5', (taccepElectrons*100/tlepton))
        hAE.Fill('3 lepton |eta| jets < 2.4', (taccepJets*100/tlepton))
        hAE.Fill('3 letpon combined', (ttotal*100/tlepton))
        hAE.Fill('3 lepton without cuts', (tlepton*100/tlepton))
        hAE.Fill('4 lepton |eta| muons < 2.1', ((faccepMuons*100)/flepton))
        hAE.Fill('4 lepton |eta| electrons < 2.5', (faccepElectrons*100/flepton))
        hAE.Fill('4 lepton |eta| jets < 2.4', (faccepJets*100/flepton))
        hAE.Fill('4 letpon combined', (ftotal*100/flepton))
        hAE.Fill('4 lepton without cuts', (flepton*100/flepton))
	can1 = TCanvas('can1', '', 1200, 600)
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
        ddeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_dilepton_deltaR.png')
	tlpt.Draw()
        c1.SaveAs(outfile+'_3lepton_lepton_pt.png')
        tleta.Draw()
        c1.SaveAs(outfile+'_3lepton_lepton_eta.png')
        tppt.Draw()
        c1.SaveAs(outfile+'_3lepton_parton_pt.png')
        tpeta.Draw()
        c1.SaveAs(outfile+'_3lepton_parton_eta.png')
        tdeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_3lepton_deltaR.png')
        flpt.Draw()
        c1.SaveAs(outfile+'_4lepton_lepton_pt.png')
        fleta.Draw()
        c1.SaveAs(outfile+'_4lepton_lepton_eta.png')
        fppt.Draw()
        c1.SaveAs(outfile+'_4lepton_parton_pt.png')
        fpeta.Draw()
        c1.SaveAs(outfile+'_4lepton_parton_eta.png')
        fdeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_4lepton_deltaR.png')




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
