cmake .. -DBoost_NO_SYSTEM_PATHS=TRUE -DBOOST_ROOT:PATHNAME=/afs/cern.ch/user/c/cburgard/public/boost-1.59/
make -j4
cp libRooLagrangianMorphing.so ../
cp libRooLagrangianMorphing_rdict.pcm ../