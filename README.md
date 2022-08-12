# MyPacker

# About
In order to learn more about packers and executing pe in memory i decided to write my own packer.

# Features
  1. Api-Hashing with custom ror hashing algorithm implemented by the stub.
  2. Compression MSZIP algorithm.
  3. XOR cipher.
  
# Usage
 <pre>
  PS E:\My_packer\Release> .\My_packer.exe
  Usage: My_packer input_name.exe output_name.exe
  PS E:\My_packer\Release> .\My_packer.exe C:\Windows\SysWOW64\calc.exe packed.exe
  [1] Size Of Original Executable: 26112B
  [2] Size Of Compressed Executable(COMPRESS_ALGORITHM_MSZIP): 5691B
  [3] Size Of stub: 4608B
  [*] Final size of packed executable: 10240B
  PS E:\My_packer\Release> .\packed.exe
 </pre>
 
 # Todo
   1. x64 support
   2. Adding more sophisticated encryption algorithms.
