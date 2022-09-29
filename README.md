# Programming-Concept

<pre>
Content-->Convert the function to a non-recursive function using the simulating recursion method using stack.
int F(int n)
{
    if(n<=1) return 1;
    int a=n+F(n-1);
    int b=n*F(n/2);
    int c=n-2-(a+b)%2;
    int d=F(c);
    return a+b+d;
}
<pre/>

