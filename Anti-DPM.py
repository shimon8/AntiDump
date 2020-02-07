from shutil import copyfile
import sys
from ctypes import *
##########Text################
def updateFileLocation(file,i):
    x = i
    file.seek(x, 0)
def findEncEndText(fileRead,i ,numberFunc):#find the size of the area we want to encrypt
    j=i
    size = 0;#the size of the area we want to encrypt
    bytes = b'1';
    while bytes != b'':
        bytes = fileRead.read(15);#our flags 15 bytes
        #checking when you got to finish flag of the currnet area and return the size
        if (bytes == checkingEndFuncT1 and numberFunc == 1) or (bytes == checkingEndFuncT2 and numberFunc == 2) or  (bytes == checkingEndFuncT3 and numberFunc == 3):
            return size+15;#our flags 15 bytes
        j += 1#our current index in the file
        updateFileLocation(fileRead,j)#update index if we didnt found yet the flag end
        size = size + 1;
    return 0;

def removeFile(nameFile):
    import os
    os.remove(nameFile)
def encryptTextByBytes(fileRead, fileWrite,i,numberFunc,password):

    updateFileLocation(fileRead,i)
    size = findEncEndText(fileRead,i ,numberFunc)#find the size of the area we want to encrypt
    updateFileLocation(fileRead, i)
    roundsModulo = size % 8 # we need this because TEA works in 8 bytes and we have extra that we not encrypt
    nameWriteToC = "files/WriteTheTextWeNeedToEncrypt.bin"
    nameReadFromC ="files/ReadAfterCfileEncryptedHim.bin"

    fileWriteToForC = open(nameWriteToC, "wb")# we write the area we want to encrypt to anthor file
    bytes = fileRead.read(size-roundsModulo)
    fileWriteToForC.write(bytes)

    import subprocess
    fileWriteToForC.close()
    #cFile.exe read from fileWriteToForC and encrypt him to nameReadFromC
    subprocess.Popen([r'files/cFile.exe',nameWriteToC,nameReadFromC,str(size-roundsModulo),password[0],password[1],password[2],password[3]])
    import time
    time.sleep(0.2)#wait the cFile.exe finish to write
    fileReadFromC = open(nameReadFromC, "rb")#read what cFile wrote
    bytes2 = fileReadFromC.read()
    fileReadFromC.close()
    fileWrite.write(bytes2)
    updateFileLocation(fileRead, i+size-roundsModulo)
    if(roundsModulo != 0):#take care of the rest byte we didnt encrypt the last byte are part of our flag
        bytes = fileRead.read(roundsModulo)
        fileWrite.write(bytes)
    removeFile(nameWriteToC)
    removeFile(nameReadFromC)
    return size;

############### the flags of .text secrets areas
checkingStartFuncT1=bytes.fromhex("BBBD0C4D42BBBD0C4D42BBBD0C4D42")
checkingEndFuncT1 =bytes.fromhex("BBBDED4248BBBDED4248BBBDED4248")

checkingStartFuncT2=bytes.fromhex("BBBDD6E77DBBBDD6E77DBBBDD6E77D")
checkingEndFuncT2 =bytes.fromhex("BBBDB7DD83BBBDB7DD83BBBDB7DD83")

checkingStartFuncT3=bytes.fromhex("BBBDA082B9BBBDA082B9BBBDA082B9")
checkingEndFuncT3 =bytes.fromhex("BBBD8178BFBBBD8178BFBBBD8178BF")
def encrtptFirstFuncT(fileRead,i,lastIndex,fileWrite,password):
    updateFileLocation(fileRead, 0)
    bytes = fileRead.read(i - lastIndex)#read currnetIndex - lastIndex
    fileWrite.write(bytes);
    updateFileLocation(fileRead, i)
    size = encryptTextByBytes(fileRead, fileWrite, i , 1,password)#encrypt the current area of .text and return the size of the secret area
    updateFileLocation(fileRead, size + i)
    return i + size
