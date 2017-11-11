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

def genAffects(num,idx):
    for i in range(num+1):
        string = str(i+idx)+" - Combo Stress Test "+str(i+idx)+"\n"
        a=randint(0,6)
        if (a==0):
            string+="procedure p;\n"
        elif (a==1):
            string+="stmt p;\n"
        elif (a==2):
            string+="stmtLst p;\n"
        elif (a==3):
            string+="call p;\n"
        elif (a==4):
            string+="while p;\n"
        elif (a==5):
            string+="if p;\n"
        else:
            string+="variable p;\n"
        string+="Select p such that"
        for j in range(1,i+2):
            string+=" Affects("+str(j)+","+str(j+1)+") and"
        string = string[:-4]+"\nnone\n"
        string+="5000"
        print(string)
    return string

def ranGenAffects(n,idx):
    print(str(idx)+" - Combo Stress Test "+str(idx))
    stmt=0
    asgn=0
    
    line3="such that "
    for i in range(n):
        a=randint(1,4)
        if (a==1):
            stmt+=2
            line3+="Affects(s"+str(stmt-1)+",s"+str(stmt)+") "
        elif (a==2):
            asgn+=2
            line3+="Affects(a"+str(asgn-1)+",a"+str(asgn)+") "
        elif (a==3):
            asgn+=1
            stmt+=1
            line3+="Affects(s"+str(stmt)+",a"+str(asgn)+") "
        else:
            asgn+=1
            stmt+=1
            line3+="Affects(a"+str(asgn)+",s"+str(stmt)+") "

        a=randint(0,1)
        if a:
            line3+="and "
        else:
            line3+="such that "
    if a:
        line3=line3[:-4]
    else:
        line3=line3[:-10]

    a=randint(1,5)
    if (a==1):
        if (stmt>0):
            line3 = "s"+str(randint(1,stmt))+" " + line3
        else:
            line3 = "a"+str(asgn)+" " + line3
    elif (a==2):
        if (asgn>0):
            line3 = "a"+str(randint(1,asgn))+" " + line3
        else:
            line3 = "s"+str(stmt)+" " + line3
    elif (a==3):
        if (stmt>0):
            line3 = "s"+str(randint(1,stmt))+".stmt# " + line3
        else:
            line3 = "a"+str(asgn)+".stmt#  " + line3
    else:
        if (asgn>0):
            line3 = "a"+str(randint(1,asgn))+".stmt# " + line3
        else:
            line3 = "s"+str(stmt)+".stmt#  " + line3
    line3 = "Select " + line3

    declarations = [stmt,asgn]
    line2=""
    for d in range(len(declarations)):
        if (d==0):
            if not(declarations[d]==0):
                line2 += "stmt "
                for i in range(1,declarations[d]+1):
                    line2 += "s"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        else:
            if not(declarations[d]==0):
                line2 += "assign "
                for i in range(1,declarations[d]+1):
                    line2 += "a"+str(i)+","
                line2=line2[:-1]
                line2+="; "
    print(line2)
    print(line3)
    print()
    print(5000)
    return None

def MultiRGA(n):
    for i in range(1,n+1):
        ranGenAffects(i,15+i)
    return None

