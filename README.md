# Illumio-Coding-Assignment-2019--2020

Dear Evaluator,

Kindly find the details of the code below.

Since I wrote the code in C++, there is a slight modification in the way in which the input is given to the program.

After executing the program, below line will pop up.

 *Input the name of the csv file :*

Kindly give the name of the input file. Eg : input_file.csv

Second prompt just after entering the file name is below,

 *Enter the value to be tested in csv format or type EXIT to stop eg " inbound,udp,24,52.12.48.92 " :*
 
The value to be tested has to be given as a comma separated value like inbound,udp,24,52.12.48.92

Inorder to exit the program, kindly enter *EXIT*


# How did I test my solution.
   I had created two files.
   One named input_file.csv, and other test.txt.
   In input_file.cvs, comma separated values were given from the set of values {inbound, outbount}, {tcp,udp}, {port/port range}, {ip/ip range}
   In the test.txt, combination from the set of values {inbound, outbount}, {tcp,udp}, {port}, {ip} were given.
   
   Both files were read through program (instead of giving the values through command prompt).

# algorithmic choices I’d like to point out
  To check the range of IP, every octet was converted into 3 digit integer numbers and then formed a final number. Then I compared them using normal comarison.
  That is, 192.16.0.1 becamse 1 192 016 000 001. The preceeding 1 was the logic I came up with to count the zeroes comes in the 1st octet.
  
# refinements or optimizations that you would’ve implemented if I had more time
  1. A better logic for IP range match
  2. Although I converted the input set into 4 parts (to reduce the search initially itself. (Divide by 4)), since we have huge amount of data, the data reading I have implemented is not really efficient. 
  Since the port is a power of 2, a logic such that the - 2 power "the value" - can be directly accessed from the "list" that contain port and IP objects. (This does not mean that I will store a list entry with empty values, but I will create another container to store this mapping and use that to access the actual list that contains IP/IP range - making the lookup even faster)
  3. Multiple IPs/IP range can have same port. I would have improved that implementation. Currently, my code do not support.

# I’d like the reviewer to know
  That I started coding in Python just after reading the question. Since I am a beginner in python, I reached a point at which I reazed that in the given 1 to 2 hours , I would not be able to debug the issues that came across while coding in Python. Hence, I moved to C++, and took another 1 hour 30 min to code and do some basic tests. The logic for both Python and C++ are same. 

# Kindly consider me for the position of DATA ENGINEER INTERN (Data Team.) 

