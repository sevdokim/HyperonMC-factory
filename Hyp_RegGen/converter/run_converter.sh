#we need h_s_soft.dat and coeff_old.dat
cd $WD/$SUFFIX
cp $ANCHORS/h_s_new.dat .
cp $ANCHORS/coeff_old.dat .
#start converter
./exe MC_res.dat $SEED > log_converter

# you'll have file 'RunNNNN.gz' after convertion is done. NNNN is a random seed number which was used for simulation. 
#The file is ready to be analyzed.


