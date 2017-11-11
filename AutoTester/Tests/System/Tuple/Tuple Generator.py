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
        if s=='':
            s += i
        else:
            s += ", " + str(i)
    print(s)
    return None

foo(10, [])
combi=bar(combi)
hoo(combi)

print("")

combi=[]
foo(5, [])
combi=bar(combi)
hoo(combi)
