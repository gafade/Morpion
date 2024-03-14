# -*- coding: utf-8 -*-
"""
Created on Wed Mar 13 11:05:22 2024

@author: gfade
"""
from numpy import random as rd

listClient=[34,67]
if len(listClient) >= 2:
    print("game found",flush=True)
    indClient1 = rd.randint(0, len(listClient) - 1)
    indClient2 = rd.randint(0, len(listClient) - 1)
    while indClient2 == indClient1:
        indClient2 = rd.randint(0, len(listClient) - 1)
        print("Attributing Inds : 1-"+str(indClient1)+"   2-"+str(indClient2),flush=True)