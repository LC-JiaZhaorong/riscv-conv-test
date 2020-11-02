# riscv-conv-test
#coe文件是卷积输入数据
#RISCV-OUTPUT.TXT是卷积输出
#卷积过程如下：feature（CHW:64X56X56),weight(KCHW:256X64X1X1)
  #1)在channel方向取64字节的wt数据作为一个atomic，然后与feature的atomic（取法与wt一致）卷积，得到CHW为1x56x56的输出；
  #2）重复1）过程256次，完成所有的卷积过程，得到256x56x56的数据；
  #3）整理输出数据，使其按照向量化的方法排列；
#运行：
  make  #注意修改gcc的路径
