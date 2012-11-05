#!/bin/tcsh -f

#foreach Process (0050 0075 0100 0125 0150 0175 0200)
foreach Process (0225 0250 0275 0300 0325 0350)
set version = UED6
set results = results110/Nbjets3/

set Main_Dir = /uscms/home/algomez/work/CMSSW_4_2_4/src/theta/utils/examples/fourtop/
set Sample_Dir = ${Main_Dir}/results110_Nbjets3_BDT${version}/ #${version}/
#set Sample_Dir = ${Main_Dir}/results110_Nbjets3_stjet${version}/

if ( -d $Sample_Dir ) then
else
mkdir $Sample_Dir 
endif
cd $Sample_Dir

/bin/rm -f analysis_${version}_tttt_UED6${Process}.py
cat > analysis_${version}_tttt_UED6${Process}.py << EOF
points = [['fourtop${Process}']]
    
# for model building:
def get_model():
    # Read in and build the model automatically from the histograms in the root file. 
    # This model will contain all shape uncertainties given according to the templates
    # which also includes rate changes according to the alternate shapes.
    # For more info about this model and naming conventuion, see documentation
    # of build_model_from_rootfile.
    model = build_model_from_rootfile('/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/mu_discued6${Process}_templates.root', include_mc_uncertainties = True)
    #model = build_model_from_rootfile('/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/mu_stjet${Process}_templates.root', include_mc_uncertainties = True)
    #model = build_model_from_rootfile('/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/mu_discsm_templates.root')
            
    # If the prediction histogram is zero, but data is non-zero, teh negative log-likelihood
    # is infinity which causes problems for some methods. Therefore, we set all histogram
    # bin entries to a small, but positive value:
    model.fill_histogram_zerobins() # default is 1e-4

    # define what the signal processes are. All other processes are assumed to make up the 
    # 'background-only' model.
    model.set_signal_processes('fourtop*')
            
    # Add some lognormal rate uncertainties. The first parameter is the name of the
    # uncertainty (which will also be the name of the nuisance parameter), the second
    # is the 'effect' as a fraction, the third one is the process name. The fourth parameter
    # is optional and denotes the channl. The default '*' means that the uncertainty applies
    # to all channels in the same way.
    # Note that you can use the same name for a systematic here as for a shape
    # systematic. In this case, the same parameter will be used; shape and rate changes 
    # will be 100% correlated.
    model.add_lognormal_uncertainty('lumi', math.log(1.022), '*')
    model.add_lognormal_uncertainty('lepeff', math.log(1.03), '*')
    model.add_lognormal_uncertainty('hlteff', math.log(1.03), '*')
    model.add_lognormal_uncertainty('wjets_rate', math.log(1.3), 'wjets')
    model.add_lognormal_uncertainty('zjets_rate', math.log(1.3), 'zjets')
    model.add_lognormal_uncertainty('ttbar_rate', math.log(1.15), 'ttbar')
    model.add_lognormal_uncertainty('singletop_rate', math.log(1.15), 'singletop')
                            
    return model

model = get_model()

# first, it is a good idea to generate a summary report to make sure everything has worked
# as expected. The summary will generate quite some information which should it make easy to spot
# errors like typos in the name of uncertainties, missing shape uncertaintie, etc.
model_summary(model)

# 2. apply the methods

# 2.a. Bayesian limits
# Calculate expected and observed Bayesian limits. For faster run time of this example,
# only make a few mass points. (Omitting the 'signal_procsses' parameter completely would
# process all signals defined as signal processes before; see Section "Common Parameters"
# on the theta auto intro doxygen page for details)
#res = ml_fit_coefficients(model, signal_processes = [''], nuisance_constraint="shape:fix")
#for p in res['']['mu_disc*']:
#    print('%s: %f' % (p, res['']['mu_disc*'][p]))
plot_exp, plot_obs = bayesian_limits(model, signal_processes = points)
#plot_exp, plot_obs = bayesian_limits(model, ts="lhclike", signal_processes = points)

# plot_exp and plot_obs are instances of plotutil.plotdata. they contain x/y values and
# bands. You can do many things with these objects such as inspect the x/y/ban
# data, pass then to plotutil.plot routine to make pdf plots, ...
# Here, we will just create text files of the plot data. This is useful if you want
# to apply your own plotting routines or present the result in a text Table.
plot_exp.write_txt('bayesian_limits_expected_BDT'+points[0][0]+'_${version}.txt')
plot_obs.write_txt('bayesian_limits_observed_BDT'+points[0][0]+'_${version}.txt')
#plot_exp.write_txt('cls_limits_expected_BDT'+points[0][0]+'_${version}.txt')
#plot_obs.write_txt('cls_limits_observed_BDT'+points[0][0]+'_${version}.txt')

