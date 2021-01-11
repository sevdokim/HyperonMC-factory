#!/bin/bash
#PBS -V
cd /lustre/ihep.su/user/s/sevdokim/HyperonMC_05.02.2019/2008-11_Gener
for con in t2 s4 ; do for tar in al35mm be79mm c78mm cu7mm sn5mm pb3mm ch80mm ; do rsync -R --archive nov08_${tar}_rho0_${con}/MCruns/ evdokimov@alice5.ihep.su:/data3/users/evdokimov/hyperon/Hyperon_2007_2018_Rev/cluster_MCruns/2008-11_Gener/ ; done ; done