def encrtptMiddleFuncT(fileRead,i , lastIndex,fileWrite,password):
    updateFileLocation(fileRead, lastIndex)
    bytes = fileRead.read(i - lastIndex)#read currnetIndex - lastIndex
    fileWrite.write(bytes);
    updateFileLocation(fileRead, i)
    size = encryptTextByBytes(fileRead, fileWrite, i , 2,password)#encrypt the current area of .text and return the size of the secret area
    updateFileLocation(fileRead, size + i)
    return i + size
def encrtptLastFuncT(fileRead,i,lastIndex,fileWrite,password):
    updateFileLocation(fileRead, lastIndex)
    bytes = fileRead.read(i - lastIndex)#read currnetIndex - lastIndex
    fileWrite.write(bytes)
    updateFileLocation(fileRead, i)
    size = encryptTextByBytes(fileRead, fileWrite, i , 3,password);#encrypt the current area of .text and return the size of the secret area
    updateFileLocation(fileRead, size + i)
    return i + size

def encrptText(FileName,password):
    fileRead = open(FileName, "rb")
    fileWrite = open("files/temp.exe", "wb")
    try:

     i=0; #the currnet index
     lastIndex = 0
     bytes = b'1'
     while bytes != b'':

        bytes = fileRead.read(15)#the size of the flags is 15 bytes
        if bytes == checkingStartFuncT1:# if we get the to the begin of 1 we want to encrypt
            i = encrtptFirstFuncT(fileRead,i,  lastIndex,fileWrite,password)#encrypt until the end of the first area and return the currnet index
            lastIndex = i
        elif bytes == checkingStartFuncT2:# if we get the to the begin of 2 we want to encrypt
            i = encrtptMiddleFuncT(fileRead,i, lastIndex,fileWrite,password)#encrypt until the end of the  second area and return the currnet index
            lastIndex = i
        elif bytes == checkingStartFuncT3:# if we get the to the begin of 3 we want to encrypt
            i = encrtptLastFuncT(fileRead , i , lastIndex,fileWrite,password)#encrypt until the end of the third area and return the currnet index
            bytes = fileRead.read()
            fileWrite.write(bytes)


        else :
            fileRead.seek(i,0)
            bytes = fileRead.read(1)
        i += 1


    finally:
     fileRead.close()
     fileWrite.close()
############################Data#################################

def findEncEndD(fileRead,i ,numberFunc):
    j=i#the current index in the file
    size = 0;#the size of the area we want to encrypt
    bytes = b'1';
    while bytes != b'':
        bytes = fileRead.read(30);#the flag data size is 30 bytes
        if (bytes == checkingEndFuncD1 and numberFunc == 1) or (bytes == checkingEndFuncD2 and numberFunc == 2) or  (bytes == checkingEndFuncD3 and numberFunc == 3):
            #if I get to the end of area that we want to encrypt return the size + the size of the flag
            return size+30;#the flag data size is 30 bytes
        j += 1#update index if we didnt found yet the flag end
        updateFileLocation(fileRead,j)#update index if we didnt found yet the flag end
        size = size + 1;
    return 0;
def encryptDataByBytes(fileRead, fileWrite,i,numberFunc,password):

    updateFileLocation(fileRead,i)
    size = findEncEndD(fileRead,i ,numberFunc)#find the size of the area we need to encrypt
    updateFileLocation(fileRead, i)
    nameWriteToC = "files/WriteTheTextWeNeedToEncrypt.bin"
    nameReadFromC ="files/ReadAfterCfileEncryptedHim.bin"

    fileWriteToForC = open(nameWriteToC, "wb")#write the bytes we want to encrypt to file
    bytes = fileRead.read(size)
    fileWriteToForC.write(bytes)#write the bytes we want to encrypt to file

    import subprocess
    fileWriteToForC.close()
    subprocess.Popen([r'files/xorByte.exe',nameWriteToC,nameReadFromC,str(size),password[0],password[1],password[2],password[3]])#xorByte.exe encrypt what we send to him and write it to nameReadFromC
    import time
    time.sleep(0.2)#wait until xorByte finish to write to nameReadFromC
    fileReadFromC = open(nameReadFromC, "rb")#reading the encrytped bytes
    bytes2 = fileReadFromC.read()
    fileReadFromC.close()
    fileWrite.write(bytes2)
    updateFileLocation(fileRead, i+size)

    removeFile(nameWriteToC)#now we not needed it
    removeFile(nameReadFromC)#now we not needed it
    return size;

