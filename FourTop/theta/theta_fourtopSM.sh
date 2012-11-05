#!/bin/tcsh -f

foreach Process (sm)
set version = SM
set results = results110/Nbjets3/

set Main_Dir = /uscms/home/algomez/work/CMSSW_4_2_4/src/theta/utils/examples/fourtop/
#set Sample_Dir = /uscms/home/algomez/work/CMSSW_4_2_4/src/theta/utils/examples/fourtop
#set Sample_Dir = ${Main_Dir}/${results}${version}_cls/
set Sample_Dir = ${Main_Dir}/results110_Nbjets3_${version}_cls/
#set Sample_Dir = ${Main_Dir}/results110_Nbjets3_${version}stjet_cls/
#set Sample_Dir = ${Main_Dir}/${results}${version}stjet_cls/

if ( -d $Sample_Dir ) then
else
mkdir $Sample_Dir 
endif
cd $Sample_Dir

/bin/rm -f analysis_${version}_tttt_${Process}.py
cat > analysis_${version}_tttt_${Process}.py << EOF
points = [['fourtop${Process}']]
    
# for model building:
def get_model():
    # Read in and build the model automatically from the histograms in the root file. 
    # This model will contain all shape uncertainties given according to the templates
    # which also includes rate changes according to the alternate shapes.
    # For more info about this model and naming conventuion, see documentation
    # of build_model_from_rootfile.
    model = build_model_from_rootfile('/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/mu_discsm_templates.root', include_mc_uncertainties = True)
    #model = build_model_from_rootfile('/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}mu_stjetsm_templates.root', include_mc_uncertainties = True)
            
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
#plot_exp, plot_obs = bayesian_limits(model, signal_processes = points)
plot_exp, plot_obs = bayesian_limits(model, ts="lhclike", signal_processes = points)

# plot_exp and plot_obs are instances of plotutil.plotdata. they contain x/y values and
# bands. You can do many things with these objects such as inspect the x/y/ban
# data, pass then to plotutil.plot routine to make pdf plots, ...
# Here, we will just create text files of the plot data. This is useful if you want
# to apply your own plotting routines or present the result in a text Table.
#plot_obs.write_txt('bayesian_limits_observed_fourtopsm.txt')
#plot_obs.write_txt('bayesian_limits_observed_fourtopsm_stjet.txt')
plot_obs.write_txt('cls_limits_observed_fourtopsm_stjet.txt')
#plot_exp.write_txt('bayesian_limits_observed_fourtopsm.txt')
#plot_exp.write_txt('bayesian_limits_expected_fourtopsm_stjet.txt')
plot_exp.write_txt('cls_limits_expected_fourtopsm_stjet.txt')

    #plot_exp, plot_obs = cls_limits(model,ts="lhclike", signal_processes = points)
    ##plot_exp, plot_obs = cls_limits(model, signal_processes = points)
    #plot_exp.write_txt('cls_limits_expected_'+points[0][0]+'.txt')
    #plot_obs.write_txt('cls_limits_observed_'+points[0][0]+'.txt')

# model_summary, bayesian_limits, and cls_limits also write their results to the 'report' object
# which we can ask to write its results as html page to a certain directory. Use an existing, empty
# directory and point your web browser to it.
#report.write_html('htmlout_fourtopBDT_SM')
report.write_html('htmlout_fourtopStjet_SM')

# After running theta-auto, you probably want to delete the 'analysis' directory which
# contains intermediate results. Keeping it avoids re-running theta unnecessarily for unchanged configurations
# (e.g., because you just want to change the plot). However, this directory can grow very large over time.
EOF

/bin/rm -f tttt_${Process}_limits_${version}.sh
cat > tttt_${Process}_limits_${version}.sh << EOF
#!/bin/tcsh -f

date
source /uscmst1/prod/sw/cms/setup/cshrc prod
setenv SCRAM_ARCH slc5_amd64_gcc434

set MAIN_Dir = $Sample_Dir/

cd \$MAIN_Dir

eval \`scram runtime -csh\`
echo "SCRAM_ARCH " \$SCRAM_ARCH
pwd

../../../theta-auto.py analysis_${version}_tttt_${Process}.py 
EOF

chmod +x tttt_${Process}_limits_${version}.sh

/bin/rm -f tttt_${Process}_limits_${version}.condor
cat > tttt_${Process}_limits_${version}.condor << EOF
universe = vanilla
Executable = $Sample_Dir/tttt_${Process}_limits_${version}.sh 
Requirements = Memory >= 199 &&OpSys == "LINUX"&& (Arch != "DUMMY" )&& Disk > 1000000
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Output = $Sample_Dir/tttt_${Process}_limits_${version}.stdout
Error = $Sample_Dir/tttt_${Process}_limits_${version}.stderr
Log = $Sample_Dir/tttt_${Process}_limits_${version}.condorlog
notify_user = algomez@FNAL.GOV
Arguments = 
Queue 1

EOF

condor_submit tttt_${Process}_limits_${version}.condor

echo Done.
date
end

