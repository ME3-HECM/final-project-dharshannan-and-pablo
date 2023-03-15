import numpy as np
import statistics as st
# =============================================================================
# Read the txt file and pre-process the RGBC values
# =============================================================================
f = open("Pink.txt","r")
a = f.readlines()
# Lets split this into the RGBC arrays for each Red, Green, Blue and Clear
Red = []
Green = []
Blue = []
Clear = []
# Now we append them as integers
for item in a:
    # We split using spaces 
    Red.append(item.split(" ")[0]) # Keep Red as string, avoids errors later on...
    Green.append(int(item.split(" ")[1]))
    Blue.append(int(item.split(" ")[2]))
    Clear.append(int(item.split(" ")[3]))
    
# Now for the Red list there is an extra random number at the 1st position, so we need to remove it
# To do this we turn the red list into a string again and remove the 1st character
redstr = [] # List to store string values
for i in range(0, len(Red)):
    redstr.append(str(Red[i]))
# Replace the 1st character with a "0"
for j in range(0, len(redstr)):
    if(len(redstr[j])>4):
        redstr[j] = redstr[j].replace(redstr[j][0],"0",1) # Only replace 1st occurance
# Make a new red list       
Red2 = []
# Now convert the string red list into an integer
for k in range(0,len(redstr)):
    Red2.append(int(redstr[k]))

#print(Red2, Green, Blue, Clear)

# =============================================================================
# Now we will process the data !!!
# =============================================================================
percentage = [] # Holds %distribution values for each data point
for i in range(0, len(Red2)):
    # Calculate distribution values for each RGB values
    distR = Red2[i]/(Red2[i] + Green[i] + Blue[i]) 
    distG = Green[i]/(Red2[i] + Green[i] + Blue[i])
    distB = Blue[i]/(Red2[i] + Green[i] + Blue[i])
    # Append these values
    percentage.append([distR,distG,distB])

#print(percentage)
percentage = np.array(percentage) # Turn to numpy array
# Now we average to get an average value for each RGB distribution
avgRED = sum(percentage[:,0])/len(percentage)
avgGREEN = sum(percentage[:,1])/len(percentage)
avgBLUE = sum(percentage[:,2])/len(percentage)

print(avgRED,avgGREEN,avgBLUE) # Print the average RGB values
# Calculate standard deviation for each RGB values also
stdRED = st.stdev(percentage[:,0])
stdGREEN = st.stdev(percentage[:,1])
stdBLUE = st.stdev(percentage[:,2])

print(stdRED, stdGREEN, stdBLUE) # Print the standard deviation of RGB values