def ranGenAffects_(n,idx):
    print(str(idx)+" - Combo Stress Test "+str(idx))
    stmt=0
    asgn=0
    
    line3="such that "
    for i in range(n):
        a=randint(1,4)
        if (a==1):
            stmt+=2
            line3+="Affects*(s"+str(stmt-1)+",s"+str(stmt)+") "
        elif (a==2):
            asgn+=2
            line3+="Affects*(a"+str(asgn-1)+",a"+str(asgn)+") "
        elif (a==3):
            asgn+=1
            stmt+=1
            line3+="Affects*(s"+str(stmt)+",a"+str(asgn)+") "
        else:
            asgn+=1
            stmt+=1
            line3+="Affects*(a"+str(asgn)+",s"+str(stmt)+") "

        a=randint(0,1)
        if a:
            line3+="and "
        else:
            line3+="such that "
    if a:
        line3=line3[:-4]
    else:
        line3=line3[:-10]

    a=randint(1,5)
    if (a==1):
        if (stmt>0):
            line3 = "s"+str(randint(1,stmt))+" " + line3
        else:
            line3 = "a"+str(asgn)+" " + line3
    elif (a==2):
        if (asgn>0):
            line3 = "a"+str(randint(1,asgn))+" " + line3
        else:
            line3 = "s"+str(stmt)+" " + line3
    elif (a==3):
        if (stmt>0):
            line3 = "s"+str(randint(1,stmt))+".stmt# " + line3
        else:
            line3 = "a"+str(asgn)+".stmt#  " + line3
    else:
        if (asgn>0):
            line3 = "a"+str(randint(1,asgn))+".stmt# " + line3
        else:
            line3 = "s"+str(stmt)+".stmt#  " + line3
    line3 = "Select " + line3

    declarations = [stmt,asgn]
    line2=""
    for d in range(len(declarations)):
        if (d==0):
            if not(declarations[d]==0):
                line2 += "stmt "
                for i in range(1,declarations[d]+1):
                    line2 += "s"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        else:
            if not(declarations[d]==0):
                line2 += "assign "
                for i in range(1,declarations[d]+1):
                    line2 += "a"+str(i)+","
                line2=line2[:-1]
                line2+="; "
    print(line2)
    print(line3)
    print()
    print(5000)
    return None

def ranGenNext_(n,idx):
    print(str(idx)+" - Combo Stress Test "+str(idx))
    stmt=0
    asgn=0
    
    line3="such that "
    for i in range(n):
        a=randint(1,4)
        if (a==1):
            stmt+=2
            line3+="Next*(s"+str(stmt-1)+",s"+str(stmt)+") "
        elif (a==2):
            asgn+=2
            line3+="Next*(a"+str(asgn-1)+",a"+str(asgn)+") "
        elif (a==3):
            asgn+=1
            stmt+=1
            line3+="Next*(s"+str(stmt)+",a"+str(asgn)+") "
        else:
            asgn+=1
            stmt+=1
            line3+="Next*(a"+str(asgn)+",s"+str(stmt)+") "

        a=randint(0,1)
        if a:
            line3+="and "
        else:
            line3+="such that "
    if a:
        line3=line3[:-4]
    else:
        line3=line3[:-10]

    a=randint(1,5)
    if (a==1):
        if (stmt>0):
            line3 = "s"+str(randint(1,stmt))+" " + line3
        else:
            line3 = "a"+str(asgn)+" " + line3
    elif (a==2):
        if (asgn>0):
            line3 = "a"+str(randint(1,asgn))+" " + line3
        else:
            line3 = "s"+str(stmt)+" " + line3
    elif (a==3):
        if (stmt>0):
            line3 = "s"+str(randint(1,stmt))+".stmt# " + line3
        else:
            line3 = "a"+str(asgn)+".stmt#  " + line3
    else:
        if (asgn>0):
            line3 = "a"+str(randint(1,asgn))+".stmt# " + line3
        else:
            line3 = "s"+str(stmt)+".stmt#  " + line3
    line3 = "Select " + line3

    declarations = [stmt,asgn]
    line2=""
    for d in range(len(declarations)):
        if (d==0):
            if not(declarations[d]==0):
                line2 += "stmt "
                for i in range(1,declarations[d]+1):
                    line2 += "s"+str(i)+","
                line2=line2[:-1]
                line2+="; "
        else:
            if not(declarations[d]==0):
                line2 += "prog_line "
                for i in range(1,declarations[d]+1):
                    line2 += "a"+str(i)+","
                line2=line2[:-1]
                line2+="; "
    print(line2)
    print(line3)
    print()
    print(5000)
    return None

def MultiRGA_(n):
    for i in range(1,n+1):
        ranGenAffects_(i,15+i)
    return None

def MultiRGN_(n):
    for i in range(1,n+1):
        ranGenNext_(i,15+i)
    return None

MultiRGA_(50)
MultiRGN_(50)
