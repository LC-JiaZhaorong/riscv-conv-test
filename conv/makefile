##################################################################
# To run the make, you should have the following tools available:
#
#   * rvv clang/llvm (https://github.com/isrc-cas/rvv-llvm)
#   * riscv gcc elf toolchain (https://github.com/riscv/riscv-gnu-toolchain)
#   * spike (https://github.com/riscv/riscv-isa-sim)
#   * pk (https://github.com/riscv/riscv-pk)
#
##################################################################

# Change the path for your environment.


SPIKE := ~jzr/work/rvv_tools/spike

PK := pk

AS := ~jzr/work/rvv_tools/riscv64-unknown-elf-as
LD := ~jzr/work/rvv_tools/riscv64-unknown-elf-ld
CC := ~jzr/work/rvv_tools/riscv64-unknown-elf-gcc
OBJDUMP := ~jzr/work/rvv_tools/riscv64-unknown-elf-objdump

ASFLAGS = -march=rv64gcv
CFLAGSW_GCC = -Wall -Wextra -Wno-missing-field-initializers \
    -Wno-parentheses -Wno-missing-braces \
    -Wmissing-prototypes -Wfloat-equal \
    -Wwrite-strings -Wpointer-arith -Wcast-align \
    -Wnull-dereference \
    -Werror=multichar -Werror=sizeof-pointer-memaccess -Werror=return-type \
    -fstrict-aliasing

CFLAGS = $(CFLAGSW_GCC) $(CFLAGS0) $(CFLAGS1)



vec_mul: vec_mul_test.o vec_mul.o 
	$(CC) $(LDFLAGS) $^ -o $@
	$(OBJDUMP) -d $@>$@.dump
	$(SPIKE) --isa=RV64gcv pk $@


#vec_mul: start_vec_mul.o vec_mul.o 
#	$(LD) $(LDFLAGS) $^ -o $@ 
#	$(OBJDUMP) -d $@>$@.dump
#	$(SPIKE) --isa=RV64gcv pk $@
#
TEMP += vec_mul_test.o start_vec_mul.o vec_mul.o vec_mul.dump vec_mul 

clean :
	rm -f $(TEMP)
