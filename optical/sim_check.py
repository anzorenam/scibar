#!/usr/bin/env python3.7
# -*- coding: utf8 -*-

import matplotlib as mat
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

sns.set(rc={"figure.figsize":(8,4)})
sns.set_context('paper',font_scale=1.5,rc={'lines.linewidth':1.5})
sns.set_style('ticks')
mat.rc('text',usetex=True)
mat.rc('font',family='DejaVu Serif',serif='palatino')
mat.rcParams['text.latex.preamble']=[r'\usepackage[utf8]{inputenc}',r'\usepackage[T1]{fontenc}',r'\usepackage[spanish]{babel}',r'\usepackage{amsmath,amsfonts,amssymb}',r'\usepackage{siunitx}']

f=open('absp.txt','r')
a=f.readlines()
f.close()
ebins=np.arange(300,700,10)
abs=np.zeros(22153)
k=0
for elem in a:
  abs[k]=float(elem.split()[4])
  k+=1

f=open('emit.txt','r')
a=f.readlines()
f.close()
emi=np.zeros(101219)
k=0
for elem in a:
  emi[k]=float(elem.split()[3])
  k+=1



fig,ax=plt.subplots(nrows=1,ncols=1)
ax.hist(abs,bins=ebins,density=True)
ax.hist(emi,bins=ebins,density=True)
plt.xlabel(r'$\lambda \left[\si{\nano\metre}\right]$',x=0.9,ha='right')
plt.ylabel(r'$\text{Arbitrary units}$')
plt.show()
