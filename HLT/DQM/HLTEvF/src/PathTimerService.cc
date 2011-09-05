// -*- C++ -*-
//
// Package:     Services
// Class  :     PathTimerService
// 
// Implementation:
//
// Original Author:  Jim Kowalkowski
// Modified by: Alejandro Gomez
// $Id: PathTimerService.cc,v 1.14 2008/07/30 09:35:08 wittich Exp $
//

#include "DQM/HLTEvF/interface/PathTimerService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include <iostream>
#include <string>
#include <vector>
#include <sys/time.h>
#include <sstream>
#include <math.h>

using namespace std ;
namespace edm {
    namespace service {

        static double getTime()  {
            struct timeval t;
            if(gettimeofday(&t,0)<0)
                throw cms::Exception("SysCallFailed","Failed call to gettimeofday");
      
            return (double)t.tv_sec + (double(t.tv_usec) * 1E-6);
        }

        edm::CPUTimer* PathTimerService::_CPUtimer = 0;       // for total time event by adding modules
        edm::CPUTimer* PathTimerService::_CPUtimerevent = 0;  // for total time event as stop watch
        edm::CPUTimer* PathTimerService::_CPUtimerModules = 0;  // for total time only for modules

        PathTimerService::PathTimerService(const ParameterSet& iPS, ActivityRegistry&iRegistry):
            total_event_count_(0),
            _perfInfo(new HLTPerformanceInfo)
        {
            iRegistry.watchPostBeginJob(this,&PathTimerService::postBeginJob);
            iRegistry.watchPostEndJob(this,&PathTimerService::postEndJob);
            
            iRegistry.watchPreProcessEvent(this,&PathTimerService::preEventProcessing);
            iRegistry.watchPostProcessEvent(this,&PathTimerService::postEventProcessing);
      
            iRegistry.watchPreModule(this,&PathTimerService::preModule);
            iRegistry.watchPostModule(this,&PathTimerService::postModule);

            iRegistry.watchPostProcessPath(this,&PathTimerService::postPathProcessing);
            
            _myPS=iPS;

            if (!_CPUtimer) _CPUtimer = new edm::CPUTimer();              // for total time event by adding modules
            if (!_CPUtimerevent) _CPUtimerevent = new edm::CPUTimer();    // for total time event as stop watch
            if (!_CPUtimerModules) _CPUtimerModules = new edm::CPUTimer(); 
        }


        PathTimerService::~PathTimerService() {
            if (_CPUtimer) {
                delete _CPUtimer ;
                _CPUtimer = 0 ;
            }
            if (_CPUtimerevent) {
                delete _CPUtimerevent ;
                _CPUtimerevent = 0;
            }  
	    if (_CPUtimerModules) {
                delete _CPUtimerModules ;
                _CPUtimerModules = 0;
            }
        }


        void PathTimerService::postBeginJob() {

            edm::Service<edm::service::TriggerNamesService> tns;
            std::vector<std::string> trigPaths= tns->getTrigPaths();
            for ( unsigned int i=0; i<trigPaths.size(); i++) {
                _pathMapping[i]=trigPaths[i];
                HLTPerformanceInfo::Path hltPath(trigPaths[i]);
                std::vector<unsigned int> loc ; 
                const std::vector<std::string> modules=tns->getTrigPathModules(trigPaths[i]);
                unsigned int mIdx = 0 ; 
                _perfInfo->addPath(hltPath);
                for ( unsigned int j=0; j<modules.size(); j++) {
                    _moduleTime[modules[j]]=0. ;
                    _moduleCPUTime[modules[j]]=0. ;
                    HLTPerformanceInfo::Modules::const_iterator iMod =
                        _perfInfo->findModule(modules[j].c_str());
                    if ( iMod == _perfInfo->endModules() ) {
                        HLTPerformanceInfo::Module hltModule(modules[j].c_str(),0.,0.,hlt::Ready);
                        _perfInfo->addModule(hltModule);
                    }

                    //--- Check the module frequency in the path ---//
                    bool duplicateModule = false ; 
                    for (unsigned int k=0; k<j; k++) {
                        if (modules[k] == modules[j]) {
                            if (!duplicateModule) loc.push_back(k) ; 
                            duplicateModule = true ;
                        }
                    }
                    if (!duplicateModule) {
		      _perfInfo->addModuleToPath(modules[j].c_str(),trigPaths[i].c_str());
                        loc.push_back(mIdx++) ; 
                    }
                }
                _newPathIndex.push_back(loc) ;
            }
            curr_job_ = getTime();
        }

