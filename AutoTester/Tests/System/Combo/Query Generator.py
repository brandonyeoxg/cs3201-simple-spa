from random import randint

def genFollows(num,idx):
    for i in range(num+1):
        print(str(i+idx)+" - Combo Stress Test "+str(i+idx))
        a=randint(0,6)
        if (a==0):
            print("procedure p;")
        elif (a==1):
            print("stmt p;")
        elif (a==2):
            print("stmtLst p;")
        elif (a==3):
            print("call p;")
        elif (a==4):
            print("while p;")
        elif (a==5):
            print("if p;")
        else:
            print("variable p;")
        print("Select p such that Follows("+str(i)+","+str(i+1)+")")
        print("")
        print(5000)
    return None

def genParent(num,idx):
    for i in range(1,num+1):
        print(str(i+idx)+" - Combo Stress Test "+str(i+idx))
        a=randint(0,6)
        if (a==0):
            print("procedure p;")
        elif (a==1):
            print("stmt p;")
        elif (a==2):
            print("stmtLst p;")
        elif (a==3):
            print("call p;")
        elif (a==4):
            print("while p;")
        elif (a==5):
            print("if p;")
        else:
            print("variable p;")
        print("Select p such that Parent("+str(i)+","+str(i+1)+")")
        print("none")
        print(5000)
    return None

def genFollows_(num,idx):
    for i in range(1,num+1):
        for j in range(1,num+1):
            print(str(((i-1)*num)+j+idx)+" - Combo Stress Test "+str(((i-1)*num)+j+idx))
            a=randint(0,6)
            if (a==0):
                print("procedure p;")
            elif (a==1):
                print("stmt p;")
            elif (a==2):
                print("stmtLst p;")
            elif (a==3):
                print("call p;")
            elif (a==4):
                print("while p;")
            elif (a==5):
                print("if p;")
            else:
                print("variable p;")
            print("Select p such that Follows*("+str((i-1)*num)+","+str((i-1)*num+j)+")")
            print("")
            print(5000)
    return None

def genParent_(num,idx):
    for i in range(1,num+1):
        for j in range(1,num+1):
            print(str(((i-1)*num)+j+idx)+" - Combo Stress Test "+str(((i-1)*num)+j+idx))
            a=randint(0,6)
            if (a==0):
                print("procedure p;")
            elif (a==1):
                print("stmt p;")
            elif (a==2):
                print("stmtLst p;")
            elif (a==3):
                print("call p;")
            elif (a==4):
                print("while p;")
            elif (a==5):
                print("if p;")
            else:
                print("variable p;")
            print("Select p such that Parent*("+str((i-1)*num)+","+str((i-1)*num+j)+")")
            print("none")
            print(5000)
    return None

