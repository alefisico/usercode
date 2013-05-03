#! /usr/bin/env python
#################################
# Python script to plot nice histos  
# Version 0.1 - It works but is not the best way to do it. 
#################################
import sys,getopt
import operator
from ROOT import *
#sys.argv.append('-b')
#from ROOT import TLorentzVector # triggers auto-load of libPhysics
#from ROOT import std
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
	'gh': 1000021, 'GH':1000021,
	'stop1': 1000002, "stop2":1000004}

Variables = {'Pt':1, 'M':2, 'Eta':3, 'Number':4, 
		'1Pt':11, '1Eta':12}

######################################
def get_info(infile,particle, outfile, histo, variable):
#####################################

	################# File
	#file = ROOT.TFile(particle+"_tmp.root", "recreate")  			# open new root file
	#tree = ROOT.TTree("plots", "plots")					# Open a tree
	#t.Branch(particle, n, particle'/D')

        events = Events (infile)
        handleGen = Handle ("vector<reco::GenParticle>")
        #label = "prunedGenParticles"
        label = "genParticles"
	#print handleGen
	histo.UseCurrentStyle()

	if particle in PDG.keys():
		for event in events:
			#print event
			#nparticles = 0
			event.getByLabel (label, handleGen)
			gens = handleGen.product()
			#storeParticles = std.vector(float)()
			Ptvector=[]
#    
			for p in gens:
				infoParticle = TLorentzVector()
				#print 'id',p.pdgId(), p.status(), 'status'
				#print 'name', p.pdgId(), p.status(), p.p4()
				tmpValue=0
				if abs( int(p.pdgId()) ) == int(PDG[particle]):
					if p.status() == 3:    
						#nparticles += 1
						infoParticle.SetPxPyPzE(p.px(),p.py(),p.pz(),p.et())
						Ptvector.append(infoParticle.Pt())
						if int(Variables[variable])==1: histo.Fill(infoParticle.Pt())
						if int(Variables[variable])==2: histo.Fill(infoParticle.M())
						if int(Variables[variable])==3: histo.Fill(infoParticle.Eta())
						#histo.Fill(infoParticle.E())
			if int(Variables[variable])==4: histo.Fill(len(Ptvector))
			if int(Variables[variable])==11: histo.Fill(max(Ptvector))
						
						#storeParticles.push_back(infoParticle.Et())

	can = TCanvas('can', '', 800,600)
	can.cd()
	histo.Draw()
#if histo.GetNbinsX() > 50:
#histo.GetXaxis().SetLabelSize(0.02)
#else:
#histo.GetXaxis().SetLabelSize(0.03)
	can.SetBorderMode(0)
	can.SetBorderSize(0)
	can.SetFillColor(kWhite)
	can.SetLeftMargin(0.10)
#can.SaveAs(allnames.replace('_','')+'temp.png')
	can.SaveAs(particle+'_'+variable+'temp.pdf')
	del can

		# For histos 
#		hmass = TH1F( particle+' mass', particle+' mass',100,480,520) # gh 500 MeV 
#		#hmass = TH1F( particle+' mass', particle+' mass',100,980,1020)  # gh 1 TeV
#		#hmass = TH1F( particle+' mass', particle+' mass',100,160,180)  # 4Top 
#		hmass.SetXTitle(particle+' Mass [GeV]')
#		hnparticles = TH1F('number of '+particle+' and '+particle+'bar','number of '+particle+' and '+particle+'bar',5,2,6)
#		#hpt2 = TH1F(particle +' pt', 'hardest '+ particle + 'pt',100,0,500)
#		#hpt = TH1F('pt','pt',100,0,500)
#		hpt = TH1F(particle+' pt','pt',500,0,1000)
#		hpt.SetXTitle(particle+' pt [GeV]')
#		heta = TH1F(particle+' eta',particle+' eta',100,-5,5)
#		heta.SetXTitle(particle+' eta')
#		hmass.Draw()
#		c1.SaveAs(outfile+'_'+particle+'_mass.png')
##		hpt2.SetFillColor(kBlue)
##		hpt.SetFillColor(kWhite)
##		hpt2.GetYaxis().SetRangeUser(0.,400)
##	        hpt2.Draw()
##		hpt.Draw("same")
#	        hpt.Draw()
#	        c1.SaveAs(outfile+'_'+particle+'_pt.png')
#	        heta.Draw()
#	        c1.SaveAs(outfile+'_'+particle+'_eta.png')
#	        hnparticles.Draw()
#	        c1.SaveAs(outfile+'_'+particle+'_nparticles.png')

