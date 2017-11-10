from random import randint

def generateTestCases():
    clauses = 500
    variables = 
    while clauses!=0:
        number = randint(1,3)
        clause = randint(0,9)
        if clause==0:
            clause = "Follows"
        elif clause==1:
            clause = "Follows*"
        elif clause==2:
            clause = "Parent"
        elif clause==3:
            clause = "Parent*"
        elif clause==4:
            clause = "Uses*"
        elif clause==5:
            clause = "Modifies"
        elif clause==6:
            clause = "Calls"
        elif clause==7:
            clause = "Calls*"
        elif clause==8:
            clause = "Next"
        elif clause==9:
            clause = "Next*"
        """elif clause==10:
            clause = "Affects"
        else:
            clause = "Affects*"""
        
