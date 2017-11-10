from random import randint
def gen(num):
    for i in range(num):
        print("  stress"+str(i)+" = test"+str(i+1)+";")
    return None

def gen2(num):
    for i in range(-num,num):
        print("  test"+str(abs(i-round(num/2)))+" = stress"+str(i-1)+";")
    return None

def gen3(num):
    for i in range(-num*2, round(num/2)):
        print("  thankYou"+str(abs(i))+" = Python"+str(i+3*num)+";")
    return None

def gen4(num):
    for i in range(num%7, round(num*3-1)):
        print("  master"+str(abs(i))+" = Hacka"+str(i+3*num)+";")
    return None

def genRandAsgn():
    lst=["A1aTr3oN","Ari","Azi","Cry","C4ed3U5","Caribbean","Z3rgLiNg","a","a1Ar4K","fURry","started","oN","of","ofAShower","off","ok","old","on","stay","steal","step","stop","face","1","10","100","2","3","4","3201","3202","4243"]
    operators = randint(0,10)
    string = lst[randint(0,len(lst)-10)]+" = "+lst[randint(0,len(lst)-1)]
    for i in range(operators):
        oprt=randint(0,2)
        v=lst[randint(0,len(lst)-1)]
        if (oprt==0):
            string += "+ "
        elif (oprt==1):
            string += "- "
        else:
            string += "* "
        string += v + " "
    return string[:-1]+";"

def genWhile(nest,string,depth):
    if not(depth==nest):
        string += ((nest+1) * "  ") + "while i" + str(nest)+"{\n"
        return genWhile(nest+1,string,depth)
    else:
        a = randint(1,5)
        if (a==1):
            return string+"\n"+(nest * "  ")+genRandAsgn()+((nest+1) * "}")
        elif (a==2):
            return string+((nest+1) * "  ")+genRandAsgn()+((nest+1) * "}")
        elif (a==3):
            i=nest+1
            while not(i==1):
                string += (i*"  ")+genRandAsgn()+"}\n"
                i=i-1
            return string
        else:
            i=nest+1
            while not(i==1):
                i=i-1
                string += "\n"+(i*"  ")+genRandAsgn()+"}"
            return string

def genIf(nest,string,depth):
    if not(depth==nest):
        string += ((nest+1) * "  ") + "if i" + str(nest)+" then{\n"
        return genIf(nest+1,string,depth)
    else:
        a = randint(1,2)
        if (a==1):
            i=nest+1
            while not(i==1):
                string += (i*"  ")+genRandAsgn()+"}\n"
                i=i-1
                string += (i*"  ") + "else{\n" + ((i+1)*"  ") + genRandAsgn() + "}\n"
            return string
        else:
            i=nest+1
            while not(i==1):
                string += "\n"+(i*"  ")+genRandAsgn()
                i=i-1
                string += "\n"+(i*"  ")+"}else{\n"+((i+1)*"  ")+genRandAsgn()+"\n"+(i*"  ")+"}"
            return string

#print(genRandAsgn())
print(genIf(0,"",100))

