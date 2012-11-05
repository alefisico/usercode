////////////////////////////////////////
///    Loading the BDT or Other TMVA Training File Names: Background and Signal
///    Author: Wei Zou
///    Email:  weizou.pku@gmail.com
/////////////////////////////////////
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class LOFileName
{
   public:

      LOFileName() {}

      void LoadFile(string filename);
      vector<string> GetSignalFileName(){return signalname_;}
      vector<string> GetBackgroundFileName(){return backgroundname_;}
      vector<string> GetBackgroundClassificationFileName(){return backgroundclassificationname_;}
      vector<string> GetSignalBackgroundFileName(){return signalbackgroudnname_;}

   private:
      vector< string > signalname_;
      vector< string > backgroundname_;
      vector< string > backgroundclassificationname_;
      vector< string > signalbackgroudnname_;

      //Find the Tag
      bool findtag(string s1, string s2);
};

bool LOFileName::findtag(string s1, string s2)
{
    bool tmpfind = false;
    size_t found;
    found = s1.find(s2);
    if(found != string::npos)
      {tmpfind = true;}
    return tmpfind;
}     

void LOFileName::LoadFile(string filename)
{
   ifstream myfile (filename.c_str());
   string tmpsignalname, tmpbackname, tmpbackclassname, tmpbackclassjesupname, tmpbackclassjesdownname;
   string tmpbackclassjerupname, tmpbackclassjerdownname, tmpbackclasspuupname, tmpbackclasspudownname, tmpbackclassbtagupname, tmpbackclassbtagdownname;
   string tmptag;
   bool   findtag = false;
   bool   findsignal = false;
   bool   findback = false;
   bool   findbackclassification = false;
   signalname_.clear();
   backgroundname_.clear();
   backgroundclassificationname_.clear();
   signalbackgroudnname_.clear();

   if (myfile.is_open())
   {
      while ( myfile.good() )
      {   

         myfile >> tmptag;

         //Find the Tag
         findtag = findtag(tmptag,"#"); 
         if(findtag){
            findsignal = findtag(tmptag,"Signal_Name");
            findback = findtag(tmptag,"Back_Name");
            findbackclassification = findtag(tmptag,"BackClassification_Name");
            bool tmptotal = findsignal || findback;
            if(tmptotal)//if we find any Tag, we use the continue
            {
                continue;
            }
         }
          
         //Find The Signal Names
         if(findsignal && !findtag)
         {  
            tmpsignalname = "results_"+tmptag+".root";
            //cout << tmpsignalname << endl;
            signalname_.push_back(tmpsignalname);
            signalbackgroudnname_.push_back(tmpsignalname);
         }

         //Find The Background Names
         if(findback && !findtag)
         {
            tmpbackname = "results_"+tmptag+".root";
            //cout << tmpbackname << endl;
            backgroundname_.push_back(tmpbackname);
            signalbackgroudnname_.push_back(tmpbackname);
         }
         
         //Find The Background Names for Classification
         if(findbackclassification && !findtag)
         {
            tmpbackclassname = "results_"+tmptag+".root";
            tmpbackclassjesupname = "results_"+tmptag+"_JECUP.root";
            tmpbackclassjesdownname = "results_"+tmptag+"_JECDOWN.root";
            tmpbackclassjerupname = "results_"+tmptag+"_JERUP.root";
            tmpbackclassjerdownname = "results_"+tmptag+"_JERDOWN.root";
            tmpbackclasspuupname = "results_"+tmptag+"_PUUP.root";
            tmpbackclasspudownname = "results_"+tmptag+"_PUDOWN.root";
            tmpbackclassbtagupname = "results_"+tmptag+"_BTAGUP.root";
            tmpbackclassbtagdownname = "results_"+tmptag+"_BTAGDOWN.root";
            //cout << tmpbackname << endl;
            backgroundclassificationname_.push_back(tmpbackclassname);
            backgroundclassificationname_.push_back(tmpbackclassjesupname);
            backgroundclassificationname_.push_back(tmpbackclassjesdownname);
            backgroundclassificationname_.push_back(tmpbackclassjerupname);
            backgroundclassificationname_.push_back(tmpbackclassjerdownname);
            backgroundclassificationname_.push_back(tmpbackclasspuupname);
            backgroundclassificationname_.push_back(tmpbackclasspudownname);
            backgroundclassificationname_.push_back(tmpbackclassbtagupname);
            backgroundclassificationname_.push_back(tmpbackclassbtagdownname);
            signalbackgroudnname_.push_back(tmpbackclassname);
         }
         
      }
   }
   myfile.close();

}
