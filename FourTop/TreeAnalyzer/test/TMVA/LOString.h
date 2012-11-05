////////////////////////////////////////
///    Loading the BDT or Other TMVA Training Value Names and Value Type, and Traning Directory
///    Author: Wei Zou
///    Email:  weizou.pku@gmail.com
/////////////////////////////////////
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class LOString
{
   public:

      LOString() {}

      void LoadFile(string filename);
      vector<string> GetMassPointName(){return masspointname_;}
      vector<string> GetVariableName(){return variablename_;}
      vector<string> GetVariableType(){return variabletype_;}
      vector<string> GetSpectatorName(){return spectatorname_;}
      vector<string> GetSpectatorType(){return spectatortype_;}
      vector<string> GetWeightName(){return weightname_;}
      string         GetCutName(){return cutname_;}
      string         GetCutType(){return cuttype_;}
      string         GetTrainDirectorty(){return traindirectory_;}

   private:
      vector< string > masspointname_;
      vector< string > variablename_;
      vector< string > variabletype_;
      vector< string > spectatorname_;
      vector< string > spectatortype_;
      string           traindirectory_;
      vector<string>   weightname_;
      string           cutname_;
      string           cuttype_;
      //Find the Tag
      bool findtag(string s1, string s2);
};

bool LOString::findtag(string s1, string s2)
{
    bool tmpfind = false;
    size_t found;
    found = s1.find(s2);
    if(found != string::npos)
      {tmpfind = true;}
    return tmpfind;
}     

void LOString::LoadFile(string filename)
{
   ifstream myfile (filename.c_str());
   string tmpmass, tmpvariablename, tmpvariabletype, tmpdir, tmpspectatorname, tmpspectatortype, tmpweight, tmpcut, tmpcuttype;
   string tmptag;
   bool   findtag = false;
   bool   findmass = false;
   bool   finddir = false;
   bool   findvariable = false;
   bool   findspectator = false;
   bool   findweight = false;
   bool   findcut = false;
   bool   findcuttype = false;

   if (myfile.is_open())
   {
      while ( myfile.good() )
      {   

         myfile >> tmptag;

         //Find the Tag
         findtag = findtag(tmptag,"#"); 
         if(findtag){
            finddir = findtag(tmptag,"Directory_Name");
            findvariable = findtag(tmptag,"Vairable_Name");
            findspectator = findtag(tmptag,"Spectator_Name");
            findweight = findtag(tmptag,"Weight_Name");
            findcut = findtag(tmptag,"Cut_Name");
            findcuttype = findtag(tmptag,"Cut_Type");
            findmass = findtag(tmptag,"Mass_Name");
            bool tmptotal = finddir || findvariable || findspectator || findweight || findcut || findmass || findcuttype;
            if(tmptotal)//if we find any Tag, we use the continue
            {
                continue;
            }
         }
          
         //Find the Mass Point
         if(findmass && !findtag)
         {
            tmpmass = tmptag;
            //cout << tmpmass << endl;
            masspointname_.push_back(tmpmass);
         }

         //Find the Training Directory
         if(finddir && !findtag)
         {
            tmpdir = tmptag;
            //cout << tmpdir << endl;
            traindirectory_ = tmpdir;
         }

         //Find Trainning Variable Names and Variable Types 
         if(findvariable && !findtag)
         {  
            tmpvariablename = tmptag;
            myfile >> tmpvariabletype;
            //cout << tmpvariablename << " " << tmpvariabletype << endl;
            variablename_.push_back(tmpvariablename);
            variabletype_.push_back(tmpvariabletype);
         }

         //Find Training Spectator Names and Types
         if(findspectator && !findtag)
         {
            tmpspectatorname = tmptag;
            myfile >> tmpspectatortype;
            //cout << tmpspectatorname << " " <<tmpspectatortype << endl;
            spectatorname_.push_back(tmpspectatorname);
            spectatortype_.push_back(tmpspectatortype);
         }
         
         //Find the Weight Name
         if(findweight && !findtag)
         {
             tmpweight =  tmptag;
             //cout << tmpweight << endl;
             weightname_.push_back(tmpweight);
         }

         //Find the Cut Name
         if(findcut && !findtag)
         {
            tmpcut = tmptag;
            //cout << tmpcut << endl;
            cutname_ = tmpcut;
         }

         //Find the Cut Type 
         if(findcuttype && !findtag)
         {
            tmpcuttype = tmptag;
            //cout << tmpcut << endl;
            cuttype_ = tmpcuttype;
         }
      }
   }
   myfile.close();

}
