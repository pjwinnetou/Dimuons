#ifndef drawPlot_C
#define drawPlot_C

#include "../../CMS/tdrstyle.C"
#include "../../CMS/CMS_lumi.C"
#include "TCanvas.h"
#include "TPad.h"
#include "TFrame.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TSystem.h"

void drawPlot(RooPlot* frame, RooAbsPdf* pdf, struct InputOpt opt, bool doFit = false,bool fitExcited=false) {

   setTDRStyle();

   float txtSize = opt.oniaMode==1 ? 0.032 : 0.028;
   if (doFit) {
     float dx = opt.oniaMode==1 ? 0.61 : 0.61;
     pdf->paramOn(frame,Layout(dx,dx+0.3,0.73)) ;
     frame->getAttText()->SetTextSize(0.022);
   }

   frame->SetTitle("");
   frame->GetXaxis()->SetTitle("#mu#mu mass (GeV/c^{2})");
   frame->GetXaxis()->CenterTitle(kTRUE);
   frame->GetXaxis()->SetTitleSize(0.045);
   frame->GetXaxis()->SetTitleFont(42);

   frame->GetYaxis()->SetTitleSize(0.04);
   frame->GetYaxis()->SetTitleOffset(1.7);
   frame->GetYaxis()->SetTitleFont(42);
   //   frame->GetYaxis()->SetRangeUser(0,(nSig));
 
   TCanvas* cFig = new TCanvas("cFig", "cFig",800,800);
   cFig->cd();
 
   gPad->Update();
   frame->Draw();

   TLatex *t = new TLatex(); t->SetNDC(); t->SetTextSize(txtSize);
   float dy = 0; float deltaY = 0.001;
   if (opt.oniaMode==1) {
     deltaY = 0.08;
   } else if (opt.oniaMode==2) {
     deltaY = 0.035;
   } 

   t->SetTextSize(0.03);
   if (opt.Centrality.Start!=-1 && (opt.Centrality.Start!=0 || opt.Centrality.End!=0)) {
     t->DrawLatex(0.21, 0.81-dy, Form("Centrality: %d%% - %d%%", opt.Centrality.Start, (int)(opt.Centrality.End/2))); dy+=deltaY;
   }
   t->DrawLatex(0.21, 0.81-dy, "Soft Muons"); dy+=0.045;
   t->DrawLatex(0.21, 0.81-dy, Form("p_{T}^{#mu} > %.1f GeV/c",opt.sMuon.Pt.Min)); dy+=0.045;
   t->DrawLatex(0.21, 0.81-dy, Form("|#eta^{#mu}| < %.1f",opt.sMuon.Eta.Max)); dy+=0.045;

   if (opt.oniaMode==1){
     t->SetTextSize(0.05);
     t->DrawLatex(0.51, 0.80, "J/#psi");
     if(fitExcited){t->DrawLatex(0.78, 0.25, "#psi(2S)");}
     t->SetTextSize(0.03);
   } else if (opt.oniaMode==2) {
     t->SetTextSize(0.05);
     t->DrawLatex(0.47, 0.80, "#varUpsilon(1S)");
     if(fitExcited){t->DrawLatex(0.51, 0.47, "#varUpsilon(2S)");
     t->DrawLatex(0.56, 0.32, "#varUpsilon(3S)");}
     t->SetTextSize(0.03);
   } else if (opt.oniaMode==3) {
     t->SetTextSize(0.05);
     t->DrawLatex(0.6, 0.80, "Z");
     t->SetTextSize(0.03);
   } 
   /*
   if (opt.Centrality.Start!=-1 && (opt.Centrality.Start!=0 || opt.Centrality.End!=0)) {
     t->DrawLatex(0.21, 0.81-dy, Form("Centrality: %d%% - %d%%", opt.Centrality.Start, (int)(opt.Centrality.End/2))); dy+=deltaY;
   }
   if (opt.dMuon.AbsRap.Min==0){
     t->DrawLatex(0.21, 0.81-dy, Form("|y^{#mu^{+}#mu^{-}}| < %.1f",opt.dMuon.AbsRap.Max)); dy+=0.035;
   }else{
     t->DrawLatex(0.21, 0.81-dy, Form(" %.1f < |y^{#mu^{+}#mu^{-}}| < %.1f", opt.dMuon.AbsRap.Min, opt.dMuon.AbsRap.Max)); dy+=deltaY;
   }
   if (opt.dMuon.Pt.Min==0 && opt.dMuon.Pt.Max!=0){
     t->DrawLatex(0.21, 0.81-dy, Form("p_{T}^{#mu^{+}#mu^{-}} < %.1f GeV/c", opt.dMuon.Pt.Max));  dy+=deltaY;
   } else if (opt.dMuon.Pt.Min!=0 && opt.dMuon.Pt.Max==0){
     t->DrawLatex(0.21, 0.81-dy, Form("%.1f < p_{T}^{#mu^{+}#mu^{-}} GeV/c", opt.dMuon.Pt.Min));  dy+=deltaY;
   } else if (opt.dMuon.Pt.Min!=0 && opt.dMuon.Pt.Max!=0){
     t->DrawLatex(0.21, 0.81-dy, Form("%.1f < p_{T}^{#mu^{+}#mu^{-}} < %.1f GeV/c", opt.dMuon.Pt.Min, opt.dMuon.Pt.Max));  dy+=deltaY; 
   }
   */

   TLegend* leg = new TLegend(0.17 ,0.51,0.40,0.66); leg->SetTextSize(0.025);
   leg->AddEntry(frame->findObject("dataOS_FIT"),"Opposite charge","pe");
   leg->AddEntry(frame->findObject("dataSS_FIT"),"Same charge","pe");
   leg->Draw("same");

   //Drawing the title
   TString label;
   if (opt.RunNb.Start==opt.RunNb.End){
     label = opt.lumi + Form(" Run %d", opt.RunNb.Start);
   } else {
     label = opt.lumi + Form("[Express %d-%d]", opt.RunNb.Start,opt.RunNb.End);
   }
   CMS_lumi(cFig, opt.isPbPb ? 105 : 104, 33, label);
   cFig->Update();
   
   gSystem->mkdir(opt.plotDir, kTRUE);
   cFig->SaveAs(opt.plotDir+Form("/%sDataRun2_%d_%d_%s.png", (opt.isPbPb ? "PbPb" : "PP"), opt.RunNb.Start, opt.RunNb.End, (opt.isPbPb ? "JPsi" : "Upsilon")));
   
}

#endif // #ifndef drawPlot_C