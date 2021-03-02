#!/bin/bash

echo "parsing trade data"

BTC_USD_isbuy=( $(cut -d ',' -f6 Gemini_BTC_USD.csv) )
ETH_BTC_isbuy=( $(cut -d ',' -f6 Gemini_ETH_BTC.csv) )
ETH_USD_isbuy=( $(cut -d ',' -f6 Gemini_ETH_USD.csv) )
BTC_USD_prices_all=( $(cut -d ',' -f4 Gemini_BTC_USD.csv) )
BTC_USD_timestamps_all=( $(cut -d ',' -f8 Gemini_BTC_USD.csv) )
ETH_USD_prices_all=( $(cut -d ',' -f4 Gemini_ETH_USD.csv) )
ETH_USD_timestamps_all=( $(cut -d ',' -f8 Gemini_ETH_USD.csv) )
ETH_BTC_prices_all=( $(cut -d ',' -f4 Gemini_ETH_BTC.csv) )
ETH_BTC_timestamps_all=( $(cut -d ',' -f8 Gemini_ETH_BTC.csv) )

BTC_USD_prices=()
BTC_USD_timestamps=()
ETH_USD_prices=()
ETH_USD_timestamps=()
ETH_BTC_prices=()
ETH_BTC_timestamps=()

for i in "${!BTC_USD_isbuy[@]}"; do
	if [ "${BTC_USD_isbuy[$i]}" = "TRUE" ]; then
		BTC_USD_prices+=( "${BTC_USD_prices_all[$i]}" )
		BTC_USD_timestamps+=( "${BTC_USD_timestamps_all[$i]}" )
	fi
done

for j in "${!ETH_USD_isbuy[@]}"; do
	if [ "${ETH_USD_isbuy[$j]}" = "TRUE" ]; then
		ETH_USD_prices+=( "${ETH_USD_prices_all[$j]}" )
		ETH_USD_timestamps+=( "${ETH_USD_timestamps_all[$j]}" )
	fi
done

for k in "${!ETH_BTC_isbuy[@]}"; do
	if [ "${ETH_BTC_isbuy[$k]}" = "TRUE" ]; then
		ETH_BTC_prices+=( "${ETH_BTC_prices_all[$k]}" )
		ETH_BTC_timestamps+=( "${ETH_BTC_timestamps_all[$k]}" )
	fi
done

echo "complete!"

echo "printing to file..."

for ((i=0; i<=${#BTC_USD_prices[@]}; i++)); do
	printf '%s %s\n' "${BTC_USD_prices[i]}" "${BTC_USD_timestamps[i]}" >> output1.txt
done
column -t output1.txt > BTC_USD.dat
rm -rf output1.txt

for ((i=0; i<=${#ETH_BTC_prices[@]}; i++)); do
	printf '%s %s\n' "${ETH_BTC_prices[i]}" "${ETH_BTC_timestamps[i]}" >> output2.txt
done
column -t output2.txt > ETH_BTC.dat
rm -rf output2.txt


for ((i=0; i<=${#ETH_USD_prices[@]}; i++)); do
	printf '%s %s\n' "${ETH_USD_prices[i]}" "${ETH_USD_timestamps[i]}" >> output3.txt
done
column -t output3.txt > ETH_USD.dat
rm -rf output3.txt



