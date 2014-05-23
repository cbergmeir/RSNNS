RSNNS
=====

RSNNS: Neural Networks in R using the Stuttgart Neural Network Simulator (SNNS)

Possible TODOs for the next version:

- fix remaining memory leaks, detected by valgrind, e.g. this one:

==32137== 206,480 (80 direct, 206,400 indirect) bytes in 1 blocks are definitely lost in loss record 2,312 of 2,347
==32137==    at 0x4A081D4: calloc (in /usr/lib64/valgrind/vgpreload_memcheck-amd64-linux.so)
==32137==    by 0x1019F8DD: SnnsCLib::allocMixupArray() (dlvq_learn.cpp:212)
==32137==    by 0x101A0B28: SnnsCLib::allocArrays() (dlvq_learn.cpp:942)
==32137==    by 0x101A10B7: SnnsCLib::LEARN_DLVQ(int, int, float*, int, float**, int*) (dlvq_learn.cpp:1072)
==32137==    by 0x101AB52C: SnnsCLib::kr_callNetworkFunctionSTD(int, float*, int, float**, int*, int, int) (kernel.cpp:3952)
==32137==    by 0x101AB5D6: SnnsCLib::kr_callNetworkFunction(int, float*, int, float**, int*, int, int) (kernel.cpp:4019)
==32137==    by 0x101D276F: SnnsCLib::krui_learnAllPatterns(float*, int, float**, int*) (kr_ui.cpp:3505)
==32137==    by 0x1018347A: SnnsCLib__learnAllPatterns (SnnsCLibWrapper.cpp:1278)

- Remove printf throughout the code (replace commented printf's with Rprintf, diff of commit May-15h)
- include visualization functionality as in https://gist.github.com/fawda123/7471137
- use the "colorspace" package for heatmaps
- add JSS paper as vignette

TODOs for a far away future:

- Implement Softmax/Entropy for elman?
- Make it possible to implement learning functions in R
