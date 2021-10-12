#Creating a term Calculator that can handle (),*,/,+ and - 
#using python as an approach 

#Mauro Lopez 
from enum import Enum

#Using enumartation classes allow for a much cleaner and nicer comparison
class Operators(Enum):
    ##order of operation and their ranks
    ADDITION=0
    SUBSTRACTION=0
    MULTIPLICATION=1
    DIVISION=1
    EXPONENT=2
    L_PARENTHESIS=3
    R_PARENTHESIS=3
    ERROR= -1 ## operator not valid

def get_operator(x):
    #convert operators into enum for easy comparison 
    if x =='+':
        return Operators.ADDITION
    elif x=='-':
        return Operators.SUBSTRACTION
    elif x=='*':
        return Operators.MULTIPLICATION
    elif x=='/':
        return Operators.DIVISION
    elif x=='(':
        return Operators.L_PARENTHESIS
    elif x==')':
        return Operators.R_PARENTHESIS
    elif x=='^':
        return Operators.EXPONENT
    else:
        return Operators.ERROR

def convert_infix_to_postfix(expression):
    #convert a infix notation to an postfix 
    post_fix_list = list()
    operator_stack = list()

    for x in expression :
        #print(f"NEW TOKEN : {x}")

        #if its an operand/number we added to the list 
        if (x.isnumeric()):
            post_fix_list.append(int(x))
        #if its a left parenthesis it takes precedence 
        elif(x=='('):
            #prioritized left parenthesis
            operator_stack.append(get_operator(x))
        elif(x==')'):
            ## we found the right parenthesis so we pop everything until left
            ## parenthesis is found 
            new_operator= get_operator(x)
            post_fix_list.append(operator_stack.pop()) 
            l_parenthesis_flag =False
            ## we make sure the stack is not empty and havent found an element 
            while(len(operator_stack)>0 and l_parenthesis_flag== False ):
                temp_operator = operator_stack[-1]
                if temp_operator== Operators.L_PARENTHESIS:
                    operator_stack.pop() ## disscard L parenthesis since we dont need it 
                    l_parenthesis_flag= True
                else:
                    ## we keep popping the operators 
                    post_fix_list.append(temp_operator)


        elif(len(operator_stack)<1): ## we have an empty operator stack, so anything automatically goes there
            new_operator = get_operator(x) ## convert to our operator class
            operator_stack.append(new_operator)
        else:
            #ELSE we do have somthing in our stack so we need to do some comparison 
            new_operator = get_operator(x) #convert operator to class 

            #NOTE: According to algorithm if anything inside the stack is less in precedence, then just add it to 
            ## the top of the stack. OR if its a left parenthesis since, parenthesis are higher priority 
            if(new_operator.value>operator_stack[-1].value or operator_stack[-1]== Operators.L_PARENTHESIS):
                operator_stack.append(new_operator)
            else:
            ##NOTE: now we have the case in which the top of the stack is equal or bigger than the new operator in terms 
            ## of precedence.
            ## We need to pop the top of the stack until we find a operator less than our new operator or 
            ## we find a L parenthesis. Once we have that we add the new operator to the stack 
                while( (len(operator_stack)>0) and ( operator_stack[-1].value>=new_operator.value)  ):
                    post_fix_list.append(operator_stack.pop())
                    if(operator_stack[-1]==Operators.L_PARENTHESIS):
                        break
                operator_stack.append(new_operator)


    while(len(operator_stack)>0):
        post_fix_list.append(operator_stack.pop())
    return post_fix_list

def perform_operation(l_operand,r_operand,operator):
    if(operator== Operators.ADDITION):
        return l_operand+ r_operand
    elif(operator == Operators.SUBSTRACTION):
        return l_operand- r_operand
    elif(operator == Operators.MULTIPLICATION):
        return l_operand* r_operand
    elif(operator == Operators.DIVISION):
        return l_operand/ r_operand
    elif(operator == Operators.EXPONENT):
        return l_operand** r_operand

def solve_postfix(expression):
    operand_stack = list()
    for x in expression:
        if( isinstance(x,int)):
            operand_stack.append(x)
        else:
            right_operand = operand_stack.pop()
            left_operand = operand_stack.pop()
            sol= perform_operation(left_operand,right_operand,x)
            operand_stack.append(int(sol))
    return operand_stack.pop()












def main():
    infix_notation = input("input expression to evaluate: ")
    postfix_notation = convert_infix_to_postfix(infix_notation)
    solution = solve_postfix(postfix_notation)
    print("SOLUTION: ",solution)


if __name__== "__main__":
    main()
