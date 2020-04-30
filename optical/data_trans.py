#!/usr/bin/env python3.7
# -*- coding: utf8 -*-

import matplotlib as mat
import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as interp
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('write', help='guardar',type=int)
parser.add_argument('plot', help='graficar',type=int)

args = parser.parse_args()
plot=args.plot
write=args.write

qe=1.602176634
c=0.299792458
h=6.62607015
Kconv=h*(1.0/qe)*c*(1e3)
data0=np.loadtxt('WLS_trans-raw.csv',skiprows=1,delimiter=',')
data1=np.loadtxt('WLS_abs.csv',skiprows=1,delimiter=',')
x=np.flip(Kconv/data0[:,0])
ydb=(1.0/1000.0)*np.flip(data0[:,1])
y=(1.0/np.log(10.0))*(10.0/ydb)
y=1.055*y+7.5e-6*(y**2.0)

xs=np.arange(1.0,5.0,0.01)
xmin,xmax=np.amin(x),np.amax(x)
cs=interp.InterpolatedUnivariateSpline(x,y,k=1,ext=1)
ys=cs(xs)
ys[xs<xmin]=3.5
ys[xs>xmax]=3.5

w=data1[:,0]
z=data1[:,1]

ws=np.arange(1.0,5.0,0.01)
cs=interp.InterpolatedUnivariateSpline(w,z,k=1,ext=3)
zs=cs(ws)
wmin,wmax=np.amin(w),np.amax(w)
test=np.logical_and(ws>=wmin,ws<=wmax)
ys[test]=5e4

print(Kconv/5.0,Kconv/1.96,np.shape(ys))
if write==True:
  f=open('scibar_optics.dat','a')
  np.savetxt(f,ys,fmt='%1.14f',newline=' ')
  f.write('\n')
  np.savetxt(f,zs,fmt='%1.14f',newline=' ')
  f.close()

if plot==True:
  fig,ax=plt.subplots(nrows=1,ncols=1)
  ax.plot(xs,ys)
  #ax.semilogy(ws,zs)
  #plt.xlim(2.4,4.0)
  plt.show()
