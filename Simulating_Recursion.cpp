#include <iostream>
#include <stack>

using namespace std;

/* Each call to the F Iterative function is either a call to calculate
   the value of a -> a = n + F(n - 1), or a call to calculate the value of b -> b = n * F(n / 2),
   or to calculate the value of d ->  d = F(c) or to to return a result from any of the previous call types.
*/
enum CallType
{
    ACall,          //if it is a call to calculate the value of a
    BCall,         //if it is a call to calculate the value of b
    DCall,        //if it is a call to calculate the value of d
    ResultCall    // if it is a call to return a result of the above call types
};

// a structure representing the activation record of each call to the F Iterative function
struct FCall
{
    CallType type;
    int actualN, parameter = NULL, result;       /* the type of the call(a,b,d or result)
                                                    ex:of explain actualN: if F(2) that will be must call a -> a = n + F(n - 1)
                                                    the actualN of  the f(2-1) is the 2,and parameter =1.
                                                    result only be used in CallType(ResultCall).*/
};


// a function to calculate the result of FCalls
int calEquations(int actualN,int addNumber,CallType type )
{

    // a variable to store the result
    int result;
    // First, if the type is ACall which means that we need to calculate the value of a
    if (type == ACall)
    {
        // a = n + F(n - 1) with addNumber representing the value of F(n-1)
        result = actualN + addNumber;
    }
    // Second, if the type is BCall which means that we need to calculate the value of b
    else if (type == BCall)
    {
        // b = n * F(n / 2) with addNumber representing the value of F(n / 2)
        result = actualN *addNumber;
    }
    // Third, if the type is DCall which means we need to calculate the value of d
    else
    {
        //  d = F(c) with addNumber representing the value of F(c)
        result = addNumber;
    }
    // we return this calculated result
    return result;

}

