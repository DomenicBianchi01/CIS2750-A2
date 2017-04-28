#!/usr/bin/python

#Domenic Bianchi
#CIS 2750 Assignment 2
#February 19, 2017
#This program displays an interface to display posts from a signle or mutliple streams

import os
import sys
import tty
import termios
import operator
import math

#An object of this class represents a single post
class Post:

	def __init__(self, stream, sender, date, intDate, text, lineCount, read, multiPage):
		
		self.stream = stream
		self.sender = sender
		self.date = date
		self.intDate = intDate
		self.text = text
		self.lineCount = lineCount
		self.read = read
		self.multiPage = multiPage

	def __str__(self):

		tempStreamName = ""

		if (self.stream[len(self.stream)-6:] == "Stream"):
			tempStreamName = self.stream[:-6]
		else:
			tempStreamName = self.stream

		if (self.multiPage == True):
			return tempStreamName + "\nSender: " + self.sender + "\n" + self.date + "\n" + str(self.text)
		else:
			return tempStreamName + "\nSender: " + self.sender + "\n" + self.date + "\n" + str(self.text) + "---------"

#Gets all data required to create an object post (sender, stream, date, text, etc)
def getStringsFromFile(tempArray, byteIndexs, index, postFile, streamName, flag):

	postCountArray = []
	streamNameToAdd = ""
	newTextToAdd = ""
	dateToAdd = ""
	senderToAdd = ""
	textToAdd = ""
	lineCount = 0
	splitPost = False
	i = 0

	#Using the bytes from the data file, retrieve the posts
	postFile.seek(int(byteIndexs[index]))
	line = postFile.read(int(byteIndexs[index+1]) - int(byteIndexs[index]))

	streamNameToAdd = "Stream: " + streamName
	senderToAdd = line.splitlines()[0]
	dateToAdd = line.splitlines()[1]

	#Get all lines of the text portion of the post
	for textLine in line.splitlines()[2:]:
		textToAdd = textToAdd + textLine + "\n"
		lineCount = lineCount + 1

	#Using the string date, generate a number representing the date (to be used to sort posts by date)
	intDate = parseDate(dateToAdd)

	numOfPagesRequired = math.ceil(len(textToAdd.splitlines()) + 3) // 23 + 1

	#If the post needs to be divided over more than one page
	if (numOfPagesRequired > 1):

		maxIndex = 19
		minIndex = -1
		firstPartOfPost = True

		while numOfPagesRequired > 0:

			tempText = ""
			splitPost = True
			counter = 0

			for i in textToAdd.splitlines():

				if (counter > minIndex):
					tempText = tempText + i + "\n"

				counter = counter + 1

				if (counter == maxIndex):
					minIndex = maxIndex - 1
					maxIndex = maxIndex + 19
					break

			if (firstPartOfPost == True):

				firstPartOfPost = False
				post = Post(streamNameToAdd, senderToAdd[8:], dateToAdd, intDate, tempText, len(tempText.splitlines()) + 4, flag, True)
				tempArray.append(post)
			else:
				post = Post("", senderToAdd[8:], "", intDate, tempText, len(tempText.splitlines()) + 1, flag, False)
				tempArray.append(post)

			numOfPagesRequired = numOfPagesRequired - 1

	if (splitPost == False):
		#Create post object
		post = Post(streamNameToAdd, senderToAdd[8:], dateToAdd, intDate, textToAdd, lineCount+4, flag, False)
		tempArray.append(post)

	return {"array": tempArray}

#Creates two arrays of posts (objects), one for read posts and one for unread posts.
def preloadPosts(byteIndexs, postFile, offsetIndex, streamName):

	tempArray = []
	tempArray2 = []
	returnArray = []
	returnArray2 = []

	if (offsetIndex == len(byteIndexs)):
		offsetIndex = 0

	#Get all unread posts
	for index in range(offsetIndex, len(byteIndexs)): #set to 0 to go back to viewing all posts

		if (index+2 > len(byteIndexs)):
			break

		returnArray = getStringsFromFile(tempArray, byteIndexs, index, postFile, streamName, False)
		tempArray = returnArray["array"]

	#Get all read posts
	if (offsetIndex != 0):
		for index in range(0, offsetIndex):
			if (index+2 > len(byteIndexs)):
				break

			returnArray2 = getStringsFromFile(tempArray2, byteIndexs, index, postFile, streamName, True)
			tempArray2 = returnArray2["array"]

	return {"unreadArray": tempArray, "readArray": tempArray2}

