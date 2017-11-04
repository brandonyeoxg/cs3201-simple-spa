file = "affects-test-3-prog.txt"
def parseProg(filename):
    F = open(filename,'r')
    txt=F.read()
    lst=[]
    string = ""
    for c in txt:
        if c=="\n":
            lst.append(string)
            string=""
        else:
            string+=c
    return lst

def format_lst(lst):
    new_lst=[]
    for i in lst:
        if not(("procedure" in i) or ("else" in i) or (i=="}") or ("//" in i)):
            new_lst.append(i)
    return new_lst

def getAllStmt(lst):
    string=""
    for i in range(len(lst)):
        string += str(i+1)+", "
    string = string[:-2]
    return string

def print_lst(lst):
    for i in lst:
        print(i)
    return None

def getAsgnStmt(lst):
    string=""
    for i in range(len(lst)):
        if ("=" in lst[i]):
            string += str(i+1)+", "
    string = string[:-2]
    return string

def getCallStmt(lst):
    string=""
    for i in range(len(lst)):
        if ("call " in lst[i]):
            string += str(i+1)+", "
    string = string[:-2]
    return string

def getWhileStmt(lst):
    string=""
    for i in range(len(lst)):
        if ("while " in lst[i]):
            string += str(i+1)+", "
    string = string[:-2]
    return string

def getIfStmt(lst):
    string=""
    for i in range(len(lst)):
        if ("if " in lst[i]):
            string += str(i+1)+", "
    string = string[:-2]
    return string


lst = format_lst(parseProg(file))
print(getAllStmt(lst))
print(getAsgnStmt(lst))
print(getCallStmt(lst))
print(getWhileStmt(lst))
print(getIfStmt(lst))