############### the flags of .data secrets areas
checkingStartFuncD1=bytes.fromhex("4F757253746172744F757253746172744F75725374617231313131313131")
checkingEndFuncD1 =bytes.fromhex("4F7572456E644F7572456E644F7572456E644F7572456E64313131313131")

checkingStartFuncD2=bytes.fromhex("4F757253746172744F757253746172744F75725374617232323232323232")
checkingEndFuncD2 =bytes.fromhex("4F7572456E644F7572456E644F7572456E644F7572456E64323232323232")

checkingStartFuncD3=bytes.fromhex("4F757253746172744F757253746172744F75725374617233333333333333")
checkingEndFuncD3 =bytes.fromhex("4F7572456E644F7572456E644F7572456E644F7572456E64333333333333")
def encrtptFirstFuncD(fileRead,i,lastIndex,fileWrite,password):
    updateFileLocation(fileRead, 0)
    bytes = fileRead.read(i - lastIndex)#read number bytes correctIndex - lastIndex
    fileWrite.write(bytes);
    updateFileLocation(fileRead, i)
    size = encryptDataByBytes(fileRead, fileWrite, i , 1,password);#encrypt the current area of .data and return the size of the secret area
    updateFileLocation(fileRead, size + i)
    return i + size
def encrtptMiddleFuncD(fileRead,i , lastIndex,fileWrite,password):
    updateFileLocation(fileRead, lastIndex)
    bytes = fileRead.read(i - lastIndex)#read number bytes correctIndex - lastIndex
    fileWrite.write(bytes);
    updateFileLocation(fileRead, i)
    size = encryptDataByBytes(fileRead, fileWrite, i , 2,password);#encrypt the current area of .data and return the size of the secret area
    updateFileLocation(fileRead, size + i)
    return i + size

def encrtptLastFuncD(fileRead,i,lastIndex,fileWrite,password):
    updateFileLocation(fileRead, lastIndex)
    bytes = fileRead.read(i - lastIndex)#read number bytes correctIndex - lastIndex
    fileWrite.write(bytes)
    updateFileLocation(fileRead, i)
    size = encryptDataByBytes(fileRead, fileWrite, i , 3,password);#encrypt the current area of .data and return the size of the secret area
    updateFileLocation(fileRead, size + i)
    return i + size
def encryptData(FileNameRead,originName,password):
    fileRead = open(FileNameRead, "rb")
    fileWrite = open("TheFinalProduct/"+originName[0:len(originName)-1]+"exe", "wb")
    try:

     i=0;
     lastIndex = 0
     size = 0;
     bytes = b'1'
     while bytes != b'':
        bytes = fileRead.read(30)#the flag data size is 30 bytes
        if bytes == checkingStartFuncD1:#if we get to the begin of area
            i = encrtptFirstFuncD(fileRead,i,  lastIndex,fileWrite,password)#encrypt the current area of data and return the current index
            lastIndex = i
        elif bytes == checkingStartFuncD2:
            i = encrtptMiddleFuncD(fileRead,i, lastIndex,fileWrite,password)#encrypt the current area of data and return the current index
            lastIndex = i
        elif bytes == checkingStartFuncD3:
            i = encrtptLastFuncD(fileRead , i , lastIndex,fileWrite,password)#encrypt the current area of data and return the current index
            bytes = fileRead.read()#read the rest of the file
            fileWrite.write(bytes)


        else :
            fileRead.seek(i,0)
            bytes = fileRead.read(1)
        i += 1


    finally:
     fileRead.close()
     removeFile("files/temp.exe")
     fileWrite.close()
############################PutToC#########################
def readFromFile(fileName):
    file = open(fileName,"r")
    fileCode =file.read()
    file.close()
    return fileCode
