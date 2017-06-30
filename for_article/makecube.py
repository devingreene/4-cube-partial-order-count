#!/usr/bin/python3

import re
from sys import argv

MAX_GENOTYPE = 15
DEGREE = 4
EDGES = 32

# See the comments in main.c for 
# an explanation of the coding
if len(argv) < 4:
	raise Exception("Usage: \
makecube.py <code> <vertical gap> <smallest horizontal gap>")

code = argv[1]
vg = float(argv[2])
shg = float(argv[3])

def make_pairs():

	""" Makes a list of directional adjacent pairs from encoding """

	mat = []
	code_0 = int(code,16)
	for miss in range(DEGREE):
		for pos in range(8):
			# low mask
			mask = (1<<miss)-1
			src = (pos&mask) + ((mask+1)*(code_0&1)) + ((pos - (pos&mask))<<1)
			dst = src^(mask+1)
			mat.append((src,dst))
			code_0 >>= 1
	return mat

def sum_bits(s):
	""" Sum the bits in a bit string """
	res = 0
	for c in s:
		res += int(c)
	return res

def make_cube(mat,color='black'):

	# File name has form "landscape+<code>+.tex"
	fd_w = open("landscape-"+"{:0>8}".format(code)+".tex","w")

	#Tikz header
	fd_w.write('\\begin{tikzpicture}\n\
[very thick,'+color+',->,outer sep=1mm]\n')
	
	#labels
	labels = ['0000','0001','0010','0011',
			'0100','0101','0110','0111',
			'1000','1001','1010','1011',
			'1100','1101','1110','1111']
	
	#provide coordinates for labels
	coord = []

	# current horizontal and vertical positions
	cur_hp = [0,2*shg,5/2*shg,2*shg,0]
	hadv = [0,4/3*shg,shg,4/3*shg,0]
	cur_vp = [-2*vg,-vg,0,vg,2*vg]

	for lab in labels:
		num_ones = sum_bits(lab)

		# Give coordinates and transform them to 
		# strings
		x,y = cur_hp[num_ones],cur_vp[num_ones]

		# Print as ordered pair
		coord.append("{}".format((x,y)))

		# Increment for next round
		cur_hp[num_ones] -= hadv[num_ones]
		
	# Write in nodes for tikz file
	for i,k in enumerate(labels):
		fd_w.write('\\node (n'+str(i)+\
			') at '+coord[i]+' {'+k+'};\n')

	# write in edges (arrows) for tikz file
	for i in range(MAX_GENOTYPE+1):
		for j in range(MAX_GENOTYPE+1):
			if (i,j) in mat:
				fd_w.write(
				'\\draw '+
				'(n'+str(i)+') -- (n'+str(j)+');\n')
	
	# Closing line
	fd_w.write('\\end{tikzpicture}\n')

	fd_w.close()

mat = make_pairs()
make_cube(mat)