#Adds all the byte data from the stream data file into an array
def getBytesAndLoadPosts(lines, postFile, offsetIndex, streamName):

	newByteIndexs = []

	newByteIndexs.append(0)

	#Add all lines of the data file to an array which will be used to retrieve the posts
	for byteNum in lines:
		byteNum = byteNum.strip('\n')
		newByteIndexs.append(byteNum)

	returnArray = preloadPosts(newByteIndexs, postFile, offsetIndex, streamName)

	return returnArray

#Checks if new posts have been added to the specified stream
def checkForNew(streamName, originalDataLineCount):

	#Use the data file to check for new posts
	dataFileName = "./messages/" + streamName + "StreamData.txt"
	dataFile = open(dataFileName, 'r')
	lines = dataFile.readlines()
	dataFile.close()

	#If the number of lines in the old data file are equal to the current number of lines in the data file thar means no new posts were added (since posts cannot be deleted)
	if (len(lines) == originalDataLineCount):
		return False
	else:
		return True

#Checks if new posts have been added to any streams that the user has permissions to view
def checkForNewAllMode(streamNames, originalDataLineCount):

	lineCount = 0
	
	#Get and combine the number of lines from all data files (from streams the user has permissions to view)
	for stream in streamNames:

		dataFileName = "./messages/" + stream + "StreamData.txt"
		dataFile = open(dataFileName, 'r')
		lines = dataFile.readlines()
		lineCount = lineCount + len(lines)
		dataFile.close()

	#If the number of lines in the old data file are equal to the current number of lines in the data file thar means no new posts were added (since posts cannot be deleted)
	if (lineCount == originalDataLineCount):
		return False
	else:
		return True

#This function takes in a date in the format of a string and converts that string into a number that represents the date.
#For example, Feb. 14, 2017 10:23 will be converted to 021420171023
def parseDate(date):
	
	newDate = ""

	date = date.strip("\n")
	date = date.split(" ")

	#Conver the month name into its equivalent integer value
	if (date[1] == "Jan."):
		newDate = "1"
	elif (date[1] == "Feb."):
		newDate = "2"
	elif (date[1] == "Mar."):
		newDate = "3"
	elif (date[1] == "Apr."):
		newDate = "4"
	elif (date[1] == "May."):
		newDate = "5"
	elif (date[1] == "Jun."):
		newDate = "6"
	elif (date[1] == "Jul."):
		newDate = "7"
	elif (date[1] == "Aug."):
		newDate = "8"
	elif (date[1] == "Sep."):
		newDate = "9"
	elif (date[1] == "Oct."):
		newDate = "10"
	elif (date[1] == "Nov."):
		newDate = "11"
	elif (date[1] == "Dec."):
		newDate = "12"

	#date[2] contains the day of the month. Remove the comma.
	#date[3] contains the year
	#date[4] contains the time (24-hour clock). Remove the : from the time
	newDate = newDate + date[2].strip(",") + date[3] + date[4].replace(":", "")

	return newDate

