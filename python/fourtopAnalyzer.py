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
	'Higgs0':25,
	'gh': 1000021, 'GH':1000021}

######################################
def get_info(infile,particle, outfile):
#####################################
        events = Events (infile)
        handleGen = Handle ("vector<reco::GenParticle>")
        label = "genParticles"

	if particle in PDG.keys():
		# For histos 
		hmass = TH1F( particle+' mass', particle+' mass',100,480,520) # gh 500 MeV 
		#hmass = TH1F( particle+' mass', particle+' mass',100,980,1020)  # gh 1 TeV
		#hmass = TH1F( particle+' mass', particle+' mass',100,160,180)  # 4Top 
		hmass.SetXTitle(particle+' Mass [GeV]')
		hnparticles = TH1F('number of '+particle+' and '+particle+'bar','number of '+particle+' and '+particle+'bar',5,2,6)
		#hpt2 = TH1F(particle +' pt', 'hardest '+ particle + 'pt',100,0,500)
		#hpt = TH1F('pt','pt',100,0,500)
		hpt = TH1F(particle+' pt','pt',500,0,1000)
		hpt.SetXTitle(particle+' pt [GeV]')
		heta = TH1F(particle+' eta',particle+' eta',100,-5,5)
		heta.SetXTitle(particle+' eta')

#		entry = 0
		for event in events:
			#entry += 1
			nparticles = 0
		#	print "Event ", count
			event.getByLabel (label, handleGen)
			gens = handleGen.product()
			highpt = []
    
			for p in gens:
				#print 'id',p.pdgId(), p.status(), 'status'
				if abs( int(p.pdgId()) ) == int(PDG[particle]):
					if p.status() == 22:     # 22 for ttbar, W and gh, 23 for 'final' particles
						nparticles += 1
#						highpt.append(p.pt())
#						print 'nparticles', nparticles
						hpt.Fill(float(p.pt())) 
						heta.Fill(float(p.eta())) 
						hmass.Fill(float(p.mass()))
			hnparticles.Fill(nparticles)
#			hpt2.Fill(max(highpt))

		hmass.Draw()
		c1.SaveAs(outfile+'_'+particle+'_mass.png')
#		hpt2.SetFillColor(kBlue)
#		hpt.SetFillColor(kWhite)
#		hpt2.GetYaxis().SetRangeUser(0.,400)
#	        hpt2.Draw()
#		hpt.Draw("same")
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

	hdeltaR = TH1F('deltaR','deltaR',100,0,2)

	for event in events:
#		print event
		L1 = []
		Q1 = []
		event.getByLabel (label, handleGen)
		gens = handleGen.product()
		
		for p in gens:
#			print 'particle', p.pdgId(), 'status', p.status()
			if p.status() == 23:
				if (abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13):
#					print 'name', p.pdgId(), p.status(), p.p4()
					L1.append(p.p4())
#			if p.status() == 3: 
				if 1 <= abs(p.pdgId()) <= 5:# in range(1,7)) or (p.pdgId() == 21) :
#					print 'name', p.pdgId(), p.status()
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
			if p.mother() and (abs(p.mother().pdgId()) == 24):# and  (p.status() == 23):
				#print e, p.status(), p.pdgId(), mother
 				if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)):
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
                        if p.mother() and (abs(p.mother().pdgId()) == 24):
				if (abs(p.pdgId()) == 13) and (abs(p.eta()) < 2.1):
					muons += 1
				elif (abs(p.pdgId()) == 11) and (abs(p.eta()) < 2.5):
					electrons += 1
			elif p.mother() and ((abs(p.mother().pdgId()) == 24) or (abs(p.mother().pdgId()) == 6)):
				if (1 <= abs(p.pdgId()) <= 5) and (abs(p.eta()) < 2.4):
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
                

