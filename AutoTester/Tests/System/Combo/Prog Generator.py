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
    string = lst[randint(0,len(lst)-10)]+" = "+lst[randint(0,len(lst)-1)]+" "
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
            return string+((nest+1) * "  ")+genRandAsgn()+"\n"+(nest * "  ")+((nest) * "}")
        elif (a==2):
            return string+((nest+1) * "  ")+genRandAsgn()+((nest) * "}")
        elif (a==3):
            i=nest+1
            while not(i==1):
                string += (i*"  ")+genRandAsgn()+"}\n"
                i=i-1
            return string[:-1]
        else:
            i=nest+1
            while not(i==1):
                i=i-1
                string += ((i+1)*"  ")+genRandAsgn()+"\n"+(i*"  ")+"}\n"
            return string[:-1]

def genIf(nest,string,depth):
    if not(depth==nest):
        string += ((nest+1) * "  ") + "if i" + str(nest)+" then{\n"
        return genIf(nest+1,string,depth)
    else:
        i=nest+1
        while not(i==1):
            for j in range(randint(1,10)):
                string += (i*"  ")+genRandAsgn()+"\n"
            string = string[:-1]+"}\n"
            i=i-1
            string += (i*"  ") + "else{\n" + ((i+1)*"  ") + genRandAsgn() + "}\n"
        return string[:-1]

def genProcs(n):
    procs=[]
    abc123=["a","b",'c','d','r','c','m','y','R','C','M','Y','A','B','C','D','J','j','P','p','V','v','E','e','I','i','O','o','U','u','W','w','T','t','1','2','3','4','5','6','7','8','9','0']
    alpha=len(abc123)-10
    for i in range(n):
        PNL = randint(0,20)
        procName = abc123[randint(0,alpha-1)]
        for j in range(PNL):
            procName += abc123[randint(0,len(abc123)-1)]
        procs.append(procName)
        print("procedure "+procName+"{")
        for k in range(randint(1,100)):
            a=randint(0,5)
            if (a==1):
                print("  "+genRandAsgn())
            elif (a==2):
                gen2(randint(1,6))
            elif (a==3):
                gen3(n%5)
            elif (a==4):
                gen4(randint(n%7,n%7+2))
            elif (a==5):
                print(genWhile(0,"",randint(1,10)))
            else:
                print(genIf(0,"",randint(1,10)))
        print("}")
    return procs

#print(genRandAsgn())
#print(genWhile(0,"",50))
a=genProcs(15)
print(a)
