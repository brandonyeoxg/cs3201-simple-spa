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

def genWhile(nest,string,depth):
    if not(depth==nest):
        string += (nest * "  ") + "while i" + str(nest)+"{\n"
        return genWhile(nest+1,string,depth)
    else:
        a = randint(1,5)
        if (a==1):
            return string+"\n"+((nest-1) * "  ")+(nest * "}")
        elif (a==2):
            return string+((nest) * "  ")+(nest * "}")
        elif (a==3):
            i=nest
            while not(i==0):
                string += (i*"  ")+"}\n"
                i=i-1
            return string
        else:
            i=nest
            while not(i==0):
                i=i-1
                string += "\n"+(i*"  ")+"}"
            return string

def genIf(nest,string,depth):
    if not(depth==nest):
        string += (nest * "  ") + "if i" + str(nest)+"{\n"
        return genIf(nest+1,string,depth)
    else:
        a = randint(1,2)
        if (a==1):
            i=nest
            while not(i==0):
                i=i-1
                string += (i*"  ")+"}\n"
                string += (i*"  ") + "else{\n" + (i*"  ") +"}\n"
            return string
        else:
            i=nest
            while not(i==0):
                i=i-1
                string += "\n"+(i*"  ")+"}else{\n"+((i+1)*"  ")+"\n"+(i*"  ")+"}"
            return string

print(genIf(0,"",10))