#	hpt = TH1F('pt for all jets', 'pt for all jets',100,0,500)
	hpt2 = TH1F('partons pt for all jets', 'partons pt for all jets',100,100,700)
	hpt2.SetXTitle('parton pt [GeV]')
	heta = TH1F('partons eta for all jets','partons eta for all jets',100,-3,3)
	hdeltaR = TH1F('deltaR for all jets','deltaR for all jets',100,0,2)

#	llpt = TH1F('lpt for lepton + jets', 'lpt for lepton + jets',100,0,200)
	llpt2 = TH1F('leptons pt for lepton + jets', 'leptons pt for lepton + jets',100,0,500)
	llpt2.SetXTitle('electron+muon pt [GeV]')
	lleta = TH1F('leptons eta for lepton + jets','leptons eta for lepton + jets',100,-3,3)
#	lppt = TH1F('ppt for lepton + jets', 'ppt for lepton + jets',100,0,500)
	lppt2 = TH1F('partons pt for lepton + jets', 'partons pt for lepton + jets',100,100,700)
	lppt2.SetXTitle('partons pt [GeV]')
	lpeta = TH1F('partons eta for lepton + jets','partons eta for lepton + jets',100,-3,3)
	ldeltaR = TH1F('deltaR for lepton + jets','deltaR for lepton + jets',100,0,2.5)

#        dlpt = TH1F('lpt for dilepton + jets', 'lpt for dilepton + jets',100,0,200)
        dlpt2 = TH1F('leptons pt for dilepton + jets', 'leptons pt for dilepton + jets',100,0,500)
	dlpt2.SetXTitle('electron+muon pt [GeV]')
        dleta = TH1F('leptons eta for dilepton + jets','leptons eta for dilepton + jets',100,-3,3)
#        dppt = TH1F('ppt for dilepton + jets', 'ppt for dilepton + jets',100,0,400)
        dppt2 = TH1F('partons pt for dilepton + jets', 'partons pt for dilepton + jets',100,0,700)
	dppt2.SetXTitle('partons pt [GeV]')
        dpeta = TH1F('partons eta for dilepton + jets','partons eta for dilepton + jets',100,-3,3)
	ddeltaR = TH1F('deltaR for dilepton + jets','deltaR for dilepton + jets',100,0,2.5)

#        tlpt = TH1F('lpt for 3 lepton + jets', 'lpt for 3 lepton + jets',100,0,200)
        tlpt2 = TH1F('leptons pt for 3 lepton + jets', 'leptons pt for 3 lepton + jets',100,0,200)
	tlpt2.SetXTitle('electron+muon pt [GeV]')
        tleta = TH1F('leptons eta for 3 lepton + jets','leptons eta for 3 lepton + jets',100,-5,5)
 #       tppt = TH1F('ppt for 3 lepton + jets', 'ppt for 3 lepton + jets',100,0,500)
        tppt2 = TH1F('partons pt for 3 lepton + jets', 'partons pt for 3 lepton + jets',100,0,500)
	tppt2.SetXTitle('partons pt [GeV]')
        tpeta = TH1F('partons eta for 3 lepton + jets','partons eta for 3 lepton + jets',100,-3,3)
	tdeltaR = TH1F('deltaR for 3 lepton + jets','deltaR for 3 lepton + jets',100,0,2)

#        flpt = TH1F('lpt for 4 lepton + jets', 'lpt for 4 lepton + jets',100,50,200)
        flpt2 = TH1F('leptons pt for 4 lepton + jets', 'leptons pt for 4 lepton + jets',100,50,200)
	flpt2.SetXTitle('electron+muon pt [GeV]')
        fleta = TH1F('leptons eta for 4 lepton + jets','leptons eta for 4 lepton + jets',100,-5,5)
