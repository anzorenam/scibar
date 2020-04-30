#!/usr/bin/env python3.7
# -*- coding: utf8 -*-

import matplotlib as mat
import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as interp
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('conv', help='convertir de longitud a eV',type=int)
parser.add_argument('w', help='guardar',type=int)
parser.add_argument('plot', help='graficar',type=int)

args = parser.parse_args()
conv=args.conv
plot=args.plot
w=args.w

qe=1.602176634
c=0.299792458
h=6.62607015
Kconv=h*(1.0/qe)*c*(1e3)
data=np.loadtxt('rindex_pmma-raw.csv',skiprows=1,delimiter=',')
if conv==True:
  x=np.flip(Kconv/data[:,0])
  y=np.flip(data[:,1])
else:
  x=data[:,0]
  y=data[:,1]
print(Kconv/5.0,Kconv/1.0)

cs=interp.CubicSpline(x,y)
xs=np.arange(1.0,5.0,0.01)
ys=cs(xs)
if w==True:
  f=open('scibar_optics.dat','a')
  np.savetxt(f,ys,fmt='%1.14f',newline=' ')
  f.write('\n')
  f.close()

if plot==True:
  fig,ax=plt.subplots(nrows=1,ncols=1)
  ax.plot(x,y)
  ax.plot(xs,ys,ls=':')
  plt.show()
