#!/usr/bin/env python
# coding: utf-8

# In[8]:


import pandas as pd
import numpy as mp
import warnings
import json

class OrderBook(object):
    def __init__(self, pair, exchange):
        """
        Input: 
            pair: string, e.g. "btcusd"
            exchange: string, with the first letter uppercase
        """
        self.pair, self.exchange = pair, exchange
        self.book, self.order = None, None
    def load(self,init_file, order_file):
        self.book = pd.read_csv(init_file)
        self.order = pd.read_csv(order_file)
        self.book.set_index('price',inplace=True)
        if (self.exchange == "Gemini"):
            self.book = self.book[['remaining','reason']]
            self.book.rename(columns={'reason':'side'},inplace=True)
            self.order = self.order[['timestamp','price','remaining','side']]
        elif (self.exchange=="Bitfinex"):
            self.book = self.book[['count','amount']]
    
        
    def track(self,timestamp):
        """
        Input: 
            timestamp as an number : int/float
        Returns:
            dataframe with price as the index 
        """
        if self.exchange == "Bitfinex":
            i = 0
            while self.order['timestamps'][i]<=timestamp:
                price = self.order['price'][i]
                count = self.order['count'][i]
                amount = self.order['amount'][i]
                if price not in self.book.index:
                    new = pd.DataFrame([[price,count,amount]],columns=['price','count','amount'])
                    new.set_index('price',inplace=True)
                    self.book = self.book.append(new,sort=True)
                    i = i + 1
                    continue
                if count == 0:
                    self.book = self.book.drop([price])
                else:
                    self.book.loc[price,'amount'] = amount
                i = i +1
        elif self.exchange == "Gemini":
            i = 0
            while self.order['timestamp'][i]<=timestamp:
                price = self.order['price'][i]
                remaining = self.order['remaining'][i]
                side = self.order['side'][i]
                if price not in self.book.index:
                    new = pd.DataFrame([[price,remaining,side]],columns=['price','remaining','side'])
                    new.set_index('price',inplace=True)
                    self.book = self.book.append(new,sort=True)
                    i = i + 1
                    continue
                if remaining == 0:
                    self.book = self.book.drop([price])
                else:
                    self.book.loc[price,'remaining'] = remaining
                i = i +1
        else:
            warnings.warn('Exchange not available')
            assert(False)
        return self.book.sort_index()
        
        