#        fppt = TH1F('ppt for 4 lepton + jets', 'ppt for 4 lepton + jets',100,50,300)
        fppt2 = TH1F('partons pt for 4 lepton + jets', 'partons pt for 4 lepton + jets',100,50,300)
	fppt2.SetXTitle('partons pt [GeV]')
        fpeta = TH1F('partons eta for 4 lepton + jets','partons eta for 4 lepton + jets',100,-3,3)
	fdeltaR = TH1F('deltaR for 4 lepton + jets','deltaR for 4 lepton + jets',100,0,2)

        nevent = 0
        for event in events:
                nevent += 1
		#print event
                event.getByLabel (label,handleGen)
                gens = handleGen.product()

		repo = {}
		particle = 0
		nlepton = 0
		electron = 11	
		muon = 13
                for p in gens:
			if p.mother() and (((abs(p.mother().pdgId()) == 24) and not (abs(p.pdgId()) == 24)) or ((abs(p.mother().pdgId()) == 6) and (abs(p.pdgId()) == 5))):
				particle += 1
				repo[particle] = {}
				repo[particle][p.pdgId()] = {}
				repo[particle][p.pdgId()][p.pt()] = {}
				repo[particle][p.pdgId()][p.pt()][p.eta()] = p.p4()
				nlepton = sum(electron in id for id in repo.itervalues()) + sum(muon in id for id in repo.itervalues())


#				if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)) in event:#and (p.status() == 23):
#					print p.pdgId(), p.mother().pdgId(), p.status()
#					leptonsTau +=1
#					if not (abs(p.pdgId()) == 15) : leptons += 1

		if nlepton == 0:
			kin = {}
			Q1 = []
			for nparticle in repo.keys():
				for id in repo[nparticle].keys():
					if (1 <= abs(id) <= 5):
						for pt in repo[nparticle][id].keys():
							for eta, p4 in repo[nparticle][id][pt].iteritems():
								if (abs(eta)) < 2.4:
#									#print id, pt, eta
									kin[pt] = eta