int FIterative(int n)
{
    // First we create a call for the parameter n with the actualN equals to n and the parameter equals to n
    FCall call;
    call.actualN = n;
    call.parameter = n;

    // We create a stack of F Iterative Function calls to store FCalls in it simulating the run-time stack
    stack<FCall> s;
    // We add the first call to the stack
    s.push(call);
    //The flag==false-- indicate to the first call (start call) because we don't need to push the call again in stack.
     bool flag = false;
     /*we need to enter this loop only the first time without checking the condition
       because the calls stack has a size of 1, and after the first call if the size
       of the stack becomes 1 it means that the program has its final result in that one FCall of type result*/

    do
    {
        // We store the value of the top of the stack in a variable called f
        FCall f = s.top();
        s.pop(); // And then remove it from the stack

        /*First, we check if it is the base case (parameter = 1) and its a call to calculate a,b or d and not a result call
           because the ResultCall not have any parameter just store value result.*/

        // base case
        if (f.parameter <= 1 && f.type != ResultCall)
        {
            FCall r;                    // a FCall called r to store the result of the f call
            r.type = ResultCall;        // it's type is ResultCall because it stores the value of the f call result
            r.result=calEquations(f.actualN,1,f.type);    // the result value from calling the function calEquations with actualN
            // = the f call actualN and the addNumber = 1 and the type = f call type.
            s.push(r);                 // and then we push the ResultCall r to the calls stack.
        }
        // Second, we check if the f call is a result call and the call after it is a result call
        // also we means that we have calculated the result of the call after those 2 calls
        // case1EX result1 is (a),and result2 is(b) then remove two results and make new result have (a+b)
        // case2EX result1 is(a+b) ,result2 is (d)  then remove two results and make new result have (a+b+d).
        else if (f.type == ResultCall&&s.top().type == ResultCall)
        {
            // we store the fCall after the f call in a variable called f1,the f1 is result.
            FCall f1 = s.top();
            // and then we remove it from the stack
            s.pop();

            // we create a variable r of call type equals to ResultCall to store the new result after summation.
            FCall r;
            r.type = ResultCall;
            /* We need to add those to results, for example if the first ResultCall is the result of ACall
               and the second is the result of BCall,Then we need to calculate the value a + b to
               use it in calculating the return value a + b + d*/
            r.result = f.result + f1.result;

            // if there is another FCall in the stack after the 2 ResultCalls
            if (s.size() != 0)
            {
                // We store this another FCall in a variable called f2
                FCall f2 = s.top();
                // Then we remove this FCall from the stack
                s.pop();
                /*If this FCall is of type DCall that means that we now have the value of
                   a and b and we calculate the value of c*/
                if (f2.type == DCall && f2.parameter == NULL)
                {
                    // Then we calculate the value of c using the f2 actualN which equals to n and r.result equals to a + b.
                    // c = n - 2 - (a+b )% 2;
                    int c = f2.actualN - 2 - r.result % 2;
                    // Then we update the parameter with the calculated c value as it was default NULL value.
                    f2.parameter = c;
                    // Then we push the f2 again with the updated parameter value
                    s.push(f2);
                }
                // If this FCall f2 is not of type DCall for example a ACall or a BCall
                else
                {
                    /*then we calculate its result value by calling the calEquations by passing the
                      f2 actualN and the r result value as the addNumber and the type of f2*/
                    r.result=calEquations(f2.actualN,r.result,f2.type);
                }

            }
            // Finally we add this ResultCall r to the stack
            s.push(r);
        }


        /*Third, we check if the fCall f is of type ResultCall but the fCall after it is not a ResultCall
          we need to switch these two calls to execute the second call first ,make switch because
          example result=result of equation a, but the next top stack is BCall then to continue calculate other equations.*/

        else if (f.type==ResultCall)
        {
            // We store FCall under the FCall of type ResultCall f in a variable f1
            FCall f1=s.top();
            // We remove this FCall from the stack
            s.pop();
            // and then we add these two FCalls in reverse order to make the FCall of type a,b or d f1 at the top of the stack
            // and the FCall of type ResultCall f after it.
            s.push(f);
            s.push(f1);
        }

        /*that case to store the sub_calls of the one call like f(2) is the
          top we make 3 FCalls to calculate ACALL,BCALL and DCALL.*/
        else
        {

            // First we check that flag to not store first call (start call) again in stack
            //but others call will store it again.
            if (flag == true)
            {
                // if we did , then we need to the f call again after popping.
                s.push(f);
            }
            // Second, we create the FCall to calculate the value of a and we call it c1
            FCall c1;
            // We make its type equals ACall because its calculating the value of a
            c1.type = ACall;
            // And we make it's parameter equals to f.parameter - 1 because a = n + F(n - 1)
            // we are calculating the value of F(n - 1) with f.parameter representing n
            c1.parameter = f.parameter - 1;
            /*And we make the c1 actualN equals to f parameter ,
               to know the value of n when make backtracking to calculate  a = n + F(n - 1).*/
            c1.actualN = f.parameter;

            // Third, we create the FCall to calculate the value of b and we call it c2
            FCall c2;
            // We make its type equals BCall because its calculating the value of b
            c2.type = BCall;
              /* And we make it's parameter equals to (f.parameter / 2)
               because b = n * F(n / 2) and we are calculating the value of F(n / 2) with f.parameter representing n.*/
            c2.parameter = f.parameter / 2;
            // And we make the c2 actualN equals to f parameter
            c2.actualN = f.parameter;

            // Third, we create the FCall to calculate the value of d and we call it c3
            FCall c3;
            // We make its type equals DCall because its calculating the value of d
            c3.type = DCall;
            // And we make the c3 actualN equals to f parameter
            c3.actualN = f.parameter;
            //we leave c3 parameter equals to null because it will get updated to c after calculating the value of a and b.

            //add all subCalls to the stack in reverse order to make c1 representing the ACall at the top and then the BCall and DCall.
            s.push(c3);
            s.push(c2);
            s.push(c1);
            // we also need to push the f call again except the first call(Start Call) so we make it equal to true.
            flag = true;

        }

    }
    /*while there is more than one FCall which means that there is more than 1 FCall in the stack
       and we have not finished calculating But if it is equal to 1 that means that we
       have finished calculating that is the final Result .*/
    while (s.size()!=1);

    // a variable to store the final result and return value of the function
    int result;
    // we create a variable final of type FCall to store the last FCall in the stack which contains the result
    FCall final =  s.top();
    // then we empty the stack by popping the last FCall
    s.pop();
    // the final result is the value of the attribute result in the last FCall in the stack
    result = final.result;
    // we return this result
    return result;

}


// the original recursive function
int F(int n)
{
    if (n <= 1)
        return 1;
    int a = n + F(n - 1);
    int b = n * F(n / 2);
    int c = n - 2 - (a + b) % 2;
    int d = F(c);
    return a + b + d;
}
int main()
{
    // test cases for the values from 1 to 20
    for (int i = 1; i <= 20; i++)
    {
        // the result of calling the F iterative Function
        int result = FIterative(i);
        cout << "Result of F iterative Function F(" << i << ") :" << result << endl;
        // the result of calling the F recursive Function
        int result2 = F(i);
        cout << "Result of F Recursive Function F(" << i << ") :" << result2 << endl;
    }
    return 0;
}
