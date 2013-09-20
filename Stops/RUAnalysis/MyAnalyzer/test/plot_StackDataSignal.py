#!/usr/bin/env python

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptStat(0)

trigger = '4jet80_6jet60' #str ( sys.argv[1] )
#logscale = int ( sys.argv[4] )
st1 = [ 100, 200, 300, 400, 500]
st2 = [ 350, 450, 550, 650, 750]


scaledHistos = {# 'recoBjets_num':'scaleBasicPlots',
#		'recoJets_pt':'scaleBasicPlots',
#		'recoJets_1pt':'scaleBasicPlots',
#		'recoJets_2pt':'scaleBasicPlots',
#		'recoJets_HT':'scaleBasicPlots',
#		'recoBjets_num_Step1':'scaleBasicPlots',
#		'massRecoBjetsCSVM':'scaleStep1plots1D', 
		'avgMassRecoBjetsCSVM':'scaleStep1plots1D', 
		'massdijetWORecoBjetsCSVM':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'scaleStep2plots1D',
#		'massRecoDiBjetDiJet':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'scaleStep3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj10':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj20':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj30':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj40':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'scaleStep3plots1D',
#		'massRecoDiBjetDiJetSmallestDeltaM':'scaleStep4plots1D'
		}

#input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_"+trigger+"_plots.root"
#input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_"+trigger+"_plots.root"
#input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_"+trigger+"_plots.root"
#input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots_v1.root"
#input5 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_200_"+trigger+"_plots.root"
#input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_550_200_"+trigger+"_plots.root"
#input7 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_650_200_"+trigger+"_plots.root"
#input8 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_750_200_"+trigger+"_plots.root"

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

for st2mass in st2:

	for st1mass in st1:

		if ( (st2mass - st1mass) < 150 ): continue

		input0 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots.root"
		input1 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_"+str(st1mass)+"_"+trigger+"_plots.root"

		f0 = TFile(input0)
		f1 = TFile(input1)
	
		for hist1, folder in scaledHistos.iteritems():
			h0 = f0.Get(folder.replace("scaleS", "s")+'/' + hist1)
			h1 = f1.Get(folder+'/' + hist1)
		
			h0clone = h0.Clone("h0")
			h1clone = h1.Clone("h1")
			h2clone = h1.Clone("h2")
			
			h1clone.SetLineColor(2)
			h1clone.SetFillColor(2)
			h2clone.SetLineColor(2)
			
			hs = THStack( "hs", "hs" )
			hs.Add( h0clone )
			hs.Add( h1clone )

			c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
			#c.SetLogy()
			hs.Draw("hist")
			hs.SetTitle("")
			if 'pt' in hist1: hs.GetXaxis().SetTitle("p_T [GeV]")
			elif 'num' in hist1: hs.GetXaxis().SetTitle("Number of Jets")
			else: hs.GetXaxis().SetTitle("Invariant Mass [GeV]")
			if "avg" in hist1: hs.GetYaxis().SetTitle("Events / 10 GeV")
			elif "DiJet" in hist1: hs.GetYaxis().SetTitle("QuadJet / 10 GeV")
			elif "pt" in hist1: hs.GetYaxis().SetTitle("Events / 10 GeV")
			elif "num" in hist1: hs.GetYaxis().SetTitle("Events / 10 GeV")
			else: hs.GetYaxis().SetTitle("Dijet / 10 GeV")
			hs.GetYaxis().SetTitleOffset(1.2)
			h2clone.Draw("histsame")
			
			legend=TLegend(0.70,0.7,0.90,0.9)
			legend.SetFillColor(0);
			legend.AddEntry(h0clone,"DATA", "l")
			legend.AddEntry(h1clone, "jj_"+str(st2mass)+"_"+str(st1mass), "l")
			legend.Draw()
			  
			textBox=TLatex()
			textBox.SetNDC()
			textBox.SetTextSize(0.05) 
			textBox.SetTextColor(50)
			textBox.DrawText(0.10,0.91,"CMS Preliminary")
		
			textBox2=TLatex()
			textBox2.SetNDC()
			textBox2.SetTextSize(0.04) 
			textBox2.SetTextColor(50)
			textBox2.DrawLatex(0.60,0.65,"Signal Scale to 19.5 fb^{-1}")
		
			c.SaveAs(outputDir + hist1 + "_Stack_DATA_Signal_jj_"+str(st2mass)+"_"+str(st1mass)+"_"+trigger+".pdf")
			del c
		
