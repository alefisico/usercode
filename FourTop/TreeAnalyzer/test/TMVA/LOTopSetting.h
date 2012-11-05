////////////////////////////////////////
///    Loading the BDT or Other TMVA Training Input and Output directory and other settings 
///    Author: Wei Zou
///    Email:  weizou.pku@gmail.com
/////////////////////////////////////
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class LOTopSetting
{
   public:

      LOTopSetting() {}

      void LoadFile(string filename);
      string GetTrainInputDirName(){return traininputdirname_;}
      string GetTrainOutputDirName(){return trainoutputdirname_;}
      string GetTrainVariableTxtName(){return trainvariabletxtname_;}
      string GetTrainMethodName(){return trainmethodname_;}
      string GetTrainFileTxtName(){return trainfiletxtname_;}
      string GetTrainTreeName(){return traintreename_;}
      string GetApplicationInputDirName(){return applicationinputdirname_;}
      string GetApplicationOutputDirName(){return applicationoutputdirname_;}

   private:
      string traininputdirname_;
      string trainoutputdirname_;
      string trainvariabletxtname_;
      string trainmethodname_;
      string trainfiletxtname_;
      string traintreename_;
      string applicationinputdirname_;
      string applicationoutputdirname_;

      //Find the Tag
      bool findtag(string s1, string s2);
};

bool LOTopSetting::findtag(string s1, string s2)
{
    bool tmpfind = false;
    size_t found;
    found = s1.find(s2);
    if(found != string::npos)
      {tmpfind = true;}
    return tmpfind;
}     

void LOTopSetting::LoadFile(string filename)
{
   ifstream myfile (filename.c_str()); // c_str() split the file 
   string tmptraininputdirname, tmptrainoutputdirname, tmptrainvariabletxtname, tmptrainmethodname, tmptrainfiletxtname, tmptraintreename, tmpapplicationinputdirname, tmpapplicationoutputdirname;
   string tmptag;
   bool   findtag = false;
   bool   findtraininput = false;
   bool   findtrainoutput= false;
   bool   findtrainvariabletxt = false;
   bool   findtrainmethod = false;
   bool   findtrainfiletxt = false;
   bool   findtraintree = false;
   bool   findapplicationinputdir = false;
   bool   findapplicationoutputdir = false;

   if (myfile.is_open())
   {
      while ( myfile.good() )
      {   

         myfile >> tmptag;

         //Find the Tag
         findtag = findtag(tmptag,"#"); 
         if(findtag){
            findtraininput = findtag(tmptag,"Train_Input_Direc");
            findtrainoutput = findtag(tmptag,"Train_Output_Direc");
            findtrainvariabletxt = findtag(tmptag,"Train_Variable_Txt");
            findtrainmethod = findtag(tmptag,"Train_Method_Tag");
            findtrainfiletxt = findtag(tmptag,"Train_Input_FileName");
            findtraintree = findtag(tmptag,"Train_Tree_Name");
            findapplicationinputdir = findtag(tmptag,"Application_Input_Direc");
            findapplicationoutputdir = findtag(tmptag,"Application_Output_Direc");

            bool tmptotal = findtraininput || findtrainoutput || findtrainvariabletxt || findtrainmethod || findtrainfiletxt || findtraintree || findapplicationinputdir || findapplicationoutputdir;
            if(tmptotal)//if we find any Tag, we use the continue
            {
                continue;
            }
         }
          
         //Find The Train_Input_Direc Names
         if(findtraininput && !findtag)
         {  
            tmptraininputdirname = tmptag;
             traininputdirname_ = tmptraininputdirname;
            cout << tmptag << endl;
         }

         //Find The Train_Output_Direc Names
         if(findtrainoutput && !findtag)
         {
            tmptrainoutputdirname = tmptag;
            trainoutputdirname_ = tmptrainoutputdirname;
            cout << tmptag << endl;
         }

         //Find Train_Variable_Txt
         if(findtrainvariabletxt && !findtag)
         {
            tmptrainvariabletxtname = tmptag;
            trainvariabletxtname_ = tmptrainvariabletxtname; 
            cout << tmptag << endl;
         }
         
         //Find Train_Method
         if(findtrainmethod && !findtag)
         {
            tmptrainmethodname = tmptag;
            trainmethodname_ = tmptrainmethodname;
            cout << tmptag << endl;
         }
         
         //Find Train_Input_FileName_Txt
         if( findtrainfiletxt && !findtag)
         {
            tmptrainfiletxtname = tmptag;
            trainfiletxtname_ = tmptrainfiletxtname;
            cout << tmptag << endl;
         }
         
         //Find Train_Tree_Name
         if(findtraintree && !findtag)
         {
            tmptraintreename = tmptag;
            traintreename_ = tmptraintreename;
            cout << tmptag << endl;
         }
         
         //Find Application_Input_Direc
         if(findapplicationinputdir && !findtag)
         {
           tmpapplicationinputdirname = tmptag;
           applicationinputdirname_ = tmpapplicationinputdirname;
            cout << tmptag << endl;
         }
         
         //Find Application_Output_Direc
         if(findapplicationoutputdir && !findtag)
         {
           tmpapplicationoutputdirname = tmptag;
           applicationoutputdirname_ = tmpapplicationoutputdirname;
            cout << tmptag << endl;
         }

      }
   }
   myfile.close();

}
