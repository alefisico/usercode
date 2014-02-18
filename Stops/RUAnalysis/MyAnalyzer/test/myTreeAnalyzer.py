#!/usr/bin/env python

'''
File: myTreeAnalyzer.py
Author: Alejandro Gomez Espinosa
Description: Analyzer in python
'''

from ROOT import *
import sys, os


def analyzer( file ):
	"""docstring for analyzer"""

	f = TFile("tree.root", "recreate")
	######## Book Histograms

	histosPtName = [ 'jetPt',
			'1stjetPt',
			'2ndjetPt']
	histosEtaName = [ 'jetEta',
			'1stjetEta']
	histosNumName = [ 'jetNum' ]

	histosPt = {}
	histosEta = {}
	histosNum = {}

	for i in histosPtName:
		histo = TH1F( i, i, 100, 0., 1000.) 
		histo.SetTitle( i )
		histo.GetXaxis().SetTitle( 'p_{T} [GeV]')
		histo.GetYaxis().SetTitle( 'Events / 10 [GeV]')
		histosPt[ i ] = histo
	for i in histosEtaName:
		histo = TH1F( i, i, 101 , -5.05, 5.05 ) 
		histo.SetTitle( i )
		histo.GetXaxis().SetTitle( '#eta')
		histo.GetYaxis().SetTitle( 'Events / 0.1')
		histosEta[i] = histo
	for i in histosNumName:
		histo = TH1F( i, i, 15, 0, 15 )
		histo.SetTitle( i )
		histo.GetXaxis().SetTitle( 'Number of Jets')
		histo.GetYaxis().SetTitle( 'Events / 1')
		histosNum[i] = histo

	# Set some formatting options
	#histos[i][j].SetMinimum(0)

	tree = TChain( 'EvTree' )
	# Loop over the filenames and add to tree.
	for filename in file:
		print("Adding file: " + filename)
		tree.Add(filename)

	# Get number of enries and make sure it's not greater than the max number
	# we passed in on the command line.
	nentries = tree.GetEntries()
	if nentries==0: # Break if empty TTree/TChain
		print "Empty files!"
		sys.exit(1)
	print "Entries: %d" % (nentries)

	# Loop over the entries
	for n in xrange (nentries):
		if n % 1000 == 0: print "Event number %d out of %d " % (n, nentries)
		# Grab the n'th entry
		tree.GetEntry(n)
		#print n

		Jets = []

		for i in range( tree.nPFJets ):
			if ( tree.jet_PF_pt[i] > 20.0 ) and ( abs( tree.jet_PF_eta[i] ) < 2.5 ):
				jets = TLorentzVector()
				jets.SetPtEtaPhiE( tree.jet_PF_pt[i], tree.jet_PF_eta[i], tree.jet_PF_phi[i], tree.jet_PF_e[i] )
				Jets.append( jets )

		for j in range( len(Jets) ):
			histosPt['jetPt'].Fill( Jets[j].Pt() )
			histosEta['jetEta'].Fill( Jets[j].Eta() )
			histosNum['jetNum'].Fill( len(Jets) )
		if len(Jets) > 0 : histosPt['1stjetPt'].Fill( Jets[0].Pt() )
		if len(Jets) > 1 : histosPt['2ndjetPt'].Fill( Jets[1].Pt() )


	f.Write()
	f.Close()
	del tree

######################################
if __name__ == '__main__':

	inputDir = '/cms/dan/FourJets/Signal/Ntuples/stopUDD_M100_312_100k_none/'
	outlist = os.popen('ls -1 '+inputDir+'*tree.root').read().splitlines()
	print outlist
	file = ['/cms/dan/FourJets/Signal/Ntuples/stopUDD_M100_312_100k_none/stopUDD_M100_312_100k_none_FullSimSummer12_TLBSM53xv5_0_tree.root',
			'/cms/dan/FourJets/Signal/Ntuples/stopUDD_M100_312_100k_none/stopUDD_M100_312_100k_none_FullSimSummer12_TLBSM53xv5_100_tree.root',
			'/cms/dan/FourJets/Signal/Ntuples/stopUDD_M100_312_100k_none/stopUDD_M100_312_100k_none_FullSimSummer12_TLBSM53xv5_101_tree.root']

	analyzer( outlist )
