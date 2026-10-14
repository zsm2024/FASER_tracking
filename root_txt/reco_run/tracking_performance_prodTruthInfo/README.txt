ckf_truthBased_efficiency.C: deal with one file truthBased efficiency(using ACTS outPut(reco and truth))
ckf_truthBased_efficiency1.C : compared the efficiency of forward、backward in CKF and mergeTracks(method1 or method2) vs prod_theta/pT, include two kinds of efficiency : eff and Leff(truthParticle_nHits>=18);

ckf_trackSeparation1.C:  compared the efficiency of forward、backward in CKF and mergeTracks(method1 or method2) vs trackSeparation(ΔR1)

ckf_rate.C: deal with one file; fake rate(truthHitRatio<0.5) vs prod_theta / duplicates /dulicates vs prod_theta
ckf_rate1.C: deal with forward and backward Tracks and mergeTracks

draw_targetZ: targetZ of forward and backward
draw_varInfo: we can see the distribution of different varibles at the same time (three dark photon samples compared)

ckf_residual_perLayer.C: we can see resolution per layer for CKF. include:CommonUtils.h & SetStyle.hpp  Detail about this script in work ppt(tracking笔记)
