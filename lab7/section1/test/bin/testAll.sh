#!bin/bash

for thread in {1..4}; do
	for total in 5000 10000 20000 30000; do
		echo "${thread} thread(s), ${total} total:"
		/usr/bin/time -p ./test${thread}threads${total}total
	done
done