    void PathTimerService::postEndJob() {}

    void PathTimerService::preEventProcessing(const edm::EventID& iID,
				    const edm::Timestamp& iTime) {
      curr_event_ = iID;
      curr_event_time_ = getTime();

      _perfInfo->clearModules();
      
      std::map<std::string, double>::iterator iter=_moduleTime.begin();
      std::map<std::string, double>::iterator iCPU=_moduleCPUTime.begin();

      while ( iter != _moduleTime.end()) {
	(*iter).second=0.;
	iter++;
      }
      while ( iCPU != _moduleCPUTime.end()) {
	(*iCPU).second=0.;
	iCPU++;
      }

    }
    void PathTimerService::postEventProcessing(const Event& e, const EventSetup&)  {
      total_event_count_ = total_event_count_ + 1;
	_CPUtimerModules->reset();
        _perfInfo->modStatus(); 
    }

    void PathTimerService::preModule(const ModuleDescription& desc) {
        _CPUtimer->reset() ; 
        _CPUtimer->start() ;
	//double tWall = _CPUtimer->realTime() ;
        //_CPUtimerModules->start() ;
	//double tWallModules = _CPUtimerModules->realTime() ;
	//std::cout << "preMod = " << tWall << ", preMod_Mod = " << tWallModules << std::endl ;

	//----------------- for in between modules time
          HLTPerformanceInfo::Modules::iterator iMod =
              _perfInfo->findModule(desc.moduleLabel().c_str());
        _CPUtimerModules->stop() ;
	double tbModules = _CPUtimerModules->realTime();
	//std::cout << desc.moduleLabel().c_str() << ", Time between this module = " << tbModules << std::endl;
	//std::cout << desc.moduleLabel().c_str() << ", " << tbModules << std::endl;

    }

