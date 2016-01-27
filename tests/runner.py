import os
import subprocess

def getTestList():
	tests = []
	for file in os.listdir("./bin"):
		if ".bin" in file:
			tests.append("./bin/" + file)
	return tests

print "\n"

testNames = getTestList()
testsPassed = 0
testsRan = 0

for file in testNames:
	print file
	ret = subprocess.call([file, str(testsRan), str(len(testNames))])

	if not ret:
		testsPassed += 1

	testsRan += 1

status = "\033[0;32m"

# check to see if any failed
if testsPassed != len(testNames):
	status = "\033[1;31m"

print str(testsRan) + " of " + str(len(testNames)) + " ran"
print status + str(testsPassed) + " of " + str(len(testNames)) + " passed\033[0m"
