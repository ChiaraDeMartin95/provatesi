#include "Riostream.h"
#include "TTimer.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include <TH3F.h>
#include "TNtuple.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TF1.h"
#include "TProfile.h"
#include <TTree.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TFile.h>
#include <TSpline.h>
#include <TLine.h>
void CfrPtSpectraDiffConfig(Bool_t CfrDiffPeriods=1, TString year0="2016", Int_t ishhCorr=0, Int_t NumberOfRegions=4,  Int_t type=8,  TString yearMC=/*"2015g3b1"*/"2016kl", TString yeardati=/*"2015f"*/"Run2DataRed_hXi", TString Path1=""){// Int_t HistoType=0){

 
  TString yeardati15f = "2015f";
  TString yearMC15g3b1 = "2015g3b1";
  gStyle->SetOptStat(0);

  TString PathIn;
  TString PathOut;
  TFile *filein;
  TFile *fileout;
  const Int_t numhistoType=3;

  TString NameHisto[numhistoType]={"S", "Efficiency", "SEffCorr"};
  TString Srap[2] = {"_Eta0.8", "_y0.5"};
  TString SSkipAssoc[2] = {"_AllAssoc", ""};
  const Int_t numtipo=10;
  TString tipo[numtipo]={"kK0s", "Lambda", "AntiLambda","LambdaAntiLambda", "XiNeg", "XiPos", "OmegaNeg", "OmegaPlus", "Xi", "Omega"};
  TString nomefileoutput = "CfrPtSpectraDiffConfig_"+yeardati;
  if (CfrDiffPeriods) nomefileoutput += "_CfrDiffPeriods";
  TString  nomefileoutputPDF=nomefileoutput;
  nomefileoutput += ".root";
  cout << "nome file output " << nomefileoutput << endl;
  fileout = new TFile (nomefileoutput, "RECREATE");


  TString innamedati="FinalOutput/DATA" +year0+"/invmass_distribution_thesis/invmass_distribution";
  TString innamecompleto[20];
  TString innameMC = innamedati;
  innameMC+="_MCEff";
  innameMC+=Path1;
  innamedati+=Path1;

  TString innamedataorMC[2]={innamedati, innameMC};


  const Int_t nummolt=5;
  const Int_t numzeta=1;
  const Int_t numPtV0=7;
  const Int_t numPtTrigger=1;
  const Int_t numSelTrigger=3;
  const Int_t numSelV0=6;
  const Int_t numSysTrigger = 3;
  const Int_t numSysV0 = 7;
  const Int_t numPeriod=2;
  //colori e marker diversi per jet, OJ, J+OJ *********************************
  Int_t Marker[6]={20,21,20,21, 20, 21};
  Int_t Color[6]={628,797, 433,881,991,1};
  Int_t MarkerBis[2]={23,24};
  Int_t MarkerTris[2]={20, 33};
  Int_t MarkerMBStat[6]={20,4,20,4, 20, 4};
  Int_t MarkerMBSist[6]={21,25,21,25, 21, 25};

  //cfr spettri pubblicati per le Xi
  cout << "\nprendo histo per confronto con dati pubblicati " << endl;
  TFile *filedatipubblicati = new TFile("HEPData-1583750454-v1-Table_3.root", "");
  if (!filedatipubblicati) {cout << "file dati pubblicati not there " << endl; return;}
  TDirectoryFile *dirspectra = (TDirectoryFile*)filedatipubblicati->Get("Table 3");
  if (!dirspectra)  {cout << "directory dati pubblicati not there " << endl; return;}

  TH1F* hspectrum[11];
  TH1F* hspectrum1[11];
  TH1F* hspectrum2[11];
  TH1F* hspectrum3[11];
  TH1F* hspectrumetot[11];
  TH1F* hspectrumCfr[6];
  TSpline3 *splineFio[6];

  for (Int_t i=0; i<11; i++){
    hspectrum[i] = (TH1F*)dirspectra->Get(Form("Hist1D_y%i", i+1));
    hspectrum1[i] = (TH1F*)dirspectra->Get(Form("Hist1D_y%i_e1", i+1));
    hspectrum2[i] = (TH1F*)dirspectra->Get(Form("Hist1D_y%i_e2", i+1));
    hspectrum3[i] = (TH1F*)dirspectra->Get(Form("Hist1D_y%i_e3", i+1));

    if (!hspectrum[i] ||     !hspectrum1[i] || !hspectrum2[i]|| !hspectrum3[i] ) { cout << "histo is missing " << endl; return;}
    hspectrumetot[i]= (TH1F*)    hspectrum3[i]->Clone(Form("Hist1D_y%i_etot", i+1));

    //    cout << " I clone a histo " << endl;                                                                                                     
    if (i<=nummolt)    hspectrumCfr[i] = (TH1F*)hspectrum[i]->Clone(Form("Spectrum_mult%i",i+1));
    //    cout << " I clone a histo " << endl;                                                                                                     
    for (Int_t b=1; b<= hspectrumCfr[0]->GetNbinsX();b++){
      hspectrumetot[i]->SetBinContent(b,   sqrt(pow(hspectrum1[i]->GetBinContent(b),2) + pow(hspectrum2[i]->GetBinContent(b),2) + pow(hspectrum3[i]->GetBinContent(b),2)) );
    }
  }
  for (Int_t b=1; b<= hspectrumCfr[0]->GetNbinsX();b++){
    hspectrumCfr[0]->SetBinContent(b,1./5 * (hspectrum[0]->GetBinContent(b)+ hspectrum[1]->GetBinContent(b)*4));
    hspectrumCfr[0]->SetBinError(b, sqrt(pow(hspectrumetot[0]->GetBinContent(b) * 1./5,2) + pow(hspectrumetot[1]->GetBinContent(b)*4./5,2)));

    hspectrumCfr[1]->SetBinContent(b,hspectrum[2]->GetBinContent(b));
    hspectrumCfr[1]->SetBinError(b, hspectrumetot[2]->GetBinContent(b));

    hspectrumCfr[2]->SetBinContent(b,1./20 * (hspectrum[3]->GetBinContent(b)*5+ hspectrum[4]->GetBinContent(b)*5 + hspectrum[5]->GetBinContent(b)*10));
    hspectrumCfr[2]->SetBinError(b, sqrt(pow(hspectrumetot[3]->GetBinContent(b)*5./20,2) + pow(hspectrumetot[4]->GetBinContent(b)*5./20,2) + pow(hspectrumetot[5]->GetBinContent(b)*10./20,2)));

    hspectrumCfr[3]->SetBinContent(b,1./20 * (hspectrum[6]->GetBinContent(b)*10+ hspectrum[7]->GetBinContent(b)*10));
    hspectrumCfr[3]->SetBinError(b, sqrt(pow(hspectrumetot[6]->GetBinContent(b) * 10./20,2) + pow(hspectrumetot[7]->GetBinContent(b)*10./20,2)));

    hspectrumCfr[4]->SetBinContent(b,1./50 * (hspectrum[8]->GetBinContent(b)*20+ hspectrum[9]->GetBinContent(b)*30));
    hspectrumCfr[4]->SetBinError(b, sqrt(pow(hspectrumetot[8]->GetBinContent(b) * 20./50,2) + pow(hspectrumetot[9]->GetBinContent(b)*30./50,2)));

    hspectrumCfr[5]->SetBinContent(b,hspectrum[10]->GetBinContent(b));
    hspectrumCfr[5]->SetBinError(b, hspectrumetot[10]->GetBinContent(b));
  }

  for (Int_t i=0; i<6; i++){
    if (type==4 || type==5)    hspectrumCfr[i]->Scale(1./2.);// this was to consider only Xi+ or Xi-
    splineFio[i] = new TSpline3(hspectrumCfr[i],Form("splineFio_%i",i));
  }

  cout << " \n\n*************\nspline of spectra obtained from Fiorella's ones and evaluation of their mean relative error " << endl;
  Float_t  MeanRelErr[6]={0};
  Float_t  SigmaRelErr[6]={0};
  for (Int_t i=0; i<6; i++){
    splineFio[i] = new TSpline3(hspectrumCfr[i],Form("splineFio_%i",i));
    MeanRelErr[i]=0;
    SigmaRelErr[i]=0;

    for(Int_t b=1; b <= hspectrumCfr[i]->GetNbinsX(); b++){
      MeanRelErr[i]+=(hspectrumCfr[i]->GetBinError(b)/hspectrumCfr[i]->GetBinContent(b));
    }
    MeanRelErr[i]= MeanRelErr[i]/hspectrumCfr[i]->GetNbinsX();
    for(Int_t b=1; b <= hspectrumCfr[i]->GetNbinsX(); b++){
      SigmaRelErr[i]+=pow(MeanRelErr[i]-(hspectrumCfr[i]->GetBinError(b)/hspectrumCfr[i]->GetBinContent(b)),2);
    }
    SigmaRelErr[i]=sqrt( SigmaRelErr[i]/(hspectrumCfr[i]->GetNbinsX()-1));
  }

  //************************************                                       

  TString Region[4] = {"AllAssoc0.15", "SkipAssoc0.15", "AllAssoc3", "SkipAssoc3"};
  TString RegionCfrDiffPeriods[4] = {"AllAssocRun2", "SkipAssocRun2", "AllAssocLHC15", "SkipAssocLHC15"};
  if (CfrDiffPeriods){
  for (Int_t i=0; i<4; i++){
    Region[i] = RegionCfrDiffPeriods[i] ;
  }
  }
  TString isDataorMC[2]={"Data", "MC"};
  TString StringSystV0Analysis[2]={"", "_SystV0Analysis"};
  TString BkgType[2]={"BkgRetta", "BkgParab"};
  TString MassFixedPDG[2]={"", "isMeanFixedPDG_"};
  TH1F * fHistSpectrum_master[NumberOfRegions][nummolt+1];
  TH1F * fHistSpectrum_ratio[NumberOfRegions][nummolt+1];

  TCanvas * canvasSpectrum[2][numhistoType];
  for (Int_t i=0; i<2; i++){
    for (Int_t HistoType=0; HistoType<numhistoType; HistoType++){
      canvasSpectrum[i][HistoType] = new TCanvas(Form("canvasSpectrum_%i_", i)+NameHisto[HistoType], Form("canvasSpectrum_%i_", i)+NameHisto[HistoType], 1600, 800);
      canvasSpectrum[i][HistoType]->Divide(3,2);
      gPad->SetLeftMargin(0.0015);
    }
  }

  //**************** prendo gli istogrammi **********************************************
  Int_t israp=0;
  Bool_t SkipAssoc=0;
  Float_t PtTrigMin=0;
  TLegend *  legendRegion=new TLegend(0.6,0.7, 0.9, 0.9);
  TLegend *  legendRatio=new TLegend(0.6,0.7, 0.9, 0.9);
  TLegend *  legendRatioCompFio=new TLegend(0.6,0.7, 0.9, 0.9);
  TLine * lineOne = new TLine (0,1,8,1);
  TF1 * rettaOne = new TF1 ("pl0", "pol0", 0,8);
  rettaOne->FixParameter(0,1);
  Float_t Max[numhistoType] = {0.003, 0.25, 0.05};
  for (Int_t HistoType=0; HistoType<numhistoType; HistoType++){  
    for(Int_t l=0; l<NumberOfRegions; l++){
      if (l==0) {israp=1; SkipAssoc=0; PtTrigMin=0.15;}
      else     if (l==1) {israp=1; SkipAssoc=1; PtTrigMin=0.15;}
      else     if (l==2) {israp=1; SkipAssoc=0; if (CfrDiffPeriods)PtTrigMin=0.15; else PtTrigMin=3; }
      else     if (l==3) {israp=1; SkipAssoc=1; if (CfrDiffPeriods)PtTrigMin=0.15; else PtTrigMin=3; }
      if (CfrDiffPeriods){
	if (l==0 || l==1){    innamedataorMC[0]=innamedati+"_"+yeardati;    innamedataorMC[1]=innameMC+"_"+yearMC;}
	if (l==2 || l==3){     innamedataorMC[0]=innamedati+"_"+yeardati15f;    innamedataorMC[1]= innameMC+"_"+yearMC15g3b1;}
      }
      else {
	if (l==0 && HistoType==0)	innamedati+="_"+yeardati;    innameMC+="_"+yearMC;
      }

      //************************** Drawing the pt spectra
      for (Int_t m=0; m< nummolt+1; m++){
	Int_t sysTrigger=0;
	Int_t sysV0=0;
	Int_t sys=0;
	Int_t dataorMC =0;
	Int_t isMeanFixedPDG=1;
	Int_t isBkgParab=0;
	if (!CfrDiffPeriods){
	  innamedataorMC[0]=innamedati;
	  innamedataorMC[1]= innameMC;
	}
	PathIn= Form(innamedataorMC[dataorMC]+"_" +tipo[type] +Srap[israp]+SSkipAssoc[SkipAssoc]+"_"+MassFixedPDG[isMeanFixedPDG]+ BkgType[isBkgParab]+"_molt%i_sysT%i_sysV0%i_Sys%i_PtMin%.1f.root", m, sysTrigger, sysV0, sys, PtTrigMin);
	cout << PathIn << endl;
	filein=new TFile(PathIn, "");   
	fHistSpectrum_master[l][m] = (TH1F*) filein->Get("histo_"+NameHisto[HistoType]);
	if (!fHistSpectrum_master[l][m]) return;
	fHistSpectrum_master[l][m]->SetLineColor(Color[l]);
	fHistSpectrum_master[l][m]->SetMarkerColor(Color[l]);
	fHistSpectrum_master[l][m]->SetMarkerStyle(20);
	if (m==0 && HistoType==0) legendRegion->AddEntry(  fHistSpectrum_master[l][m], Region[l], "pl");
	if (type==4 || type==5 || type==8)   {
	  fHistSpectrum_master[l][m]->GetYaxis()->SetRangeUser(0,Max[HistoType]);
	}
	if (type==8 && NameHisto[HistoType]!="Efficiency"){
	  fHistSpectrum_master[l][m]->GetYaxis()->SetRangeUser(0,2*Max[HistoType]);
	}
	canvasSpectrum[0][HistoType]->cd(m+1);
	fHistSpectrum_master[l][m]->Draw("same ep");
	hspectrumCfr[m]->Sumw2();
	if (l==0)      hspectrumCfr[m]->Scale(1);
	hspectrumCfr[m]->GetYaxis()->SetRangeUser(0,0.5);
	hspectrumCfr[m]->GetXaxis()->SetRangeUser(0,8);
	hspectrumCfr[m]->SetMarkerStyle(33);
	if (!CfrDiffPeriods && NameHisto[HistoType]=="SEffCorr"){
	  hspectrumCfr[m]->Draw("same ep");
	  splineFio[m]->Draw("same");
	}
        legendRegion->Draw("");

	canvasSpectrum[1][HistoType]->cd(m+1);
	fHistSpectrum_ratio[l][m] = (TH1F*)       fHistSpectrum_master[l][m] ->Clone(NameHisto[HistoType]+"_Ratio");
	if (!CfrDiffPeriods && NameHisto[HistoType]=="SEffCorr"){
	  if (m==0) legendRatioCompFio->AddEntry(  fHistSpectrum_ratio[l][m],"Ratio " + Region[l] + "/Fiorella's" , "pl");
	  for(Int_t b=1; b <=  fHistSpectrum_ratio[l][m]->GetNbinsX(); b++){
	    fHistSpectrum_ratio[l][m]->SetBinContent(b, fHistSpectrum_master[l][m]->GetBinContent(b)/splineFio[m]->Eval(fHistSpectrum_master[l][m]->GetBinCenter(b)));
	    fHistSpectrum_ratio[l][m]->SetBinError(b, sqrt(pow(fHistSpectrum_master[l][m]->GetBinError(b),2) +pow(fHistSpectrum_ratio[l][m]->GetBinContent(b)*MeanRelErr[m]* fHistSpectrum_master[l][m]->GetBinContent(b),2)) /splineFio[m]->Eval(fHistSpectrum_master[l][m]->GetBinCenter(b))); //error calculated assuming zero correlation between my spectra and Fiorella's ; 
	    //	fHistSpectrum_ratio[l][m]->SetBinError(b, TMath::Abs(fHistSpectrum_master[l][m]->GetBinError(b) -fHistSpectrum_ratio[l][m]->GetBinContent(b)*MeanRelErr[m]* fHistSpectrum_master[l][m]->GetBinContent(b)) /splineFio[m]->Eval(fHistSpectrum_master[l][m]->GetBinCenter(b))); //error calculated assuming full correlation between my spectra and Fiorella's ;  
	  }
	}
	else {
	  //	  fHistSpectrum_ratio[l][m]->Sumw2(); //maybe already done?
	  if (l==2 || l==3){
	    if (m==0 && HistoType==0) legendRatio->AddEntry(  fHistSpectrum_ratio[l][m],"Ratio " + Region[l]+"/"+Region[l-2] , "pl");
	    fHistSpectrum_ratio[l][m]->Divide( fHistSpectrum_master[l-2][m]);
	  }
	}

	fHistSpectrum_ratio[l][m] ->GetYaxis()->SetRangeUser(0,10);
	if (CfrDiffPeriods){
	  fHistSpectrum_ratio[l][m] ->GetYaxis()->SetRangeUser(0,1.5);
	  if (l==2 || l==3)	fHistSpectrum_ratio[l][m] ->Draw("same ep");
	}
	else {
	  if (l==2 || l==3 ||  NameHisto[HistoType]=="SEffCorr"){
	fHistSpectrum_ratio[l][m] ->Draw("same ep");
	lineOne ->Draw("");
	  }
	}
	//	lineOne ->Draw("");
	if (l==3 && NameHisto[HistoType]=="SEffCorr")        legendRatioCompFio->Draw("");
	if (l==3 && NameHisto[HistoType]!="SEffCorr") legendRatio->Draw("");

      } //end loop mult
  }//endl loop histo type
    }
    cout << "...Done\n\n" << endl;

  //drawin the pt spectra
  for (Int_t i=0; i<2; i++){
    for (Int_t HistoType=0; HistoType<numhistoType; HistoType++){
      fileout->WriteTObject(canvasSpectrum[i][HistoType]);
      if (i==0 && HistoType==0) canvasSpectrum[i][HistoType]->SaveAs(nomefileoutputPDF+".pdf(");
      else   if (i!=2 && HistoType!=numhistoType-1)  canvasSpectrum[i][HistoType]->SaveAs(nomefileoutputPDF+".pdf");
      else  canvasSpectrum[i][HistoType]->SaveAs(nomefileoutputPDF+".pdf)");
    }
  }
  fileout->Close();
  cout << " I've produced the file " << nomefileoutput << endl;

}