#plot_obs.write_txt('bayesian_limits_observed_fourtopsm.txt')
#plot_exp.write_txt('cls_limits_expected_BDTfinalv6.txt')
#plot_exp.write_txt('bayesian_limits_expected_BDTfinalv1.txt')
#plot_exp.write_txt('bayesian_limits_expected_stjet_final.txt')
#plot_obs.write_txt('bayesian_limits_observed_BDTfinalv1.txt')
#plot_obs.write_txt('cls_limits_observed_BDTfinalv6.txt')
#plot_obs.write_txt('bayesian_limits_observed_stjet_final.txt')

    #plot_exp, plot_obs = cls_limits(model,ts="lhclike", signal_processes = points)
    ##plot_exp, plot_obs = cls_limits(model, signal_processes = points)
    #plot_exp.write_txt('cls_limits_expected_'+points[0][0]+'.txt')
    #plot_obs.write_txt('cls_limits_observed_'+points[0][0]+'.txt')

# model_summary, bayesian_limits, and cls_limits also write their results to the 'report' object
# which we can ask to write its results as html page to a certain directory. Use an existing, empty
# directory and point your web browser to it.
#report.write_html('htmlout_fourtopBDT'+points[0][0]+'_$version')
#report.write_html('htmlout_fourtopstjet'+points[0][0])
#report.write_html('htmlout_fourtopBDT_SM')
#report.write_html('htmlout_fourtopBDTvfinalv1')
#report.write_html('htmlout_fourtop_stjet_final')
#report.write_html('htmlout_stjet_'+points[0][0])

# After running theta-auto, you probably want to delete the 'analysis' directory which
# contains intermediate results. Keeping it avoids re-running theta unnecessarily for unchanged configurations
# (e.g., because you just want to change the plot). However, this directory can grow very large over time.

#mlfitres = ml_fit(model,nuisance_constraint = 'shape:fix',signal_processes = [''])
#lumimean = mlfitres['']['lumi'][0][0]
#pumean = mlfitres['']['pu'][0][0]
#singletopmean = mlfitres['']['singletop_rate'][0][0]
#lepeffmean = mlfitres['']['lepeff'][0][0]
#print'lumi fit mean: ' +  str(mlfitres['']['lumi'][0][0])
#print'pu   fit mean: ' +  str(mlfitres['']['pu'][0][0])
#print'singletop fit mean: ' +  str(mlfitres['']['singletop_rate'][0][0])
#print'lepeff fit mean: ' +  str(mlfitres['']['lepeff'][0][0])
#print'lumi fit error: ' +  str(mlfitres['']['lumi'][0][1])
#print'pu   fit error: ' +  str(mlfitres['']['pu'][0][1])
#print'singletop fit error: ' +  str(mlfitres['']['singletop_rate'][0][1])
#print'lepeff fit error: ' +  str(mlfitres['']['lepeff'][0][1])

EOF

/bin/rm -f tttt_UED6${Process}_limits_${version}.sh
cat > tttt_UED6${Process}_limits_${version}.sh << EOF
#!/bin/tcsh -f

date
source /uscmst1/prod/sw/cms/setup/cshrc prod
setenv SCRAM_ARCH slc5_amd64_gcc434

set MAIN_Dir = $Sample_Dir/

cd \$MAIN_Dir

eval \`scram runtime -csh\`
echo "SCRAM_ARCH " \$SCRAM_ARCH
pwd

../../../theta-auto.py analysis_${version}_tttt_UED6${Process}.py 
EOF

#chmod +x tttt_UED6${Process}_limits_${version}.sh

/bin/rm -f tttt_UED6${Process}_limits_${version}.condor
cat > tttt_UED6${Process}_limits_${version}.condor << EOF
universe = vanilla
Executable = $Sample_Dir/tttt_UED6${Process}_limits_${version}.sh 
Requirements = Memory >= 199 &&OpSys == "LINUX"&& (Arch != "DUMMY" )&& Disk > 1000000
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Output = $Sample_Dir/tttt_UED6${Process}_limits_${version}.stdout
Error = $Sample_Dir/tttt_UED6${Process}_limits_${version}.stderr
Log = $Sample_Dir/tttt_UED6${Process}_limits_${version}.condorlog
notify_user = algomez@FNAL.GOV
Arguments = 
Queue 1

EOF

condor_submit tttt_UED6${Process}_limits_${version}.condor

echo Done.
date
end