def  orderCodeOfFuncStart(numberFunc,fileRead):#return string that have  the code from the last place (the last end or the end of the endOfinclude) until the end of start(numberOfFunc) and inject before the start(numerOfFunc) our code
    code =''
    line = "a"
    start="//start" + str(numberFunc)
    while line != '':
        line = fileRead.readline()
        code += line
        if start in line :
            code += "\n" + readFromFile("readFiles/BeginOfFun"+str(numberFunc)+".txt")
            return code

    return 'error'

def orderCodeOfFuncEnd(numberFunc,fileRead):#return string that have the code from the last end of start(numerFunc) until the correct end (end(numerFunc)) and inject our code after the end of the (end(numerFunc))
    code = ''
    line = "a"
    end =  "//end"+str(numberFunc)
    while line != '':
        line = fileRead.readline()
        code += line
        if end in line:
            code += "\n" + readFromFile("readFiles/EndOfFun" + str(numberFunc) + ".txt")
            return code

    return 'error'

def  orderCodeOfFunc(numberFunc,fileRead):
    code =''
    code += orderCodeOfFuncStart(numberFunc, fileRead)
    code += orderCodeOfFuncEnd(numberFunc,fileRead)
    return code
def orderCodeOfAfterInclude(fileRead,password):
        passwordCode = "char * b = \"OurStartOurStartOurStar1111111\";\n"
        passwordCode += "unsigned char * key1=\"" + password[0] + "\";\nunsigned char * key2 = \"" + password[1] + "\"; \n"
        passwordCode += "unsigned char * key3 = \"" + password[2] + "\";\nunsigned char * key4 = \"" + password[3] + "\";\n"
        passwordCode += " char * c = \"OurEndOurEndOurEndOurEnd111111\";\n"
        code = passwordCode
        line = "a"
        endOfInclude =  "//endOfInclude"
        while line != '':
            line = fileRead.readline()
            code +=line
            if endOfInclude in line:
                code += "\n" + readFromFile("readFiles/EndOfInclude.txt")
                return code
        return 'error'


def passwordCollecte():#gets the password from the user
    password=''
    while len(password) != 16:
        password = input("Enter your key,please enter 16 digits and press enter:\n")
        if len(password) != 16:
            print("The key is invalid")
    passwordList =[password[0:4],password[4:8],password[8:12],password[12:16]]
    return passwordList
def PutInFile(fileName,password):

    fileRead = open(fileName,"r")
    fileWriteC = open("DumpFile/Dump"+fileName, "w")
    code =''
    code += orderCodeOfAfterInclude(fileRead,password)#bring the code until the end of the include (the customer) and our functions
    code += orderCodeOfFunc(1 , fileRead)#bring the code of fun 1 (from customer file and the code we want to inject)
    code += orderCodeOfFunc(2 , fileRead)#bring the code of fun 2 (from customer file and the code we want to inject)
    code += orderCodeOfFunc(3 , fileRead)#bring the code of fun 3 (fromcustomer file and the code we want to inject)
    code += fileRead.read()#bring the rest of customer file
    fileWriteC.write(code)
    fileRead.close()
    fileWriteC.close()
def main():
    print("Insert the source code to the main folder")
    originFileName = input("Enter the file name, please: ")
    password = passwordCollecte()#gets the password from the user
    try:
        PutInFile(originFileName,password)  # make a new file and puts the customer code in it and inject our codes in the correct placecs
        print("compile", "Dump" + originFileName, "in DumpFile", '\n')
        print("Insert the exe file to the main folder, please\n")
        fileName = input("Enter the name of the exe: ")
        print("wait a few seconds")
        try:
            encrptText(fileName,
                       password)  # make a new file temp.exe in /files and encrypt the text and put the rest of the file in it
            encryptData("files/temp.exe",
                        originFileName,password)  # read from temp.exe and encrypt the data to /TheFinalProduct."originName".exe
            print("the file: " + originFileName[0:len(originFileName) - 1] + "exe" + " is ready in TheFinalProduct")

        except:
            print("error try again please\n")
    except:
        print("error try again please\n")


if __name__== "__main__":
    main()
