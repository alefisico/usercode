//////////////////////////////////////////
///   Define some common functions in the same head file for reusing
///   Author: Wei Zou
///   email: weizou.pku@gmail.com
/////////////////////////////////////////
#ifndef RootUsefulFun_h
#define RootUsefulFun_h
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TParticle.h>
#include <TVector3.h>
#include <iostream>
#include <TFile.h>
#include <TMath.h>
#include <TH1.h>
#include <TLegend.h>

void CoutTH1DIntegral(TH1D *h, Double_t SF = 1. ,Double_t times = 1.){
   //This Integral function will calculated the underflow and overflow entries 
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1)/times;
   Double_t tmperror = TMath::Sqrt(tmp * SF);
   std::cout<< "The Event number of the " << h->GetTitle()<< " is :" << Form("%.1f",h->Integral(0,h->GetNbinsX() + 1)/times) << " error: " << Form("%.1f",tmperror)<< std::endl; 
}

void CoutTH2DIntegral(TH2D *h, Double_t SF = 1. ,Double_t times = 1.){
   //This Integral function will calculated the underflow and overflow entries 
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1,0,h->GetNbinsY()+1)/times;
   Double_t tmperror = TMath::Sqrt(tmp * SF);
   std::cout<< "The Event number of the " << h->GetTitle()<< " is :" << Form("%.1f",h->Integral(0,h->GetNbinsX() + 1,0,h->GetNbinsY() + 1)/times) << " error: " << Form("%.1f",tmperror)<< std::endl; 
}

Double_t NoCoutTH1DIntegral(TH1D *h, Double_t SF = 1. ,Double_t times = 1.){
   //This Integral function will calculated the underflow and overflow entries 
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1)/times;
   Double_t tmperror = TMath::Sqrt(tmp * SF);
   return tmp;
}

Double_t NoCoutTH2DIntegral(TH2D *h, Double_t SF = 1. ,Double_t times = 1.){
   //This Integral function will calculated the underflow and overflow entries 
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1,0,h->GetNbinsY() + 1)/times;
   Double_t tmperror = TMath::Sqrt(tmp * SF);
   return tmp;
}

void CoutTH1DIntegralSF(TH1D *h, Double_t SF = 1. ,Double_t times = 1.){
   //This Integral function will calculated the underflow and overflow entries 
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1) /times;
   Double_t tmperror = TMath::Sqrt(tmp);
   std::cout<< "The Event number of the " << h->GetTitle()<< " is :" << Form("%.1f",h->Integral(0,h->GetNbinsX() + 1) * SF /times) <<  " error: " << Form("%.1f",tmperror * SF )<<std::endl; 
}

void CoutTH1DWeight(TH1D *h, Double_t SF = 1., Double_t times = 1.){

   //Tis GetSumOfWeights will return thie weighted Entries, DON"T CONTAINT the overflow and underflow entries
   std::cout<< "The Event number of the " << h->GetTitle()<< " is :" << Form("%.1f",h->GetSumOfWeights()/times) << std::endl;
}

void CoutTH1DEntries(TH1D *h, Double_t SF = 1.){

   //This is the normal entires
   std::cout<< "The Event number of the " << h->GetTitle()<< " is :" << Form("%.1f",h->GetEntries() * SF) << std::endl;
}

Double_t CountTH1DIntegral(TH1D *h, Double_t SF = 1., Double_t times = 1.){
   CoutTH1DIntegral(h,SF, times);
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1) / times;
   return tmp;
}

Double_t CountTH2DIntegral(TH2D *h, Double_t SF = 1., Double_t times = 1.){
   CoutTH2DIntegral(h,SF, times);
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1,0,h->GetNbinsY() + 1) / times;
   return tmp;
}

Double_t CountTH1DIntegralSF(TH1D *h, Double_t SF = 1., Double_t times = 1.){
   CoutTH1DIntegralSF(h,SF, times);
   Double_t tmp = h->Integral(0,h->GetNbinsX() + 1) * SF / times;
   return tmp;
}

Double_t CountTH1DWeight(TH1D *h, Double_t SF = 1., Double_t times = 1.){

   CoutTH1DWeight(h,SF, times);
   Double_t tmp = h->GetSumOfWeights()/times;
   return tmp;
}

Double_t CountTH1DEntries(TH1D *h, Double_t SF = 1.){

   CoutTH1DEntries(h,SF);
   Double_t tmp = h->GetEntries() * SF;
   return tmp;
}

Double_t Integral(TH1D *h){
   
   Double_t tmp = h->Integral(0,h->GetNbinsX()+1);
   return tmp;
}

void TPadSet(){

   TCanvas *c1 = new TCanvas("c1","c1",10,10,700,700);

   TPad *pad1 = new TPad("pad1","pad1",0.00,0.25,1.00,0.97);
   TPad *pad2 = new TPad("pad2","pad2",0.00,0.00,1.00,0.25);
   pad1->SetFillColor(0);
   pad2->SetFillColor(0);
   pad1->Draw();
   pad2->Draw();
   pad1->SetTicks(1,1);

   pad2->SetTicks(1,1);
   pad2->SetGridx();
   pad2->SetGridy();

}

TString TrailStripString(TString a, string b)
{
  for(Int_t i = b.size() - 1; i > -1; i-- )
  {
         char tmpchar = dynamic_cast<char>b[i];
         a.Remove(TString::kTrailing,tmpchar);
  }

  return a;
}

//Change the Weight into the Format: Weight1 * Weight2 * Weight3....
string WeightMerge(vector<string> weight)
{
   string tmpweight = "";
   string seperate = "*";
   for(Int_t i = 0; i < weight.size(); i++)
   {
      tmpweight = tmpweight + weight[i] + seperate;
   }
   tmpweight = TrailStripString(tmpweight,seperate);
   return tmpweight;
}

void LegendSet(TLegend *L){

   //TLegend *L = new TLegend(0.3663793,0.5911017,0.6364943,0.8919492);
   L->SetBorderSize(0);
   L->SetLineStyle(0);
   L->SetTextFont(42);
   L->SetFillStyle(0);
   L->SetMargin(0.12);
   L->SetTextSize(0.025);
   L->SetFillColor(10);
   L->SetBorderSize(0);

}
#endif
