#!/usr/bin/python
import sys
import os.path

if len(sys.argv) < 3:
    print "[Error] Correct use is [prefix_files, merged_file]. The two parameters are mandatory."
    quit()

# Load the parameters
prefix_files = sys.argv[1]
result = sys.argv[2]

result_file = open(result, "w")

files = []
for i in range(10):
    files.append(open(prefix_files + str(i+1), "r"))


# Parse the files
line = files[9].readline()
last = 0
inst = 1
while line != '':
    # Calculate the average
    lsplit = line.split() 
    sum = int(lsplit[-1:][0])
    for i in range(9):
        file_line = files[i].readline() 
        sum = sum + int((file_line.split())[-1:][0])
    
    avg = (sum/10) + last
    
    # Print to the result file
    result_file.write(str(inst) + " " + 
                      lsplit[1] + " " + 
                      lsplit[2] + " " + str(avg) + "\n")
    
    # Save the latest avg
    last = avg    

    # Load next line
    line = files[9].readline()
    inst += 1
    
# Close the resources
result_file.close()
for i in range(10):
    files[i].close()
