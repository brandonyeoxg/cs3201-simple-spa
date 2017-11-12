import sys

combi=[]
def foo(n, lst):
    if len(lst)==n:
        combi.append(lst)
    else:
        nlst=list(lst)
        lst.append("A")
        nlst.append("B")
        foo(n, lst)
        foo(n, nlst)

def bar(c):
    ncombi = []
    for l in c:
        #print("now formatting")
        ns = ''
        for s in l:
            if ns=='':
                ns += s
            else:
                ns += " " + str(s)
        ncombi.append(ns)
    return ncombi

def hoo(c):
    s = ''
    for i in c:
        #print("now printing")
        sys.stdout.write(i+", ")
        sys.stdout.flush()
    return None

def foo2(n, lst):
    if len(lst)==n:
        combi.append(lst)
    else:
        #print("still working " + str(len(lst)))
        nlst=list(lst)
        nlst2=list(lst)
        lst.append("A")
        nlst.append("B")
        nlst2.append("C")
        foo2(n, lst)
        foo2(n, nlst)
        foo2(n, nlst2)

def foo3(n, lst):
    if len(lst)==n:
        combi.append(lst)
    else:
        nlst=list(lst)
        nlst2=list(lst)
        nlst3=list(lst)
        lst.append("A")
        nlst.append("B")
        nlst2.append("C")
        nlst3.append("D")
        foo3(n, lst)
        foo3(n, nlst)
        foo3(n, nlst2)
        foo3(n, nlst3)

foo2(10, [])
combi=bar(combi)
hoo(combi)

"""
foo(10, [])
combi=bar(combi)
hoo(combi)

print("")

combi=[]
foo(5, [])
combi=bar(combi)
hoo(combi)
"""