#When the user selects to view all streams that they have permissions to view, this function will take the data from all relevant stream files (data, user, stream) to create post objects and organize the posts into a read array and unread array
def loadAllStreams(allFileNames, username):

	unreadArray = []
	readArray = []
	dataArray = []
	userArray = []
	streamArray = []
	postOffset = []
	dataInfo = []
	newByteIndexs = []
	temp = ""
	posts = []
	parsedDate = ""
	textToAdd = ""
	lineCount = 0
	originalDataLineCount = 0
	counter = -1
	i = 0
	splitPost = False

	#Add all files that need to be used to one of the arrays (data array, user array, and stream/post array)
	for name in allFileNames:
		dataArray.append("./messages/" + name + "StreamData.txt")
		userArray.append("./messages/" + name + "StreamUsers.txt")
		streamArray.append("./messages/" + name + "Stream.txt")

	#Get byte data for each stream
	for dataFile in dataArray:
		file = open(dataFile, 'r')

		lines = file.readlines()

		newByteIndexs.append(0)
		
		for byteNum in lines:
			byteNum = byteNum.strip('\n')
			newByteIndexs.append(byteNum)

		dataInfo.append(newByteIndexs)
		newByteIndexs = []
		file.close()

		originalDataLineCount = originalDataLineCount + len(lines)

	#Get data that tells the program what the last post read was in each stream
	for userFile in userArray:
		#Open the user file
		file = open(userFile, 'r')

		lines = file.readlines()

		for user in lines:

			user = user.strip('\n')
			#Get the username
			name = user.rsplit(' ', 1)[0]
			#The number that "count" holds represents the last post read. For example, "7" would mean the user has read the first 7 posts
			count = user.rsplit(' ', 1)[1] 
			
			#If the name from the text file matches the active user in the program, then save the data to an array
			if (name == username):
				postOffset.append(count)

		file.close()

	#Get posts and order them by date
	for streamFile in streamArray:
		file = open(streamFile, 'r')
		counter = counter + 1

		#Get all unread posts
		for index in range(int(postOffset[i]), len(dataInfo[counter])-1): #set to 0 to get all posts from all streams

			#Get post text from stream file
			file.seek(int(dataInfo[counter][index]))
			lines = file.read((int(dataInfo[counter][index+1])) - int(dataInfo[counter][index]))

			#Format the strings needed to create a post object
			streamNameToAdd = "Stream: " + streamFile[11:-4]
			senderToAdd = lines.splitlines()[0]
			dateToAdd = lines.splitlines()[1]

			for textLine in lines.splitlines()[2:]:
				textToAdd = textToAdd + textLine + "\n"
				lineCount = lineCount + 1

			intDate = parseDate(dateToAdd)

			numOfPagesRequired = math.ceil(len(textToAdd.splitlines()) + 3) // 23 + 1

			#If the post needs to be divided over more than one page
			if (numOfPagesRequired > 1):

				maxIndex = 19
				minIndex = -1
				firstPartOfPost = True

				while numOfPagesRequired > 0:

					tempText = ""
					splitPost = True
					counter2 = 0

					for y in textToAdd.splitlines():

						if (counter2 > minIndex):
							tempText = tempText + y + "\n"

						counter2 = counter2 + 1

						if (counter2 == maxIndex):
							minIndex = maxIndex - 1
							maxIndex = maxIndex + 19
							break

					if (firstPartOfPost == True):

						firstPartOfPost = False
						post = Post(streamNameToAdd, senderToAdd[8:], dateToAdd, intDate, tempText, len(tempText.splitlines()) + 4, False, True)
						unreadArray.append(post)
					else:
						post = Post("", senderToAdd[8:], "", intDate, tempText, len(tempText.splitlines()) + 1, False, False)
						unreadArray.append(post)

					numOfPagesRequired = numOfPagesRequired - 1

			if (splitPost == False):
				#Create post object
				post = Post(streamNameToAdd, senderToAdd[8:], dateToAdd, intDate, textToAdd, lineCount+4, False, False)
				unreadArray.append(post)

			textToAdd = ""
			lineCount = 0
			splitPost = False

		#Get all read posts
		if (int(postOffset[i]) != 0):
			for index in range(0, int(postOffset[i])):

				#Get post text from stream file
				file.seek(int(dataInfo[counter][index]))
				lines = file.read((int(dataInfo[counter][index+1])) - int(dataInfo[counter][index]))

				#Format the strings needed to create a post object
				streamNameToAdd = "Stream: " + streamFile[11:-4]
				senderToAdd = lines.splitlines()[0]
				dateToAdd = lines.splitlines()[1]

				for textLine in lines.splitlines()[2:]:
					textToAdd = textToAdd + textLine + "\n"
					lineCount = lineCount + 1

				intDate = parseDate(dateToAdd)

				numOfPagesRequired = math.ceil(len(textToAdd.splitlines()) + 3) // 23 + 1

				#If the post needs to be divided over more than one page
				if (numOfPagesRequired > 1):

					maxIndex = 19
					minIndex = -1
					firstPartOfPost = True

					while numOfPagesRequired > 0:

						tempText = ""
						splitPost = True
						counter2 = 0

						for z in textToAdd.splitlines():

							if (counter2 > minIndex):
								tempText = tempText + z + "\n"

							counter2 = counter2 + 1

							if (counter2 == maxIndex):
								minIndex = maxIndex - 1
								maxIndex = maxIndex + 19
								break

						if (firstPartOfPost == True):

							firstPartOfPost = False
							post = Post(streamNameToAdd, senderToAdd[8:], dateToAdd, intDate, tempText, len(tempText.splitlines()) + 4, False, True)
							readArray.append(post)
						else:
							post = Post("", senderToAdd[8:], "", intDate, tempText, len(tempText.splitlines()) + 1, False, False)
							readArray.append(post)

						numOfPagesRequired = numOfPagesRequired - 1

				if (splitPost == False):
					#Create post object
					post = Post(streamNameToAdd, senderToAdd[8:], dateToAdd, intDate, textToAdd, lineCount+4, False, False)
					readArray.append(post)

				textToAdd = ""
				lineCount = 0
				splitPost = False

		i = i + 1

	#Sort based of the value at index 1 of each array element
	#Unread and read posts are sorted seperatly
	unreadArray = sorted(unreadArray, key=operator.attrgetter('intDate'))
	readArray = sorted(readArray, key=operator.attrgetter('intDate'))
	
	return {"array": unreadArray, "readArray": readArray, "dataLineCount": originalDataLineCount}