########################################
#def deltaR(infile,outfile):
#######################################
#	events = Events (infile)
#	handleGen = Handle ("vector<reco::GenParticle>")
#	label = "prunedGenParticles"
#
#	hdeltaR = TH1F('deltaR','deltaR',100,0,2)
#
#	for event in events:
##		print event
#		L1 = []
#		Q1 = []
#		event.getByLabel (label, handleGen)
#		gens = handleGen.product()
#		
#		for p in gens:
##			print 'particle', p.pdgId(), 'status', p.status()
#			if p.status() == 23:
#				if (abs(p.pdgId()) == 11) or (abs(p.pdgId()) == 13):
##					print 'name', p.pdgId(), p.status(), p.p4()
#					L1.append(p.p4())
##			if p.status() == 3: 
#				if 1 <= abs(p.pdgId()) <= 5:# in range(1,7)) or (p.pdgId() == 21) :
##					print 'name', p.pdgId(), p.status()
#					Q1.append(p.p4())
##		print L1
##		print Q1
#		for pLepton in L1:
#			deltaR = []
#			for pQuark in Q1:
#				deltaR.append(Math.VectorUtil.DeltaR(pLepton, pQuark))
#			hdeltaR.Fill(min(deltaR))
##
#	c1 = TCanvas('c1','c1',800,600)
#	hdeltaR.Draw()
#	c1.SetLogy()
#	c1.SaveAs(outfile+'_deltaR.png')
#
#
#	c1 = TCanvas('c1', '', 800, 600)
#	#peta2.SetFillColor(kBlue)
#	#peta.SetFillColor(kWhite)
#	#peta2.GetYaxis().SetRangeUser(0,100)
#	#peta2.GetYaxis().SetTitle("Events/20 GeV")
#	peta2.GetXaxis().SetTitle("p_{T} GeV")
#	peta2.SetStats(0)
#	peta.SetStats(0)
#	peta2.SetLineWidth(2)
#	peta2.SetLineStyle(2)
#	peta2.SetLineColor(1)
#	peta.SetLineWidth(2)
#	peta.SetLineColor(2)
#	peta2.DrawNormalized("hist")
#        peta.DrawNormalized("hist same")
#	aleg = TLegend(0.65,0.70,0.93,0.90)
#	SetOwnership( aleg, 0 )
#	aleg.SetHeader("CMS Simulation 2011")
#	aleg.SetMargin(0.12)
#	aleg.SetTextSize(0.035)
#	aleg.SetFillColor(0)
#	aleg.SetFillStyle(0)
#	aleg.SetBorderSize(0)
#	aleg.AddEntry(peta,"t #bar{t} Pythia6","L");
#	aleg.AddEntry(peta2,"t #bar{t} Pythia8","L");
#	aleg.Draw("same");
#        c1.SaveAs('jetseta_comparative'+ infile.replace('.root','')+'_'+infile2.replace('.root','')+'.pdf')
#	del c1

#######################################
def main(argv):
#######################################
	files = ['/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_12_1_Xdk.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_10_1_3xv.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_11_1_JEx.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_13_1_nxt.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_14_1_rV0.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_15_1_WdR.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_16_1_R6M.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_17_1_QDe.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_18_1_Prt.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_19_1_jSu.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_1_1_h0W.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_20_1_KMi.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_2_1_4al.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_3_1_6z4.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_4_1_dE9.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_5_1_HZK.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_6_1_f6a.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_7_1_O7S.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_8_1_ykb.root',
			'/uscms_data/d3/algomez/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD/st2_h_bb_st1_bj_250_100_AOD_9_1_fyg.root'
			]
	outfile = 'test'
	histo = TH1F( 'test', 'test',50,0,500) 

	try: 
		opts, args = getopt.getopt(argv, 'hp:v:draec:', ['help','input=', 'particle='])
		if not opts:
			print 'No options supplied. Please insert input -i and pdgID -p'
	except getopt.GetoptError,e:
		print e
		sys.exit(2)

	for opt, arg in opts:
		if opt in ('-h', '--help'):
			sys.exit(2)
#		elif opt == '-b':
#			continue

#		elif opt in ('-o', '--output'):
#			outfile = infile.replace('.root','')
		elif opt in ('-p', '--particle'):
			particle = arg
		elif opt in ('-v', '--variable'):
			variable = arg
		
			if int(Variables[variable]) == 1: 
				histo = TH1F('Pt', particle+' Pt', 50, 0, 500)
				histo.SetXTitle(particle+' pt [GeV/c^{2}]')
				histo.SetYTitle('Events/10 GeV')
			elif int(Variables[variable]) == 2: 
				histo = TH1F('Mass', particle+' Inv. Mass', 50, 0, 500)
				histo.SetXTitle(particle+' Mass [GeV/c^{2}]')
				histo.SetYTitle('Events/10 GeV')
			elif int(Variables[variable]) == 3: 
				histo = TH1F('Eta', particle+' Eta', 32 , -4.5, 4.5)
				histo.SetXTitle(particle+' #Eta')
				histo.SetYTitle('Events/0.25')
			elif int(Variables[variable]) == 4: 
				histo = TH1F('Multiplicity', 'Number of '+particle, 8, -0.5, 7.5)
				histo.SetXTitle(particle+' Multiplicity')
				histo.SetYTitle('Events/1')
			elif int(Variables[variable]) == 11: 
				variable=11
				histo = TH1F('1Pt', 'Leading Jet Pt', 50, 0, 500)
				histo.SetYTitle('Events/10 GeV')
			else:
				print 'Variable Unknown'
			
			get_info(files, particle, outfile, histo, variable)

if __name__ == '__main__':
	main(sys.argv[1:])
