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

