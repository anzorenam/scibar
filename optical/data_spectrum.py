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
data=np.loadtxt('WLS_trans-raw.csv',skiprows=1,delimiter=',')
if conv==True:
  x=np.flip(Kconv/data[:,0])
  y=np.flip(data[:,1])
else:
  x=data[:,0]
  y=data[:,1]
xs=np.arange(1.0,5.0,0.01)

#cs=interp.CubicSpline(x,y)
cs=interp.InterpolatedUnivariateSpline(x,y,k=1,ext=0)
x0,x1=4.0,4.0
exp_0=np.flip(np.exp(-15.0*(xs-x0))*(xs>x0)+1.0*(xs<x0))
exp_1=np.exp(-10.0*(xs-x1))*(xs>x1)+1.0*(xs<x1)
ys=cs(xs)*exp_0*exp_1
s=interp.Akima1DInterpolator(xs,ys)
ys=s(xs)
ys=(1.0/np.amax(ys))*ys
print(Kconv/5.0,Kconv/1.96,np.shape(ys))
#ys[ys<0]=0.0
if w==True:
  f=open('scibar_optics.dat','a')
  np.savetxt(f,ys,fmt='%1.14f',newline=' ')
  f.close()

if plot==True:
  fig,ax=plt.subplots(nrows=1,ncols=1)
  ax.plot(x,y)
  ax.plot(xs,ys,ls=':')
  #plt.ylim(0,1)
  plt.show()