      void PathTimerService::postModule(const ModuleDescription& desc) {

          _CPUtimer->stop() ;
          double tWall = _CPUtimer->realTime() ; 
          //----------------------------------------------------
          //_CPUtimerModules->stop() ;
          //double tWallModules = _CPUtimerModules->realTime() ;
	  //double tiWallevent = _CPUtimerevent->realTime();
 	  //double Twall = _perfInfo->totalTime();
          //double timWallevent1 = tiWallevent - Twall;  
          //double timWallevent2 = tiWallevent - tWallModules;
          //std::cout << desc.moduleLabel().c_str() << ", time event 1 = " << tiWallevent << ", time per module = " << tWall << ", total time = " << Twall << ", Module time = " << tWallModules << ", diff1 = " << timWallevent1 << ", diff2 = " << timWallevent2 << std::endl ;
//          std::cout << desc.moduleLabel().c_str() << "," << tWall << std::endl;
          //----------------------------------------------------
          double tCPU  = _CPUtimer->cpuTime() ;
          _CPUtimer->reset() ; 
      
          _moduleTime[desc.moduleLabel()] = tWall ;
          _moduleCPUTime[desc.moduleLabel()] = tCPU ; 

       
          HLTPerformanceInfo::Modules::iterator iMod =
              _perfInfo->findModule(desc.moduleLabel().c_str());

	//------------------------------------------------------------------
	  //double tiWallevent1 = _CPUtimerevent->realTime();
 	  //double Twall1 = _perfInfo->totalTime();
          //double tWallModules1 = _CPUtimerModules->realTime() ;
          //double tWall1 = _CPUtimer->realTime() ; 
	  //double timWallevent11 = tiWallevent1 - Twall1; 
	  //double timWallevent12 = tiWallevent1 - tWallModules1; 
	  //double timWallevent12 = Twall1 - tWallModules1; 
          //std::cout << desc.moduleLabel().c_str() << ", total time = " << Twall1 << status.state() << std::endl; //", time event 2 = " << tiWallevent1 << ", time per module = " << tWall1 << ", total time = " << Twall1 << ", Module time = " << tWallModules1 << ", diff1 = " << timWallevent11 << ", diff2 = " << timWallevent12 << std::endl ;
          //std::cout << desc.moduleLabel().c_str() << ", total time = " << Twall1 << ", between Module time = " << tWallModules1 << ", diff1 = " << timWallevent12 << std::endl ;
	//---------------------------------------------------------------------

	//--------------------------- Module Status
	//for (HLTPerformanceInfo::ModulesInPath::const_iterator j = _perfInfo->beginModules(); j != _perfInfo->endModules(); ++j){
	//	HLTPerformanceInfo::Module & mod = _perfInfo_>Modules.at(*j);
	//	mod.status()
 	//}
	//std::cout << modStatus << std::endl; 
	//double modStatus = _perfInfo->status();
	//--------------------------------------


	//---------------------- Summed Modules
          if ( iMod != _perfInfo->endModules() ) {
	    iMod->setTime(tWall) ;
	    iMod->setCPUTime(tCPU) ;
          }

	//--------------------------- for Total Time Event as Stop Watch
          //if (iMod == _perfInfo->beginModules()+1 ){
            //_CPUtimerevent->reset() ;   
            //double tiWallevent = _CPUtimerevent->realTime();
            //std::cout << "initial time = " << tiWallevent << std::endl;

            //_CPUtimerevent->start(); 
          //}

          //else if (iMod == _perfInfo->endModules()-1 ){
            //_CPUtimerevent->stop();
            //double tWallevent = _CPUtimerevent->realTime();
            //double tCPUevent  = _CPUtimerevent->cpuTime();
            //_CPUtimerevent->reset() ;

	    //---------------------------------------
	    //_CPUtimerModules->stop();
	    //double tWallModules = _CPUtimerModules->realTime();
	    //_CPUtimerModules->reset();
 	    //---------------------------------------

            //_perfInfo->setTimeEvent(tWallevent);
            //_perfInfo->setCPUTimeEvent(tCPUevent);

            //std::cout << "final time event= " << tWallevent << std::endl;//", final Module time = " << tWallModules << std::endl;
          //}
      //------------------------------------------------

	//double tiWallevent2 = _CPUtimerevent->realTime();
          //double Twall2 = _perfInfo->totalTime();
          //double tWallModules2 = _CPUtimerModules->realTime() ;
          //double tWall2 = _CPUtimer->realTime() ; 
          //double timWallevent21 = tiWallevent2 - Twall2; 
          //double timWallevent22 = tiWallevent2 - tWallModules2;
          //std::cout << desc.moduleLabel().c_str() << ", time event 3 = " << tiWallevent2 << ", time per module = " << tWall2 << ", total time = " << Twall2 << ", Module time = " << tWallModules2 << ", diff1 = " << timWallevent21 << ", diff2 = " << timWallevent22 <<  std::endl ;
	//--------------------------------------------------


	//---------------------- For in between module time
        _CPUtimerModules->reset() ;
        _CPUtimerModules->start() ;

	//----------------------------------------------
      }

      void PathTimerService::postPathProcessing(const std::string &name, 
                                                const HLTPathStatus &status) {
          HLTPerformanceInfo::PathList::iterator iPath=_perfInfo->beginPaths();
          int ctr = 0 ; 
          while ( iPath != _perfInfo->endPaths() ) {
	    if ( iPath->name() == name) { 
	      unsigned int pIndex = _newPathIndex.at(ctr).at(status.index()) ;
	      iPath->setStatus(HLTPathStatus(status.state(),pIndex)) ; 
	      _perfInfo->setStatusOfModulesFromPath(name.c_str());
	    }
	    iPath++;
	    ctr++; 
	  }

      }
  }
}
