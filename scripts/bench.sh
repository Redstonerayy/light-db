make build
cd ./src/bench
python data.py
cd ../../
mkdir -p ./run
cp ./build/Release/bench ./run/bench
cp ./src/bench/records.csv ./run/records.csv
./run/bench > ./results/benchmarks.txt
