export HYCONVERTER=/afs/ihep.su/user/s/sevdokim/HyperonMC-factory/Hyp_RegGen/converter/exe
export RECONVERTING_TOP_DIRECTORY=/lustre/ihep.su/data/hyperon/HYPERON_MC/evdokimov/reconvert_from_2008-11/
export MC_DIRECTORY=/lustre/ihep.su/data/hyperon/HYPERON_MC/evdokimov/2008-11_Gener/
export USE_TORQUE=NO
source reconvert_bash_functions.sh
/bin/ls -d $MC_DIRECTORY/nov08_be79mm_pi0_PDG_s4eff > pi0eta.list
/bin/ls -d $MC_DIRECTORY/nov08_be79mm_eta_PDG_s4eff >> pi0eta.list


for prod in $(cat pi0eta.list)
do 
    reconvert_production $prod /afs/ihep.su/user/s/sevdokim/HyperonMC-factory/2008-11_Gener 2008-11
done
