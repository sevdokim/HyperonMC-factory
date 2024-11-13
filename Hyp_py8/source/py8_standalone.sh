startdir=$(pwd)
mkdir -p ../simulations/
for sim in pi9Be pi12C pi27Al pi63Cu pi120Sn pi208Pb
do
    mkdir -p ../simulations/$sim
    cp -ap run_py8_standalone.C ../simulations/$sim
    cp -ap py8Config_${sim}.cfg ../simulations/$sim/py8Config.cfg
    echo "root -l run_py8_standalone.C" > ../simulations/$sim/run.sh
    echo "cp py8_standalone.root $startdir/py8_standalone_${sim}.root" >> ../simulations/$sim/run.sh
    cd ../simulations/$sim
    chmod a+x run.sh
    nohup ./run.sh &
    cd -
done
