import numpy as np
import sys
import numpy
from market import *

class Papa_PTAP():

	def __init__(self, argv):
		self.markets={}
		self.updates_matrix={}
		self.argv=argv
		self.pair_orders=[self._get_pair(argv[0]), self._get_pair(argv[2]), self._get_pair(argv[4])]
		self.reverse=False

		if self.pair_orders[0]=='ethusd': 
			self.reverse=True

		for i in range(0, len(argv), 2):
			initial_order_file = argv[i]
    		updates_file = argv[i+1]
    		initial_order_matrix = numpy.load(initial_order_file)
    		updates_matrix = numpy.load(updates_file)
			self.markets[self._get_pair(argv[i])] = Market(initial_order_matrix, updates_matrix)
			self.updates_matrix[self._get_pair(argv[i])] = updates_matrix

   	def inspect_orderbook(self):
   		# for each row of the updates matrix of first file specified
    	# note: row[5] is the timestamp in ms
    	start=self.pair_orders[0]
    	
    	for row in range(0,self.updates_matrix[start].shape[0]):
    		time = self.updates_matrix[self.start][row][5]
    		
    		orderbook_start=self.markets[self.start].calculate_orderbook(time)
    		orderbook_im=self.markets[self.pair_orders[1]].calculate_orderbook(time)
    		orderbook_last=self.markets[self.pair_orders[2]].calculate_orderbook(time)

    		if self.reverse:
    			last_pair=orderbook_start.get_highest_bid_info()
    			im_pair=orderbook_im.get_lowest_ask_info()
    			start_pair=orderbook_last.get_lowest_ask_info()
    		else:
    			start_pair=orderbook_start.get_lowest_ask_info()
    			im_pair=orderbook_im.get_highest_bid_info()
    			last_pair=orderbook_last.get_highest_bid_info()

    		"""
				for each pair, 
					pair['price'] gives lowest ask or highest buy
					pair['volume'] gives volume

    		"""
    		
    		calc_ptap(start_pair, im_pair, last_pair)

    def calc_ptap(self, start_pair, im_pair, last_pair):
    	firstq = start_pair['price'] * start_pair['volume']
	secondq = start_pair['volume']

	if (im_pair['price'] < secondq) :
		secondq = im_pair['volume']
		firstq = start_pair['price'] * secondq

	thirdq = secondq / im_pair['price']

	if (last_pair['volume'] < thirdq) :
		thirdq = last_pair['volume']
		secondq = thirdq * im_pair['price']
		firstq = start_pair['price'] * secondq

	revenue = thirdq * last_pair['price']

	start_pair['volume'] -= secondq
	im_pair['volume'] -= secondq
	last_pair['volume'] -= thirdq

	if (revenue > firstq):
		return revenue - firstq
	else:
		return 0

    	pass 

    def _get_pair(self,file):
    	if 'btcusd' in file: 
			return 'btcusd'
		if 'ethbtc' in file: 
			return 'ethbtc'
		if 'ethusd' in file: 
			return 'ethusd'
		raise("NOT VALID PAIR")



def main(argv):
	papa = Papa_PTAP(argv)


if __name__ == '__main__':
	main(sys.argv[1:])