#When the user selects to view a single stream, this function will take the data from all relevant stream files (data, user, stream/post) to create post objects and organize the posts into a read array and unread array
def loadStream(userStreams, userReadPostIndex, fullStreamNames, selection):

	i = 0
	offsetIndex = 0

	#Get names of all 3 files for the stream
	for fileName in userStreams:
		if (selection == fileName):
			temp = (fullStreamNames[i])[:-15]
			#offsetIndex tells the program what was the last post read in the stream
			offsetIndex = userReadPostIndex[i]
			postFileName = temp + "Stream.txt"
			dataFileName = temp + "StreamData.txt"
			usersFileName = temp + "StreamUsers.txt"
			break
		i = i + 1

	postFile = open(postFileName, 'r')
	dataFile = open(dataFileName, 'r')

	lines = dataFile.readlines()
	dataFile.close()

	originalDataLineCount = len(lines)

	#Call helper functions to create the post objects
	returnArray = getBytesAndLoadPosts(lines, postFile, offsetIndex, selection)

	postFile.close()

	return {"array": returnArray["unreadArray"], "readArray": returnArray["readArray"], "dataLineCount": originalDataLineCount}

#When the user selects option "s", this function will prompt the user to select a new stream
def promptForStream(streamList, printErrorMessage):

	postFileName = ""
	dataFileName = ""
	returnArray = []
	i = 0

	print("")
	os.system('clear')

	#Display an error message if the user lost permission to view the stream that was active
	if (printErrorMessage == True):
		print("User no longer has permission to view this stream. Please select a new stream.")

	print("Select a stream below: ")

	for fileName in streamList:
		print(fileName),
	print("all")

	#Prompt until the user inputs a valid stream name or the word "all"
	while 1:

		streamName = raw_input()

		if ((streamName not in streamList) and streamName != "all"):
			print("Unable to access stream")
		else:
			break

	return streamName

#When ever a stream is first loaded or the user pages down, update the user file to reflect which posts have been read
def updateReadPosts(posts, lastPostRead, user):

	userFileName = "./messages/" + posts[0].stream[8:] + "StreamUsers.txt"
	dataFileName = "./messages/" + posts[0].stream[8:] + "StreamData.txt"

	oldUserFile = open(userFileName, 'r')
	dataFile = open(dataFileName, 'r')
	newUserFile = open("./messages/temp.txt", 'w')

	lines = oldUserFile.readlines()

	postCount = len(dataFile.readlines())
	dataFile.close()

	#Loop through all line of the data file looking for a matching username
	for username in lines:
		username = username.strip('\n')
		count = len(username)

		#Looping through each line backwards, when the first space is found, everything to the right is the post read count; everything to the left is the username
		for character in reversed(username):
			count = count - 1
			if character == " ":
				usernameShort = username[:count]

				#If the usernames match and the number of posts read in the file is less than the "new" last post read, update the text file to reflect the change
				if (user == usernameShort and int(username[count+1:]) < lastPostRead+1 and lastPostRead+1 < postCount):
					newUserFile.write(usernameShort + " " + str(lastPostRead+1) + "\n")
				elif(user == usernameShort and int(username[count+1:]) < lastPostRead+1):
					newUserFile.write(usernameShort + " " + str(postCount) + "\n")
				else:
					newUserFile.write(username + "\n")
				break

	oldUserFile.close()
	newUserFile.close()

	os.rename("./messages/temp.txt", userFileName)

#When in "all" mode, if the user selects the 'm' option, loop through all relevant stream user files and set the read post count to the # of lines in the matching data file (total number of posts in the stream)
def markAllStreams(streamList, user):

	for fileName in streamList:
		userFileName = "./messages/" + fileName + "StreamUsers.txt"
		dataFileName = "./messages/" + fileName + "StreamData.txt"
		oldUserFile = open(userFileName, 'r')
		dataFile = open(dataFileName, 'r')
		newUserFile = open("./messages/temp.txt", 'w')

		lines = oldUserFile.readlines()
		dataLineCount = dataFile.readlines()

		for username in lines:
			username = username.strip('\n')
			count = len(username)

			for character in reversed(username):
				count = count - 1
				if character == " ":
					usernameShort = username[:count]
					#If the usernames match, upate the read count to the highest possible value (total number of posts in the stream)
					if (user == usernameShort):
						newUserFile.write(usernameShort + " " + str(len(dataLineCount)) + "\n")
					else:
						newUserFile.write(username + "\n")
					break

		oldUserFile.close()
		newUserFile.close()
		dataFile.close()

		os.rename("./messages/temp.txt", userFileName)