def ranGenMultiClause(n,idx):
    print(str(idx)+" - Combo Stress Test "+str(idx))
    stmt=0
    proc=0
    asgn=0
    iiff=0
    whle=0
    vari=0
    call=0
    cons=0
    prog=0
    sLst=0
    line3 = "Select "
    a=randint(1,15)
    if (a==1):
        stmt+=1
        line3 += "s"+str(stmt)+" "
    elif (a==2):
        proc+=1
        line3 += "p"+str(proc)+" "
    elif (a==3):
        asgn+=1
        line3 += "a"+str(asgn)+" "
    elif (a==4):
        iiff+=1
        line3 += "ifs"+str(iiff)+" "
    elif (a==5):
        whle+=1
        line3 += "w"+str(whle)+" "
    elif (a==6):
        vari+=1
        line3 += "v"+str(vari)+" "
    elif (a==7):
        call+=1
        line3 += "ca"+str(call)+" "
    elif (a==8):
        cons+=1
        line3 += "co"+str(cons)+" "
    elif (a==9):
        line3 += "BOOLEAN "
    elif (a==10):
        prog+=1
        line3 += "pl"+str(prog)+" "
    elif (a==11):
        stmt+=1
        line3 += "s"+str(stmt)+".stmt# "
    elif (a==12):
        vari+=1
        line3 += "v"+str(vari)+".varName "
    elif (a==13):
        proc+=1
        line3 += "p"+str(proc)+".procName "
    elif (a==14):
        cons+=1
        line3 += "co"+str(cons)+".value "
    else:
        line3 += "sl"+str(sLst)+" "

    for i in range(n):
        line3+="such that "
        mult=randint(1,3)
        for j in range(mult):
            a=randint(1,20)
            if (a==1):
                stmt+=2
                line3+="Follows(s"+str(stmt-1)+",s"+str(stmt)+") "
            elif (a==2):
                stmt+=2
                line3+="Follows*(s"+str(stmt-1)+",s"+str(stmt)+") "
            elif (a==3):
                stmt+=2
                line3+="Parent(s"+str(stmt-1)+",s"+str(stmt)+") "
            elif (a==4):
                stmt+=2
                line3+="Parent*(s"+str(stmt-1)+",s"+str(stmt)+") "
            elif (a==5):
                stmt+=2
                line3+="Affects(s"+str(stmt-1)+",s"+str(stmt)+") "
            elif (a==6):
                stmt+=2
                line3+="Affects*(s"+str(stmt-1)+",s"+str(stmt)+") "
            elif (a==7):
                prog+=2
                line3+="Next(pl"+str(prog-1)+",pl"+str(prog)+") "
            elif (a==8):
                prog+=2
                line3+="Next*(pl"+str(prog-1)+",pl"+str(prog)+") "
            elif (a==9):
                proc+=2
                line3+="Calls(p"+str(proc-1)+",p"+str(proc)+") "
            elif (a==10):
                proc+=2
                line3+="Calls*(p"+str(proc-1)+",p"+str(proc)+") "
            elif (a==11):
                stmt+=1
                vari+=1
                line3+="Uses(s"+str(stmt)+",v"+str(vari)+") "
            elif (a==12):
                stmt+=1
                vari+=1
                line3+="Modifies(s"+str(stmt)+",v"+str(vari)+") "
            elif (a==13):
                whle+=1
                stmt+=1
                line3+="Parent(w"+str(whle)+",s"+str(stmt)+") "
            elif (a==14):
                whle+=1
                stmt+=1
                line3+="Parent*(w"+str(whle)+",s"+str(stmt)+") "
            elif (a==15):
                iiff+=1
                stmt+=1
                line3+="Parent(ifs"+str(iiff)+",s"+str(stmt)+") "
            elif (a==16):
                iiff+=1
                stmt+=1
                line3+="Parent*(ifs"+str(iiff)+",s"+str(stmt)+") "
            elif (a==17):
                line3+="Next("+str(randint(1,1887))+","+str(randint(1,1887))+") "
            elif (a==18):
                line3+="Next*("+str(randint(1,1887))+","+str(randint(1,1887))+") "
            elif (a==19):
                stmt+=1
                line3+="Affects*(s"+str(stmt)+",s"+str(stmt)+") "
            else:
                prog+=1
                line3+="Next*(pl"+str(prog)+",pl"+str(prog)+") "
            line3+="and "
        line3=line3[:-4]

    declarations = [stmt,proc,asgn,iiff,whle,vari,call,cons,prog,sLst]
    line2=""
    for d in range(len(declarations)):
        if (d==0):
            if not(declarations[d]==0):
                line2 += "stmt "
                for i in range(1,declarations[d]+1):
                    line2 += "s"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==1):
            if not(declarations[d]==0):
                line2 += "procedure "
                for i in range(1,declarations[d]+1):
                    line2 += "p"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==2):
            if not(declarations[d]==0):
                line2 += "assign "
                for i in range(1,declarations[d]+1):
                    line2 += "a"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==3):
            if not(declarations[d]==0):
                line2 += "if "
                for i in range(1,declarations[d]+1):
                    line2 += "ifs"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==4):
            if not(declarations[d]==0):
                line2 += "while "
                for i in range(1,declarations[d]+1):
                    line2 += "w"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==5):
            if not(declarations[d]==0):
                line2 += "variable "
                for i in range(1,declarations[d]+1):
                    line2 += "v"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==6):
            if not(declarations[d]==0):
                line2 += "call "
                for i in range(1,declarations[d]+1):
                    line2 += "ca"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==7):
            if not(declarations[d]==0):
                line2 += "constant "
                for i in range(1,declarations[d]+1):
                    line2 += "co"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        elif (d==8):
            if not(declarations[d]==0):
                line2 += "prog_line "
                for i in range(1,declarations[d]+1):
                    line2 += "pl"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        else:
            if not(declarations[d]==0):
                line2 += "stmtLst "
                for i in range(1,declarations[d]+1):
                    line2 += "sl"+str(i)+","
                line2=line2[:-1]
                line2+="; "
    print(line2)
    print(line3)
    print()
    print(5000)
    return None

def genMultipleMultiClause(num,idx,mult):
    for i in range(num):
        ranGenMultiClause(randint(1,mult),idx+i)
    return None

genMultipleMultiClause(9,3002,10)
genMultipleMultiClause(10,3011,25)
genMultipleMultiClause(10,3021,50)
genMultipleMultiClause(10,3031,100)
genMultipleMultiClause(10,3041,250)
genMultipleMultiClause(10,3051,300)
genMultipleMultiClause(10,3061,350)
genMultipleMultiClause(10,3071,400)
genMultipleMultiClause(10,3081,500)
genMultipleMultiClause(10,3091,600)