#									highpt.append(pt)
#									heta.Fill(eta)
									Q1.append(p4)
			high = dict(sorted(kin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			for pt, eta in high.iteritems():
				hpt2.Fill(pt)
				heta.Fill(eta)
#			hpt2.Fill(max(highpt))	
#			print high
			for pQuark in Q1:
				deltaR = []
				for qQuark in Q1:
					dR = (Math.VectorUtil.DeltaR(pQuark, qQuark))
					if dR != 0: deltaR.append(dR)
#				print deltaR
			hdeltaR.Fill(min(deltaR))
			#print nevent, htotal

		elif nlepton == 1:
			pkin = {}
			lkin = {}
			Q1 = []
			L1 = []
			for nparticle in repo.keys():
				for id in repo[nparticle].keys():
					for pt in repo[nparticle][id].keys():
						for eta, p4 in repo[nparticle][id][pt].iteritems():
#							print id, eta
							if ((id == muon) and (abs(eta) < 2.1)) or ((id == electron) and (abs(eta) < 2.5)): 
								lkin[pt] = eta
								L1.append(p4)
							elif (1 <= abs(id) <= 5) and (abs(eta)) < 2.4:
								pkin[pt] = eta
								Q1.append(p4)

			phigh = dict(sorted(pkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			lhigh = dict(sorted(lkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			for pt, eta in lhigh.iteritems():
				llpt2.Fill(pt)
				lleta.Fill(eta)
			for pt, eta in phigh.iteritems():
				lppt2.Fill(pt)
				lpeta.Fill(eta)
			for lepton in L1:
				deltaR = []
				for quark in Q1:
					dR = (Math.VectorUtil.DeltaR(lepton, quark))
					if dR != 0: deltaR.append(dR)
			ldeltaR.Fill(min(deltaR))

		elif nlepton == 2:
			pkin = {}
			lkin = {}
			Q1 = []
			L1 = []
			for nparticle in repo.keys():
				for id in repo[nparticle].keys():
					for pt in repo[nparticle][id].keys():
						for eta, p4 in repo[nparticle][id][pt].iteritems():
							if ((id == muon) and (abs(eta) < 2.1)) or ((id == electron) and (abs(eta) < 2.5)): 
								L1.append(p4)
								lkin[pt] = eta
							elif (1 <= abs(id) <= 5) and (abs(eta)) < 2.4:
								pkin[pt] = eta
								Q1.append(p4)

			phigh = dict(sorted(pkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			lhigh = dict(sorted(lkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			for pt, eta in lhigh.iteritems():
				dlpt2.Fill(pt)
				dleta.Fill(eta)
			for pt, eta in phigh.iteritems():
				dppt2.Fill(pt)
				dpeta.Fill(eta)
			for lepton in L1:
				deltaR = []
				for quark in Q1:
					dR = (Math.VectorUtil.DeltaR(lepton, quark))
					if dR != 0: deltaR.append(dR)
			ddeltaR.Fill(min(deltaR))


		elif nlepton == 3:
			pkin = {}
			lkin = {}
			Q1 = []
			L1 = []
			for nparticle in repo.keys():
				for id in repo[nparticle].keys():
					for pt in repo[nparticle][id].keys():
						for eta, p4 in repo[nparticle][id][pt].iteritems():
							if ((id == muon) and (abs(eta) < 2.1)) or ((id == electron) and (abs(eta) < 2.5)): 
								L1.append(p4)
								lkin[pt] = eta
							elif (1 <= abs(id) <= 5) and (abs(eta)) < 2.4:
								pkin[pt] = eta
								Q1.append(p4)

			phigh = dict(sorted(pkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			lhigh = dict(sorted(lkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			for pt, eta in lhigh.iteritems():
				tlpt2.Fill(pt)
				tleta.Fill(eta)
			for pt, eta in phigh.iteritems():
				tppt2.Fill(pt)
				tpeta.Fill(eta)
			for lepton in L1:
				deltaR = []
				for quark in Q1:
					dR = (Math.VectorUtil.DeltaR(lepton, quark))
					if dR != 0: deltaR.append(dR)
			tdeltaR.Fill(min(deltaR))

		elif nlepton == 4:
			pkin = {}
			lkin = {}
			Q1 = []
			L1 = []
			for nparticle in repo.keys():
				for id in repo[nparticle].keys():
					for pt in repo[nparticle][id].keys():
						for eta, p4 in repo[nparticle][id][pt].iteritems():
							if ((id == muon) and (abs(eta) < 2.1)) or ((id == electron) and (abs(eta) < 2.5)): 
								L1.append(p4)
								lkin[pt] = eta
							elif (1 <= abs(id) <= 5) and (abs(eta)) < 2.4:
								pkin[pt] = eta
								Q1.append(p4)

			phigh = dict(sorted(pkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			lhigh = dict(sorted(lkin.iteritems(), key=operator.itemgetter(0),reverse=True)[:1])
			for pt, eta in lhigh.iteritems():
				flpt2.Fill(pt)
				fleta.Fill(eta)
			for pt, eta in phigh.iteritems():
				fppt2.Fill(pt)
				fpeta.Fill(eta)
			for lepton in L1:
				deltaR = []
				for quark in Q1:
					dR = (Math.VectorUtil.DeltaR(lepton, quark))
					if dR != 0: deltaR.append(dR)
			fdeltaR.Fill(min(deltaR))
	

	c1 = TCanvas('c1', '', 800, 600)
	heta.Draw()
	c1.SaveAs(outfile+'_alljets_eta.png')
#	hpt2.SetFillColor(kBlue)
#	hpt.SetFillColor(kWhite)
#	hpt2.GetYaxis().SetRangeUser(0,800)
	hpt2.Draw()
#        hpt.Draw("same")
        c1.SaveAs(outfile+'_alljets_pt.png')
	hdeltaR.Draw()
	c1.SaveAs(outfile+'_alljets_deltaR.png')
	del c1

	c1 = TCanvas('c1', '', 800, 600)
#	llpt2.SetFillColor(kBlue)
#	llpt.SetFillColor(kWhite)
#	llpt2.GetYaxis().SetRangeUser(0,100)
	llpt2.Draw()
#        llpt.Draw("same")
        c1.SaveAs(outfile+'_leptonjets_lepton_pt.png')
	lleta.Draw()
        c1.SaveAs(outfile+'_leptonjets_lepton_eta.png')
#	lppt2.SetFillColor(kBlue)
#	lppt.SetFillColor(kWhite)
#	lppt2.GetYaxis().SetRangeUser(0,1000)
	lppt2.Draw()
#	lppt.Draw("same")
        c1.SaveAs(outfile+'_leptonjets_parton_pt.png')
	lpeta.Draw()
        c1.SaveAs(outfile+'_leptonjets_parton_eta.png')
        ldeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_leptonjets_deltaR.png')
	del c1
#
	c1 = TCanvas('c1', '', 800, 600)
#	dlpt2.SetFillColor(kBlue)
#	dlpt.SetFillColor(kWhite)
#	dlpt2.GetYaxis().SetRangeUser(0,50)
	dlpt2.Draw()
#        dlpt.Draw("same")
        c1.SaveAs(outfile+'_dilepton_lepton_pt.png')
        dleta.Draw()
        c1.SaveAs(outfile+'_dilepton_lepton_eta.png')
#	dppt2.SetFillColor(kBlue)
#	dppt.SetFillColor(kWhite)
#	dppt2.GetYaxis().SetRangeUser(0,500)
	dppt2.Draw()
#        dppt.Draw("same")
        c1.SaveAs(outfile+'_dilepton_parton_pt.png')
        dpeta.Draw()
        c1.SaveAs(outfile+'_dilepton_parton_eta.png')
        ddeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_dilepton_deltaR.png')
	del c1
#
	c1 = TCanvas('c1', '', 800, 600)
#	tlpt2.SetFillColor(kBlue)
#	tlpt.SetFillColor(kWhite)
#	tlpt2.GetYaxis().SetRangeUser(0,70)
	tlpt2.Draw()
#	tlpt.Draw("same")
        c1.SaveAs(outfile+'_3lepton_lepton_pt.png')
        tleta.Draw()
        c1.SaveAs(outfile+'_3lepton_lepton_eta.png')
#	tppt2.SetFillColor(kBlue)
#	tppt.SetFillColor(kWhite)
#	tppt2.GetYaxis().SetRangeUser(0,50)
	tppt2.Draw()
#        tppt.Draw("same")
        c1.SaveAs(outfile+'_3lepton_parton_pt.png')
        tpeta.Draw()
        c1.SaveAs(outfile+'_3lepton_parton_eta.png')
        tdeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_3lepton_deltaR.png')
	del c1
#
	c1 = TCanvas('c1', '', 800, 600)
#	flpt2.SetFillColor(kBlue)
#	flpt.SetFillColor(kWhite)
#	flpt2.GetYaxis().SetRangeUser(0,10)
	flpt2.Draw()
#        flpt.Draw("same")
        c1.SaveAs(outfile+'_4lepton_lepton_pt.png')
        fleta.Draw()
        c1.SaveAs(outfile+'_4lepton_lepton_eta.png')
#	fppt2.SetFillColor(kBlue)
#	fppt.SetFillColor(kWhite)
#	fppt2.GetYaxis().SetRangeUser(0,10)
	fppt2.Draw()
        fppt.Draw("same")
        c1.SaveAs(outfile+'_4lepton_parton_pt.png')
        fpeta.Draw()
        c1.SaveAs(outfile+'_4lepton_parton_eta.png')
        fdeltaR.Draw()
        c1.SetLogy()
        c1.SaveAs(outfile+'_4lepton_deltaR.png')
	del c1

######################################
def comparative(infile, infile2):
######################################
        events = Events (infile)
        events2 = Events (infile2)
        handleGen = Handle ("vector<reco::GenParticle>")
        label = "genParticles"

        lpt = TH1F('lpt for lepton + jets', 'lpt for lepton + jets',100,0,200)
        lpt2 = TH1F('leptons pt for lepton + jets', 'leptons pt for lepton + jets',100,0,200)
        ppt = TH1F('ppt for lepton + jets', 'ppt for lepton + jets',100,0,400)
        ppt2 = TH1F('partons pt for lepton + jets', 'partons pt for lepton + jets',100,0,400)


        for event in events:
		leptonsTau = 0
		leptons = 0
                event.getByLabel (label,handleGen)
                gens = handleGen.product()

                for p in gens:
			if p.status() == 3:
				if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)):# and ((abs(p.mother().pdgId()) == 11) or (abs(p.mother().pdgId()) == 13) or (abs(p.mother().pdgId()) == 15)):
					leptonsTau +=1
					if not (abs(p.pdgId()) == 15) : leptons += 1
		if leptons == 1: 
			llhighpt = []
			lphighpt = []
			for p in gens:
				if p.status() == 3:
					if (abs(p.pdgId()) in range(11,15)):
						llhighpt.append(p.pt())
					elif ((abs(p.pdgId()) in range (1,7)) or (p.pdgId() == 21)) and (abs(p.eta()) < 2.4):
						lphighpt.append(p.pt())
			lpt.Fill(max(llhighpt))
			ppt.Fill(max(lphighpt))

        for event in events2:
		leptonsTau = 0
		leptons = 0
                event.getByLabel (label,handleGen)
                gens = handleGen.product()

                for p in gens:
			if p.status() == 3:
				if ((abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13) or (abs(p.pdgId()) == 15)):# and ((abs(p.mother().pdgId()) == 11) or (abs(p.mother().pdgId()) == 13) or (abs(p.mother().pdgId()) == 15)):
					leptonsTau +=1
					if not (abs(p.pdgId()) == 15) : leptons += 1
#		print nevent, leptons
		if leptons == 1: 
			llhighpt = []
			lphighpt = []
			for p in gens:
				if p.status() == 3:
					if (abs(p.pdgId()) in range(11,15)):
						llhighpt.append(p.pt())
					elif ((abs(p.pdgId()) in range (1,7)) or (p.pdgId() == 21)) and (abs(p.eta()) < 2.4):
						lphighpt.append(p.pt())
			lpt2.Fill(max(llhighpt))
			ppt2.Fill(max(lphighpt))

	c1 = TCanvas('c1', '', 800, 600)
	lpt2.SetFillColor(kBlue)
	lpt.SetFillColor(kWhite)
	lpt2.GetYaxis().SetRangeUser(0,100)
	lpt2.Draw()
        lpt.Draw("same")
        c1.SaveAs('leptonPt_comparative'+ infile.replace('.root','')+'_'+infile2.replace('.root','')+'.png')
	del c1

	c1 = TCanvas('c1', '', 800, 600)
	ppt2.SetFillColor(kBlue)
	ppt.SetFillColor(kWhite)
	ppt2.GetYaxis().SetRangeUser(0,100)
	ppt2.Draw()
        ppt.Draw("same")
        c1.SaveAs('partonPt_comparative'+ infile.replace('.root','')+'_'+infile2.replace('.root','')+'.png')
	del c1


#######################################
def main(argv):
#######################################
	infile = None
	infile2 = None
	outfile = None
	try: 
		opts, args = getopt.getopt(argv, 'hbi:p:draec:', ['help','input=', 'particle='])
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
		elif opt == '-c':
			infile2 = arg
			comparative(infile, infile2)
		else:
			sys.exit(2)

if __name__ == '__main__':
	main(sys.argv[1:])