#When in "all" mode, when ever a stream is first loaded or the user pages down, update the user file to reflect which posts have been read
def updateReadPostsAllMode(posts, postIndexs, user):

	streamList = {}

	#In the list, there is a key for each stream where the value is the number of posts read in that stream
	for i in range(postIndexs[0], postIndexs[1]+1):
		if (posts[i].stream[8:] in streamList):
			streamList[posts[i].stream[8:]] = int(streamList[posts[i].stream[8:]]) + 1
		else:
			streamList[posts[i].stream[8:]] = 1

	#Loop through all stream user files that the active user has permission to
	for element in streamList:

		userFileName = "./messages/" + element + "Users.txt"

		if not os.path.exists(userFileName):
			continue

		oldUserFile = open(userFileName, 'r')
		newUserFile = open("./messages/temp.txt", 'w')

		lines = oldUserFile.readlines()

		#Loop through all lines of the user file looking for a matching username. When a match is found, update the read post count
		for username in lines:
			username = username.strip('\n')
			count = len(username)

			for character in reversed(username):
				count = count - 1
				if character == " ":
					usernameShort = username[:count]
					if (user == usernameShort):
						newUserFile.write(usernameShort + " " + str(int(username[count+1:]) + int(streamList[element])) + "\n")
					else:
						newUserFile.write(username + "\n")
					break

		oldUserFile.close()
		newUserFile.close()

		os.rename("./messages/temp.txt", userFileName)

#Print a page of posts. Note: Only full posts are printed. If a post fit on the page, it will be printed on the next page
def printPosts2(posts, pageRanges):

	print("")
	os.system('clear')

	lineCount = 0

	#If the page range is a range, than print out the posts that correspond to the indexes within the range
	if (pageRanges[0] != "Blank"):
		for i in range(pageRanges[0], pageRanges[1]+1):
			print(posts[i])
			lineCount = lineCount + posts[i].lineCount
	#If the page range is not a range but there are posts in the stream, displat a message saying all messages have been read
	elif posts:
		print("No unread messages. Page up to view older messages.")
		lineCount = lineCount + 1
	#Otherwise there are no posts in the stream
	else:
		print("There are no messages in this stream.")
		lineCount = lineCount + 1

	#The menu bar should be the very last line of the page/screen so fill all the empty lines with newline characters
	while (lineCount < 23):
		print("\n"),
		lineCount = lineCount + 1

	print("Pg Up  Pg Down  O-order toggle  M-mark all  S-stream  C-check for new  q-quit"),

#Given an array for posts, index them into pages. In other words, group postings so that they can all be displayed on multiple pages
def divideIntoPages(posts):

	startOfBlock = True
	startingIndex = 0
	lineCount = 0
	blockArray = []
	i = 0
	j = 0

	while i < len(posts):

		j = 0

		#If the post can fit on the current page
		if (lineCount + len(posts[i].text.splitlines()) + 4 < 24):

			if (startOfBlock == True):
				startingIndex = i
				startOfBlock = False

			lineCount = lineCount + len(posts[i].text.splitlines()) + 4
			i = i + 1
		#Otherwise, start indexing the next page
		else:
			startOfBlock = True
			lineCount = 0
			blockArray.append([startingIndex, i-1])

	if (startOfBlock == False):
		blockArray.append([startingIndex, i-1])

	return blockArray

#When the user selects the "o" option, sort the posts based on name or date
def changePostOrder(posts, sortOption):

	newArray = []

	#Sort by name
	if (sortOption == 1):
		newArray = sorted(posts, key=operator.attrgetter('sender'))
	#Sort by date
	else:
		newArray = sorted(posts, key=operator.attrgetter('intDate'))

	return newArray

#Every time a key is pressed, check to make sure the active user still has permission to view the stream. For example, if the user is removed from the stream as they are viewing it, the next time they press a key, an error message will print
def hasPermissionToView(stream, user):

	userFileName = "./messages/" + stream + "StreamUsers.txt"
	userFile = open(userFileName, 'r')
	lines = userFile.readlines()

	#Within the stream user file, look for a matching username. If a match is made, that the means the user still has permission to view the stream. Otherwise, they do not.
	for username in lines:
		username = username.strip('\n')
		count = len(username)

		for character in reversed(username):
			count = count - 1
			if character == " ":
				usernameShort = username[:count]
				if (user == usernameShort):
					userFile.close()
					return True

	userFile.close()
	return False

