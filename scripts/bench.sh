cd ..
make build
python ./src/bench/data.py
mkdir -p ./run
cp ./build/Release/bench ./run/bench
cp ./src/bench/records.csv ./run/records.csv
./run/bench >> ./results/benchmarks.txt
