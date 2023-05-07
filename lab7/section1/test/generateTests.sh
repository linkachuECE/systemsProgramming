#!/bin/bash

for threadNo in {1..4}; do
	for total in 5000 10000 20000 30000; do
		make total=${total} threads=${threadNo}
	done
done
