cd ./testing
make build
cd ./bench
echo 'Generating Data'
python data.py
mkdir -p ./results
cd ..
echo 'Running ...'
./build/Debug/bench > ./bench/results/benchmarks.txt
