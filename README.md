# ase24_ETEM

This is ETEM, a compiler bug isolation method proposed in the paper "Compiler Bug Isolation via Enhanced Test Program Mutation". This GitHub repository provides the source code of ETEM, the datasets used in experiments, and the experimental results.

## File Organization
- ./benchmark: Datasets used in the experiments, including 60 real bugs on GCC (based on SVN repository) and 60 real bugs on LLVM (based on GitHub repository).

- ./experimental_result: Experimental results. Within ./experimental_result/[ETEM, ODFL, RecBi]/.../[gccbugs, llvmbugs], the results of [ETEM, ODFL, RecBi] on [gcc, llvm] datasets are stored.
    - Data in the paper: The experimental results in the paper can be found directly in [gccbugs, llvmbugs]/eval.txt.
    - Detailed rank list: Each method's bug isolation result rank list is in [gccbugs, llvmbugs]/rankFile-[bugid].

- ./mutator: Feature mutators defined by ETEM.
    - ./mutator/exefile: Executable files of feature mutators.
    - ./mutator/srcfile: Source code of feature mutators.
    - ./mutator/testFile: Compiler test suite, serving as the library of program elements inserted by add-type mutators.

- ./run: Source code of ETEM.

- ./ODFL-src: Source code of ODFL. Based on their paper and the provided ODFL code for GCC, we implemented ODFL for LLVM.

- other: The source code of RecBi is the same as that in their GitHub repository: https://github.com/haoyang9804/RecBi



## Run ETEM

```shell
https://github.com/cabbageAnonymous/ase24_anonymous.git  # Clone this repository
cd run
sudo python ./[gcc/llvm]-setup.py # Setup folder and basic bug info
sudo python ./[gcc/llvm]-run.py # Run ETEM
sudo python ./[gcc/llvm]-get-res.py # Get the bug isolation result rank lists
sudo python ./[gcc/llvm]-eval.py # Get Top-n/MFR/MAR
```