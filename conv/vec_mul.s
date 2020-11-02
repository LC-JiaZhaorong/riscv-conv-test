    .text
    .balign 4
    .global vec_mul_64e
# void
# vec_mul_64e(size_t n, char *dat, char *wt, short *output, size_t *param)
# {
#   size_t i;
#   for (i=0; i<n; i++)
#     z[i] +=  x[i] * y[i];
# }
#
# register arguments:
#     a0      n
#     a1      dat
#     a2      wt
#     a3      output
#     a4      param
#	(e8,m2)=(e16,m4)=(e32,m8)=32 elements
# 
vec_mul_64e:
	vsetvli t0, a0, e8, m4
	vle8.v v8, (a1)			# load feature
	add a1, a1, t0
	vle8.v v16, (a2)			# load weight
	add a2, a2, t0
	vwmul.vv v24, v8, v16	#feature*weight
	sub a0, a0, t0
	vsetvli t0, t0, e16, m8
	vse16.v v24, (a3)
	slli t0, t0, 1
	add a3, a3, t0
	bnez a0, vec_mul_64e		#calculate 64 elements
	ret