def getFileData(argUsername):

	fullStreamNames = []
	userReadPostIndex = []
	userStreams = []
	printErrorMessage = False				

	#Get all file names in the message directory
	for fileName in os.listdir("./messages"):
		#Search for all files containing user data
		if "StreamUsers" in fileName: 
			fileName = "./messages/" + fileName
			file = open(fileName, 'r')
			lines = file.readlines()

			#Search for a line that matches the username of the active user and save the index that tells the program how many posts in that strea have already been read
			for username in lines:
				username = username.strip('\n')
				fullName = username
				count = len(username)

				for character in reversed(username):
					count = count - 1
					if character == " ":
						username = username[:count]
						break

				if argUsername == username:
					fullStreamNames.append(fileName)
					fileName = fileName[11:-15]
					userStreams.append(fileName)
					userReadPostIndex.append(int(fullName[count+1:]))

			file.close()

	return {"fullStreamNames": fullStreamNames, "userReadPostIndex": userReadPostIndex, "userStreams": userStreams, "printErrorMessage": printErrorMessage}

def programLoop():

	userStreams = []
	userReadPostIndex = []
	fullStreamNames = []
	byteIndexs = []
	postCountArray = []
	pageRanges = []
	pageRanges2 = []
	fullName = ""
	userFileName = ""
	dataFileName = ""
	postFileName = ""
	argUsername = ""
	lineCount = 0
	originalDataLineCount = 0
	i = 0
	postArray = []
	readPostArray = []
	returnArray = []
	currentOrder = 1
	currentViewPage = 0
	lastPageViewed = 0
	startOfUnread = 0
	markAllPressed = False
	allStreamMode = False
	readPosts = False
	displayNoMessagePage = False
	allRead = False
	permission = True

	for arg in sys.argv[1:]:
		argUsername = argUsername + arg + " "

	argUsername = argUsername[:-1]

	#Find all files containing username
	returnArray = getFileData(argUsername)
	fullStreamNames = returnArray["fullStreamNames"]
	userReadPostIndex = returnArray["userReadPostIndex"]
	userStreams = returnArray["userStreams"]
	printErrorMessage = returnArray["printErrorMessage"]

	for fileName in userStreams:
		print(fileName),

	if len(userStreams) == 0:
		print("No Streams")
		exit()
	else:
		print("all")

	while 1:

		selection = raw_input()

		if ((selection not in userStreams) and selection != "all"):
			print("Unable to access stream")
		else:
			break

	if (selection == "all"):
		allStreamMode = True
		returnArray = loadAllStreams(userStreams, argUsername)
		postArray = returnArray["array"]
		readPostArray = returnArray["readArray"]
		originalDataLineCount = returnArray["dataLineCount"]

		for i in readPostArray:
			readPosts = True

		pageRanges = divideIntoPages(postArray)

		if (readPosts == True):
			pageRanges2 = divideIntoPages(readPostArray)

			tempR = pageRanges

			for i in range(0, len(tempR)):
				for j in range(0, 2):
					tempR[i][j] = tempR[i][j] + len(readPostArray)

			postArray = readPostArray + postArray;
			pageRanges = pageRanges2 + pageRanges

			currentViewPage = len(pageRanges2)
			startOfUnread = currentViewPage

		#If not all posts have been viewed
		if (len(pageRanges) > currentViewPage):
			printPosts2(postArray, pageRanges[currentViewPage])
			updateReadPostsAllMode(postArray, pageRanges[currentViewPage], argUsername)
		else:
			pageRanges.append(["Blank"])
			displayNoMessagePage = True
			allRead = True
			printPosts2(postArray, pageRanges[currentViewPage])

	else:
		allStreamMode = False
		returnArray = loadStream(userStreams, userReadPostIndex, fullStreamNames, selection)
		postArray = returnArray["array"]
		readPostArray = returnArray["readArray"]
		originalDataLineCount = returnArray["dataLineCount"]

		for i in readPostArray:
			readPosts = True

		pageRanges = divideIntoPages(postArray)

		if (readPosts == True):
			pageRanges2 = divideIntoPages(readPostArray)

			tempR = pageRanges

			for i in range(0, len(tempR)):
				for j in range(0, 2):
					tempR[i][j] = tempR[i][j] + len(readPostArray)

			postArray = readPostArray + postArray;
			pageRanges = pageRanges2 + pageRanges

			currentViewPage = len(pageRanges2)
			startOfUnread = currentViewPage

		#If not all posts have been viewed
		if (len(pageRanges) > currentViewPage):
			printPosts2(postArray, pageRanges[currentViewPage])
			updateReadPosts(postArray, pageRanges[currentViewPage][1], argUsername)
		else:
			pageRanges.append(["Blank"])
			displayNoMessagePage = True
			printPosts2(postArray, pageRanges[currentViewPage])

	while True:

		savedSettings = termios.tcgetattr(sys.stdin)
		tty.setraw(sys.stdin)
		optionInput = sys.stdin.read(1)

		if (optionInput == '\x1b'):
			optionInput = optionInput + sys.stdin.read(2)

		termios.tcsetattr(sys.stdin, termios.TCSADRAIN, savedSettings)

		if (selection != "all"):

			permission = hasPermissionToView(selection, argUsername)

			if (permission == False):
				optionInput = 's'

		#End program (quit)
		if (optionInput == 'q'):
			break
		#Page down
		elif optionInput == '\x1b[B' and currentViewPage < len(pageRanges)-1:

			goUpdate = False

			printPosts2(postArray, pageRanges[currentViewPage+1])
			currentViewPage = currentViewPage + 1

			if (currentViewPage > lastPageViewed):
				lastPageViewed = currentViewPage
				goUpdate = True

			if (allStreamMode == False and currentOrder == 1):
				updateReadPosts(postArray, pageRanges[currentViewPage][1], argUsername)
			elif (allStreamMode == True and goUpdate == True and allRead == False and currentViewPage > startOfUnread and currentOrder == 1):
				updateReadPostsAllMode(postArray, pageRanges[currentViewPage], argUsername)

		#Page up
		elif optionInput == '\x1b[A' and currentViewPage > 0:

			printPosts2(postArray, pageRanges[currentViewPage-1])
			currentViewPage = currentViewPage - 1

			if (displayNoMessagePage == True):
				displayNoMessagePage = False
				#Remove the element containg "Blank" since the message does not need to be displayed again
				pageRanges.pop()

		elif optionInput == 's':

			fullStreamNames = []
			userReadPostIndex = []
			userStreams = []
			printErrorMessage = False
			displayNoMessagePage = False	
			currentOrder = 1			

			#Find all files containing username
			returnArray = getFileData(argUsername)
			fullStreamNames = returnArray["fullStreamNames"]
			userReadPostIndex = returnArray["userReadPostIndex"]
			userStreams = returnArray["userStreams"]
			printErrorMessage = returnArray["printErrorMessage"]

			if (permission == False):
				permission = True
				printErrorMessage = True

			selection = promptForStream(userStreams, printErrorMessage)

			if (selection == "all"):
				allStreamMode = True
			else:
				allStreamMode = False

			if (allStreamMode == False):
				returnArray = loadStream(userStreams, userReadPostIndex, fullStreamNames, selection)
				postArray = returnArray["array"]
				readPostArray = returnArray["readArray"]
				originalDataLineCount = returnArray["dataLineCount"]

				for i in readPostArray:
					readPosts = True

				pageRanges = divideIntoPages(postArray)

				if (readPosts == True):
					pageRanges2 = divideIntoPages(readPostArray)

					tempR = pageRanges

					for i in range(0, len(tempR)):
						for j in range(0, 2):
							tempR[i][j] = tempR[i][j] + len(readPostArray)

					postArray = readPostArray + postArray;
					pageRanges = pageRanges2 + pageRanges

					currentViewPage = len(pageRanges2)
					startOfUnread = currentViewPage

				#If not all posts have been viewed
				if (len(pageRanges) > currentViewPage):
					printPosts2(postArray, pageRanges[currentViewPage])
					updateReadPosts(postArray, pageRanges[currentViewPage][1], argUsername)
				else:
					pageRanges.append(["Blank"])
					displayNoMessagePage = True
					printPosts2(postArray, pageRanges[currentViewPage])

			else:
				returnArray = loadAllStreams(userStreams, argUsername)
				postArray = returnArray["array"]
				readPostArray = returnArray["readArray"]
				originalDataLineCount = returnArray["dataLineCount"]

				for i in readPostArray:
					readPosts = True

				pageRanges = divideIntoPages(postArray)

				if (readPosts == True):
					pageRanges2 = divideIntoPages(readPostArray)

					tempR = pageRanges

					for i in range(0, len(tempR)):
						for j in range(0, 2):
							tempR[i][j] = tempR[i][j] + len(readPostArray)

					postArray = readPostArray + postArray;
					pageRanges = pageRanges2 + pageRanges

					currentViewPage = len(pageRanges2)
					startOfUnread = currentViewPage

				#If not all posts have been viewed
				if (len(pageRanges) > currentViewPage):
					printPosts2(postArray, pageRanges[currentViewPage])
					updateReadPostsAllMode(postArray, pageRanges[currentViewPage], argUsername)
				else:
					pageRanges.append(["Blank"])
					displayNoMessagePage = True
					printPosts2(postArray, pageRanges[currentViewPage])

		#Mark all
		elif optionInput == 'm' and postArray:
			if (allStreamMode == False):
				updateReadPosts(postArray, len(postArray)-1, argUsername)
			else:
				markAllStreams(userStreams, argUsername)

		#Check for new posts in the current stream (or all streams if in All Mode)
		elif optionInput == 'c':
			fullStreamNames = []
			userReadPostIndex = []
			userStreams = []
			printErrorMessage = False
			displayNoMessagePage = False
			currentOrder = 1

			#Find all files containing username
			returnArray = getFileData(argUsername)
			fullStreamNames = returnArray["fullStreamNames"]
			userReadPostIndex = returnArray["userReadPostIndex"]
			userStreams = returnArray["userStreams"]
			printErrorMessage = returnArray["printErrorMessage"]

			if (allStreamMode == False):

				returnArray = loadStream(userStreams, userReadPostIndex, fullStreamNames, selection)
				postArray = returnArray["array"]
				readPostArray = returnArray["readArray"]
				originalDataLineCount = returnArray["dataLineCount"]

				for i in readPostArray:
					readPosts = True

				pageRanges = divideIntoPages(postArray)

				if (readPosts == True):
					pageRanges2 = divideIntoPages(readPostArray)

					tempR = pageRanges

					for i in range(0, len(tempR)):
						for j in range(0, 2):
							tempR[i][j] = tempR[i][j] + len(readPostArray)

					postArray = readPostArray + postArray;
					pageRanges = pageRanges2 + pageRanges

					currentViewPage = len(pageRanges2)
					startOfUnread = currentViewPage

				#If not all posts have been viewed
				if (len(pageRanges) > currentViewPage):
					printPosts2(postArray, pageRanges[currentViewPage])
					updateReadPosts(postArray, pageRanges[currentViewPage][1], argUsername)
				else:
					pageRanges.append(["Blank"])
					displayNoMessagePage = True
					printPosts2(postArray, pageRanges[currentViewPage])

			elif (allStreamMode == True):
				returnArray = loadAllStreams(userStreams, argUsername)
				postArray = returnArray["array"]
				readPostArray = returnArray["readArray"]
				originalDataLineCount = returnArray["dataLineCount"]
				currentViewPage = 0

				for i in readPostArray:
					readPosts = True

				pageRanges = divideIntoPages(postArray)

				if (readPosts == True):
					pageRanges2 = divideIntoPages(readPostArray)

					tempR = pageRanges

					for i in range(0, len(tempR)):
						for j in range(0, 2):
							tempR[i][j] = tempR[i][j] + len(readPostArray)

					postArray = readPostArray + postArray;
					pageRanges = pageRanges2 + pageRanges

					currentViewPage = len(pageRanges2)
					startOfUnread = currentViewPage

				#If not all posts have been viewed
				if (len(pageRanges) > currentViewPage):
					printPosts2(postArray, pageRanges[currentViewPage])
					updateReadPostsAllMode(postArray, pageRanges[currentViewPage], argUsername)
				else:
					pageRanges.append(["Blank"])
					displayNoMessagePage = True
					printPosts2(postArray, pageRanges[currentViewPage])

		elif optionInput == 'o' and postArray:
			if currentOrder == 1:
				postArray = changePostOrder(postArray, 1)
				pageRanges = divideIntoPages(postArray)
				currentViewPage = 0
				currentOrder = 0
				printPosts2(postArray, pageRanges[currentViewPage])
			else:
				postArray = changePostOrder(postArray, 2)
				pageRanges = divideIntoPages(postArray)
				currentViewPage = 0
				currentOrder = 1
				#currentViewPage = len(pageRanges2)
				#startOfUnread = currentViewPage
				printPosts2(postArray, pageRanges[currentViewPage])

def main():

	programLoop()

if __name__ == "__main__":

	main()
