function assert(expression)
{
    var result;
    var message;
    if (typeof(expression) == 'string')
    {
        if (!eval(expression))
        {
            alert('Assertion failed: ' + expression);
        }
    }
    else
    {
        if (!expression)
        { 
            alert('Assertion failed. Result: ' + expression);
        }
    }
}
